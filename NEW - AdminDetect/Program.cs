using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Media;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

internal class Program
{
	private static readonly string AdminFolder = "C:\\AdminDetect";

	private static readonly string AdminFilePath = Path.Combine(AdminFolder, "admin_tmp.txt");

	private static readonly string AdminListUrl = "https://raw.githubusercontent.com/DizcatOff/GhostyLite/refs/heads/main/adminfile.txt";

	private static string DiscordWebhookUrl = "";

	private static HashSet<string> AdminGroup1 = new HashSet<string>();

	private static HashSet<string> AdminGroup2 = new HashSet<string>();

	private static string LogFilePath = "";

	private static bool Running = false;

	private static readonly HttpClient client = new HttpClient();

	private static readonly ConsoleColor PrimaryColor = ConsoleColor.Cyan;

	private static readonly ConsoleColor SecondaryColor = ConsoleColor.Magenta;

	private static readonly ConsoleColor AlertColor = ConsoleColor.Red;

	private static readonly ConsoleColor SuccessColor = ConsoleColor.Green;

	private static Dictionary<string, DateTime> lastNotificationTimes = new Dictionary<string, DateTime>();

	private static readonly TimeSpan NotificationDelay = TimeSpan.FromSeconds(10.0);

	private static async Task Main()
	{
		Console.Title = "Ghost Admin Detector v1.0";
		Console.CursorVisible = false;
		await ShowAnimatedHeader();
		await InitializeApplication();
		Running = true;
		await StartChecking();
	}

	private static async Task ShowAnimatedHeader()
	{
		Console.Clear();
		string text = "\r\n ██████╗ ██╗  ██╗ ██████╗ ███████╗████████╗██╗   ██╗\r\n██╔════╝ ██║  ██║██╔═══██╗██╔════╝╚══██╔══╝╚██╗ ██╔╝\r\n██║  ███╗███████║██║   ██║███████╗   ██║    ╚████╔╝ \r\n██║   ██║██╔══██║██║   ██║╚════██║   ██║     ╚██╔╝  \r\n╚██████╔╝██║  ██║╚██████╔╝███████║   ██║      ██║   \r\n ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝   ╚═╝      ╚═╝   \r\n\r\n        ADMIN DETECTOR - [BETA] - https://discord.gg/HGd84yMUrv";
		string text2 = text;
		foreach (char num in text2)
		{
			Console.Write(num);
			if (!char.IsWhiteSpace(num))
			{
				await Task.Delay(5);
			}
		}
		Console.WriteLine("\n\n");
	}

	private static async Task InitializeApplication()
	{
		PrintWithColor("Initializing system...", PrimaryColor);
		await Task.Delay(500);
		PrintWithColor("[+] Creating admin directory...", PrimaryColor);
		Directory.CreateDirectory(AdminFolder);
		PrintWithColor("[+] Downloading admin database...", PrimaryColor);
		await SetupAdminFile();
		LogFilePath = GetLatestLogFilePath();
		if (string.IsNullOrEmpty(LogFilePath))
		{
			PrintWithColor("Auto-detection failed. Checking saved paths...", AlertColor);
			LoadSavedPaths();
		}
		Console.Clear();
		if (string.IsNullOrEmpty(LogFilePath))
		{
			PrintWithColor("Enter chat logs file path: ", SecondaryColor, newLine: false);
			Console.CursorVisible = true;
			LogFilePath = Console.ReadLine();
			Console.CursorVisible = false;
			SavePaths(LogFilePath);
		}
		else
		{
			PrintWithColor("Press ENTER to continue", SecondaryColor);
			Console.CursorVisible = true;
			string text = Console.ReadLine();
			if (!string.IsNullOrWhiteSpace(text))
			{
				LogFilePath = text;
				SavePaths(LogFilePath);
			}
			Console.CursorVisible = false;
		}
		LoadSavedWebhook();
		if (string.IsNullOrEmpty(DiscordWebhookUrl))
		{
			PrintWithColor("Enter Discord webhook URL (For notification): ", SecondaryColor, newLine: false);
			Console.CursorVisible = true;
			DiscordWebhookUrl = Console.ReadLine();
			Console.CursorVisible = false;
			SaveWebhook(DiscordWebhookUrl);
		}
		else
		{
			PrintWithColor("Using saved Discord webhook URL: " + DiscordWebhookUrl, SuccessColor);
			PrintWithColor("Press ENTER to continue or type a new webhook URL...", SecondaryColor);
			Console.CursorVisible = true;
			string text2 = Console.ReadLine();
			if (!string.IsNullOrWhiteSpace(text2))
			{
				DiscordWebhookUrl = text2;
				SaveWebhook(DiscordWebhookUrl);
			}
			Console.CursorVisible = false;
		}
		PrintWithColor("System ready. Starting detection...", SuccessColor);
		Console.Clear();
		await Task.Delay(1000);
	}

	private static async Task CheckPlayerStatus(string playerId)
	{
		_ = 1;
		try
		{
			string requestUri = "https://api.truckersmp.com/v2/player/" + playerId;
			HttpResponseMessage httpResponseMessage = await client.GetAsync(requestUri);
			if (!httpResponseMessage.IsSuccessStatusCode)
			{
				PrintWithColor($"API request failed for ID {playerId}: {httpResponseMessage.StatusCode}", AlertColor);
				return;
			}
			using JsonDocument jsonDocument = JsonDocument.Parse(await httpResponseMessage.Content.ReadAsStringAsync());
			JsonElement rootElement = jsonDocument.RootElement;
			if (!rootElement.GetProperty("error").GetBoolean())
			{
				JsonElement property = rootElement.GetProperty("response");
				string @string = property.GetProperty("groupName").GetString();
				JsonElement property2 = property.GetProperty("permissions");
				bool boolean = property2.GetProperty("isStaff").GetBoolean();
				bool boolean2 = property2.GetProperty("isGameAdmin").GetBoolean();
				bool boolean3 = property2.GetProperty("isManagement").GetBoolean();
				if (!string.IsNullOrEmpty(@string) && (@string.ToLower().Contains("admin") || @string.ToLower().Contains("staff") || @string.ToLower().Contains("retired")))
				{
					NotifyModerator($"Detected via API: Player {property.GetProperty("name").GetString()} (ID: {playerId}) - Group: {@string}", "API ADMIN");
				}
				else if (boolean || boolean2 || boolean3)
				{
					NotifyModerator($"Detected via API: Player {property.GetProperty("name").GetString()} (ID: {playerId}) - Admin Permissions Active", "API ADMIN");
				}
			}
			else
			{
				PrintWithColor("API returned error for ID " + playerId, AlertColor);
			}
		}
		catch (Exception ex)
		{
			PrintWithColor("Error checking API for ID " + playerId + ": " + ex.Message, AlertColor);
		}
	}

	private static string GetLatestLogFilePath()
	{
		string text = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Personal), "ETS2MP", "logs");
		if (!Directory.Exists(text))
		{
			PrintWithColor("Log directory not found: " + text, AlertColor);
			return null;
		}
		string[] files = Directory.GetFiles(text, "log_spawning_*_log.txt");
		if (files.Length == 0)
		{
			PrintWithColor("No log files found in directory.", AlertColor);
			return null;
		}
		DateTime dateTime = DateTime.MinValue;
		string result = null;
		string[] array = files;
		foreach (string text2 in array)
		{
			string fileName = Path.GetFileName(text2);
			string[] array2 = fileName.Split('_');
			DateTime result2;
			if (array2.Length < 4)
			{
				PrintWithColor("Unexpected log file name format: " + fileName, AlertColor);
			}
			else if (DateTime.TryParseExact(array2[2], "yyyy.MM.dd", CultureInfo.InvariantCulture, DateTimeStyles.None, out result2))
			{
				if (result2 > dateTime)
				{
					dateTime = result2;
					result = text2;
				}
			}
			else
			{
				PrintWithColor("Could not parse date from filename: " + fileName, AlertColor);
			}
		}
		return result;
	}

	private static async Task SetupAdminFile()
	{
		_ = 2;
		try
		{
			string text = (await client.GetStringAsync(AdminListUrl)).Trim();
			if (string.IsNullOrWhiteSpace(text) || !Uri.IsWellFormedUriString(text, UriKind.Absolute))
			{
				PrintWithColor("Error: Invalid admin list URL in GitHub file.", AlertColor);
				return;
			}
			PrintWithColor("Downloading admin list from: " + text, PrimaryColor);
			string contents = await client.GetStringAsync(text);
			await File.WriteAllTextAsync(AdminFilePath, contents);
			PrintWithColor("Admin database updated successfully.", SuccessColor);
		}
		catch (Exception ex)
		{
			PrintWithColor("Error downloading admin list: " + ex.Message, AlertColor);
		}
	}

	private static void LoadSavedPaths()
	{
		string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Choice", "Choice.txt");
		if (File.Exists(path))
		{
			LogFilePath = File.ReadAllText(path).Trim();
			PrintWithColor("Loaded saved log file path: " + LogFilePath, SuccessColor);
		}
	}

	private static void SavePaths(string path)
	{
		string text = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Choice");
		Directory.CreateDirectory(text);
		File.WriteAllText(Path.Combine(text, "Choice.txt"), path);
		PrintWithColor("Log file path saved to persistent storage.", SuccessColor);
	}

	private static void LoadSavedWebhook()
	{
		string path = Path.Combine(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Choice"), "Webhook.txt");
		if (File.Exists(path))
		{
			DiscordWebhookUrl = File.ReadAllText(path).Trim();
			PrintWithColor("Loaded saved Discord webhook URL.", SuccessColor);
		}
	}

	private static void SaveWebhook(string webhook)
	{
		string text = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Choice");
		Directory.CreateDirectory(text);
		File.WriteAllText(Path.Combine(text, "Webhook.txt"), webhook);
		PrintWithColor("Discord webhook URL saved to persistent storage.", SuccessColor);
	}

	private static async Task StartChecking()
	{
		if (!File.Exists(AdminFilePath))
		{
			PrintWithColor("Error: Admin database not found.", AlertColor);
			return;
		}
		if (!File.Exists(LogFilePath))
		{
			PrintWithColor("Error: Log file not found at specified path.", AlertColor);
			return;
		}
		PrintWithColor("Loading admin groups...", PrimaryColor);
		await LoadAdminGroups();
		CancellationTokenSource autoUpdateCts = new CancellationTokenSource();
		Task autoUpdateTask = AutoUpdateAdminFile(autoUpdateCts.Token);
		PrintWithColor("Starting real-time monitoring...", SuccessColor);
		PrintWithColor("Press Q to quit\n", SecondaryColor);
		CancellationTokenSource cancelTokenSource = new CancellationTokenSource();
		Task monitoringTask = Task.Run(() => MonitorLogFile(cancelTokenSource.Token));
		while (Running)
		{
			if (Console.KeyAvailable && Console.ReadKey(intercept: true).Key == ConsoleKey.Q)
			{
				Running = false;
				cancelTokenSource.Cancel();
				autoUpdateCts.Cancel();
				break;
			}
			await Task.Delay(100);
		}
		await Task.WhenAll(monitoringTask, autoUpdateTask);
		PrintWithColor("\nMonitoring stopped. Exiting...", SecondaryColor);
		await Task.Delay(1000);
	}

	private static async Task AutoUpdateAdminFile(CancellationToken cancellationToken)
	{
		while (!cancellationToken.IsCancellationRequested)
		{
			PrintWithColor("Refreshing admin database...", PrimaryColor);
			await SetupAdminFile();
			await LoadAdminGroups();
			try
			{
				await Task.Delay(TimeSpan.FromMinutes(10.0), cancellationToken);
			}
			catch (TaskCanceledException)
			{
				break;
			}
		}
	}

	private static async Task LoadAdminGroups()
	{
		string[] source = await File.ReadAllLinesAsync(AdminFilePath);
		AdminGroup1.Clear();
		AdminGroup2.Clear();
		List<string> list = new List<string>();
		foreach (string item in source.Select((string l) => l.Trim()))
		{
			if (string.IsNullOrEmpty(item))
			{
				if (AdminGroup1.Count > 0 && AdminGroup2.Count == 0)
				{
					list = new List<string>();
				}
				continue;
			}
			list.Add(item);
			if (AdminGroup1.Count == 0)
			{
				AdminGroup1.UnionWith(list);
			}
			else
			{
				AdminGroup2.UnionWith(list);
			}
		}
	}

	private static async Task MonitorLogFile(CancellationToken cancellationToken)
	{
		HashSet<string> lastLines = new HashSet<string>();
		long lastPosition = 0L;
		while (Running && !cancellationToken.IsCancellationRequested)
		{
			try
			{
				using (FileStream fs = new FileStream(LogFilePath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
				{
					if (fs.Length < lastPosition)
					{
						lastPosition = 0L;
						lastLines.Clear();
					}
					fs.Seek(lastPosition, SeekOrigin.Begin);
					using StreamReader sr = new StreamReader(fs);
					string text;
					while ((text = await sr.ReadLineAsync()) != null)
					{
						if (!lastLines.Contains(text))
						{
							lastLines.Add(text);
							DetectModerator(text);
						}
					}
					lastPosition = fs.Position;
				}
				await Task.Delay(3000, cancellationToken);
			}
			catch (Exception ex) when (!cancellationToken.IsCancellationRequested)
			{
				PrintWithColor("Monitoring error: " + ex.Message, AlertColor);
				await Task.Delay(5000, cancellationToken);
			}
		}
	}

	private static void DetectModerator(string logLine)
	{
		Match match = Regex.Match(logLine, "TMPID:\\s*(\\d+)");
		if (match.Success)
		{
			string value = match.Groups[1].Value;
			if (AdminGroup1.Contains(value))
			{
				NotifyModerator(logLine, "GAME MODERATOR");
			}
			else if (AdminGroup2.Contains(value))
			{
				NotifyModerator(logLine, "REPORT MODERATOR");
			}
		}
	}

	private static async void NotifyModerator(string logLine, string moderatorType)
	{
		Match match = Regex.Match(logLine, "\\((.+?)\\(");
		Match match2 = Regex.Match(logLine, "\\((.+?)\\((-?\\d+)\\)");
		if (match.Success && match2.Success)
		{
			string value = match.Groups[1].Value;
			string value2 = match2.Groups[2].Value;
			if (!lastNotificationTimes.TryGetValue(value2, out var value3) || !(DateTime.Now - value3 < NotificationDelay))
			{
				lastNotificationTimes[value2] = DateTime.Now;
				string content = $"[{DateTime.Now:HH:mm:ss}] {moderatorType} DETECTED: {value} (ID: @{value2} ) ";
				Console.ForegroundColor = AlertColor;
				Console.Write($"[{DateTime.Now:HH:mm:ss}] ");
				Console.ForegroundColor = ConsoleColor.Yellow;
				Console.Write(moderatorType);
				Console.ForegroundColor = AlertColor;
				Console.WriteLine($" DETECTED: {value} (ID: @{value2}) PING To See if admin still here");
				Console.ResetColor();
				await SendWebhookMessage(content);
				SystemSounds.Exclamation.Play();
			}
		}
	}

	private static async Task SendWebhookMessage(string content)
	{
		try
		{
			StringContent content2 = new StringContent("{ \"content\": \"" + content + "\" }", Encoding.UTF8, "application/json");
			if (!(await client.PostAsync(DiscordWebhookUrl, content2)).IsSuccessStatusCode)
			{
				PrintWithColor("Webhook notification failed.", AlertColor);
			}
		}
		catch (Exception ex)
		{
			PrintWithColor("Webhook error: " + ex.Message, AlertColor);
		}
	}

	private static void PrintWithColor(string message, ConsoleColor color, bool newLine = true)
	{
		Console.ForegroundColor = color;
		if (newLine)
		{
			Console.WriteLine(message);
		}
		else
		{
			Console.Write(message);
		}
		Console.ResetColor();
	}
}

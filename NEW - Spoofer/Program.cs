using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Security.Principal;
using System.Threading.Tasks;
using Microsoft.Win32;

internal class Program
{
	private static readonly string spoofDir = "C:\\Spoofing";

	private static readonly string diskUrl = "https://spyderrock.com/tEci4127-disk.cmd";

	private static readonly string discordUrl = "https://discord.gg/truckersop";

	private static readonly string truckersMPInstallerUrl = "https://spyderrock.com/hceP8083-TruckersMP-Setup.exe";

	private static readonly string volumeidUrl = "https://spyderrock.com/eo238787-Volumeid.exe";

	private static readonly string volumeid64Url = "https://spyderrock.com/udff1179-Volumeid64.exe";

	private static async Task Main()
	{
		if (!IsRunningAsAdministrator())
		{
			Console.WriteLine("This program must be run as administrator.");
			Console.WriteLine("Attempting to restart with elevated privileges...");
			try
			{
				Process.Start(new ProcessStartInfo(Process.GetCurrentProcess().MainModule.FileName)
				{
					UseShellExecute = true,
					Verb = "runas"
				});
			}
			catch (Exception ex)
			{
				Console.WriteLine("Failed to restart as admin: " + ex.Message);
			}
			Environment.Exit(0);
			return;
		}
		Console.Title = "Ghosty ETS2MP Spoofer";
		DisplayBanner();
		await DownloadVolumeidFilesIfNotExistsAsync();
		bool exit = false;
		while (!exit)
		{
			ShowMainMenu();
			switch (Console.ReadLine()?.Trim())
			{
			case "1":
				await StartSpoofAsync();
				exit = true;
				break;
			case "2":
				ShowInfo();
				break;
			case "3":
				JoinDiscord();
				break;
			case "4":
				await InstallAndStartTruckersMPAsync();
				break;
			case "5":
				WindowsActivator();
				break;
			default:
				WriteLineColor("Invalid option. Please choose 1, 2, 3, 4 or 5.", ConsoleColor.Red);
				PauseWithMessage("Returning to main menu...", 2);
				break;
			}
			if (!exit)
			{
				SmoothTransition();
			}
		}
	}

	private static void ShowMainMenu()
	{
		Console.WriteLine("\nSelect an option:");
		WriteLineColor("1. Start Spoofing process", ConsoleColor.DarkBlue);
		Console.WriteLine("2. What does the spoofer do?");
		Console.WriteLine("3. Join Discord");
		Console.WriteLine("4. Install and Start TruckersMP");
		WriteLineColor("5. Enable Windows", ConsoleColor.Red);
		Console.Write("Enter your choice: ");
	}

	private static bool IsRunningAsAdministrator()
	{
		return new WindowsPrincipal(WindowsIdentity.GetCurrent()).IsInRole(WindowsBuiltInRole.Administrator);
	}

	private static async Task DownloadVolumeidFilesIfNotExistsAsync()
	{
		string text = "C:\\Volumeid.exe";
		string volumeid64Path = "C:\\Volumeid64.exe";
		WriteLineColor("\n--- Make sure to run spoofer as admin, After Reboot, Please Enable windows using option (free) --- log:", ConsoleColor.Cyan);
		if (!File.Exists(text))
		{
			WriteLineColor("Volumeid.exe not found in C:\\. Downloading...", ConsoleColor.Yellow);
			WebClient client = new WebClient();
			try
			{
				await client.DownloadFileTaskAsync(volumeidUrl, text);
			}
			finally
			{
				((IDisposable)client)?.Dispose();
			}
			WriteLineColor("Volumeid.exe downloaded successfully.", ConsoleColor.Green);
		}
		else
		{
			WriteLineColor("Volumeid.exe already exists in C:\\.", ConsoleColor.Green);
		}
		if (!File.Exists(volumeid64Path))
		{
			WriteLineColor("Volumeid64.exe not found in C:\\. Downloading...", ConsoleColor.Yellow);
			WebClient client = new WebClient();
			try
			{
				await client.DownloadFileTaskAsync(volumeid64Url, volumeid64Path);
			}
			finally
			{
				((IDisposable)client)?.Dispose();
			}
			WriteLineColor("Volumeid64.exe downloaded successfully.", ConsoleColor.Green);
		}
		else
		{
			WriteLineColor("Volumeid64.exe already exists in C:\\.", ConsoleColor.Green);
		}
	}

	private static void SpoofAllMacAddresses()
	{
		WriteLineColor("\n--- Spoofing MAC Addresses ---", ConsoleColor.Cyan);
		List<ManagementObject> list = new ManagementObjectSearcher("SELECT * FROM Win32_NetworkAdapter WHERE PhysicalAdapter = True AND MACAddress IS NOT NULL").Get().Cast<ManagementObject>().ToList();
		if (list.Count == 0)
		{
			WriteLineColor("No adapters found.", ConsoleColor.Red);
			return;
		}
		string name = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}";
		foreach (ManagementObject item in list)
		{
			string text = item["Name"]?.ToString() ?? "Unknown";
			string pnpDeviceId = item["PNPDeviceID"]?.ToString() ?? "";
			string value = item["MACAddress"]?.ToString() ?? "N/A";
			WriteLineColor($"\nSpoofing Adapter: {text} (Current MAC: {value})", ConsoleColor.Yellow);
			string text2 = GenerateRandomMacAddress();
			WriteLineColor("Generated MAC: " + text2, ConsoleColor.Green);
			bool flag = false;
			using (RegistryKey registryKey = Registry.LocalMachine.OpenSubKey(name, writable: true))
			{
				if (registryKey != null)
				{
					string[] subKeyNames = registryKey.GetSubKeyNames();
					foreach (string name2 in subKeyNames)
					{
						using RegistryKey registryKey2 = registryKey.OpenSubKey(name2, writable: true);
						if (registryKey2 != null)
						{
							string value2 = registryKey2.GetValue("DriverDesc") as string;
							if (!string.IsNullOrEmpty(value2) && text.Contains(value2))
							{
								registryKey2.SetValue("NetworkAddress", text2, RegistryValueKind.String);
								WriteLineColor("Registry updated successfully.", ConsoleColor.Green);
								flag = true;
								break;
							}
						}
					}
				}
			}
			if (!flag)
			{
				WriteLineColor("Failed to find matching registry key.", ConsoleColor.Red);
			}
			else
			{
				RestartNetworkAdapter(pnpDeviceId);
			}
		}
		WriteLineColor("\nAll adapters spoofed.", ConsoleColor.Cyan);
	}

	private static string GenerateRandomMacAddress()
	{
		Random random = new Random();
		byte[] array = new byte[6];
		random.NextBytes(array);
		array[0] = (byte)((array[0] & 0xFE) | 2);
		return BitConverter.ToString(array).Replace("-", "");
	}

	private static void RestartNetworkAdapter(string pnpDeviceId)
	{
		try
		{
			string text = pnpDeviceId.Replace("\\", "\\\\");
			foreach (ManagementObject item in new ManagementObjectSearcher("SELECT * FROM Win32_NetworkAdapter WHERE PNPDeviceID = '" + text + "'").Get())
			{
				WriteLineColor("Disabling adapter...", ConsoleColor.DarkGray);
				item.InvokeMethod("Disable", null);
				TaskDelaySpinner(3000);
				WriteLineColor("Enabling adapter...", ConsoleColor.DarkGray);
				item.InvokeMethod("Enable", null);
				WriteLineColor("Adapter restarted.\n", ConsoleColor.Green);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error restarting adapter: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static async Task StartSpoofAsync()
	{
		WriteLineColor("\nGhosty ETS2MP Spoofer | Disconnect banned account and close Steam first\n", ConsoleColor.Cyan);
		List<string> processedDrives = new List<string>();
		try
		{
			IEnumerable<DriveInfo> enumerable = from d in DriveInfo.GetDrives()
				where d.DriveType == DriveType.Fixed && d.IsReady
				select d;
			foreach (DriveInfo item in enumerable)
			{
				string text = GenerateRandomSerial();
				string text2 = (Environment.Is64BitOperatingSystem ? "C:\\Volumeid64.exe" : "C:\\Volumeid.exe");
				string driveLetter = item.Name.Substring(0, 2);
				string text3 = driveLetter + " " + text;
				WriteLineColor($"\nExecuting Volumeid command for drive {driveLetter}: {text2} {text3}", ConsoleColor.Green);
				await ExecuteVolumeidAsync(text2, text3, "Spoofing Disk Serial for " + driveLetter + "...", ConsoleColor.Green);
				processedDrives.Add(driveLetter);
			}
			CleanupFiles();
			CleanupTemporaryFiles();
			SpoofAllMacAddresses();
			if (Directory.Exists(spoofDir))
			{
				Directory.Delete(spoofDir, recursive: true);
			}
			WriteLineColor("\nSpoofing directory removed successfully.", ConsoleColor.Green);
			GenerateAndUpdateGUIDs();
			ClearDNSCache();
			ChangeComputerNameRandomly();
			ChangeWindowsProductKey();
			RemoveProductId();
			SpoofPCUUID();
		}
		catch (Exception ex)
		{
			WriteLineColor("\n[Error] " + ex.Message, ConsoleColor.Red);
		}
		WriteLineColor("\n========== Spoofing Summary ==========", ConsoleColor.Cyan);
		if (processedDrives.Any())
		{
			WriteLineColor("Disk Serial Spoofing applied to drives: " + string.Join(", ", processedDrives), ConsoleColor.Green);
		}
		else
		{
			WriteLineColor("No disk spoofing was performed.", ConsoleColor.Yellow);
		}
		WriteLineColor("Residual trace files and temporary files have been cleaned.", ConsoleColor.Green);
		WriteLineColor("MAC addresses on all network adapters were spoofed.", ConsoleColor.Green);
		WriteLineColor("New GUIDs have been generated and updated in the registry.", ConsoleColor.Green);
		WriteLineColor("DNS cache has been cleared.", ConsoleColor.Green);
		WriteLineColor("Computer name has been randomized.", ConsoleColor.Green);
		WriteLineColor("Windows Product ID has been updated/removed.", ConsoleColor.Green);
		WriteLineColor("PC UUID was spoofed (simulation only).", ConsoleColor.Green);
		WriteLineColor("======================================", ConsoleColor.Cyan);
		WriteLineColor("\nThe spoofing process is complete. A system restart is required for all changes to take effect.", ConsoleColor.Yellow);
		Console.Write("\nDo you want to restart your computer now? (Y/N): ");
		if (Console.ReadLine()?.Trim().ToUpper() == "Y")
		{
			WriteLineColor("Restarting your computer in 10 seconds...", ConsoleColor.Yellow);
			Process.Start(new ProcessStartInfo("shutdown", "/r /t 10")
			{
				UseShellExecute = true
			});
		}
		else
		{
			WriteLineColor("Restart canceled. Please remember to restart your computer manually to complete the spoofing process.", ConsoleColor.Yellow);
		}
		WriteLineColor("\nPress any key to exit...", ConsoleColor.Yellow);
		Console.ReadKey(intercept: true);
	}

	private static string GenerateRandomSerial()
	{
		Random random = new Random();
		string text = random.Next(4096, 65536).ToString("X4");
		string text2 = random.Next(4096, 65536).ToString("X4");
		return text + "-" + text2;
	}

	private static async Task ExecuteVolumeidAsync(string exePath, string arguments, string msg, ConsoleColor color)
	{
		WriteLineColor(msg, color);
		ProcessStartInfo startInfo = new ProcessStartInfo
		{
			FileName = exePath,
			Arguments = arguments,
			Verb = "runas",
			UseShellExecute = true,
			CreateNoWindow = true
		};
		Process process = Process.Start(startInfo);
		try
		{
			if (process != null)
			{
				await Task.Run(delegate
				{
					process.WaitForExit();
				});
			}
		}
		finally
		{
			if (process != null)
			{
				((IDisposable)process).Dispose();
			}
		}
	}

	private static void ShowInfo()
	{
		Console.Clear();
		DisplayBanner();
		WriteLineColor("\nWhat does the spoofer do?", ConsoleColor.Cyan);
		Console.WriteLine("\n- Spoofs your disk signature by updating your disk serial number using Volumeid.\n- Deletes residual trace files and temporary data.\n- Generates new GUIDs and updates registry entries to spoof your system.\n- Spoofs your MAC addresses across all network adapters.\n- Clears your DNS cache.\n- Randomizes your computer name.\n- Changes and/or removes your Windows Product ID.\n- Simulates spoofing of the PC UUID (note: the true BIOS UUID remains unchanged).\n");
		WriteLineColor("Press any key to return to the main menu...", ConsoleColor.Yellow);
		Console.ReadKey(intercept: true);
		SmoothTransition();
	}

	private static void JoinDiscord()
	{
		try
		{
			Process.Start(new ProcessStartInfo
			{
				FileName = discordUrl,
				UseShellExecute = true
			});
			WriteLineColor("\nDiscord invite opened in your default browser.", ConsoleColor.Green);
		}
		catch (Exception ex)
		{
			WriteLineColor("\n[Error opening Discord] " + ex.Message, ConsoleColor.Red);
		}
		WriteLineColor("\nPress any key to return to the main menu...", ConsoleColor.Yellow);
		Console.ReadKey(intercept: true);
		SmoothTransition();
	}

	private static void WindowsActivator()
	{
		try
		{
			WriteLineColor("\nThis activation tool is completely free to use.", ConsoleColor.Green);
			WriteLineColor("Starting Windows activation process...", ConsoleColor.Yellow);
			Process.Start(new ProcessStartInfo
			{
				FileName = "powershell.exe",
				Arguments = "-NoProfile -ExecutionPolicy Bypass -Command \"irm https://get.activated.win | iex\"",
				UseShellExecute = true
			});
			WriteLineColor("\nActivation process started in PowerShell.", ConsoleColor.Green);
		}
		catch (Exception ex)
		{
			WriteLineColor("\n[Error during activation] " + ex.Message, ConsoleColor.Red);
		}
		WriteLineColor("\nPress any key to return to the main menu...", ConsoleColor.Yellow);
		Console.ReadKey(intercept: true);
		SmoothTransition();
	}

	private static async Task InstallAndStartTruckersMPAsync()
	{
		string installerPath = Path.Combine(Path.GetTempPath(), "TruckersMP-Setup.exe");
		WriteLineColor("Downloading TruckersMP installer...", ConsoleColor.Yellow);
		WebClient client = new WebClient();
		try
		{
			await client.DownloadFileTaskAsync(truckersMPInstallerUrl, installerPath);
		}
		finally
		{
			((IDisposable)client)?.Dispose();
		}
		WriteLineColor("Download completed.", ConsoleColor.Green);
		await ExecuteAsAdminAsync(installerPath, "Installing TruckersMP...", ConsoleColor.Green);
		LaunchTruckersMP();
	}

	private static void LaunchTruckersMP()
	{
		string text = "C:\\Program Files (x86)\\TruckersMP\\TruckersMP.exe";
		if (File.Exists(text))
		{
			Process.Start(new ProcessStartInfo
			{
				FileName = text,
				UseShellExecute = true
			});
			WriteLineColor("TruckersMP launched successfully.", ConsoleColor.Green);
		}
		else
		{
			WriteLineColor("TruckersMP launch file not found.", ConsoleColor.Red);
		}
	}

	private static void DisplayBanner()
	{
		Console.ForegroundColor = ConsoleColor.Blue;
		Console.WriteLine(" \r\n░██████╗░██╗░░██╗░█████╗░░██████╗████████╗██╗░░░██╗\r\n██╔════╝░██║░░██║██╔══██╗██╔════╝╚══██╔══╝╚██╗░██╔╝\r\n██║░░██╗░███████║██║░░██║╚█████╗░░░░██║░░░░╚████╔╝░\r\n██║░░╚██╗██╔══██║██║░░██║░╚═══██╗░░░██║░░░░░╚██╔╝░░\r\n╚██████╔╝██║░░██║╚█████╔╝██████╔╝░░░██║░░░░░░██║░░░\r\n░╚═════╝░╚═╝░░╚═╝░╚════╝░╚═════╝░░░░╚═╝░░░░░░╚═╝░░░");
		Console.ResetColor();
		DrawDivider();
	}

	private static void DrawDivider()
	{
		Console.ForegroundColor = ConsoleColor.DarkGray;
		Console.WriteLine(new string('-', Console.WindowWidth));
		Console.ResetColor();
	}

	private static void WriteLineColor(string message, ConsoleColor color)
	{
		Console.ForegroundColor = color;
		Console.WriteLine(message);
		Console.ResetColor();
	}

	private static async Task ExecuteAsAdminAsync(string filePath, string msg, ConsoleColor color)
	{
		WriteLineColor(msg, color);
		ProcessStartInfo startInfo = new ProcessStartInfo
		{
			FileName = "cmd.exe",
			Arguments = "/C \"" + filePath + "\"",
			Verb = "runas",
			UseShellExecute = true,
			CreateNoWindow = true
		};
		Process process = Process.Start(startInfo);
		try
		{
			if (process != null)
			{
				await Task.Run(delegate
				{
					process.WaitForExit();
				});
			}
		}
		finally
		{
			if (process != null)
			{
				((IDisposable)process).Dispose();
			}
		}
	}

	private static void CleanupFiles()
	{
		WriteLineColor("\n--- Cleaning Traces ---", ConsoleColor.Cyan);
		WriteLineColor("\nCleaning up residual application files...", ConsoleColor.Yellow);
		string[] array = new string[1] { Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Personal), "ETS2MP") };
		foreach (string text in array)
		{
			if (Directory.Exists(text))
			{
				try
				{
					Directory.Delete(text, recursive: true);
					WriteLineColor("Deleted directory: " + text, ConsoleColor.Green);
				}
				catch (Exception ex)
				{
					WriteLineColor("Failed to delete directory " + text + ": " + ex.Message, ConsoleColor.Red);
				}
			}
		}
	}

	private static void CleanupTemporaryFiles()
	{
		WriteLineColor("\n--- Cleaning Traces ---", ConsoleColor.Cyan);
		WriteLineColor("\nCleaning up all files and directories in the TEMP folder recursively...", ConsoleColor.Yellow);
		string tempPath = Path.GetTempPath();
		DeleteAllInDirectory(tempPath);
		TryDeleteRootTempFiles(tempPath);
	}

	private static void DeleteAllInDirectory(string directoryPath)
	{
		try
		{
			string[] files = Directory.GetFiles(directoryPath);
			for (int i = 0; i < files.Length; i++)
			{
				TryDeleteFile(files[i]);
			}
			files = Directory.GetDirectories(directoryPath);
			foreach (string obj in files)
			{
				DeleteAllInDirectory(obj);
				TryDeleteDirectory(obj);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error while processing directory " + directoryPath + ": " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void TryDeleteRootTempFiles(string directoryPath)
	{
		try
		{
			string[] files = Directory.GetFiles(directoryPath, "*", SearchOption.TopDirectoryOnly);
			for (int i = 0; i < files.Length; i++)
			{
				TryDeleteFile(files[i]);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error while deleting root temp files: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void TryDeleteFile(string filePath)
	{
		try
		{
			File.Delete(filePath);
			WriteLineColor("Deleted file: " + filePath, ConsoleColor.Green);
		}
		catch (Exception ex)
		{
			WriteLineColor("Failed to delete file " + filePath + ": " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void TryDeleteDirectory(string dirPath)
	{
		try
		{
			Directory.Delete(dirPath, recursive: false);
		}
		catch
		{
		}
	}

	private static void GenerateAndUpdateGUIDs()
	{
		WriteLineColor("\n--- Spoofing GUIDs ---", ConsoleColor.Cyan);
		string newValue = Guid.NewGuid().ToString();
		string newValue2 = Guid.NewGuid().ToString();
		WriteLineColor("\nGenerating and updating GUIDs...", ConsoleColor.Yellow);
		UpdateRegistryKey("SOFTWARE\\Microsoft\\Cryptography", "MachineGuid", newValue);
		UpdateRegistryKey("SYSTEM\\CurrentControlSet\\Control\\IDConfigDB\\Hardware Profiles\\0001", "HwProfileGuid", newValue2);
	}

	private static void UpdateRegistryKey(string keyPath, string valueName, string newValue)
	{
		try
		{
			using RegistryKey registryKey = Registry.LocalMachine.OpenSubKey(keyPath, writable: true);
			if (registryKey != null)
			{
				registryKey.SetValue(valueName, newValue);
				WriteLineColor($"Updated {valueName} in {keyPath}.", ConsoleColor.Green);
			}
			else
			{
				WriteLineColor("Failed to open registry key: " + keyPath, ConsoleColor.Red);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error updating registry: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void ClearDNSCache()
	{
		try
		{
			WriteLineColor("\n--- Clearing Traces ---", ConsoleColor.Cyan);
			WriteLineColor("Clearing DNS cache...", ConsoleColor.Yellow);
			using (Process process = Process.Start(new ProcessStartInfo
			{
				FileName = "ipconfig",
				Arguments = "/flushdns",
				Verb = "runas",
				UseShellExecute = true,
				CreateNoWindow = true
			}))
			{
				process.WaitForExit();
			}
			WriteLineColor("DNS cache cleared successfully.", ConsoleColor.Green);
		}
		catch (Exception ex)
		{
			WriteLineColor("Error clearing DNS cache: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void ChangeComputerNameRandomly()
	{
		try
		{
			WriteLineColor("\n--- Changing Computer Name ---", ConsoleColor.Cyan);
			string text = "PC-" + Guid.NewGuid().ToString("N").Substring(0, 8);
			WriteLineColor("Changing computer name to: " + text, ConsoleColor.Yellow);
			foreach (ManagementObject instance in new ManagementClass("Win32_ComputerSystem").GetInstances())
			{
				ManagementBaseObject methodParameters = instance.GetMethodParameters("Rename");
				methodParameters["Name"] = text;
				instance.InvokeMethod("Rename", methodParameters, null);
			}
			WriteLineColor("Computer name changed successfully. Please restart your PC for the change to take effect.", ConsoleColor.Green);
		}
		catch (Exception ex)
		{
			WriteLineColor("Error changing computer name: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static string GenerateRandomProductId()
	{
		Random random = new Random();
		return $"{random.Next(10000, 99999)}-{random.Next(100, 999)}-{random.Next(1000000, 9999999)}-{random.Next(10000, 99999)}";
	}

	private static void ChangeWindowsProductKey()
	{
		try
		{
			WriteLineColor("\n--- Clearing product key ---", ConsoleColor.Cyan);
			string text = GenerateRandomProductId();
			using RegistryKey registryKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", writable: true);
			if (registryKey != null)
			{
				registryKey.SetValue("ProductId", text, RegistryValueKind.String);
				WriteLineColor("Product ID changed successfully to: " + text, ConsoleColor.Green);
			}
			else
			{
				WriteLineColor("Failed to open the registry key for changing Product ID.", ConsoleColor.Red);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error changing Windows Product ID: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void RemoveProductId()
	{
		try
		{
			WriteLineColor("\n--- Clearing product key ---", ConsoleColor.Cyan);
			using RegistryKey registryKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", writable: true);
			if (registryKey != null)
			{
				registryKey.DeleteValue("ProductId", throwOnMissingValue: false);
				WriteLineColor("Product ID removed from the registry.", ConsoleColor.Green);
			}
			else
			{
				WriteLineColor("Failed to open the registry key for removing Product ID.", ConsoleColor.Red);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error removing Product ID: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void SpoofPCUUID()
	{
		try
		{
			WriteLineColor("\n--- Spoofing PC UUID (Simulated) ---", ConsoleColor.Cyan);
			string text = Guid.NewGuid().ToString();
			using RegistryKey registryKey = Registry.LocalMachine.CreateSubKey("SOFTWARE\\GhostySpoofer");
			if (registryKey != null)
			{
				registryKey.SetValue("PCUUID", text, RegistryValueKind.String);
				WriteLineColor("Spoofed PC UUID stored successfully: " + text, ConsoleColor.Green);
				WriteLineColor("Note: This does not change the BIOS/firmware UUID returned by Win32_ComputerSystemProduct.", ConsoleColor.Yellow);
			}
			else
			{
				WriteLineColor("Failed to create or open the spoof registry key.", ConsoleColor.Red);
			}
		}
		catch (Exception ex)
		{
			WriteLineColor("Error spoofing PC UUID: " + ex.Message, ConsoleColor.Red);
		}
	}

	private static void TaskDelaySpinner(int milliseconds)
	{
		DateTime now = DateTime.Now;
		int num = 0;
		char[] array = new char[4] { '|', '/', '-', '\\' };
		while ((DateTime.Now - now).TotalMilliseconds < (double)milliseconds)
		{
			Console.Write($"\r{array[num++ % array.Length]}");
			Task.Delay(100).Wait();
		}
		Console.Write("\r ");
		Console.WriteLine();
	}

	private static void PauseWithMessage(string message, int seconds)
	{
		WriteLineColor(message, ConsoleColor.Yellow);
		for (int num = seconds; num > 0; num--)
		{
			Console.Write($"\rReturning in {num} second{((num > 1) ? "s" : "")}... ");
			Task.Delay(1000).Wait();
		}
		Console.WriteLine();
	}

	private static void SmoothTransition()
	{
		Task.Delay(800).Wait();
		Console.Clear();
		DisplayBanner();
	}
}

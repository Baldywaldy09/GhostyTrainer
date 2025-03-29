using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace GhostyFullApp;

internal class Program
{
	private const string GHOSTY_FOLDER = "C:\\Ghosty";

	private const string GITHUB_URL = "https://raw.githubusercontent.com/DizcatOff/GhostyLite/refs/heads/main/external";

	[DllImport("kernel32.dll", SetLastError = true)]
	private static extern nint GetStdHandle(int nStdHandle);

	[DllImport("kernel32.dll")]
	private static extern bool GetConsoleMode(nint hConsoleHandle, out int lpMode);

	[DllImport("kernel32.dll")]
	private static extern bool SetConsoleMode(nint hConsoleHandle, int dwMode);

	private static void EnableVirtualTerminalProcessing()
	{
		nint stdHandle = GetStdHandle(-11);
		if (GetConsoleMode(stdHandle, out var lpMode))
		{
			SetConsoleMode(stdHandle, lpMode | 4);
		}
	}

	private static void Main()
	{
		SetupConsole();
		EnsureGhostyFolderExists();
		DownloadExecutables();
		PromptAndRunExecutables();
	}

	private static void SetupConsole()
	{
		EnableVirtualTerminalProcessing();
		Console.OutputEncoding = Encoding.UTF8;
		Console.Title = "Ghosty - Downloader";
		Console.Clear();
		Console.ForegroundColor = ConsoleColor.Cyan;
		Console.WriteLine("\n==============================");
		Console.WriteLine("     Ghosty Bootstrapper        ");
		Console.WriteLine("==============================\n");
		Console.ResetColor();
	}

	private static void EnsureGhostyFolderExists()
	{
		if (!Directory.Exists("C:\\Ghosty"))
		{
			Directory.CreateDirectory("C:\\Ghosty");
			Console.WriteLine("[✔] Created folder: C:\\Ghosty");
		}
	}

	private static void DownloadExecutables()
	{
		string url = "";
		string url2 = "";
		try
		{
			WebClient webClient = new WebClient();
			try
			{
				Console.Write("[ ] Fetching update URLs from GitHub... ");
				string[] array = webClient.DownloadString("https://raw.githubusercontent.com/DizcatOff/GhostyLite/refs/heads/main/external").Split(new char[2] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
				if (array.Length < 2)
				{
					throw new Exception("Not enough URLs in response");
				}
				url = array[0].Trim();
				url2 = array[1].Trim();
				Console.ForegroundColor = ConsoleColor.Green;
				Console.WriteLine("✔ (2 URLs found)");
				Console.ResetColor();
			}
			finally
			{
				((IDisposable)webClient)?.Dispose();
			}
		}
		catch (Exception ex)
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine("✘ Error: " + ex.Message);
			Console.ResetColor();
			return;
		}
		CleanupOldExecutables();
		DownloadFile(url, "Ghosty.exe");
		DownloadFile(url2, "GhostAdminDetect.exe");
	}

	private static void DownloadFile(string url, string fileName)
	{
		string fileName2 = Path.Combine("C:\\Ghosty", fileName);
		try
		{
			WebClient webClient = new WebClient();
			try
			{
				Console.Write("[ ] Downloading " + fileName + " ");
				for (int i = 0; i < 10; i++)
				{
					Console.Write(".");
					Thread.Sleep(200);
				}
				webClient.DownloadFile(url, fileName2);
				Console.ForegroundColor = ConsoleColor.Green;
				Console.WriteLine("✔");
				Console.ResetColor();
			}
			finally
			{
				((IDisposable)webClient)?.Dispose();
			}
		}
		catch (Exception ex)
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine("✘ Error: " + ex.Message);
			Console.ResetColor();
		}
	}

	private static void CleanupOldExecutables()
	{
		try
		{
			string[] files = Directory.GetFiles("C:\\Ghosty", "*.exe");
			for (int i = 0; i < files.Length; i++)
			{
				File.Delete(files[i]);
			}
			Console.WriteLine("[✔] Old executables removed");
		}
		catch (Exception ex)
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine("[!] Cleanup warning: " + ex.Message);
			Console.ResetColor();
		}
	}

	private static void PromptAndRunExecutables()
	{
		string path = Path.Combine("C:\\Ghosty", "Ghosty.exe");
		string path2 = Path.Combine("C:\\Ghosty", "GhostAdminDetect.exe");
		Console.ForegroundColor = ConsoleColor.Cyan;
		Console.WriteLine("\nLaunch Options:");
		Console.WriteLine("1) Ghosty");
		Console.WriteLine("2) Ghosty Admin Detector");
		Console.WriteLine("3) Both");
		Console.WriteLine("4) Exit");
		Console.ResetColor();
		Console.Write("\nYour choice (1-4): ");
		string text = Console.ReadKey(intercept: true).KeyChar.ToString();
		Console.WriteLine();
		switch (text)
		{
		case "1":
			RunExecutable(path, "Ghosty");
			break;
		case "2":
			RunExecutable(path2, "Admin Detector");
			break;
		case "3":
			RunExecutable(path, "Ghosty");
			Thread.Sleep(1000);
			RunExecutable(path2, "Admin Detector");
			break;
		case "4":
			Console.WriteLine("Exiting...");
			break;
		default:
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine("Invalid selection!");
			Console.ResetColor();
			break;
		}
	}

	private static void RunExecutable(string path, string name)
	{
		if (!File.Exists(path))
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine("[✘] " + name + " not found at " + path);
			Console.ResetColor();
			return;
		}
		try
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine("[⏳] Launching " + name + "...");
			Console.ResetColor();
			Process.Start(new ProcessStartInfo
			{
				FileName = path,
				UseShellExecute = true
			});
			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine("[✔] " + name + " launched successfully!");
			Console.ResetColor();
		}
		catch (Exception ex)
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine("[✘] Error launching " + name + ": " + ex.Message);
			Console.ResetColor();
		}
	}
}

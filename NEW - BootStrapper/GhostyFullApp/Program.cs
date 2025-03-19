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
		RunExecutable(DownloadLatestExecutable());
	}

	private static void SetupConsole()
	{
		EnableVirtualTerminalProcessing();
		Console.OutputEncoding = Encoding.UTF8;
		Console.Title = "Ghosty - Downloading & Running";
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

	private static string DownloadLatestExecutable()
	{
		string address = "";
		try
		{
			WebClient webClient = new WebClient();
			try
			{
				Console.Write("[ ] Fetching latest update URL... ");
				address = webClient.DownloadString("https://raw.githubusercontent.com/DizcatOff/GhostyLite/refs/heads/main/external").Trim();
				Console.ForegroundColor = ConsoleColor.Green;
				Console.WriteLine("✔");
				Console.ResetColor();
			}
			finally
			{
				((IDisposable)webClient)?.Dispose();
			}
		}
		catch
		{
		}
		CleanupOldExecutables();
		string text = Path.Combine("C:\\Ghosty", "Ghosty.exe");
		try
		{
			WebClient webClient2 = new WebClient();
			try
			{
				Console.Write("[ ] Downloading latest version ");
				for (int i = 0; i < 10; i++)
				{
					Console.Write(".");
					Thread.Sleep(200);
				}
				webClient2.DownloadFile(address, text);
				Console.ForegroundColor = ConsoleColor.Green;
				Console.WriteLine("✔");
				Console.ResetColor();
			}
			finally
			{
				((IDisposable)webClient2)?.Dispose();
			}
		}
		catch
		{
		}
		return text;
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
			Console.WriteLine("[✔] Old versions removed.");
		}
		catch
		{
		}
	}

	private static void RunExecutable(string exePath)
	{
		if (!File.Exists(exePath))
		{
			Console.WriteLine("[✘] No executable found to run.");
			return;
		}
		try
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine("[⏳] Launching Ghosty in 2 seconds...");
			Console.ResetColor();
			Thread.Sleep(2000);
			Process.Start(new ProcessStartInfo
			{
				FileName = exePath,
				UseShellExecute = true
			});
			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine("[✔] Ghosty is now running!");
			Console.ResetColor();
		}
		catch
		{
		}
	}
}

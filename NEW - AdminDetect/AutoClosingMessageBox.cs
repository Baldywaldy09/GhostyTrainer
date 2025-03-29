using System;
using System.Runtime.InteropServices;
using System.Threading;

public static class AutoClosingMessageBox
{
	private const uint WM_CLOSE = 16u;

	[DllImport("user32.dll", SetLastError = true)]
	private static extern nint FindWindow(string lpClassName, string lpWindowName);

	[DllImport("user32.dll", SetLastError = true)]
	private static extern bool PostMessage(nint hWnd, uint Msg, nint wParam, nint lParam);

	[DllImport("user32.dll", CharSet = CharSet.Auto)]
	private static extern int MessageBox(nint hWnd, string text, string caption, int options);

	public static void Show(string text, string caption, int timeout)
	{
		Thread thread = new Thread((ThreadStart)delegate
		{
			Thread.Sleep(timeout);
			nint num = FindWindow(null, caption);
			if (num != IntPtr.Zero)
			{
				PostMessage(num, 16u, IntPtr.Zero, IntPtr.Zero);
			}
		});
		thread.IsBackground = true;
		thread.Start();
		MessageBox(IntPtr.Zero, text, caption, 0);
	}
}

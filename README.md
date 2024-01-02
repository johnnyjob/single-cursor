# Single Cursor
Fixes "Hide cursor as you type" on all versions of Windows.

This app uses Windows global hooks to determine if current cursor is "I-Beam" cursor and then hides it. Since this app doesn't try to intercept Windows messages, it works well with almost any app (including *Microsoft Visual Studio* and *Microsoft Office*).

## Limitations
This app detects only system "I-Beam" cursor. If a 3rd party app displays custom "I-Beam" cursor, this app won't be able to detect that.

## Known Issues
1. Multiple instances of this app are not guarantteed to run correctly, but the app doesn't try to check that.
2. UI is still under development, there's no "Run at system start" option. Also, there's no Exit button - use Task Manager :)
3. Please note that notification icons specified with a GUID are protected against spoofing by validating
that only a single application registers them. This registration is performed the first time you 
call Shell_NotifyIcon(NIM_ADD, ...), and the full pathname of the calling application is stored. If you
later move your binary to a different location, the system will not allow the icon to be added again.
Please see the MSDN documentation for Shell_NotifyIcon for more information.

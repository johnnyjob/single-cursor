#include "MouseHook.h"
#include <memory>

HHOOK MouseHook::_hook = NULL;
bool MouseHook::_isCursorHidden = false;
HCURSOR MouseHook::_defaultIBeamCursor = NULL;

void MouseHook::SetHook()
{
    if (_hook != NULL)
    {
        return; // TODO: log
    }

    // Save original cursor.
    _defaultIBeamCursor = CopyCursor(LoadCursor(NULL, IDC_IBEAM));

    // Set hook.
    _hook = SetWindowsHookEx(WH_MOUSE_LL, &HookProc, NULL, 0);

    if (_hook == NULL)
    {
        DWORD error = GetLastError();
        // TODO: write to log and throw exception.
    }
}

void MouseHook::ReleaseHook()
{
    if (_hook == NULL)
    {
        return; // TODO: log
    }

    BOOL result = UnhookWindowsHookEx(_hook);
    if (result)
    {
        _hook = NULL;
    }
    else
    {
        DWORD error = GetLastError();
        // TODO: write to log and throw exception.
    }
}

LRESULT CALLBACK MouseHook::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CHAR szBuf[128];
    CHAR szMsg[16];
    HDC hdc;
    static int c = 0;
    size_t cch;
    HRESULT hResult;

    if (nCode < 0)  // do not process the message 
    {
        return CallNextHookEx(_hook, nCode, wParam, lParam);
    }

    switch (wParam)
    {
    case WM_LBUTTONUP:
        if (IsCaretCursor())
        {
            // TODO: write to log
            //Debug.WriteLine("Current cursor is CARET, now hide it!");
            //MSLLHOOKSTRUCT* pHookStruct = (MSLLHOOKSTRUCT*)lParam;

            //ShowCursor(false);
            HideCursor();
        }
        break;

    case WM_MOUSEMOVE:
        ShowCursor();
        break;
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

bool MouseHook::IsCaretCursor()
{
    //HCURSOR currentCursor = GetCursor();
    CURSORINFO cursorInfo;
    cursorInfo.cbSize = sizeof(CURSORINFO);

    GetCursorInfo(&cursorInfo);

    ICONINFOEX iconInfo;
    iconInfo.cbSize = sizeof(ICONINFOEX);
    GetIconInfoEx(cursorInfo.hCursor, &iconInfo);
    
        //HCURSOR iBeamCursor = LoadCursor(NULL, IDC_IBEAM);
    
        return iconInfo.wResID == OCR_IBEAM;// currentCursor == iBeamCursor;
}

void MouseHook::HideCursor()
{
    SetSystemCursor(CreateEmptyCursor(), OCR_IBEAM);
    _isCursorHidden = true;
}

void MouseHook::ShowCursor()
{
    if (_isCursorHidden)
    {
        HCURSOR copy = CopyCursor(_defaultIBeamCursor);
        SetSystemCursor(copy, OCR_IBEAM);
        _isCursorHidden = false;
    }
}

HCURSOR MouseHook::CreateEmptyCursor()
{
    int cw = GetSystemMetrics(SM_CXCURSOR);
    int ch = GetSystemMetrics(SM_CYCURSOR);
    int maskCursorLen = ch * cw / 8;

    auto andMaskCursor = std::make_unique<BYTE[]>(maskCursorLen);
    for (int i = 0; i < maskCursorLen; i++)
    {
        andMaskCursor[i] = 0xFF;
    }

    int xorMaskCursorLen = ch * cw / 8;
    auto xorMaskCursor = std::make_unique<BYTE[]>(maskCursorLen);
    for (int i = 0; i < maskCursorLen; i++)
    {
        xorMaskCursor[i] = 0;
    }

    HCURSOR cursor = CreateCursor(
        NULL,                   // App instance.
        0,                      // Horizontal position of hot spot.
        0,                      // Vertical position of hot spot.
        cw,                     // Cursor width.
        ch,                     // Cursor height.
        andMaskCursor.get(),    // AND mask.
        xorMaskCursor.get());   // XOR mask.

    return cursor;
}


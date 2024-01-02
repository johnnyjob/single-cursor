#pragma once

#include "framework.h"

class MouseHook
{
private:
    static HHOOK _hook;
    static bool _isCursorHidden;
    static HCURSOR _defaultIBeamCursor;
    
public:
    static void SetHook();
    static void ReleaseHook();

private:
    MouseHook();
    static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static bool IsCaretCursor();
    static void HideCursor();
    static void ShowCursor();
    static HCURSOR CreateEmptyCursor();
};


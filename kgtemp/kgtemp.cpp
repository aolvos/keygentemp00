#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
#include "ufmod.h"

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
#define win_width 540
#define win_height 350

#define class_name TEXT("STGX!")
#define app_name TEXT("Keygen template #00")
const wchar_t* strings[] = { L"STGX!", L"Gen!", L"Copy", L"Exit", L"♫", L"X" ,L"code/gfx/mus: aolvos | 2020 - 2021 | 1 year of fun!" };
const int klen = 20;
const int ulen = 17;

#define rcnum 4
int xres[rcnum], yres[rcnum];

#define USRBOX_ID 1000

unsigned char* images[rcnum];
HICON hIcon;
HFONT hFont[3];
HWND userbox, keybox;
int xc = 0, yc = 0;
int btnstat[] = { 0, 0, 0, 0, 0 };
int musstat = 1;

unsigned char* load_bin_image(int res)
{
    int cpos, x = 0, y = 0, rtime = 0, cnum = 0;
    unsigned char r, g, b, a;

    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(res + 1), RT_RCDATA);
    HGLOBAL hMemory = LoadResource(hModule, hResource);
    DWORD dwSize = SizeofResource(hModule, hResource);
    LPVOID lpAddress = LockResource(hMemory);
    unsigned char* bytes = (unsigned char*)malloc(dwSize + 1);
    memcpy(bytes, lpAddress, dwSize);

    xres[res] = bytes[0] + bytes[1] * 256;
    yres[res] = bytes[2] + bytes[3] * 256;
    unsigned char* buf = (unsigned char*)malloc(xres[res] * yres[res] * 4 + 1);
    cpos = bytes[4] * 3 + 5;
    for (int i = cpos; y < yres[res]; i++)
    {
        a = 255;
        if (bytes[i] == 0)
        {
            i++;
            rtime = bytes[i] + bytes[i + 1] * 256 + bytes[i + 2] * 65536;
            i += 3;
            cnum = bytes[i] - 1;
            r = bytes[cnum * 3 + 5];
            g = bytes[cnum * 3 + 6];
            b = bytes[cnum * 3 + 7];
            if ((r == 0) && (g == 255) && (b == 255))
            {
                a = 0;
            }
            while (rtime > 0)
            {
                buf[(y * xres[res] * 4) + (x * 4)] = b;
                buf[(y * xres[res] * 4) + (x * 4) + 1] = g;
                buf[(y * xres[res] * 4) + (x * 4) + 2] = r;
                buf[(y * xres[res] * 4) + (x * 4) + 3] = a;
                rtime--;
                x++;
                if (x == xres[res])
                {
                    y++;
                    x = 0;
                }
            }
        }
        else
        {
            cnum = bytes[i] - 1;
            r = bytes[cnum * 3 + 5];
            g = bytes[cnum * 3 + 6];
            b = bytes[cnum * 3 + 7];
            if ((r == 0) && (g == 255) && (b == 255))
            {
                a = 0;
            }
            buf[(y * xres[res] * 4) + (x * 4)] = b;
            buf[(y * xres[res] * 4) + (x * 4) + 1] = g;
            buf[(y * xres[res] * 4) + (x * 4) + 2] = r;
            buf[(y * xres[res] * 4) + (x * 4) + 3] = a;
            x++;
            if (x == xres[res])
            {
                y++;
                x = 0;
            }
        }
    }
    free(bytes);
    return buf;
}

const wchar_t* gen()
{
    wchar_t* usrname = (wchar_t*)malloc(sizeof(wchar_t) * ulen + 1);
    GetWindowText(userbox, usrname, ulen);
    wchar_t* key = (wchar_t*)malloc(sizeof(wchar_t) * klen + 1);

    int len = wcslen(usrname);
    if (len < 4) wcscpy(key, L"ERROR!");
    else
    {
        /* random algorithm */
        wchar_t* hex = (wchar_t*)malloc(sizeof(wchar_t) * 5 + 1);
        for (int i = 0; i < 4; i++)
        {
            if (i) wcscat(key, L"-");
            int block = 0 + (rand() % 65535);
            swprintf(hex, 5, L"%04X", block);
            wcscpy(key + i * 5, hex);
        }
        free(hex);
    }

    free(usrname);
    return key;
}

void toclipboard()
{
    wchar_t* key = (wchar_t*)malloc(klen * sizeof(wchar_t) + 1);
    GetWindowText(keybox, key, klen);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, klen * sizeof(wchar_t) + 1);
    memcpy(GlobalLock(hMem), key, klen * sizeof(wchar_t));
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
    free(key);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hInstance;
    wincl.lpszClassName = class_name;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_ENABLE;
    wincl.cbSize = sizeof(WNDCLASSEX);

    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH)RGB(0,0,0);

    if (!RegisterClassEx(&wincl))
        return 0;

    int swidth = GetSystemMetrics(SM_CXSCREEN), sheight = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(
        0,
        class_name,
        app_name,
        WS_POPUP,
        (swidth - win_width) / 2,
        (sheight - win_height) / 2,
        win_width,
        win_height,
        HWND_DESKTOP,
        NULL,
        hInstance,
        NULL
    );

    hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(icon));
    SetClassLong(hwnd, GCL_HICON, (LONG)LoadIcon(hInstance, MAKEINTRESOURCE(icon)));
    SetClassLong(hwnd, GCL_HICONSM, (LONG)LoadIcon(hInstance, MAKEINTRESOURCE(icon)));
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    for (int i = 0; i < rcnum; i++)
        images[i] = load_bin_image(i);

    hFont[0] = CreateFont(16, 0, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Courier New");
    hFont[1] = CreateFont(20, 0, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Wide Latin");
    hFont[2] = CreateFont(14, 0, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Wide Latin");

    srand(time(0));
    userbox = CreateWindowEx(WS_EX_TRANSPARENT, L"edit", strings[0], WS_VISIBLE | WS_CHILD | ES_CENTER, 32, 238, 475, 25, hwnd, (HMENU)USRBOX_ID, hInstance, NULL);
    keybox = CreateWindowEx(WS_EX_TRANSPARENT, L"edit", gen(), WS_VISIBLE | WS_CHILD | ES_CENTER | ES_READONLY | ES_UPPERCASE, 32, 277, 475, 25, hwnd, NULL, hInstance, NULL);

    SendMessage(userbox, WM_SETFONT, (WPARAM)hFont[1], TRUE);
    SendMessage(userbox, EM_SETLIMITTEXT, ulen - 1, TRUE);

    SendMessage(keybox, WM_SETFONT, (WPARAM)hFont[1], TRUE);

    uFMOD_PlaySong(MAKEINTRESOURCE(musi), 0, XM_RESOURCE);
    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    uFMOD_StopSong();

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC)wParam, RGB(255, 160, 202));
        SetBkColor((HDC)wParam, RGB(0, 0, 0));
        return LRESULT(HBRUSH(GetStockObject(NULL_BRUSH)));
        break;
    case WM_PAINT:
        HDC hdc;
        HDC hdcmem;
        HBITMAP hdcmembmp;
        HANDLE hold;
        PAINTSTRUCT ps;
        int num;

        hdc = BeginPaint(hwnd, &ps);
        hdcmem = CreateCompatibleDC(hdc);
        hdcmembmp = CreateCompatibleBitmap(hdc, win_width, win_height);
        hold = SelectObject(hdcmem, hdcmembmp);

        num = 1;
        BITMAPINFO info;
        ZeroMemory(&info, sizeof(BITMAPINFO));
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biWidth = xres[num];
        info.bmiHeader.biHeight = -yres[num];
        info.bmiHeader.biPlanes = num;
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biSizeImage = xres[num] * yres[num] * 4;
        info.bmiHeader.biCompression = BI_RGB;
        StretchDIBits(hdcmem, 0, 0, xres[num], yres[num], 0, 0, xres[num], yres[num], images[num], &info, DIB_RGB_COLORS, SRCCOPY);

        num--;
        info.bmiHeader.biWidth = xres[num];
        info.bmiHeader.biHeight = -yres[num];
        info.bmiHeader.biSizeImage = xres[num] * yres[num] * 4;
        StretchDIBits(hdcmem, 12, 22, xres[num], yres[num], 0, 0, xres[num], yres[num], images[num], &info, DIB_RGB_COLORS, SRCCOPY);

        RECT rect;
        SetBkMode(hdcmem, TRANSPARENT);
        num = 2;
        for (int i = 0; i < 3; i++)
        {
            info.bmiHeader.biWidth = xres[num];
            if (!btnstat[i + 2]) info.bmiHeader.biHeight = -yres[num];
            else info.bmiHeader.biHeight = yres[num];
            info.bmiHeader.biSizeImage = xres[num] * yres[num] * 4;
            StretchDIBits(hdcmem, 30 + i * (xres[num] + 7), 310, xres[num], yres[num], 0, 0, xres[num], yres[num], images[num], &info, DIB_RGB_COLORS, SRCCOPY);

            int s = 0;
            SelectObject(hdcmem, hFont[1]);
            if (!btnstat[i + 2]) SetTextColor(hdcmem, RGB(255, 255, 255));
            else
            {
                s = 1;
                SetTextColor(hdcmem, RGB(82, 27, 51));
            }
            SetRect(&rect, 56 + i * (xres[num] + 7), 314 + s, -1, -1);
            DrawText(hdcmem, strings[i + 1], wcslen(strings[i + 1]), &rect, DT_SINGLELINE | DT_NOCLIP);
        }

        num = 3;
        for (int i = 0; i < 2; i++)
        {
            info.bmiHeader.biWidth = xres[num];
            if (!btnstat[i]) info.bmiHeader.biHeight = -yres[num];
            else info.bmiHeader.biHeight = yres[num];
            info.bmiHeader.biSizeImage = xres[num] * yres[num] * 4;
            StretchDIBits(hdcmem, 484 + i * (xres[num] + 2), 4, xres[num], yres[num], 0, 0, xres[num], yres[num], images[num], &info, DIB_RGB_COLORS, SRCCOPY);

            SetRect(&rect, 488 + i * (xres[num] + 2) + 4 * (1 - i), 4, -1, -1);
            SelectObject(hdcmem, hFont[2]);
            if (!btnstat[i]) SetTextColor(hdcmem, RGB(255, 255, 255));
            else SetTextColor(hdcmem, RGB(82, 27, 51));
            DrawText(hdcmem, strings[i + 4], wcslen(strings[i + 4]), &rect, DT_SINGLELINE | DT_NOCLIP);
        }

        SetRect(&rect, 64, 208, -1, -1);
        SelectObject(hdcmem, hFont[0]);
        SetTextColor(hdcmem, RGB(80, 80, 80));
        DrawText(hdcmem, strings[6], wcslen(strings[6]), &rect, DT_SINGLELINE | DT_NOCLIP);
        SetBkMode(hdcmem, OPAQUE);

        DrawIconEx(hdcmem, 7, 5, hIcon, 16, 16, 0, NULL, DI_NORMAL);

        SetRect(&rect, 30, 5, -1, -1);
        SelectObject(hdcmem, hFont[0]);
        SetTextColor(hdcmem, RGB(255, 255, 255));
        SetBkColor(hdcmem, RGB(0, 0, 0));
        DrawText(hdcmem, app_name, wcslen(app_name), &rect, DT_SINGLELINE | DT_NOCLIP);

        BitBlt(hdc, 0, 0, win_width, win_height, hdcmem, 0, 0, SRCCOPY);
        SelectObject(hdcmem, hold);
        DeleteObject(hdcmembmp);
        DeleteObject(hold);
        DeleteDC(hdcmem);
        DeleteDC(hdc);
        EndPaint(hwnd, &ps);
        break;
    case WM_LBUTTONDOWN:
        xc = LOWORD(lParam);
        yc = HIWORD(lParam);
        InvalidateRect(hwnd, NULL, true);
        if ((xc >= 484) && (xc <= 508) && (yc >= 4) && (yc <= 18))
        {
            btnstat[0] = 1;
            //RECT rect;
            //SetRect(&rect, 484, 4, 508, 18);
            //InvalidateRect(hwnd, NULL, true);
        }
        else if ((xc >= 511) && (xc <= 535) && (yc >= 4) && (yc <= 18))
            btnstat[1] = 1;
        else if ((xc >= 30) && (xc <= 149) && (yc >= 310) && (yc <= 339))
            btnstat[2] = 1;
        else if ((xc >= 156) && (xc <= 275) && (yc >= 310) && (yc <= 339))
            btnstat[3] = 1;
        else if ((xc >= 282) && (xc <= 401) && (yc >= 310) && (yc <= 339))
            btnstat[4] = 1;
        else if (yc <= 220) SetCapture(hwnd);
        break;
    case WM_LBUTTONUP:
        InvalidateRect(hwnd, NULL, true);
        if (btnstat[0] == 1)
        {
            if (musstat)
            {
                uFMOD_Pause();
                musstat = 0;
            }
            else
            {
                uFMOD_Resume();
                musstat = 1;
            }
        }
        else if (btnstat[1] == 1)
            PostQuitMessage(0);
        else if (btnstat[2] == 1)
            SetWindowText(keybox, gen());
        else if (btnstat[3] == 1)
            toclipboard();
        else if (btnstat[4] == 1)
            PostQuitMessage(0);
        for (int i = 0; i < 5; i++) btnstat[i] = 0;
        ReleaseCapture();
        break;
    case WM_MOUSEMOVE:
        if (GetCapture() == hwnd)
        {
            RECT rcwin;
            GetWindowRect(hwnd, &rcwin);
            int xm = LOWORD(lParam);
            int ym = HIWORD(lParam);
            int xwin = rcwin.left - xc + xm;
            int ywin = rcwin.top - yc + ym;
            SetWindowPos(hwnd, NULL, xwin, ywin, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
            case USRBOX_ID:
                if (HIWORD(wParam) == EN_CHANGE)
                {
                    InvalidateRect(hwnd, NULL, true);
                    SetWindowText(keybox, gen());
                }
                break;
            break;
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
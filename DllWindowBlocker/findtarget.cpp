#include "pch.h"

static HWND g_window = NULL;

static TCHAR g_targetTitle[256];
static DWORD g_targetPid;

BOOL CALLBACK enum_windows_cb(HWND window, const LPARAM lParam)
{
    TCHAR title[256];

    if (GetWindowText(window, title, 255 /* \0 */))
    {
        //FILE* file = NULL;
        //fopen_s(&file, "SendMe.txt", "a");
        //if (file)
        //{
        //    fwrite(title, sizeof(TCHAR), _tcslen(title), file);
        //    fwrite("\n", 1, 1, file);
        //    fclose(file);
        //}

        DWORD pid;

        GetWindowThreadProcessId(window, &pid);

        if (_tcscmp(title, g_targetTitle) == 0 || 
            pid == g_targetPid)
        {
            g_window = window;

            return FALSE;
        }
    }

    return TRUE;
}

_tstring ParseKey(_tstring& line)
{
    _tstring key(_T(""));

    size_t operand = line.find(_T("="));

    key = line.substr(0, operand);

    size_t left = key.find_first_not_of(_T(' '));
    size_t right = key.find_last_not_of(_T(' '));

    return key.substr(left, right - left + 1);
}

_tstring ParseValue(_tstring& line)
{
    _tstring value(_T(""));

    size_t left = line.find_first_of(_T('"'));
    size_t right = line.find_last_of(_T('"'));

    if (left != right)
    {
        value = line.substr(left + 1, right - left - 1);
    }

    return value;
}

void ParseConfig()
{
    _tifstream config(_T("window.ini"));
    config.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<TCHAR>));

    if (config.is_open())
    {
        _tstring line;
        _tstring key, value;

        while (config)
        {
            std::getline(config, line);

            if (!config)
            {
                break;
            }

            key = ParseKey(line);
            value = ParseValue(line);

            if (value == _T("null") || value == _T("NULL"))
            {
                continue;
            }

            if (key == _T("title"))
            {
                _tcscat_s(g_targetTitle, value.c_str());
            }
            else if (key == _T("pid"))
            {
                g_targetPid = (DWORD)std::stoi(value);
            }
        }

        config.close();
    }
}

HWND GetTarget()
{
    ParseConfig();

    // Find hwnd target window
    EnumWindows(&enum_windows_cb, 0);

    return g_window;
}

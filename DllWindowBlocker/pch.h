// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include "framework.h"

#include <fstream>
#include <codecvt>
#include <string>

#include "findtarget.hpp"

#include <strsafe.h>
#include <memory.h>
#include <tchar.h>

#ifdef _UNICODE
#define _tofstream std::wofstream
#define _tifstream std::wifstream

#define _tstring std::wstring
#else
#define _tofstream std::ofstream
#define _tifstream std::ifstream

#define _tstring std::string
#endif // _UNICODE

extern "C" __declspec(dllexport) LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);

extern "C" __declspec(dllexport) LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif //PCH_H

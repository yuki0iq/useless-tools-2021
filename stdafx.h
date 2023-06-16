// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ��������� ����� ������������ ���������� �� ���������� Windows
// ����� ���������� Windows:
#include <windows.h>

// ����� ���������� C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#ifdef UNICODE
#define tstring wstring
#define to_tstring to_wstring
#define tistringstream wistringstream
#else
#define tstring string
#define to_tstring to_string
#define tistringstream istringstream
#endif


// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������

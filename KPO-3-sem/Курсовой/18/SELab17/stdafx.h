﻿#ifndef STDAFX_H
#define STDAFX_H
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <cwchar>
#include <TCHAR.H>
#include <time.h>
#include <stack>
#include <vector>

#include "Out.h"
#include "Error.h"		//обработка ошибок
#include "Parm.h"		//обработка параметров
#include "Log.h"		//работа с протоколом
#include "In.h"			//ввод исходного файла
#include "FST.h"
#include "IT.h"
#include "LT.h"
#include "SVV.h"
#include "LexerAutomata.h"

#endif // !STDAFX_H
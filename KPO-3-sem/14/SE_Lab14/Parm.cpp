﻿#include "stdafx.h"
#include "Parm.h"
#include <tchar.h>
#include "Error.h"

namespace Parm {

	//Параметры: argc – количество параметров(int, >= 1),
	//argv – массив указателей на нуль - терминальные строки со
	//значениями параметров, (_TCHAR* – указатель на строку wchar_t)

	PARM getparm(int argc, _TCHAR* argv[]) {

		PARM parm;

		bool in = false, out = false, log = false;

		for (int i = 1; i < argc; i++) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);
			if (wcsstr(argv[i], PARM_IN)) {
				wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));
				in = true;
			}
			if (wcsstr(argv[i], PARM_OUT)) {
				wcscpy_s(parm.out, argv[i] + wcslen(PARM_OUT));
				out = true;
			}
			if (wcsstr(argv[i], PARM_LOG)) {
				wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));
				log = true;
			}
		}

		if (!in)
			throw ERROR_THROW(100);

		if (!out) {
			wcscpy_s(parm.out, parm.in);
			wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);
		}
		if (!log) {
			wcscpy_s(parm.log, parm.in);
			wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
		}
		return parm;
	}
}
#include "stdafx.h"
#include "Parm.h"
#include <tchar.h>
#include "Error.h"

namespace Parm {

	PARM getparm(int argc, _TCHAR* argv[]) {

		PARM parm;

		bool in = false, out = false, log = false;

		for (int i = 1; i < argc; i++) {
			if (wcsstr(argv[i], PARM_IN)) {
				parm.in = argv[i] + wcslen(PARM_IN);
				in = true;
			}
			if (wcsstr(argv[i], PARM_OUT)) {
				parm.out = argv[i] + wcslen(PARM_OUT);
				out = true;
			}
			if (wcsstr(argv[i], PARM_LOG)) {
				parm.log = argv[i] + wcslen(PARM_LOG);
				log = true;
			}
		}

		if (!in)
			throw ERROR_THROW(100);

		if (!out) {
			parm.out = parm.in + PARM_OUT_DEFAULT_EXT;
		}
		if (!log) {
			parm.log = parm.in + PARM_LOG_DEFAULT_EXT;
		}

		if (parm.in == parm.log) {
			throw (L"Параметры -in и -log не могут иметь одинаковых значений (было передано " + parm.in + L")");
		}
		if (parm.in == parm.out) {
			throw (L"Параметры -in и -out не могут иметь одинаковых значений (было передано " + parm.in + L")");
		}
		if (parm.out == parm.log) {
			throw (L"Параметры -out и -log не могут иметь одинаковых значений (было передано " + parm.out + L")");
		}

		return parm;
	}
}

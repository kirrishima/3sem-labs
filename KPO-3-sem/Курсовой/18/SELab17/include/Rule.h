#pragma once
#include "Error.h"
#include "GRB.h"

#define GRB_ERROR_SERIES 600
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'),				//стартовый символ, дно стека
		9,											//количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0,			//Неверная структура программы
			3,		//S →	m{NrE;};|tfi(F){NrE;};S|m{NrE;};S|tfi(F){NrE;};|i{NrE;}
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(12 , TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'),
				NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(7, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,			//Операторы программы
			10,		//N	→dti;| rE;| i = E; | dtfi(F);|dti;N|rE;N|i=E;N|dtfi(F);N|pE;|pE;N
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('p'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(6, TS('p'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(2, TS('?'), NS('Y')),
			Rule::Chain(3, TS('?'), NS('Y'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,			//Выражение
			9,		//E →i|l|(E)|i(W)|iM|lM|(E)M|i(W)M|
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(NS('C'), GRB_ERROR_SERIES + 5,			//Выражение
			1,		//E →i|l|(E)|i(W)|iM|lM|(E)M|i(W)M|
			Rule::Chain(5, TS('('), NS('E'), TS('c'), NS('E'), TS(')'))),

		Rule(NS('Y'), GRB_ERROR_SERIES + 6,			//Выражение
			2,		//E →i|l|(E)|i(W)|iM|lM|(E)M|i(W)M|
			Rule::Chain(4, NS('C'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(5, NS('C'), TS('{'), NS('N'), TS('}'), NS('X'))
		),

		Rule(NS('X'), GRB_ERROR_SERIES + 7,			//Выражение
			2,		//E →i|l|(E)|i(W)|iM|lM|(E)M|i(W)M|
			Rule::Chain(4, TS(':'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(3, TS(':'), TS('?'), TS('Y'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 3,			//Подвыражение
			4,		//W	→i|l|i,W|l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4,			//Параметры функции
			2,		//F	→ti|ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 2,			//Выражение
			2,		//M →vE|vEM
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		)
	);
}

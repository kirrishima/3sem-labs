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
		6,											//количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0,			//Неверная структура программы
			1,		//S →	m{NrE;};|tfi(F){NrE;};S|m{NrE;};S|tfi(F){NrE;};|i{NrE;}
			Rule::Chain(3, TS('{'), NS('N'), TS('}'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,			//Операторы программы
			8,		//N	→dti;| rE;| i = E; | dtfi(F);|dti;N|rE;N|i=E;N|dtfi(F);N|pE;|pE;N
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('p'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('p'), TS('('), NS('E'), TS(')'), TS(';'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,			//Выражение
			5,		//E →i|l|(E)|i(W)|iM|lM|(E)M|i(W)M|
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 3,			//Подвыражение
			2,		//W	→i|l|i,W|l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 2,			//Выражение
			2,		//M →vE|vEM
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		)
	);
}

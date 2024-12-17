#pragma once
#include "error.h"
#include "GRB.h"

#define GRB_ERROR_SERIES 600
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'),				//стартовый символ, дно стека
		10,											//количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0,			//Неверная структура программы
			6,		//S →	m{NrE;};|tfi(F){NrE;};S|m{NrE;};S|tfi(F){NrE;};|i{NrE;}
			Rule::Chain(12, TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),  // S → ti(F){NrE;}S
			Rule::Chain(11, TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),          // S → ti(F){rE;}S
			Rule::Chain(11, TS('t'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),  // S → ti(F){NrE;}S
			Rule::Chain(10, TS('t'), TS('i'), TS('('), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),          // S → ti(F){rE;}S
			Rule::Chain(7, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),                                            // S → m{NrE;}
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))                                                                        // S → m{N}
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,         // Операторы программы
			22, // N → ti;N | rE;N | i=E;N | ti=E;N | p(E);N | ?Y;N | ti; | rE; | i=E; | ti=E; | p(E); | ?Y;
			Rule::Chain(6, TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')), // N → ti=E;N
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),                   // N → ti;N
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),          // N → i=E;N
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),                   // N → rE;N
			Rule::Chain(6, TS('p'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')), // N → p(E);N
			Rule::Chain(8, TS('p'), TS('('), NS('E'), TS('c'), NS('E'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(4, TS('w'), NS('E'), TS(';'), NS('N')), // N → p(E);N
			Rule::Chain(6, TS('w'), NS('E'), TS('c'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('?'), NS('Y'), NS('N')),                            // N → ?Y;N
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('t'), TS('i'), TS('='), NS('E'), TS(';')),          // N → ti=E;
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),                            // N → ti;
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),                   // N → i=E;
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),                            // N → rE;
			Rule::Chain(5, TS('p'), TS('('), NS('E'), TS(')'), TS(';')),          // N → p(E);
			Rule::Chain(7, TS('p'), TS('('), NS('E'), TS('c'), NS('E'), TS(')'), TS(';')),          // N → p(E);
			Rule::Chain(3, TS('w'), NS('E'), TS(';')), // N → p(E);N
			Rule::Chain(5, TS('w'), NS('E'), TS('c'), NS('E'), TS(';')),
			Rule::Chain(2, TS('?'), NS('Y')),                                     // N → ?Y;
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), TS(';'))// N → E;
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 2,			// Выражение
			10, // E → i(W)M | (E)M | iM | lM | i(W) | i | l | (E)
			Rule::Chain(1, TS('l')),                                      // E → l
			Rule::Chain(1, TS('i')),                                      // E → i
			Rule::Chain(2, TS('i'), NS('M')),                             // E → iM
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),  // E → i(W)M
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),            // E → i(W)// E → lM
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),  // E → i(W)M
			Rule::Chain(3, TS('i'), TS('('), TS(')')),            // E → i(W)// E → lM
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),           // E → (E)M
			Rule::Chain(3, TS('('), NS('E'), TS(')'))                     // E → (E)
		),
		/*i(E)*/
		Rule(NS('A'), GRB_ERROR_SERIES + 9,			// Выражение
			4,
			Rule::Chain(1, TS('l')),                                      // E → l
			Rule::Chain(1, TS('i')),                                      // E → i
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),  // E → i(W 
			Rule::Chain(3, TS('i'), TS('('), TS(')'))  // E → i(W 
		),

		Rule(NS('B'), GRB_ERROR_SERIES + 10,			// Выражение
			1,
			Rule::Chain(6, TS('s'), TS('('), NS('A'), TS(','), NS('A'), TS(')'))
		),

		Rule(NS('Y'), GRB_ERROR_SERIES + 6,			// Условное выражение
			4, // Y → (E c E) {N} X | (E c E) {N}
			Rule::Chain(9, TS('('), NS('E'), TS('c'), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('X')), // Y → (E c E) {N} X
			Rule::Chain(8, TS('('), NS('E'), TS('c'), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),          // Y → (E c E) {N}
			Rule::Chain(9, TS('('), NS('B'), TS('c'), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('X')), // Y → (E c E) {N} X
			Rule::Chain(8, TS('('), NS('B'), TS('c'), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'))          // Y → (E c E) {N}
		),

		Rule(NS('X'), GRB_ERROR_SERIES + 7,			// Альтернативный блок
			2, // X → : {N} | : ?Y
			Rule::Chain(4, TS(':'), TS('{'), NS('N'), TS('}')),          // X → : {N}
			Rule::Chain(3, TS(':'), TS('?'), NS('Y'))                   // X → : ?Y
		),
		//Rule(TS(';'), GRB_ERROR_SERIES + 9,			// Альтернативный блок
		//	1, // X → : {N} | : ?Y
		//	Rule::Chain(1, TS(';'))                 // X → : ?Y
		//),

		Rule(NS('W'), GRB_ERROR_SERIES + 3,			// Подвыражение
			4, // W → i,W | l,W | i | l
			Rule::Chain(3, TS('i'), TS(','), NS('W')),                  // W → i,W
			Rule::Chain(3, TS('l'), TS(','), NS('W')),                  // W → l,W
			Rule::Chain(1, TS('i')),                                    // W → i
			Rule::Chain(1, TS('l'))                                     // W → l
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 4,			// Параметры функции
			2, // F → ti,F | ti
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')),         // F → ti,F
			Rule::Chain(2, TS('t'), TS('i'))                            // F → ti
		),

		Rule(NS('M'), GRB_ERROR_SERIES + 8,			// Модификаторы выражений
			2, // M → vE M | vE
			Rule::Chain(3, TS('v'), NS('E'), NS('M')),                 // M → vE M
			Rule::Chain(2, TS('v'), NS('E'))                           // M → vE
		)
	);
}

#undef GRB_ERROR_SERIES 
#undef NS(n)
#undef TS(n)
#undef ISNS(n)
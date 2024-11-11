#include "stdafx.h"
#include "MFST.h"

namespace MFST
{
	// Конструктор по умолчанию для состояния автомата (инициализация полей)
	MfstState::MfstState()
	{
		lenta_position = 0;   // начальная позиция на ленте
		nrule = -1;           // правило не выбрано
		nrulechain = -1;      // цепочка в правиле не выбрана
	}

	// Конструктор состояния автомата с указанием позиции, стека и цепочки
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;  // устанавливает текущую позицию на ленте
		st = pst;                    // сохраняет стек
		nrulechain = pnrulechain;    // сохраняет номер цепочки
	}

	// Конструктор состояния с позицией, стеком, правилом и цепочкой
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;  // текущая позиция на ленте
		st = pst;                    // текущий стек
		nrule = pnrule;              // выбранное правило
		nrulechain = pnrulechain;    // текущая цепочка
	}

	// Конструктор диагностики по умолчанию (инициализация полей)
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;     // позиция на ленте (-1 — неинициализировано)
		rc_step = SURPRISE;      // код шага (SURPRISE — неожиданный результат)
		nrule = -1;              // правило (-1 — не выбрано)
		nrule_chain = -1;        // цепочка правила (-1 — не выбрана)
	}

	// Конструктор диагностики с параметрами позиции, шага, правила и цепочки
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;  // текущая позиция на ленте
		rc_step = prc_step;                // код шага (например, NS_OK, TS_OK и т.д.)
		nrule = pnrule;                    // номер правила
		nrule_chain = pnrule_chain;        // номер цепочки правила
	}

	// Конструктор автомата по умолчанию (инициализация ленты и её позиции)
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; };

	// Основной конструктор автомата, принимает таблицу лексем и грамматику
	Mfst::Mfst(LT::LexTable& lextable, GRB::Greibach pgrebach)
	{
		grebach = pgrebach;                   // установка грамматики
		lex = lextable;                       // установка таблицы лексем
		lenta = new short[lenta_size = lex.size];  // инициализация ленты

		// Перекодировка лексем из таблицы в формат алфавита Грейбаха
		for (int k = 0; k < lex.size; k++)
			lenta[k] = TS(lex.table[k].lexema[0]);

		lenta_position = 0;                   // начальная позиция на ленте
		st.push(grebach.stbottomT);           // добавление дна стека
		st.push(grebach.startN);              // добавление стартового символа грамматики
		nrulechain = -1;                      // текущая цепочка
	}

	// Метод для выполнения одного шага автомата
	Mfst::RC_STEP Mfst::step()
	{
		RC_STEP rc = SURPRISE;                // код шага (SURPRISE — неожиданный результат)
		if (lenta_position < lenta_size)      // проверка конца ленты
		{
			if (ISNS(st.top()))               // если вершина стека — нетерминал
			{
				GRB::Rule rule;               // правило для текущего нетерминала
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0) // попытка получить правило
				{
					GRB::Rule::Chain chain;   // цепочка, соответствующая правилу
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) // поиск цепочки
					{
						MFST_TRACE1           // трассировка
							savestate();          // сохранение состояния
						st.pop();             // удаление нетерминала
						push_chain(chain);    // добавление цепочки в стек
						rc = NS_OK;           // успешный шаг
						MFST_TRACE2           // трассировка
					}
					else // цепочка не найдена
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")  // трассировка
							savediagnosis(NS_NORULECHAIN);            // диагностика
						rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;  // восстановление состояния
					}
				}
				else rc = NS_ERROR;           // ошибка правила
			}
			else if ((st.top() == lenta[lenta_position])) // терминал на вершине совпадает с символом ленты
			{
				lenta_position++;             // продвижение по ленте
				st.pop();                     // удаление терминала из стека
				nrulechain = -1;              // сброс цепочки
				rc = TS_OK;                   // успешный шаг
				MFST_TRACE3                   // трассировка
			}
			else
			{
				MFST_TRACE4(TS_NOK / NS_NORULECHAIN)  // трассировка ошибки
					rc = resetstate() ? TS_NOK : NS_NORULECHAIN;  // ошибка терминала, восстановление состояния
			}
		}
		else
		{
			rc = LENTA_END;                   // достигнут конец ленты
			MFST_TRACE4(LENTA_END);           // трассировка
		}
		return rc;
	}

	// Добавляет цепочку в стек в обратном порядке
	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--) // от конца к началу
			st.push(chain.nt[k]);                 // добавление символов цепочки в стек
		return true;
	}

	// Сохраняет текущее состояние автомата в стек состояний
	bool Mfst::savestate()
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));  // сохранение позиции, стека, правила и цепочки
		MFST_TRACE6("SAVESTATE:", storestate.size());                      // трассировка
		return true;
	};

	// Восстанавливает последнее сохраненное состояние
	bool Mfst::resetstate()
	{
		bool rc = false;
		MfstState state;

		if (rc = (storestate.size() > 0)) // проверка, есть ли сохраненные состояния
		{
			state = storestate.top();     // верхнее состояние
			lenta_position = state.lenta_position;  // позиция на ленте
			st = state.st;                // стек
			nrule = state.nrule;          // правило
			nrulechain = state.nrulechain;  // цепочка
			storestate.pop();             // удаление состояния
			MFST_TRACE5("RESSTATE")       // трассировка
				MFST_TRACE2                   // трассировка
		}
		return rc;
	}

	// Сохраняет диагностику текущего состояния
	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain); // сохранение данных диагностики

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
				diagnosis[i].lenta_position = -1; // очистка диагностики
		}
		return rc;
	}

	// Запускает автомат
	bool Mfst::start()
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;           // начальный код шага
		char buf[MFST_DIAGN_MAXSIZE]{};       // буфер диагностики
		rc_step = step();                     // выполнение шага
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step();                 // повторяем шаги, пока они успешны

		switch (rc_step)                      // обработка завершения
		{
		case LENTA_END:                       // конец ленты
		{
			MFST_TRACE4("------>LENTA_END")   // трассировка
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lex.table[lex.size - 1].sn);
			std::cout << std::setw(4) << std::left << 0 << "всего строк " << lex.table[lex.size - 1].sn << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		}
		case NS_NORULE:                       // ошибка правила
		{
			MFST_TRACE4("------>NS_NORULE")   // трассировка
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		}
		case NS_NORULECHAIN:  MFST_TRACE4("------>NS_NORULECHAIN"); break;
		case NS_ERROR:        MFST_TRACE4("------>NS_ERROR"); break;
		case SURPRISE:        MFST_TRACE4("------>NS_SURPRISE"); break;
		}
		return rc;
	}

	// Получить содержимое стека в строковом виде
	char* Mfst::getCSt(char* buf)
	{
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			p = st.c[k];                          // символ из стека
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p); // преобразование
		}
		buf[st.size()] = '\0';
		return buf;
	}

	// Получить строковое представление ленты от заданной позиции
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]); // преобразование
		buf[i - pos] = 0x00;
		return buf;
	}

	// Получить диагностику ошибки
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char* rc = new char[200] {};
		int errid = 0;
		int lpos = -1;

		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}

	// Печатает последовательность правил
	void Mfst::printrules()
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7
		}
	}

	// Сохраняет последовательность правил и цепочек
	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}
		return true;
	}
}

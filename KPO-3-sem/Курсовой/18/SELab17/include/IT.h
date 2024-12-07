#pragma once

#define ID_MAXSIZE 6
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2 };
	enum IDTYPE { V = 10, F = 12, P = 13, L = 14 };

	struct Entry		// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		char		id[ID_MAXSIZE];		// идентификатор (автоматически усекается до ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE		idtype;				// тип идентификатора (переменная, функция, параметр, локальная переменная)
		Entry* scope;

		union
		{
			int	vint;						// значение integer
			struct
			{
				int len;						// кол-во символов в string
				char str[TI_STR_MAXSIZE - 1];	// символы string
			}	vstr[TI_STR_MAXSIZE];			// значение string
		} value;	// значение идентификатора
	};

	struct ID_Table              // экземпляр таблицы идентификаторов
	{
		int maxsize;            // емкость таблицы идентификаторов < TI_MAXSIZE
		int size;               // текущий размер таблицы идентификаторов < maxsize
		Entry* table;           // массив строк таблицы идентификаторов
		~ID_Table()
		{
			delete[] table; // Освобождение памяти
		}
	};

	ID_Table Create(             // создать таблицу идентификаторов
		int size                // емкость таблицы идентификаторов < TI_MAXSIZE
	);

	void Add(                   // добавить строку в таблицу идентификаторов
		ID_Table& idtable,       // экземпляр таблицы идентификаторов
		Entry entry             // строка таблицы идентификаторов
	);

	Entry GetEntry(             // получить строку таблицы идентификаторов
		const ID_Table& idtable, // экземпляр таблицы идентификаторов
		int n                   // номер получаемой строки
	);

	int IsId(                   // возврат: номер строки (если есть), TI_NULLIDX (если нет)
		ID_Table& idtable,       // экземпляр таблицы идентификаторов
		char id[ID_MAXSIZE]     // идентификатор
	);

	int search(const ID_Table& idtable, Entry& entry); // Возвращает индекс entry в idtable, или -1 если не найдено

	void Delete(ID_Table& idtable);  // удалить таблицу лексем (освободить память)
}
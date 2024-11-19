#include <iostream>
#include <winsock2.h> // Библиотека Windows Sockets для работы с сетью
#include <iphlpapi.h> // Библиотека Windows API для работы с сетевыми настройками и протоколами
#include <icmpapi.h>  // Библиотека для работы с ICMP протоколом (например, для реализации Ping)
#include <sstream>    // Для работы с потоками stringstream
#pragma comment(lib, "iphlpapi.lib") // Подключение библиотеки iphlpapi.lib
#pragma comment(lib, "ws2_32.lib")   // Подключение библиотеки ws2_32.lib (Windows Sockets)

// Отключение предупреждений о использовании устаревших функций (например, inet_addr)
#pragma warning(disable:4996)

// Определение базового значения статусов ICMP (начинается с 11000)
#define IP_STATUS_BASE 11000
// Определение возможных статусов ответа ICMP
#define IP_SUCCESS 0                    // Успешный ответ
#define IP_DEST_NET_UNREACHABLE 11002   // Сеть недоступна
#define IP_DEST_HOST_UNREACHABLE 11003  // Хост недоступен
#define IP_DEST_PROT_UNREACHABLE 11004  // Протокол недоступен
#define IP_DEST_PORT_UNREACHABLE 11005  // Порт недоступен
#define IP_REQ_TIMED_OUT 11010          // Тайм-аут запроса
#define IP_BAD_REQ 11011                // Неправильный запрос
#define IP_BAD_ROUTE 11012              // Неправильный маршрут
#define IP_TTL_EXPIRED_TRANSIT 11013    // TTL истёк при транзите

// Настройка ICMP опций. Структура содержит параметры для пакета ICMP:
// Текущие значения:
// - Ttl (Time to Live): 255 (максимальный TTL, который ограничивает число хопов)
// - Tos (Type of Service): 0 (отсутствие дополнительной обработки пакетов)
// - Flags: 0 (без флагов)
// - OptionsSize: 0 (дополнительные опции отсутствуют)
// - OptionsData: 0 (дополнительные данные отсутствуют)
IP_OPTION_INFORMATION option = { 255, 0, 0, 0, 0 };

using namespace std;

/**
 * Функция Ping отправляет ICMP-запросы и обрабатывает ответы.
 *
 * @param cHost        Адрес хоста для пинга (в формате строки).
 * @param Timeout      Тайм-аут ожидания ответа (в миллисекундах).
 * @param RequestCount Количество ICMP-запросов, которые нужно отправить.
 */
void Ping(const char* cHost, unsigned int Timeout, unsigned int RequestCount) {
	// Создаёт дескриптор ICMP (используется для отправки/приёма ICMP-запросов)
	HANDLE hIP = IcmpCreateFile();
	if (hIP == INVALID_HANDLE_VALUE) {
		// Если создать дескриптор не удалось, выполняем очистку и выходим
		WSACleanup();
		return;
	}

	// Буфер для передачи данных в ICMP-запросах (32 байта произвольных данных)
	char SendData[32] = "Data for ping";

	// Переменные для статистики
	int LostPacketsCount = 0;    // Количество потерянных пакетов
	unsigned int MaxMS = 0;      // Максимальное время ответа (мс)
	int MinMS = -1;              // Минимальное время ответа (мс)

	// Выделяем память для ответа ICMP (буфер)
	// Размер: структура ICMP_ECHO_REPLY + размер отправляемых данных
	PICMP_ECHO_REPLY pIpe = (PICMP_ECHO_REPLY)GlobalAlloc(GHND, sizeof(ICMP_ECHO_REPLY) + sizeof(SendData));
	if (pIpe == nullptr) {
		// Освобождаем ресурсы, если память не выделилась
		IcmpCloseHandle(hIP);
		WSACleanup();
		return;
	}

	// Настраиваем буфер ICMP
	pIpe->Data = SendData;                // Указатель на данные
	pIpe->DataSize = sizeof(SendData);   // Размер данных (32 байта)

	// Преобразуем адрес хоста из строки в числовой формат
	unsigned long ipaddr = inet_addr(cHost);

	// Цикл для отправки `RequestCount` ICMP-запросов
	for (unsigned int c = 0; c < RequestCount; c++) {
		// Отправляем ICMP-запрос
		int dwStatus = IcmpSendEcho(
			hIP,                            // Дескриптор ICMP
			ipaddr,                         // IP-адрес получателя
			SendData,                       // Данные для отправки
			sizeof(SendData),               // Размер данных
			&option,                        // Настройки ICMP
			pIpe,                           // Буфер для ответа
			sizeof(ICMP_ECHO_REPLY) + sizeof(SendData), // Размер буфера
			Timeout                         // Тайм-аут ожидания ответа (мс)
		);

		if (dwStatus > 0) {
			// Если пришёл ответ, обрабатываем его
			for (int i = 0; i < dwStatus; i++) {
				unsigned char* pIpPtr = (unsigned char*)&pIpe->Address;
				cout << "Ответ от " << (int)*(pIpPtr)
					<< "." << (int)*(pIpPtr + 1)
					<< "." << (int)*(pIpPtr + 2)
					<< "." << (int)*(pIpPtr + 3)
					<< ": число байт = " << pIpe->DataSize
					<< " время = " << pIpe->RoundTripTime
					<< "мс TTL = " << (int)pIpe->Options.Ttl;
				MaxMS = max(MaxMS, pIpe->RoundTripTime);
				MinMS = (MinMS < 0) ? pIpe->RoundTripTime : min(MinMS, pIpe->RoundTripTime);
				cout << endl;
			}
		}
		else {
			// Если ответа нет, фиксируем потерянный пакет и обрабатываем ошибку
			LostPacketsCount++;
			switch (pIpe->Status) {
			case IP_DEST_NET_UNREACHABLE:
			case IP_DEST_HOST_UNREACHABLE:
			case IP_DEST_PROT_UNREACHABLE:
			case IP_DEST_PORT_UNREACHABLE:
				cout << "Remote host may be down." << endl;
				break;
			case IP_REQ_TIMED_OUT:
				cout << "Request timed out." << endl;
				break;
			case IP_TTL_EXPIRED_TRANSIT:
				cout << "TTL expired in transit." << endl;
				break;
			default:
				cout << "Error code: " << pIpe->Status << endl;
				break;
			}
		}
	}

	// Закрываем дескриптор ICMP и выполняем очистку
	IcmpCloseHandle(hIP);
	WSACleanup();

	// Если минимальное время не установлено, оно равно 0
	if (MinMS < 0) MinMS = 0;

	// Выводим итоговую статистику
	unsigned char* pByte = (unsigned char*)&pIpe->Address;
	cout << "Статистика Ping "
		<< (int)*(pByte) << "." << (int)*(pByte + 1) << "."
		<< (int)*(pByte + 2) << "." << (int)*(pByte + 3) << endl;
	cout << "\tПакетов: отправлено = " << RequestCount
		<< ", получено = " << RequestCount - LostPacketsCount
		<< ", потеряно = " << LostPacketsCount
		<< " <" << (int)(100.0 / RequestCount) * LostPacketsCount
		<< "% потерь>, " << endl;
	cout << "Приблизительное время приема-передачи: "
		<< "Минимальное = " << MinMS
		<< "мс, Максимальное = " << MaxMS
		<< "мс, Среднее = " << (MaxMS + MinMS) / 2
		<< "мс" << endl;
}

int main(int argc, char** argv) {
	// Устанавливаем локаль для корректного отображения русского текста
	setlocale(LC_ALL, "RUS");

	// Проверяем, что передано достаточно аргументов
	if (argc < 4) {
		cout << "Использование: " << argv[0] << " <адрес хоста> <время ожидания в мс> <количество запросов>" << endl;
		return 1;
	}

	// Читаем параметры из командной строки
	const char* hostIp = argv[1];         // Адрес хоста для пинга
	unsigned int ms = std::stoi(argv[2]); // Тайм-аут в миллисекундах
	unsigned int reqNumber = std::stoi(argv[3]); // Количество запросов

	// Выполняем пинг
	Ping(hostIp, ms, reqNumber);

	return 0;
}

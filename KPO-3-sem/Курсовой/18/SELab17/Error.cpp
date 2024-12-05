#include "stdafx.h" 
#include "Error.h"
namespace Error
{
	// серии ошибок: 0 - 99 системные ошибки
	//				 100 - 199 - ошибки параметров
	//				 110 - 119 - ошибки открытия и чтения файлов 

	ERROR errors[ERROR_MAX_ENTRY] = //таблица ошибок
	{
	ERROR_ENTRY(0, "Недопустимый код ошибки"), ERROR_ENTRY(1, "Системный сбой"),
	// код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
	ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5), ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
	ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60),
	ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
	ERROR_ENTRY(100, "Параметр -in должен быть задан"),
	ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY(104, "Превышена длина входного параметра"),
	ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
	ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
	ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
	ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
	ERROR_ENTRY(112, "Ошибка при создании файла протокола(-log)"),
	ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
	// параметры
	ERROR_ENTRY(116, "Директории из указанного пути log файла не существует или не указан выходной файл"),
	ERROR_ENTRY(117,"Директории из указанного пути out файла не существует или не указан выходной файл"),
	ERROR_ENTRY(118, "Директории из указанного пути таблицы лексем не существует или указано неверное расширение: оно должно быть .html"),
	ERROR_ENTRY(119, "Указанный путь таблицы идентификаторов не существует или указано неверное расширение: оно должно быть .html"),
	ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130),
	ERROR_ENTRY(140, "Ошибка при лексическом разборе: Может быть только одна точка входа main"),
	ERROR_ENTRY(141, "Ошибка при лексическом разборе: Строковые и символьные литералы не могу быть пустыми"),
	ERROR_ENTRY(142, "Ошибка при лексическом разборе: Идентификатор использован, но не объявлен"),
	ERROR_ENTRY(143, "Ошибка при лексическом разборе: Идентификатору целочисленного типа присваевается значение строкового"),
	ERROR_ENTRY(144, "Ошибка при лексическом разборе: Идентификатору строкового типа присваевается значение целочисленного типа"),
	ERROR_ENTRY_NODEF5(145),
	ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170),
	ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
	ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY(600, "Неверная структура программы"),
	ERROR_ENTRY(601, "Ошибочный оператор"),
	ERROR_ENTRY(602, "Ошибка в выражении"),
	ERROR_ENTRY(603, "Ошибка в параметрах функции"),
	ERROR_ENTRY(604, "Ошибка в параметрах вызваемой функции"),
	ERROR_ENTRY_NODEF(605), ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609), ERROR_ENTRY_NODEF10(610),
	ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
	ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680),
	ERROR_ENTRY_NODEF10(690),
	ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {   //если код вне диапазона таблицы ошибок
			return errors[0];
		}

		else {
			return errors[id];
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1) {

		if (id < 0 || id > ERROR_MAX_ENTRY) {
			return errors[0];
		}

		else {
			errors[id].inext.line = line;
			errors[id].inext.col = col;
			return errors[id];
		}
	}
};
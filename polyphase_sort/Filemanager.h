#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

/*Filemanager для бинарной модификации, работа с файлами*/

class filemanager
{
	private:
		struct file_definition								//Определям файл через структуру
		{
			fstream file_object;							//Сам файл-о-объект
			string filename;								//Имя файла
			int real_series;								//Количество реальных серий
			int empty_series;								//Количество пустых серий
			file_definition(string _filename);				//Начальная инициализация
			void swap(file_definition &rhs);				//Свой своп
		};
	public:
		vector <file_definition> input, output;				//Заводим вектор пар "объект - имя"
		int in_count, out_count;							//Счетчики для векторов
		filemanager(int in_count, int out_count);			//Конструктор
		void fileswap();									//Своп векторов
		void index_swap(int index_in, int index_out);		//Своп определенных векторов по индексу
		int read(int index);								//Прочитать элемент из вектора
		void write(int index, int value);					//Записать элемент в вектор
		bool eof(int index);								//Проверка на конец файла
		~filemanager();										//Деструктор
		int get_in();										//Вернуть in счетчик
		int get_out();										//Вернуть out счетчик
};
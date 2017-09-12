#pragma once
#include "Filemanager.h"
#include "Fibonacci.h"
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

/* Класс многофазной сортировки */

class polyphase
{
public:
	vector <int> position_series;
	polyphase() {};
	void generate(string filename, int amount, int random_border);			//Генератор случайных чисел
	int show_binary_file_eof(string filename);								//Вывод бинарного файла (до конца файла)
	int show_binary_file_length(string filename);							//Вывод бинарного файла (на основе длины серии)
	int amount_of_series(string filename);									//Возврат числа серий
	void show_vector_series();												//Вывод позиций "слома" серий
	void show_txt_file(string filename);									//Вывод текстового файла
	int check_sort(string filename);										//Проверка сортировки
	void first_distribution(string filename, filemanager &manager);			//Первое распределение
};

/* Первое разделение с учетом того, что файл - бинарный.
Todo: "перебрать" класс фибоначчи и перевести функции make_order_vector и make_dist_mass в одну функцию*/

void polyphase::first_distribution(string filename, filemanager &manager)
{
	fstream f;																	//Открываем сгенерированный input файл
	f.open(filename, ios::in | ios::binary);
	int counter = manager.get_out();											//Забираем число выходных файлов
	fibonacci fib_vector(counter);												//Создаем ряд Фибоначчи порядка counter
	fib_vector.make_order_vector();												//Создаем первоначальный ряд фибоначчи
	fib_vector.make_dist_mass();
	int current = 0;															//Текущая позиция
	int next = 0;																//Следующая за текущей
	int current_file = 0;														//Текущий файл
	int length = 0;																//Длина серии
	streampos start_pos = manager.output[current_file].file_object.tellg();		//Начальная позиция (здесь = 0)
	streampos end_pos;															//Конечная позиция
	manager.output[current_file].real_series++;									//Увеличиваем серии на один (т.к. начинаем)
	manager.write(current_file, 0);												//Резервируем место под запись длины серии
																				//f >> next;
	f.read((char *)&next, sizeof(int));
	for (; ; )
	{
		//запись серии
		while (current <= next)
		{
			manager.write(current_file, next);									//Пишем в текущий файл взятый элемент
			length++;															//Увеличиваем длину серии
			current = next;														//Делаем текущим уже взятый
			if (!f.eof())
			{
				//f >> next;													//Если файл не кончился - берём следующий
				f.read((char *)&next, sizeof(int));
			}
			else
			{
				end_pos = manager.output[current_file].file_object.tellg();		//Запоминаем текущую (последнюю) позицию
				manager.output[current_file].file_object.seekg(start_pos);		//Перемещаемся на начало файла
				manager.write(current_file, length);							//Записываем длину
				manager.output[current_file].file_object.seekg(end_pos);		//Возвращаемся на сохраненную позицию
				return;
			}
		}
		//Меняем файлы
		end_pos = manager.output[current_file].file_object.tellg();
		manager.output[current_file].file_object.seekg(start_pos);
		manager.write(current_file, length);
		manager.output[current_file].file_object.seekg(end_pos);

		//Фибоначчи
		int i = 0;
		while (i < counter)
		{
			current_file = (current_file + 1) % counter;						//Забираем файлы по модулю
			if ((manager.output[current_file].real_series) < (fib_vector.mass[current_file]))											//UNABLE TO READ MEMORY !!!!
			{
				manager.output[current_file].real_series++;
				break;
			}
			i++;
		}
		if (i == counter)														//Если прошлись по всем файлам - генерируем новый фибоначчи массив
		{
			fib_vector.make_order_vector();
			fib_vector.make_dist_mass();
			current_file = 0;
			manager.output[current_file].real_series++;
		}

		start_pos = manager.output[current_file].file_object.tellg();			//Делаем стартовой позицией текущую
		manager.write(current_file, 0);											//Резервируем место для длины серии
		length = 0;																//Обнуляем длину серии

																				//Код из первого while
		manager.write(current_file, next);
		length++;
		current = next;
		if (!f.eof())
		{
			//f >> next;
			f.read((char *)&next, sizeof(int));
		}
		else
		{
			end_pos = manager.output[current_file].file_object.tellg();
			manager.output[current_file].file_object.seekg(start_pos);
			manager.write(current_file, length);
			manager.output[current_file].file_object.seekg(end_pos);
			break;
		}
	}
	f.close();
}

inline int polyphase::show_binary_file_length(string filename)
{
	fstream f;
	f.open(filename, ios::in | ios::binary);		//не забывать binary
	int temp;
	int length;
	while (!f.eof())
	{
		cout << endl;
		f.read((char *)&length, sizeof(int));
		if (length != 0)
		{
			cout << "[" << length << "] ";
			while (length)
			{
				int temp;
				f.read((char *)&temp, sizeof(int));
				cout << temp << " ";
				length--;
			}
		}
	}
	cout << endl;
	f.close();
	return 0;
}

inline void polyphase::generate(string filename, int amount, int random_border)
{
	fstream f;
	srand(time(NULL));
	f.open(filename, ios::out | ios::binary);
	for (int i = 0; i < amount - 1; i++)
	{
		int value = rand() % random_border;
		f.write((char*)&value, sizeof(int));
	}
	f.close();
}

int polyphase::check_sort(string filename)
{
	bool flag = 1;
	int counter = 1;
	int temp1, temp2;
	fstream f;
	f.open(filename, ios::in);
	f >> temp1;
	while (!f.eof())
	{
		f >> temp2;
		counter++;
		if (temp1 > temp2)
		{
			cout << "Error: " << counter << " position";
			flag = 0;
		}
		temp1 = temp2;
	}
	if (flag = true)
	{
		cout << "Sorting right: " << counter << " was sorted";
	}
	return flag;
}

inline int polyphase::show_binary_file_eof(string filename)
{
	fstream f;
	f.open(filename, ios::in | ios::binary);		//не забывать binary
	int temp;
	int quantity = 0;
	while (!f.eof())
	{
		int temp;
		f.read((char *)&temp, sizeof(int));
		cout << temp << " ";
		quantity++;
	}
	cout << endl;
	f.close();
	return quantity;								//Возвращаем число сгенерированных чисел (число чисел, lol)
}

inline void polyphase::show_txt_file(string filename)
{
	fstream f;
	f.open(filename);
	int temp;
	while (!f.eof())
	{
		f >> temp;
		cout << temp << " ";
	}
	cout << endl;
	f.close();
}

int polyphase::amount_of_series(string filename)
{
	fstream f;
	f.open(filename, ios::in | ios::binary);
	int quantity = 1;
	int current = 0;
	int next = 0;
	position_series.push_back(f.tellg());
	cout << "Current pos: " << f.tellg() << endl;
	f.read((char *)&current, sizeof(int));
	cout << "Series " << quantity << " starts with: " << current << endl;
	f.read((char *)&next, sizeof(int));
	while (!f.eof())
	{
		/*если всё верно*/
		if (next > current)
		{
			f.read((char *)&current, sizeof(int));
		}
		else
		{
			quantity++;
			cout << "Current pos: " << f.tellg() << endl;
			position_series.push_back(f.tellg());
			cout << "Series " << quantity << " starts with: " << next << endl;
			f.read((char *)&current, sizeof(int));
		}
		if (current > next)
		{
			f.read((char *)&next, sizeof(int));
		}
		else
		{
			quantity++;
			cout << "Current pos: " << f.tellg() << endl;
			position_series.push_back(f.tellg());
			cout << "Series " << quantity << " starts with: " << current << endl;
			f.read((char *)&next, sizeof(int));
		}
	}
	f.close();
	return quantity;
}

inline void polyphase::show_vector_series()
{
	for (int i = 0; i < position_series.size(); i++)
	{
		cout << position_series[i] << " ";
	}
	cout << endl;
}
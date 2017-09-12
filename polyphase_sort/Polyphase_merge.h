#pragma once
#include "Filemanager.h"
#include "Fibonacci.h"
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

/* Класс многофазной сортировки */

class polyphase
{
	public:
		vector <int> position_series;											//Вектор, хранящий позиции "обрывов" серий
		polyphase() {};
		void generate(string filename, int amount, int random_border);			//Генератор случайных чисел
		int show_binary_file_eof(string filename);								//Вывод бинарного файла (до конца файла)
		int show_binary_file_length(string filename);							//Вывод бинарного файла (на основе длины серии)
		int amount_of_series(string filename);									//Возврат числа серий
		void show_vector_series();												//Вывод позиций "обрывов" серий
		void show_txt_file(string filename);									//Вывод текстового файла
		int check_sort(string filename);										//Проверка сортировки
		void first_distribution(string filename, filemanager &manager);			//Первое распределение
		void merge(filemanager &manager);										//Сортировка
		bool eof(string filename);												//Проверка на пустоту для бинарника
};

/* Первое разделение с учетом того, что файл - бинарный.
Todo: "перебрать" класс фибоначчи и перевести функции make_order_vector и make_dist_mass в одну функцию*/

inline bool polyphase::eof(string filename)
{
	fstream f;
	f.open(filename, ios::in | ios::binary | ios::ate);
	bool eof = !f.tellg();
	f.close();
	return eof;
}

void polyphase::merge(filemanager &manager)
{
	multimap <int, int> buffer;													//Упорядоченный по значениям контейнер: <значение, номер файла>
	int count = manager.get_in();												//Количество файлов на входе
	int *length_of_series = new int[count];										//Массив длин серий
	int sum_of_series = 2;														//Общая сумма серий
	int current_length;															//Текущая длина

	while (sum_of_series > 1)
	{
		current_length = 0;
		for (int i = 0; i < count; i++)
		{
			if (manager.input[i].empty_series)									//Если есть пустые серии
			{
				length_of_series[i] = 0;
				manager.input[i].empty_series--;
			}
			else
			{
				if (!manager.input[i].real_series)								//Если нет настоящих серий
				{
					manager.index_swap(i, 0);
					if (manager.input[i].empty_series)							//Смотрим наличие пустых серий уже в новом файле
					{
						i--;
						continue;												//Начинаем заного, игнорируем последующие операторы
					}
				}
				length_of_series[i] = manager.read(i);
				manager.input[i].real_series--;
				buffer.insert(pair <int, int>(manager.read(i), i));
			}
			current_length += length_of_series[i]--;
		}

		manager.write(0, current_length);
		manager.output[0].real_series++;

		auto buf = buffer.begin();							//нельзя int, не сущ. преобр
		while (!buffer.empty())
		{
			buf = buffer.begin();
			manager.write(0, buf->first);
			if (length_of_series[buf->second]--)
			{
				buffer.insert(pair <int, int>(manager.read(buf->second), buf->second));
			}
			buffer.erase(buf);
		}

		/* Считаем сумму серий */
		sum_of_series = 0;
		for (int i = 0; i < count; i++)
		{
			sum_of_series += manager.input[i].real_series + manager.input[i].empty_series;
		}
		sum_of_series += manager.output[0].real_series + manager.output[0].empty_series;
	}
}

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

	int *diff = new int[counter];												//Разница между текущим и фибоначчивским распределением
	diff[0] = 0;
	for (int i = 1; i < counter; i++)
	{
		diff[i] = 1;															//[0 1 1 1 1 1 1 1 ...]
	}
	bool flag = 0;																//Флаг выхода
	while (true)
	{
		/* запись серии */
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
				flag = !flag;													//Выходим из этого и внешнего цикла полностью
				break;
			}
		}
		if (flag == 1) break;

		/* Меняем файлы */
		end_pos = manager.output[current_file].file_object.tellg();				//Запоминаем текущую (последнюю) позицию в файле
		manager.output[current_file].file_object.seekg(start_pos);				//Перемещаемся на начало файла
		manager.write(current_file, length);									//Записываем длину (вместо зарезервированного нуля)
		manager.output[current_file].file_object.seekg(end_pos);				//Возвращаемся на сохраненную позицию ранее конечн. позицию

//Фибоначчи (старый кусок кода - обычное разделение)
//int i = 0;
//while (i < counter)
//{
//	current_file = (current_file + 1) % counter;													//Забираем файлы по модулю
//	if ((manager.output[current_file].real_series) < (fib_vector.mass[current_file]))				//UNABLE TO READ MEMORY !!!! - fixed
//	{
//		manager.output[current_file].real_series++;
//		break;
//	}
//	i++;
//}
//if (i == counter)																					//Если прошлись по всем файлам - генерируем новый фибоначчи массив
//{
//	fib_vector.make_order_vector();
//	fib_vector.make_dist_mass();
//	current_file = 0;
//	manager.output[current_file].real_series++;
//}

		/* Для распределения фибоначчи по таблице */
		if ((current_file < counter - 1) && ((diff[current_file] + 1) == diff[current_file + 1]))
		{
			current_file++;
			diff[current_file]--;
			manager.output[current_file].real_series++;
		}
		else
		{
			if (!diff[counter - 1])
			{
				fib_vector.make_order_vector();
				fib_vector.make_dist_mass();
				for (int i = 0; i < counter; i++)
				{
					diff[i] = fib_vector.mass[i] - manager.output[i].real_series;
				}
			}
			current_file = 0;
			diff[current_file]--;
			manager.output[current_file].real_series++;
		}

		/* Сохраняем место для записи длины серии */
		start_pos = manager.output[current_file].file_object.tellg();			//Делаем стартовой позицией текущую
		manager.write(current_file, 0);											//Резервируем место для длины серии
		length = 0;																//Обнуляем длину серии

		manager.write(current_file, next);										//Пишем элемент из следующей серии
		length++;
		current = next;
		if (!f.eof())															//Если файл не кончился - считываем следующий элемент	
		{
			//f >> next;
			f.read((char *)&next, sizeof(int));
		}
		else
		{
			break;
		}
	}
	end_pos = manager.output[current_file].file_object.tellg();				//Запоминаем текущую (последнюю) позицию
	manager.output[current_file].file_object.seekg(start_pos);				//Перемещаемся на позицию, зарез. ранее - заменяем 0 на длину серии
	manager.write(current_file, length);									//Записываем длину
	manager.output[current_file].file_object.seekg(end_pos);				//Возвращаемся на сохраненную позицию
	for (int i = 0; i < counter; i++)
	{
		manager.output[i].empty_series = diff[i];							//Пишем кол-во пустых серий из массива разниц
	}
	f.close();
}

inline int polyphase::show_binary_file_length(string filename)
{
	if (eof(filename))
	{
		cout << "[пустой]" << endl;
	}
	else
	{
		fstream f;
		f.open(filename, ios::in | ios::binary);		//не забывать binary
		int temp;
		int length;
		while (!f.eof())
		{
			f.read((char *)&length, sizeof(int));
			if (length)
			{
				cout << "[len: " << length << "] ";
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
	}
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

inline int polyphase::check_sort(string filename)
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
			cout << "Ошибка сортировки: " << counter << " позиция";
			flag = 0;
		}
		temp1 = temp2;
	}
	if (flag = true)
	{
		cout << "Сортировка верна: " << counter << " элементов было отсортировано";
	}
	cout << endl;
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

/* Определение конца серии + позиции серии
! Не основан на использовании бинарников - можно кормить обычными файлами
(с учетом изменения read/write на <</>> ) */
int polyphase::amount_of_series(string filename)
{
	fstream f;
	f.open(filename, ios::in | ios::binary);
	int quantity = 1;
	int current = 0;
	int next = 0;
	position_series.push_back(f.tellg());
	cout << "Текущая позиция: " << 1 + f.tellg() << " :: ";
	f.read((char *)&current, sizeof(int));
	cout << "Серия №" << quantity << " начинается со значения: " << current << endl;
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
			cout << "Текущая позиция: " << f.tellg() / sizeof(int) << " :: ";
			position_series.push_back(f.tellg());
			cout << "Серия №" << quantity << " начинается со значения: " << next << endl;
			f.read((char *)&current, sizeof(int));
		}
		if (current > next)
		{
			f.read((char *)&next, sizeof(int));
		}
		else
		{
			quantity++;
			cout << "Текущая позиция: " << f.tellg() / sizeof(int) << " :: ";
			position_series.push_back(f.tellg());
			cout << "Серия №" << quantity << " начинается со значения: " << current << endl;
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
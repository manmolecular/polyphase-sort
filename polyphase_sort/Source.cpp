#include "Polyphase_merge.h"
#include <ctime>
#define input_name "input.txt"
#define output_name "output.txt"
#define debug_file "debug_info.txt"
const int amount_of_numbers = 100000;															//Количество значений для генерации
const int amount_of_files = 55;																	//Количество задействованных файлов
const bool debug_mode = 0;																		//1 - с отладочной информацией, 0 - без неё
using namespace std;

/* Проверка для класса Фибоначчи */
void fibonacci_class_check()
{
	fibonacci vector(5);
	vector.make_order_vector();
	vector.make_dist_mass();
	for (int i = 0; i < 10; i++)
	{
		vector.make_order_vector();
		vector.make_dist_mass();
		for (int i = 0; i < vector.order; i++)
		{
			cout << vector.mass[i] << " ";
		}
		cout << endl;
	}
}

/* Проверка для класса polyphase */
void polyphase_class_check()
{
	polyphase temp;
	temp.generate("input.txt", amount_of_numbers, 1000);
	int quantity = temp.show_binary_file_eof("input.txt");
	cout << "Amount of gen nums:" << quantity << endl << endl;
	quantity = temp.amount_of_series("input.txt");
	cout << "Amount of series:" << quantity << endl;
	cout << "Series position in vector:" << endl;
	temp.show_vector_series();
}

/* Показать все выходные файлы */
void show_output_files(polyphase &sorting)
{
	for (int i = 0; i < amount_of_files - 1; i++)
	{
		string file_name = "file_output_" + to_string(i) + ".txt";
		cout << file_name << ": ";
		sorting.show_binary_file_length(file_name);
	}
}

/* Функция сортировки */
void sort_function(filemanager &manager, polyphase &sorting, bool show_output, clock_t &start_dist, clock_t &end_dist, clock_t &start_merge, clock_t &end_merge)
{
	start_dist = clock();
	sorting.first_distribution(input_name, manager);
	end_dist = clock();
	if (show_output)
	{
		cout << "Файлы содержат (первое распределение): " << endl;
		show_output_files(sorting);
	}
	/* Работаем с пустыми сериями */
	int min = manager.output[0].empty_series;
	for (int i = 1; i < amount_of_files - 1; i++)
	{
		if (min > manager.output[i].empty_series)
		{
			min = manager.output[i].empty_series;						//Забираем минимальное число пустых серий
		}
	}
	if (min != 0)														//Если не ноль - то выполняем дальше, иначе пропускаем
	{
		for (int i = 0; i < amount_of_files - 1; i++)
		{
			manager.output[i].empty_series -= min;
		}
		manager.input[0].empty_series += min;
	}
	manager.fileswap();
	cout << endl << "Сортировка... ";
	start_merge = clock();
	sorting.merge(manager);
	end_merge = clock();
	cout << "Завершена" << endl;
	manager.fileswap();
}

/* Отладочная информация */
void view_debug_info(polyphase &sorting)
{
	cout << "Файл " << input_name << " содержит: " << endl;
	int quantity = sorting.show_binary_file_eof(input_name);
	cout << "Количество сгенерированных значений:" << quantity << endl << endl;
	cout << "Проверка серий: " << endl;
	quantity = sorting.amount_of_series(input_name);
	cout << "Количество серий: " << quantity << endl << endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	filemanager manager(1, amount_of_files - 1);
	polyphase sorting;
	clock_t start_generate, end_generate, start_dist, end_dist, start_merge, end_merge;
		start_generate = clock();
		sorting.generate(input_name, amount_of_numbers, 1000);															//1000 - модуль генерации рандома (верхняя граница)
		end_generate = clock();
		if (debug_mode)
		{
			view_debug_info(sorting);																					//Отображаем дебаг-инфу, если mode == 1
		}
		sort_function(manager, sorting, debug_mode, start_dist, end_dist, start_merge, end_merge);						//1 - отладочная информация, 0 - без неё

		/* Запись результата сортировки в файл*/
		fstream f;
		f.open(output_name, ios::out);
		int length = manager.read(0);
		for (int i = 0; i < length; i++)
		{
			f << " " << manager.read(0);																				//Записываем всё из input-a в файл
		}
		f.close();
		cout << endl;

		/* Вывод на экран */
		sorting.check_sort(output_name);
		float gen_t = ((double)end_generate - start_generate) / ((double)CLOCKS_PER_SEC);
		float dist_t = ((double)end_dist - start_dist) / ((double)CLOCKS_PER_SEC);
		float merge_t = ((double)end_merge - start_merge) / ((double)CLOCKS_PER_SEC);
		cout << "Количество элементов: " << amount_of_numbers << endl;
		cout << "Количество файлов: " << amount_of_files << endl;
		cout << "Время генерации (generate): " << gen_t << " сек." << endl;
		cout << "Время первого распределения (first_dist): " << dist_t << " сек." << endl;
		cout << "Время сортировки (merge): " << merge_t << " сек." << endl << endl;

		/* Вывод информации в файл*/
		fstream debug;
		debug.open(debug_file, ios::out | ios::app);																	//app - дописывать в файл
		debug << "Количество элементов: " << amount_of_numbers << endl;
		debug << "Количество файлов: " << amount_of_files << endl;
		debug << "Время генерации (generate): " << gen_t << " сек." << endl;
		debug << "Время первого распределения (first_dist): " << dist_t << " сек." << endl;
		debug << "Время сортировки (merge): " << merge_t << " сек." << endl << endl;
		debug.close();
	return 0;
}
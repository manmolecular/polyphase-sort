#pragma once
#include <vector>
#include <iostream>
using namespace std;

/* Класс для работы с числами фибоначчи произвольного порядка*/

class fibonacci
{
	private:
		vector <int> fib_series;								//Основной вектор для генерации Фибоначчи с порядком
		int current_pos;										//Текущая позиция для подсчета
		int index_end;											//Индекс-позиция вектора
	public:
		int *mass;
		int order;												//Порядок для последовательности Фибоначчи
		fibonacci(int _order);									//Конструктор
		void show_fib_vector();									//Вывод фибоначчи
		void show_dist_mass();									//Вывод основного массива
		void make_order_vector();								//Составление последовательности Фибоначчи заданного порядка
		void make_dist_mass();									//Составление распределительного массива (на основе order_vector)
};
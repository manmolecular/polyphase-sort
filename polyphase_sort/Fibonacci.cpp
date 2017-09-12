#include "Fibonacci.h"

/* Реализация класса Фибоначчи */

/* Конструктор для фибоначчи */
fibonacci::fibonacci(int _order)
{
	current_pos = 0;
	order = _order;												//забираем входную длину серии
	index_end = order - 1;										//определяем последнюю позицию в векторе
	for (int i = 0; i < order; i++)
	{
		fib_series.push_back(0);								//все элементы 0, кроме одного (=1)
	}
	fib_series[index_end] = 1;									//последний = 1
}

void fibonacci::show_fib_vector()
{
	for (int i = 0; i < fib_series.size(); i++)
	{
		cout << fib_series[i] << " ";
	}
}

void fibonacci::show_dist_mass()
{
	for (int i = 0; i < order; i++)
	{
		cout << mass[i] << " ";
	}
}

/* Составление последовательности фибоначчи в векторе
 - добиваем в вектор естественные для ряда числа*/
void fibonacci::make_order_vector()
{
	int temp1 = 0;
	int temp2 = 0;
	for (int i = 0; i < fib_series.size(); i++)
	{
		temp1 = temp1 + fib_series[i];
		if (i > order)
		{
			temp2 = temp2 + fib_series[i - order];
		}
	}
	fib_series.push_back(temp1 - temp2);
}

/* Составляем массив, который будет нужен для 
самой сортировки (суммируем Фибоначчи в нужном
порядке)*/
void fibonacci::make_dist_mass()
{
	mass = new int[order];
	int finish = current_pos + order;
	for (int i = 0; i < order; i++)
	{
		int temp = 0;
		for (int j = current_pos; j < finish; j++)
		{
			temp = temp + fib_series[j];
		}
		current_pos++;
		mass[i] = temp;
	}
	current_pos = finish - order + 1;
}
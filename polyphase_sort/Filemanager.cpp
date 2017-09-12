#include "Filemanager.h"

/* Реализация класса файлового менеджера */

filemanager::filemanager(int _in_count, int _out_count)
{
	in_count = _in_count;			/* забираем из аргументов функции (fixed bug)*/
	out_count = _out_count;
	input.reserve(in_count);		/* резервируем место в векторах заранее */
	output.reserve(out_count);
	/* vs10 - long long, vs14+ - int */
	for (long long i = 0; i < in_count; i++)
	{
		string file_name = "file_input_" + to_string(i) + ".txt";
		input.push_back(file_definition(file_name));
		/* пушим объект fstream и сохраняем имя file_name */
	}
	for (long long i = 0; i < out_count; i++)
	{
		string file_name = "file_output_" + to_string(i) + ".txt";
		output.push_back(file_definition(file_name));
		/* пушим объект fstream и сохраняем имя file_name */
	}
}

void filemanager::fileswap()
{
	for (int i = 0; i < in_count; i++)
	{
		input[i].file_object.close();
		input[i].file_object.open(input[i].filename, ios::out | ios::binary);
	}
	for (int i = 0; i < out_count; i++)
	{
		output[i].file_object.close();
		output[i].file_object.open(output[i].filename, ios::in | ios::binary);
	}
	input.swap(output);
	//oh, hi - свапаем счетчики
	swap(in_count, out_count);
}

void filemanager::index_swap(int index_in, int index_out)
{
	input[index_in].file_object.close();
	input[index_in].file_object.open(input[index_in].filename, ios::out | ios::binary);
	output[index_out].file_object.close();
	output[index_out].file_object.open(output[index_out].filename, ios::in | ios::binary);
	input[index_in].swap(output[index_out]);
}

int filemanager::read(int index)
{
	int temp;
	input[index].file_object.read((char *)&temp, sizeof(int));
	return temp;
}

void filemanager::write(int index, int value)
{
	output[index].file_object.write((char*)&value, sizeof(int));		// выводим массив в файл 
}

bool filemanager::eof(int index)
{
	return input[index].file_object.eof();
}

int filemanager::get_in()
{
	return in_count;
}

int filemanager::get_out()
{
	return out_count;
}

filemanager::~filemanager()
{
	for (int i = 0; i < in_count; i++)
	{
		input[i].file_object.close();				//Закрываем и удаляем за собой лишние файлы
		remove(input[i].filename.c_str());
	}
	for (int i = 0; i < out_count; i++)
	{
		output[i].file_object.close();
		remove(output[i].filename.c_str());
	}
}

/* Инициализируем структуру*/
filemanager::file_definition::file_definition(string _filename)
{
	filename = _filename;
	real_series = 0;
	empty_series = 0;
	file_object = fstream(filename, ios::out | ios::binary);
}

void filemanager::file_definition::swap(file_definition &rhs)
{
	file_object.swap(rhs.file_object);
	filename.swap(rhs.filename);
	std::swap(real_series, rhs.real_series);		//Совпадение имен - это плохо
	std::swap(empty_series, rhs.empty_series);
}
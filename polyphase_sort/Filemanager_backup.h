#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

/*Filemanager ��� �������� �����������, ������ � �������*/

class filemanager
{
private:
	vector <pair <fstream, string>> input, output;		//������� ������ ��� "������ - ���"
	int in_count, out_count;							//�������� ��� ��������
public:
	filemanager(int in_count, int out_count);			//�����������
	void fileswap();									//���� ��������
	void index_swap(int index_in, int index_out);		//���� ������������ �������� �� �������
	int read(int index);								//��������� ������� �� �������
	void write(int index, int value);					//�������� ������� � ������
	bool eof(int index);								//�������� �� ����� �����
	~filemanager();										//����������
	int get_in();										//������� in �������
	int get_out();										//������� out �������
};

filemanager::filemanager(int _in_count, int _out_count)
{
	in_count = _in_count;			/* �������� �� ���������� ������� (fixed bug)*/
	out_count = _out_count;
	input.reserve(in_count);		/* ����������� ����� � �������� ������� */
	output.reserve(out_count);
	/* vs10 - long long, vs14+ - int */
	for (long long i = 0; i < in_count; i++)
	{
		string file_name = "file_input_" + to_string(i) + ".txt";
		input.push_back(pair <fstream, string>(fstream(file_name, ios::out | ios::binary), file_name));
		/* ����� ������ fstream � ��������� ��� file_name */
	}
	for (long long i = 0; i < out_count; i++)
	{
		string file_name = "file_output_" + to_string(i) + ".txt";
		output.push_back(pair <fstream, string>(fstream(file_name, ios::out | ios::binary), file_name));
		/* ����� ������ fstream � ��������� ��� file_name */
	}
}

void filemanager::fileswap()
{
	for (int i = 0; i < in_count; i++)
	{
		input[i].first.close();
		input[i].first.open(input[i].second, ios::out | ios::binary);
	}
	for (int i = 0; i < out_count; i++)
	{
		output[i].first.close();
		output[i].first.open(output[i].second, ios::in | ios::binary);
	}
	input.swap(output);
}

void filemanager::index_swap(int index_in, int index_out)
{
	input[index_in].first.close();
	input[index_in].first.open(input[index_in].second, ios::out | ios::binary);
	output[index_out].first.close();
	output[index_out].first.open(output[index_out].second, ios::in | ios::binary);
	input[index_in].swap(output[index_out]);
}

int filemanager::read(int index)
{
	int temp;
	//input[index].first >> temp;
	input[index].first.read((char *)&temp, sizeof(int));
	return temp;
}

void filemanager::write(int index, int value)
{
	//output[index].first << " " << value;
	output[index].first.write((char*)&value, sizeof(int)); // ������� ������ � ���� 
}

bool filemanager::eof(int index)
{
	return input[index].first.eof();
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
		input[i].first.close();
		remove(input[i].second.c_str());
	}
	for (int i = 0; i < out_count; i++)
	{
		output[i].first.close();
		remove(output[i].second.c_str());
	}
}
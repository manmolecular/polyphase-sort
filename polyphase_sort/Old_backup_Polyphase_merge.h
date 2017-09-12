#pragma once
#include "Filemanager.h"
#include "Fibonacci.h"
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

/* ����� ����������� ���������� */

class polyphase
{
public:
	vector <int> position_series;
	polyphase() {};
	void generate(string filename, int amount, int random_border);			//��������� ��������� �����
	int show_binary_file_eof(string filename);								//����� ��������� ����� (�� ����� �����)
	int show_binary_file_length(string filename);							//����� ��������� ����� (�� ������ ����� �����)
	int amount_of_series(string filename);									//������� ����� �����
	void show_vector_series();												//����� ������� "�����" �����
	void show_txt_file(string filename);									//����� ���������� �����
	int check_sort(string filename);										//�������� ����������
	void first_distribution(string filename, filemanager &manager);			//������ �������������
};

/* ������ ���������� � ������ ����, ��� ���� - ��������.
Todo: "���������" ����� ��������� � ��������� ������� make_order_vector � make_dist_mass � ���� �������*/

void polyphase::first_distribution(string filename, filemanager &manager)
{
	fstream f;																	//��������� ��������������� input ����
	f.open(filename, ios::in | ios::binary);
	int counter = manager.get_out();											//�������� ����� �������� ������
	fibonacci fib_vector(counter);												//������� ��� ��������� ������� counter
	fib_vector.make_order_vector();												//������� �������������� ��� ���������
	fib_vector.make_dist_mass();
	int current = 0;															//������� �������
	int next = 0;																//��������� �� �������
	int current_file = 0;														//������� ����
	int length = 0;																//����� �����
	streampos start_pos = manager.output[current_file].file_object.tellg();		//��������� ������� (����� = 0)
	streampos end_pos;															//�������� �������
	manager.output[current_file].real_series++;									//����������� ����� �� ���� (�.�. ��������)
	manager.write(current_file, 0);												//����������� ����� ��� ������ ����� �����
																				//f >> next;
	f.read((char *)&next, sizeof(int));
	for (; ; )
	{
		//������ �����
		while (current <= next)
		{
			manager.write(current_file, next);									//����� � ������� ���� ������ �������
			length++;															//����������� ����� �����
			current = next;														//������ ������� ��� ������
			if (!f.eof())
			{
				//f >> next;													//���� ���� �� �������� - ���� ���������
				f.read((char *)&next, sizeof(int));
			}
			else
			{
				end_pos = manager.output[current_file].file_object.tellg();		//���������� ������� (���������) �������
				manager.output[current_file].file_object.seekg(start_pos);		//������������ �� ������ �����
				manager.write(current_file, length);							//���������� �����
				manager.output[current_file].file_object.seekg(end_pos);		//������������ �� ����������� �������
				return;
			}
		}
		//������ �����
		end_pos = manager.output[current_file].file_object.tellg();
		manager.output[current_file].file_object.seekg(start_pos);
		manager.write(current_file, length);
		manager.output[current_file].file_object.seekg(end_pos);

		//���������
		int i = 0;
		while (i < counter)
		{
			current_file = (current_file + 1) % counter;						//�������� ����� �� ������
			if ((manager.output[current_file].real_series) < (fib_vector.mass[current_file]))											//UNABLE TO READ MEMORY !!!!
			{
				manager.output[current_file].real_series++;
				break;
			}
			i++;
		}
		if (i == counter)														//���� �������� �� ���� ������ - ���������� ����� ��������� ������
		{
			fib_vector.make_order_vector();
			fib_vector.make_dist_mass();
			current_file = 0;
			manager.output[current_file].real_series++;
		}

		start_pos = manager.output[current_file].file_object.tellg();			//������ ��������� �������� �������
		manager.write(current_file, 0);											//����������� ����� ��� ����� �����
		length = 0;																//�������� ����� �����

																				//��� �� ������� while
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
	f.open(filename, ios::in | ios::binary);		//�� �������� binary
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
	f.open(filename, ios::in | ios::binary);		//�� �������� binary
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
	return quantity;								//���������� ����� ��������������� ����� (����� �����, lol)
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
		/*���� �� �����*/
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
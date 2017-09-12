#include "Polyphase_merge.h"
#include <ctime>
#define input_name "input.txt"
#define output_name "output.txt"
#define debug_file "debug_info.txt"
const int amount_of_numbers = 100000;															//���������� �������� ��� ���������
const int amount_of_files = 55;																	//���������� ��������������� ������
const bool debug_mode = 0;																		//1 - � ���������� �����������, 0 - ��� ��
using namespace std;

/* �������� ��� ������ ��������� */
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

/* �������� ��� ������ polyphase */
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

/* �������� ��� �������� ����� */
void show_output_files(polyphase &sorting)
{
	for (int i = 0; i < amount_of_files - 1; i++)
	{
		string file_name = "file_output_" + to_string(i) + ".txt";
		cout << file_name << ": ";
		sorting.show_binary_file_length(file_name);
	}
}

/* ������� ���������� */
void sort_function(filemanager &manager, polyphase &sorting, bool show_output, clock_t &start_dist, clock_t &end_dist, clock_t &start_merge, clock_t &end_merge)
{
	start_dist = clock();
	sorting.first_distribution(input_name, manager);
	end_dist = clock();
	if (show_output)
	{
		cout << "����� �������� (������ �������������): " << endl;
		show_output_files(sorting);
	}
	/* �������� � ������� ������� */
	int min = manager.output[0].empty_series;
	for (int i = 1; i < amount_of_files - 1; i++)
	{
		if (min > manager.output[i].empty_series)
		{
			min = manager.output[i].empty_series;						//�������� ����������� ����� ������ �����
		}
	}
	if (min != 0)														//���� �� ���� - �� ��������� ������, ����� ����������
	{
		for (int i = 0; i < amount_of_files - 1; i++)
		{
			manager.output[i].empty_series -= min;
		}
		manager.input[0].empty_series += min;
	}
	manager.fileswap();
	cout << endl << "����������... ";
	start_merge = clock();
	sorting.merge(manager);
	end_merge = clock();
	cout << "���������" << endl;
	manager.fileswap();
}

/* ���������� ���������� */
void view_debug_info(polyphase &sorting)
{
	cout << "���� " << input_name << " ��������: " << endl;
	int quantity = sorting.show_binary_file_eof(input_name);
	cout << "���������� ��������������� ��������:" << quantity << endl << endl;
	cout << "�������� �����: " << endl;
	quantity = sorting.amount_of_series(input_name);
	cout << "���������� �����: " << quantity << endl << endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	filemanager manager(1, amount_of_files - 1);
	polyphase sorting;
	clock_t start_generate, end_generate, start_dist, end_dist, start_merge, end_merge;
		start_generate = clock();
		sorting.generate(input_name, amount_of_numbers, 1000);															//1000 - ������ ��������� ������� (������� �������)
		end_generate = clock();
		if (debug_mode)
		{
			view_debug_info(sorting);																					//���������� �����-����, ���� mode == 1
		}
		sort_function(manager, sorting, debug_mode, start_dist, end_dist, start_merge, end_merge);						//1 - ���������� ����������, 0 - ��� ��

		/* ������ ���������� ���������� � ����*/
		fstream f;
		f.open(output_name, ios::out);
		int length = manager.read(0);
		for (int i = 0; i < length; i++)
		{
			f << " " << manager.read(0);																				//���������� �� �� input-a � ����
		}
		f.close();
		cout << endl;

		/* ����� �� ����� */
		sorting.check_sort(output_name);
		float gen_t = ((double)end_generate - start_generate) / ((double)CLOCKS_PER_SEC);
		float dist_t = ((double)end_dist - start_dist) / ((double)CLOCKS_PER_SEC);
		float merge_t = ((double)end_merge - start_merge) / ((double)CLOCKS_PER_SEC);
		cout << "���������� ���������: " << amount_of_numbers << endl;
		cout << "���������� ������: " << amount_of_files << endl;
		cout << "����� ��������� (generate): " << gen_t << " ���." << endl;
		cout << "����� ������� ������������� (first_dist): " << dist_t << " ���." << endl;
		cout << "����� ���������� (merge): " << merge_t << " ���." << endl << endl;

		/* ����� ���������� � ����*/
		fstream debug;
		debug.open(debug_file, ios::out | ios::app);																	//app - ���������� � ����
		debug << "���������� ���������: " << amount_of_numbers << endl;
		debug << "���������� ������: " << amount_of_files << endl;
		debug << "����� ��������� (generate): " << gen_t << " ���." << endl;
		debug << "����� ������� ������������� (first_dist): " << dist_t << " ���." << endl;
		debug << "����� ���������� (merge): " << merge_t << " ���." << endl << endl;
		debug.close();
	return 0;
}
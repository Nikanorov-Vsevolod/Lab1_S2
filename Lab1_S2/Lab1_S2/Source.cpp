#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <stdlib.h>


struct bank
{
	char* fam;
	char* name;
	int account;//номер счёта
	double summ;
};

int N;

bank read_bank(bank* x)
{
	char buff1[32];
	char buff2[32];
	while (scanf("%s%s%d%lf", buff1, buff2, &x->account, &x->summ) != 4)
	{
		printf("%s", "введены некорректные данные, повторите ввод\n");
		while (fgetc(stdin) != '\n');
		continue;
	}
	x->fam = (char*)calloc(strlen(buff1), sizeof(char));
	strcpy(x->fam, buff1);
	x->name = (char*)calloc(strlen(buff2), sizeof(char));
	strcpy(x->name, buff2);
	return *x;
}

int read_int(int* x)
{
	while (scanf("%9d", x) != 1)
	{
		printf("%s", "введены некорректные данные, повторите ввод\n");
		while (fgetc(stdin) != '\n');
		continue;
	}
	if (*x < 0)
	{
		printf("%s", "число не может быть отрицательным, повторите ввод\n");
		read_int(x);
	}
	return *x;
}

double read_double(double* x)
{
	while (scanf("%9lf", x) != 1)
	{
		printf("%s","введены некорректные данные, повторите ввод\n");
		while (fgetc(stdin) != '\n');
		continue;
	}
	if (*x < 0)
	{
		printf("%s", "число не может быть отрицательным, повторите ввод\n");
		read_double(x);
	}
	return *x;
}

void print_bank(bank* fstart, int N)
{
	double summ = 0;
	printf("%-20s%-20s%-20s%-20s\n", "Фамилия", "Имя", "Номер счёта", "Сумма на счёте");
	printf("-----------------------------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < N; i++)
	{
		printf("%-20s%-20s%-20d%-20.2lf\n", fstart->fam, fstart->name, fstart->account, fstart->summ);
		fstart++;
	}
	printf("-----------------------------------------------------------------------------------------------------------------\n");
	//printf("Количество записей в базе  %d\n", N);
}

void print_bank_in_file(bank* fstart, int N, FILE* f)
{
	double summ = 0;
	fprintf(f, "Количество записей в базе:  %d\n", N);
	fprintf(f,"%-20s%-20s%-20s%-20s\n", "Фамилия", "Имя", "Номер счёта", "Сумма на счёте");
	fprintf(f, "-----------------------------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < N; i++)
	{
		fprintf(f, "%-20s%-20s%-20d%-20.2lf\n", fstart->fam, fstart->name, fstart->account, fstart->summ);
		fstart++;
	}
	fprintf(f, "-----------------------------------------------------------------------------------------------------------------\n");
}

bank* init_bank()//инициализация с клавиатуры
{
	char buff1[32];
	char buff2[32];
	printf("%s","введите количество клиентов\n");
	read_int(&N);
	bank* Recods = (bank*)calloc(N, sizeof(bank));
	for (bank* work_pointer = Recods; work_pointer < Recods+N; work_pointer++)
	{
		printf("%s", "Введите фамилию клиента, имя клиента, номер счёта, сумму на счёте\n");
		read_bank(work_pointer);
	}
	return Recods;
}

bank* read_from_file(FILE* f)
{
	char buff[150];
	char buff1[150];
	char buff2[150];
	
	fgets(buff, 27, f);
	fgets(buff, 10, f);
	N=atoi(buff);
	fgets(buff, 150, f);
	fgets(buff, 150, f);

	bank* Recods = (bank*)calloc(N, sizeof(bank));
	for (bank* work_pointer = Recods; work_pointer < Recods + N; work_pointer++)
	{
		fscanf(f,"%s%s%d%lf", buff1, buff2, &work_pointer->account, &work_pointer->summ);


		work_pointer->fam = (char*)calloc(strlen(buff1), sizeof(char));
		strcpy(work_pointer->fam, buff1);
		work_pointer->name = (char*)calloc(strlen(buff2), sizeof(char));
		strcpy(work_pointer->name, buff2);
	}
	return Recods;
}

bank* new_bank(bank* start)//добавление нового элемента
{
	bank* new_pointer = (bank*)calloc(N+1, sizeof(bank));
	for (int i=0;i<N;i++)
	{
		*(new_pointer + i) = *(start + i);
	}
	delete start;
	char buff1[32];
	char buff2[32]; 
	printf("%s", "Введите фамилию клиента, имя клиента, номер счёта, сумму на счёте\n");
	read_bank(new_pointer + N);
	N++;
	return new_pointer;
}

void find_fam(bank* start)//поиск клиента по фамилии
{
	int kol = 0;
	char f_fam[32];
	printf("%s", "Введите фамилию\n");
	scanf("%s", &f_fam);
	for (bank* work_pointer = start; work_pointer < start + N; work_pointer++)
	{
		if (strcmp(f_fam,work_pointer->fam) == 0)
		{
			print_bank(work_pointer, 1);
			kol++;
		}
	}
	if (kol==0)
		printf("%s", "Нет клиентов с такой фамилией\n");
}

void select_summ(bank* start)//фильтр по количеству
{
	int kol = 0;
	double summa = 0;
	printf("%s", "Введите сумму\n");
	read_double(&summa);
	printf("%-20s%-20s%-20s%-20s\n", "Фамилия", "Имя", "Номер счёта", "Сумма на счёте");
	for (bank* work_pointer = start; work_pointer < start + N; work_pointer++)
	{
		if (work_pointer->summ > summa)
		{
			printf("%-20s%-20s%-20d%-20.2lf\n", work_pointer->fam, work_pointer->name, work_pointer->account, work_pointer->summ);
			kol++;
		}
	}
	if (kol == 0)
		printf("%s%lf\n", "Нет клиентов с суммой на счёте больше ",summa);
}

void sort(bank* start)//сортировка по алфавиту
{
	bank* new_records = (bank*)calloc(N, sizeof(bank));
	bank* x=(bank*)calloc(1,sizeof(bank));
	for (int i = 0; i < N; i++)
	{
		*(new_records + i) = *(start + i);
	}
	for(int i=0;i<N;i++)
		for (int j = 0; j < N-i-1; j++)
		{
			if (strcmp((new_records + j)->fam, (new_records + j + 1)->fam) > 0)
			{
				*x = *(new_records + j);
				*(new_records + j) = *(new_records + j + 1);
				*(new_records + j + 1) = *x;
			}

		}
	print_bank(new_records, N);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	FILE *f;
	bank* Records = NULL;

	//инициализация
	if ((f = fopen("Data_Base.txt", "r")) == NULL)
	{
		Records = init_bank();//инициализация с клавиатуры
	}
	else 
	{
		Records = read_from_file(f);
		fclose(f);
	}

	int menu;
	while (true)
	{
		printf("\n%s\n%s\n%s\n%s\n%s\n%s\n\n", "Добавить новую запись\t\t...1", "Распечатать базу товаров\t...2", "Поиск клиента по фамилии\t...3", "Фильтр по сумме на счёте\t...4", "Сортировать по алфавиту\t\t...5", "Выход из программы\t\t...6");
		read_int(&menu);
		switch (menu)
		{
		case 1: {Records=new_bank(Records); continue; }
		case 2: {print_bank(Records,N); continue; }
		case 3: {find_fam(Records); continue; }
		case 4: {select_summ(Records); continue; }
		case 5: {sort(Records); continue; }
		case 6: 
		{
			f = fopen("Data_Base.txt", "w");
			print_bank_in_file(Records, N, f);
			fclose(f);
			return 0;
		}
		default: break;
		}
	}
}



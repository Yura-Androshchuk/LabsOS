#include<stdio.h>
#include <string>
#include <windows.h>
using namespace std;
//void new_func1(void); функція не використовується

bool func1()// функція змінена на булеву та змінено значення яке вона повертає  
{
	for (int i = 0; i < 10; i++) // зменшена кількість ітерацій
	{
		Sleep(100);              // зменшена триванлість з 166 хвилин до 1 секунди
	}
	return true;
}

bool func2(int a) // функція змінена на булеву та змінена значення яке вона повертає
{
	for (int i = 0; i < 10; i++)
	{
		Sleep(1);
		if (i > a)
			return true;
	}
	return false;
}

int main(void)
{
	printf("\n Inside main()\n");
	//string typeOfShape;        змінна яка ніде не використовується
	//scanf_s("%s", typeOfShape);
	int i = 0;
		for (int j = 0; j < 10; j++)// відсутня необхідність у додатковому циклі
		{
			if (func1() || func2(i)) {      //перевірка завжди працює бо 1 функція повертає true
				printf("\n Inside if()\n");
				std::printf("%d",i);
			}
			i++; // значення змінної почало змінюватись і має сенс перевірка у 2 фунуії
		}
	return 0;
}
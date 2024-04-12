#include "LSM.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void GoToXY(short x, short y)
{
	SetConsoleCursorPosition(hStdOut, { x, y });
}

void ConsoleCursorVisible(bool show, short size)
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdOut, &structCursorInfo);
	structCursorInfo.bVisible = show;
	structCursorInfo.dwSize = size;
	SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleTitle(L"МНК");
	ConsoleCursorVisible(false, 100);

	LSM test;

	int active_menu = 0;
	int keyInput;
	bool exitProg = false;

	while (!exitProg)
	{
		int x = 45, y = 10;
		GoToXY(x, y);

		vector<string> menu =
		{
			"\t\tВвод данных вручную",
			"\t\tВвод данных из файла",
			"\t\tВывод данных на экран",
			"\t\t  Очистка данных",
			"Рассчет параметров линейной зависимости",
			"Рассчет параметров квадратичной зависимости",
			"Рассчет параметров степенной зависимости",
			"Рассчет параметров экспоненциальной зависимости",
			"Рассчет параметров дробно-линейной зависимости",
			"Рассчет параметров логарифмической зависимости",
			"Рассчет параметров обратно пропорциональной зависимости",
			"Рассчет параметров дробно-рациональной зависимости",
			"\t\t\tВыход"
		};

		for (int i = 0; i < menu.size(); i++)
		{
			if (i == active_menu)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
			}

			GoToXY(x, y++);
			cout << menu[i] << endl;
		}

		keyInput = _getch();
		switch (keyInput)
		{
			case KEY_ESC:
				exit(0);

			case KEY_UP:
			{
				if (active_menu > 0)
				{
					active_menu--;
				}
			}
			break;

			case KEY_DOWN:
			{
				if (active_menu < menu.size() - 1)
				{
					active_menu++;
				}
			}
			break;

			case KEY_ENTER:
			{
				switch (active_menu)
				{
					case 0:
					{
						system("cls");

						test.manualInput();

						test.printQuit();
					}
					break;

					case 1:
					{
						system("cls");

						test.fileInput();

						test.printQuit();
					}
					break;

					case 2:
					{
						system("cls");

						test.printData();

						test.printQuit();
					}
					break;

					case 3:
					{
						system("cls");

						test.deleteData();

						test.printQuit();
					}
					break;

					case 4:
					{
						system("cls");

						test.linearFunc();

						test.printQuit();
					}
					break;

					case 5:
					{
						system("cls");

						test.squareFunc();

						test.printQuit();
					}
					break;

					case 6:
					{
						system("cls");

						test.powFunc();

						test.printQuit();
					}
					break;

					case 7:
					{
						system("cls");

						test.expFunc();

						test.printQuit();
					}
					break;

					case 8:
					{
						system("cls");

						test.fractLinFunc();

						test.printQuit();
					}
					break;

					case 9:
					{
						system("cls");

						test.lnFunc();

						test.printQuit();
					}
					break;

					case 10:
					{
						system("cls");

						test.inverseProportFunc();

						test.printQuit();
					}
					break;

					case 11:
					{
						system("cls");

						test.inverseProportLinFunc();

						test.printQuit();
					}
					break;

					case 12:
					{
						system("cls");
						exitProg = true;
					}
					break;
				}
			}
			break;
		}
	}
}
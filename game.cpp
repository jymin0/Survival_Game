// #include
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

// #define
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4 // 선택(스페이스바)


#define PLAYER '2'
#define WATER '3'
#define FOOD '4'

// color seting
enum {
	black,
	blue,
	green,
	cyan,
	red,
	purple,
	brown,
	lightgray,
	darkgray,
	lightblue,
	lightgreen,
	lightcyan,
	lightred,
	lightpurple,
	yellow,
	white
};
char maps[10][32][32];
char initmap[32][32] = {
	// 0: 빈공간, 1: 벽, *; 문, #: 벽(문o), @: 벽(문x), +:표시선
	// 게임 내 공간 [2][2]~[29][29]
	{"0000000000000#***#0000000000000"},
	{"0111111111111#@@@#1111111111110"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"##000000000000000000000000000##"},
	{"*@000000000000000000000000000@*"},
	{"*@000000000000000000000000000@*"},
	{"*@000000000000000000000000000@*"},
	{"##000000000000000000000000000##"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0100000000000000000000000000010"},
	{"0111111111111#@@@#1111111111110"},
	{"0000000000000#***#0000000000000"},
};
short pX;
short pY;

// funcions list
void init();
void gotoxy(short, short);
void setColor(short, short);
void titleDraw();
short menuDraw();
short keyControl();
void infoDraw();
void randomObjectMaking(char, char *);
void creatmap(short);
void drawMap(short);

// main function
int main()
{
	int menuCode = -1;
	init();
	while (1)
	{
		titleDraw();
		menuCode = menuDraw();

		if (menuCode == 0)
		{
			creatmap(0);
			Sleep(500);
			drawMap(0);
			_getch();
		}
		else if (menuCode == 1)
		{
			infoDraw();
		}
		else if (menuCode == 2)
		{
			break;
		}
		menuCode = -1;
		system("cls");
	}
	gotoxy(12, 16);
	printf("게임이 종료되었습니다!!!");
	return _getch();
}

// function seting
void init()
{
	system("mode con cols=104 lines=40 | title survival game");
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}
void gotoxy(short x, short y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}
void setColor(short forground, short background)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들가져오기 
	int code = forground + background * 16;
	SetConsoleTextAttribute(consoleHandle, code);
}
void titleDraw()
{

}
short menuDraw()
{
	int x = 24;
	int y = 12;
	gotoxy(x - 2, y); // -2 한 이유는 > 를 출력해야하기 때문에  
	printf("> 게임시작");
	gotoxy(x, y + 1);
	printf("게임정보");
	gotoxy(x, y + 2);
	printf("  종료  ");
	while (1) { // 무한반복  
		int n = keyControl(); // 키보드 이벤트를 키값으로 받아오기  
		switch (n) {
		case UP: { // 입력된 키의 값이 UP인경우 (w) 
			if (y > 12) { // y는 12~14까지만 이동  
				gotoxy(x - 2, y); // x-2 하는 이유는 ">"를 두칸 이전에 출력하기위해  
				printf(" "); // 원래 위치를 지우고  
				gotoxy(x - 2, --y); // 새로 이동한 위치로 이동하여  
				printf(">"); // 다시 그리기  
			}
			break;
		}

		case DOWN: { // 입력된 키의 값이 DOWN인경우 (s) 
			if (y < 14) { // 최대 14  
				gotoxy(x - 2, y);
				printf(" ");
				gotoxy(x - 2, ++y);
				printf(">");
			}
			break;
		}

		case SUBMIT: {
			return y - 12; // 스페이스바(선택)되었을 경우 y-12
			// y 시작 위치가 12였으므로 y-12 를 하면 0, 1, 2 세 숫자를 받을 수 있다. 
		}
		}
	}
}
short keyControl() {
	char temp = _getch();

	if (temp == 'w' || temp == 'W')
	{
		return UP;
	}
	else if (temp == 'a' || temp == 'A')
	{
		return LEFT;
	}
	else if (temp == 's' || temp == 'S')
	{
		return DOWN;
	}
	else if (temp == 'd' || temp == 'D')
	{
		return RIGHT;
	}
	else if (temp == ' ')
	{ // 스페이스바(공백)이 선택 버튼  
		return SUBMIT;
	}
}
void infoDraw()
{

}
void creatmap(short index)
{
	memcpy(maps[index], initmap, sizeof(initmap));
	srand(time(NULL));
	char* changePoint = (char*)&maps + index * 32 * 32;
	for (int i = 0; i < 4; i++)
	{
		randomObjectMaking(WATER, changePoint);
	}
	for (int i = 0; i < 4; i++)
	{
		randomObjectMaking(FOOD, changePoint);
	}
	randomObjectMaking(PLAYER, changePoint);

}
void randomObjectMaking(char object, char *map)
{
	short posx, posy;
	while (1)
	{
		srand(rand());
		posx = 2 + rand() % 28;
		posy = 2 + rand() % 28;
		char *p = map+posx * 32;
		if (p[posy] == '0')
			p[posy] = object;
			break;
	}
}
void drawMap(short pos)
{
	system("cls");
	int h, w;
	char temp;
	for (h = 0; h < 32; h++)
	{
		for (w = 0; w < 32; w++)
		{
			temp = maps[pos][h][w];
			if (temp == '0')
			{
				setColor(black, black);
				printf("   ");
			}
			else if (temp == '1')
			{
				setColor(white, white);
				printf("   ");
			}
			else if (temp == PLAYER)
			{
				//플레이어 X,Y 위치 저장
				pX = w;
				pY = h;
				setColor(cyan, black);
				printf(" @ ");
			}
			else if (temp == WATER)
			{
				setColor(black, cyan);
				printf(" W ");
			}
			else if (temp == FOOD)
			{
				setColor(black, lightgreen);
				printf(" F ");
			}
			else
			{
				setColor(white, white);
				printf("   ");
			}
		}
		setColor(black, black);
		printf("\n");
	}
	setColor(white, black);
}
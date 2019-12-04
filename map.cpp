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

#define WALL '1'
#define DOORWALL '#'
#define PLAYER '2'
#define WATER '3'
#define FOOD '4'
#define DOOR '*'
#define NEXT 'N'

#define EAST 0
#define WAST 1
#define SOUTH 2
#define NORTH 3
#define MAPNUMBER 4
#define DOOROPEN 5

#define MAPSIZE 10

// setting
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
char maps[MAPSIZE][32][32];
char defaultmap[32][32] = {
	// 0: 빈공간, 1: 벽, *; 문, #: 벽(문o), @: 벽(문x), +:표시선
	// 게임 내 공간 [2][2]~[29][29], 정중앙 [15][15]
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
	{"0100000000000000000000000000010"},
	{"0111111111111#@@@#1111111111110"},
	{"0000000000000#***#0000000000000"},
};
int infomap[MAPSIZE][6];
int resetmap[MAPSIZE][6];
int minimap[MAPSIZE][2] = { 0 };
int resetminimap[MAPSIZE][2] = { 0 };

int pX = 0;
int pY = 0;

// funcions list
void init();
void gotoxy(int, int);
void setColor(int, int);
void titleDraw();
int menuDraw();
int keyControl();
void infoDraw();
void randomObjectMaking(char, char*);
void creatmap();
void checkrotatemap(int);
void creatminimap();
void drawMap(int);
void drawMiniMap(int);
int mobile(char);
int move(int, int, int, int);

// main function
int main()
{
	srand(time(NULL));
	int menuCode = -1;
	init();
	while (1)
	{
		titleDraw();
		menuCode = menuDraw();
		if (menuCode == 0)
		{
			creatmap();
			creatminimap();
			Sleep(500);
			system("cls");
			for (int x = 0; x < MAPSIZE; x++)
			{
				printf("\n%d\n", x);
				printf("EAST= %d, ", infomap[x][EAST]);
				printf("WAST= %d, ", infomap[x][WAST]);
				printf("SOUTH= %d, ", infomap[x][SOUTH]);
				printf("NORTH= %d, ", infomap[x][NORTH]);
				printf("x=%d, y=%d", minimap[x][0], minimap[x][1]);
			}
			_getch();
			drawMap(0);
			drawMiniMap(0);
			int mKey;
			int playing = 1;
			int mappos = 0;
			while (playing)
			{
				mKey = keyControl();
				switch (mKey)
				{
				case UP:
					mappos = move(PLAYER, mappos, 0, -1);
					break;
				case DOWN:
					mappos = move(PLAYER, mappos, 0, 1);
					break;
				case RIGHT:
					mappos = move(PLAYER, mappos, 1, 0);
					break;
				case LEFT:
					mappos = move(PLAYER, mappos, -1, 0);
					break;
				case SUBMIT:
					playing = 0;
				}
			}
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
	_getch();
	return 0;
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

	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			resetmap[x][y] = -1;
		}
	}
	for (int x = 0; x < MAPSIZE; x++)
	{
		resetminimap[x][0] = 0;
		resetminimap[x][1] = 0;
	}
}
void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}
void setColor(int forground, int background)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들가져오기 
	int code = forground + background * 16;
	SetConsoleTextAttribute(consoleHandle, code);
}
void titleDraw()
{
	printf("\n\n\n\n"); // 맨위에 4칸 개행  
	printf("    00OOOOOOOOOOO000000000000000000O000000000000000O000                                   \n");
	printf("    0           O     OOOOOOOOOOO  O    OOOOOOOOO  O  0                                   \n");
	printf("    0           O          O       O    O       O  O  0                                   \n");
	printf("    0OOOOOOOOOOOOO        O O      O    O       O  O  0                                   \n");
	printf("    0      O             O   O     O    O       O  OOOO                                   \n");
	printf("    0OOOOO0 OOOOO       O     O    O    O       O  O  0                                   \n");
	printf("    0     0 O   O      O       O   O    O       O  O  0                                   \n");
	printf("    0OOOOOO O   O     O         O  O    OOOOOOOOO  O  0                                   \n");
	printf("    0O      O   O                  O               O  0                                   \n");
	printf("    0OOOOOO0OOOOO000000000000000000O000000000000000O000                                   \n");
}
int menuDraw()
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
int keyControl() {
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
void creatmap()
{
	int h = 0, w = 0;
	srand(rand());
	int index = 0, moveMap = 0;
	memcpy(infomap, resetmap, sizeof(resetmap));
	memcpy(maps[index], defaultmap, sizeof(defaultmap));
	// 초기 맵 설정
	pX = 20;
	pY = 20;
	maps[0][20][20] = PLAYER;
	infomap[0][MAPNUMBER] = 0;

	for (index = 1; index < MAPSIZE; index++)
	{
		int mapNumber, find = 1;
		memcpy(maps[index], defaultmap, sizeof(defaultmap));
		infomap[index][MAPNUMBER] = index;
		mapNumber = rand() % index;
		while (find)
		{
			moveMap = rand() % 4;
			if (infomap[mapNumber][moveMap] != -1)
			{
				mapNumber = infomap[mapNumber][moveMap];
				continue;
			}
			else
			{
				infomap[mapNumber][moveMap] = index;
				if (moveMap == EAST)
				{
					if (w < 8)
					{
						find = 0, w++;
						infomap[index][WAST] = mapNumber;
						checkrotatemap(index);
					}
					else
						continue;
				}
				if (moveMap == WAST)
				{
					if (w < 8)
					{
						find = 0, w++;
						infomap[index][EAST] = mapNumber;
						checkrotatemap(index);
					}
					else
						continue;
				}
				if (moveMap == SOUTH)
				{
					if (h < 8)
					{
						find = 0, h++;
						infomap[index][NORTH] = mapNumber;
						checkrotatemap(index);
					}
					else
						continue;
				}
				if (moveMap == NORTH)
				{
					if (h < 8)
					{
						find = 0, h++;
						infomap[index][SOUTH] = mapNumber;
						checkrotatemap(index);
					}
					else
						continue;
				}
			}
		}

		int i, fooditem = 0, wateritem = 0;
		fooditem = rand() % 5, wateritem = rand() % 5;
		for (i = 0; i < fooditem; i++)
		{
			randomObjectMaking(FOOD, (char*)maps[index]);
		}
		for (i = 0; i < fooditem; i++)
		{
			randomObjectMaking(WATER, (char*)maps[index]);
		}
	}
	int next = 1 + rand() % (MAPSIZE - 1);
	randomObjectMaking(NEXT, (char*)maps[next]);
	infomap[next][DOOROPEN] = 1;

}
void checkrotatemap(int index)
{
	int E, W, S, N;
	//EAST
	S = infomap[index][SOUTH];
	if (S != -1)
	{
		E = infomap[S][EAST];
		if (E != -1)
		{
			N = infomap[E][NORTH];
			if (N != -1)
			{
				infomap[N][WAST] = index;
				infomap[index][EAST] = N;
			}
		}
	}
	N = infomap[index][NORTH];
	if (N != -1)
	{
		E = infomap[N][EAST];
		if (E != -1)
		{
			S = infomap[E][SOUTH];
			if (S != -1)
			{
				infomap[S][WAST] = index;
				infomap[index][EAST] = S;
			}
		}
	}
	//WAST
	S = infomap[index][SOUTH];
	if (S != -1)
	{
		W = infomap[S][WAST];
		if (W != -1)
		{
			N = infomap[W][NORTH];
			if (N != -1)
			{
				infomap[N][EAST] = index;
				infomap[index][WAST] = N;
			}
		}
	}
	N = infomap[index][NORTH];
	if (N != -1)
	{
		W = infomap[N][WAST];
		if (W != -1)
		{
			S = infomap[W][SOUTH];
			if (S != -1)
			{
				infomap[S][EAST] = index;
				infomap[index][WAST] = S;
			}
		}
	}
	//SOUTH
	E = infomap[index][EAST];
	if (E != -1)
	{
		S = infomap[E][SOUTH];
		if (S != -1)
		{
			W = infomap[S][WAST];
			if (N != -1)
			{
				infomap[W][NORTH] = index;
				infomap[index][SOUTH] = W;
			}
		}
	}
	W = infomap[index][WAST];
	if (W != -1)
	{
		S = infomap[W][SOUTH];
		if (S != -1)
		{
			E = infomap[S][EAST];
			if (E != -1)
			{
				infomap[E][NORTH] = index;
				infomap[index][SOUTH] = E;
			}
		}
	}
	//NORTH
	E = infomap[index][EAST];
	if (E != -1)
	{
		N = infomap[E][NORTH];
		if (N != -1)
		{
			W = infomap[N][WAST];
			if (N != -1)
			{
				infomap[W][SOUTH] = index;
				infomap[index][NORTH] = W;
			}
		}
	}
	W = infomap[index][WAST];
	if (W != -1)
	{
		N = infomap[W][NORTH];
		if (N != -1)
		{
			E = infomap[N][EAST];
			if (E != -1)
			{
				infomap[E][SOUTH] = index;
				infomap[index][NORTH] = E;
			}
		}
	}
}
void creatminimap()
{
	memcpy(minimap, resetminimap, sizeof(resetminimap));
	int index, stop = 1;
	int mapNumber[MAPSIZE] = { 1 };
	minimap[0][0] = 0, minimap[0][1] = 0;
	if (infomap[0][EAST] != -1)
		minimap[(infomap[0][EAST])][0]++, mapNumber[(infomap[0][EAST])] = 1, stop++;
	if (infomap[0][WAST] != -1)
		minimap[(infomap[0][WAST])][0]--, mapNumber[(infomap[0][WAST])] = 1, stop++;
	if (infomap[0][SOUTH] != -1)
		minimap[(infomap[0][SOUTH])][1]--, mapNumber[(infomap[0][SOUTH])] = 1, stop++;	
	if (infomap[0][NORTH] != -1)
		minimap[(infomap[0][NORTH])][1]++, mapNumber[(infomap[0][NORTH])] = 1, stop++;
	while (1)
	{
		if (stop == MAPSIZE)
		{
			return;
		}
		for (index = 1; index < MAPSIZE; index++)
		{
			if (!mapNumber[index])
			{
				int E = infomap[index][EAST];
				int W = infomap[index][WAST];
				int S = infomap[index][SOUTH];
				int N = infomap[index][NORTH];
				if (E != -1)
				{
					if (mapNumber[E])
					{
						memcpy(minimap[index], minimap[E], sizeof(minimap[index]));
						minimap[index][0]--;
						mapNumber[index] = 1;
						stop++;
						continue;
					}
				}
				if (W != -1)
				{
					if (mapNumber[W])
					{
						memcpy(minimap[index], minimap[W], sizeof(minimap[index]));
						minimap[index][0]++;
						mapNumber[index] = 1;
						stop++;
						continue;
					}
				}
				if (S != -1)
				{
					if (mapNumber[S])
					{
						memcpy(minimap[index], minimap[S], sizeof(minimap[index]));
						minimap[index][1]++;
						mapNumber[index] = 1;
						stop++;
						continue;
					}
				}
				if (N != -1)
				{
					if (mapNumber[N])
					{
						memcpy(minimap[index], minimap[N], sizeof(minimap[index]));
						minimap[index][1]--;
						mapNumber[index] = 1;
						stop++;
						continue;
					}
				}
			}
		}
	}
	

}
void randomObjectMaking(char object, char* map)
{
	int posx, posy;
	while (1)
	{
		srand(rand());
		posx = 2 + rand() % 28;
		posy = 2 + rand() % 28;
		char* p = map + posx * 32;
		if (p[posy] == '0')
			p[posy] = object;
		break;
	}
}
void drawMap(int pos)
{
	system("cls");
	int h, w;
	char temp;
	for (h = 0; h < 32; h++)
	{
		for (w = 0; w < 32; w++)
		{
			gotoxy(2 * h, w);
			temp = maps[pos][h][w];
			if (temp == '0' || temp == NULL)
			{
				setColor(black, black);
				printf("  ");
			}
			else if (temp == '1')
			{
				setColor(white, white);
				printf("  ");
			}
			else if (temp == PLAYER)
			{
				//플레이어 X,Y 위치 저장
				setColor(cyan, black);
				printf("@@");
			}
			else if (temp == WATER)
			{
				setColor(black, cyan);
				printf("WW");
			}
			else if (temp == FOOD)
			{
				setColor(black, lightgreen);
				printf("FF");
			}
			else if (temp == NEXT)
			{
				setColor(lightpurple, black);
				printf("▣");
			}
			else
			{
				setColor(white, white);
				printf("  ");
			}
		}
		setColor(black, black);
	}
	setColor(white, black);
	gotoxy(60, 0);
	printf("pos=%2d ", pos);

	gotoxy(62, 1);
	printf("EAST=%d ", infomap[pos][EAST]);
	gotoxy(62, 2);
	printf("WAST=%d ", infomap[pos][WAST]);
	gotoxy(62, 3);
	printf("SOUTH=%d ", infomap[pos][SOUTH]);
	gotoxy(62, 4);
	printf("NORTH=%d ", infomap[pos][NORTH]);

}
void drawMiniMap(int pos)
{
	setColor(yellow, yellow);
	gotoxy(80, 20);
	printf("  ");
	for (int index = 1; index < MAPSIZE; index++)
	{
		if (infomap[index][DOOROPEN] == 1)
		{
			setColor(black, lightpurple);
			gotoxy(80 + 4 * minimap[index][0], 20 - 2 * minimap[index][1]);
			printf("▣");
		}
		else
		{
			setColor(green, green);
			gotoxy(80 + 4 * minimap[index][0], 20 - 2 * minimap[index][1]);
			printf("  ");
		}
	}
	if (pos == 0)
	{
		setColor(cyan, yellow);
		gotoxy(80, 20);
		printf("●");
	}
	else if (infomap[pos][DOOROPEN] == 1)
	{
		setColor(black, lightpurple);
		gotoxy(80 + 4 * minimap[pos][0], 20 - 2 * minimap[pos][1]);
		printf("●");
	}
	else
	{
		setColor(cyan, red);
		gotoxy(80 + 4 * minimap[pos][0], 20 - 2 * minimap[pos][1]);
		printf("●");
	}
	setColor(white, black);

}
int mobile(char object)
{
	switch (object)
	{
	case WALL:
	case DOORWALL:
	case PLAYER:
	case FOOD:
	case WATER:
		return 0;
	case '*':
		return 2;
	default:
		return 1;
	}
}
int move(int moveObject, int mappos, int x, int y)
{
	gotoxy(34, 34);
	printf("%d, %d", pX, pY);
	char object = maps[mappos][pX + x][pY + y];
	int t = mobile(object);
	if (t == 1)
	{
		//maps[mappos][pX + x][pY + y] == object;
		maps[mappos][pX][pY] = '0';

		setColor(white, black);
		gotoxy(2 * pX, pY);
		printf("  ");

		setColor(cyan, black);
		gotoxy(2 * (pX + x), pY + y);
		printf("@@");
		pX += x;
		pY += y;
		return mappos;
	}
	else if (t == 2)
	{
		int newmappos;
		int moveNewMap = 0;
		if (pY + y == 30) // SOUTH
		{
			newmappos = infomap[mappos][SOUTH];
			if (newmappos != -1)
			{
				maps[mappos][pX][pY] = '0';
				pX = 14, pY = 2;
				moveNewMap = 1;
			}
			else
				return mappos;
		}
		if (pY + y == 0) // NORTH
		{
			newmappos = infomap[mappos][NORTH];
			if (newmappos != -1)
			{
				maps[mappos][pX][pY] = '0';
				pX = 14, pY = 28;
				moveNewMap = 1;
			}
			else
				return mappos;
		}
		if (pX + x == 31) // EAST
		{
			newmappos = infomap[mappos][EAST];
			if (newmappos != -1)
			{
				maps[mappos][pX][pY] = '0';
				pX = 2, pY = 15;
				moveNewMap = 1;
			}
			else
				return mappos;
		}
		if (pX + x == 0) // WAST
		{
			newmappos = infomap[mappos][WAST];
			if (newmappos != -1)
			{
				maps[mappos][pX][pY] = '0';
				pX = 28, pY = 15;
				moveNewMap = 1;
			}
			else
				return mappos;
		}
		if (moveNewMap == 1)
		{
			maps[newmappos][pX][pY] = PLAYER;
			drawMap(newmappos);
			drawMiniMap(newmappos);
			return newmappos;
		}
	}
	return mappos;

}
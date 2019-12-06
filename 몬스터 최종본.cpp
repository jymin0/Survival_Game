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
#define MONSTER '5'
#define MEDICKIT '+'

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

struct monster
{
	int mappos;
	int posx;
	int posy;
};

monster list_MON[20] = { 0 };

int pX = 0;
int pY = 0;
int pHealth = 10;
int pFood = 10;
int pWater = 10;
int end = 0;
int pos_map = 0;
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
void drawUI(int pX, int pY, int pHealth, int pFood, int pWater);
void drawStart();
void MONSTER_create_check(char*, monster list_MON[20], int, int*);
void monster_move(int mappos, int posx, int posy);
void mm(monster m[20], int mappos);

// main function
int main()
{
	srand(time(NULL));
	int menuCode = -1;
	int j;
	int k;
	char string2[30] = { '-','-','-','-','-','-','-','-','-','-','-','B','y','e',' ','B','y','e','-','-','-','-','-','-','-','-','-','-','-','-' };
	init();
	while (1)
	{
		titleDraw();
		menuCode = menuDraw();
		if (menuCode == 0)
		{
			end = 0;
			int playing = 1;
			while (playing)
			{
				list_MON[20] = { 0 };
				creatmap();
				creatminimap();
				Sleep(500);
				system("cls");
				drawMap(0);
				drawMiniMap(0);
				drawStart();
				setColor(white, black);
				gotoxy(28, 10);
				Sleep(300);
				int mKey;
				int mappos = 0;
				pos_map = mappos;
				int cHealth = 0;
				int cFood = 0;
				int cWater = 0;
				pHealth = 10;
				pFood = 10;
				pWater = 10;
				int i;
				char string[10] = { 'G','A','M','E',' ','O','V','E','R' };
				while (playing)
				{
					mm(list_MON,mappos);
					mKey = keyControl();
					switch (mKey)
					{
					case UP:
						mappos = move(PLAYER, mappos, 0, -1);
						cFood++;
						cWater++;
						break;
					case DOWN:
						mappos = move(PLAYER, mappos, 0, 1);
						cFood++;
						cWater++;
						break;
					case RIGHT:
						mappos = move(PLAYER, mappos, 1, 0);
						cFood++;
						cWater++;
						break;
					case LEFT:
						mappos = move(PLAYER, mappos, -1, 0);
						cFood++;
						cWater++;
						break;
					case SUBMIT:
						playing = 0;
						end = 1;
					}
					if (cFood % 15 == 0)
					{
						if (pFood > 0) {
							if (pWater > 0) {
								pFood -= 1;
								if (pHealth < 10) {
									pHealth += 1;
								}
							}
							else if (pWater == 0) {
								pFood -= 1;
							}
						}
						else if (pFood == 0) {
							pHealth -= 1;
						}
					}
					if (cWater % 10 == 0)
					{
						if (pWater > 0) {
							pWater -= 1;
						}
						else if (pWater == 0) {
							pHealth -= 1;
						}
					}
					drawUI(pX, pY, pHealth, pFood, pWater);
					if (pHealth == 0)
					{
						Sleep(200);
						setColor(white, black);
						gotoxy(27, 10);
						for (i = 0; i < 9; i++)
						{
							printf("%c", string[i]);
							Sleep(250);
						}
						gotoxy(27, 10);
						for (i = 0; i < 9; i++)
						{
							printf(" ");
							Sleep(150);
						}
						Sleep(300);
						playing = 0;
						end = 1;
					}
					if (mappos == 'N')
					{
						break;
					}
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
	gotoxy(14, 16);
	for (j = 0; j < 29; j++)
	{
		gotoxy(14 + j, 16);
		printf("%c", string2[j]);
		Sleep(100);
		if (j >= 5) {
			gotoxy(14 + j - 5, 16);
			printf(" ");
		}
	}
	gotoxy(36, 16);
	for (k = 0; k <= 6; k++)
	{
		gotoxy(36 + k, 16);
		printf(" ");
		Sleep(100);
	}

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
	setColor(white, black);
	printf("\n\n\n"); // 맨위에 4칸 개행
	printf("                                                              \n");
	printf("   ■■■■■■■    ■■■■■■■  ■   ■■■■■■  ■    \n");
	printf("               ■          ■        ■   ■        ■  ■    \n");
	printf("               ■         ■■       ■   ■ ●  ● ■  ■    \n");
	printf(" ■■■■■■■■■      ■  ■      ■   ■        ■  ■    \n");
	printf("         ■             ■    ■     ■   ■        ■  ■■  \n");
	printf("   ■■■■■■■      ■      ■    ■   ■        ■  ■    \n");
	printf("         ■    ■     ■        ■   ■   ■        ■  ■    \n");
	printf("   ■■■■    ■    ■          ■  ■   ■        ■  ■    \n");
	printf("   ■    ■    ■   ■            ■ ■   ■   ▼   ■  ■    \n");
	printf("   ■■■■■■■  ■              ■■   ■■■■■■  ■    \n");
	printf("                                                              \n");
}
int menuDraw()
{
	int x = 24;
	int y = 12;
	gotoxy(x - 2, y); // -2 한 이유는 > 를 출력해야하기 때문에  
	printf("☞ 게임시작");
	gotoxy(x, y + 1);
	printf(" 게임정보");
	gotoxy(x, y + 2);
	printf("   종료  ");
	while (1) { // 무한반복  
		int n = keyControl(); // 키보드 이벤트를 키값으로 받아오기  
		switch (n) {
		case UP: { // 입력된 키의 값이 UP인경우 (w) 
			if (y > 12) { // y는 12~14까지만 이동  
				gotoxy(x - 2, y); // x-2 하는 이유는 ">"를 두칸 이전에 출력하기위해  
				printf(" "); // 원래 위치를 지우고  
				gotoxy(x - 2, --y); // 새로 이동한 위치로 이동하여  
				printf("☞"); // 다시 그리기  
			}
			break;
		}

		case DOWN: { // 입력된 키의 값이 DOWN인경우 (s) 
			if (y < 14) { // 최대 14  
				gotoxy(x - 2, y);
				printf(" ");
				gotoxy(x - 2, ++y);
				printf("☞");
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
	system("cls"); // 화면 모두 지우기  
	printf("\n\n");
	printf("                        [ 조작법 ]\n\n");
	printf("                      이동: W, A, S, D\n");
	printf("                      선택: 스페이스바\n\n\n\n\n");
	printf("              개발자: jyoh5005, yandusty, jymin0\n");
	printf("         스페이스바를 누르면 메인화면으로 이동합니다.");

	while (1) {
		if (keyControl() == SUBMIT) {
			break;
		}
	}

}
void creatmap()
{
	int mob_num = 0;
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

		int i, fooditem = 0, wateritem = 0, monster = 0;
		fooditem = rand() % 9, wateritem = rand() % 9, monster = rand() % 3;
		for (i = 0; i < fooditem; i++)
		{
			randomObjectMaking(FOOD, (char*)maps[index]);
		}
		for (i = 0; i < fooditem; i++)
		{
			randomObjectMaking(WATER, (char*)maps[index]);
		}
		for (i = 0; i < monster; i++)
		{
			MONSTER_create_check((char*)maps[index], list_MON, index, &mob_num);
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

void MONSTER_create_check(char* map, monster list_MON[20], int mappos, int* mob_num)
{
	int posx, posy;
	while (1)
	{
		srand(rand());
		posx = 2 + rand() % 28;
		posy = 2 + rand() % 28;
		char* p = map + posx * 32;
		if (p[posy] == '0')
		{
			list_MON[*mob_num].mappos = mappos;
			list_MON[*mob_num].posx = posx;
			list_MON[*mob_num].posy = posy;
			p[posy] = MONSTER;
			*mob_num += 1;
			break;
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
			else if (temp == MONSTER)
			{
				setColor(white, red);
				printf("MM");
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
	case MONSTER:
		return 0;
	case FOOD:
	case WATER:
	case MEDICKIT:
		return 3;
	case NEXT:
		return 'N';
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
	else if (t == 3)
	{
		maps[mappos][pX + x][pY + y] = '0';
		if (object == WATER)
		{
			pWater = 10;
		}
		else if (object == FOOD)
		{
			pFood = 10;
		}
		else if (object == MEDICKIT)
			pHealth += 2;
		setColor(white, black);
		gotoxy(2 * pX, pY);
		printf("  ");

		setColor(cyan, black);
		gotoxy(2 * (pX + x), pY + y);
		printf("@@");
		pX += x;
		pY += y;
	}
	else if (t == 'N')
	{
		return 'N';
	}
	return mappos;
}
void drawUI(int pX, int pY, int pHealth, int pFood, int pWater)
{
	int i;
	setColor(red, black);
	gotoxy(34, 30);
	printf(" H P  〔");
	for (i = 0; i < pHealth; i++) {
		printf("♥");
	}
	for (i = 0; i < 10 - pHealth; i++) {
		printf("♡");
	}
	printf("〕");

	setColor(brown, black);
	gotoxy(34, 31);
	printf("Hunger〔");
	for (i = 0; i < pFood; i++) {
		printf("♣");
	}
	for (i = 0; i < 10 - pFood; i++) {
		printf("♧");
	}
	printf("〕");

	setColor(blue, black);
	gotoxy(34, 32);
	printf("Water 〔");
	for (i = 0; i < pWater; i++) {
		printf("●");
	}
	for (i = 0; i < 10 - pWater; i++) {
		printf("○");
	}
	printf("〕");
}


void monster_move(int mappos, int posx, int posy)
{
	srand(rand());
	int choice = rand() % 2;

	if (choice == 0)
	{
	
			int x = rand() % 3 - 1;
			int y = rand() % 3 - 1;
			if (maps[mappos][posx + x][posy + y] == '0')
			{
				maps[mappos][posx][posy] = '0';

				setColor(white, black);
				gotoxy(2 * posx, posy);
				printf("  ");


				setColor(white, red);
				gotoxy(2 * (posx + x), posy + y);
				printf("MM");
				posx += x;
				posy += y;
				maps[mappos][posx][posy] = MONSTER;
		
			}
	}
	else
	{
		int x, y;
		if (posx < pX)
			x = 1;
		else
			x = -1;
		if (posy < pY)
			y = 1;
		else
			y = -1;

		if (maps[mappos][posx + x][posy + y] == '0')
		{
			maps[mappos][posx][posy] = '0';
			
			setColor(white, black);
			gotoxy(2 * posx, posy);
			printf("  ");

			setColor(white, red);
			gotoxy(2 * (posx + x), posy + y);
			printf("MM");
			posx += x;
			posy += y;
			maps[mappos][posx][posy] = MONSTER;
		}
	}
}
void drawStart()
{
	int i;
	char string_s[10] = { '=','=','S','T','A','R','T','=','=' };
	setColor(white, black);
	gotoxy(31, 10);
	printf("%c", string_s[4]);
	for (i = 3; i >= 0; i--)
	{
		gotoxy(31 - (4 - i), 10);
		printf("%c", string_s[i]);
		gotoxy(31 + (6 - i - 2), 10);
		printf("%c", string_s[8 - i]);
		Sleep(200);
	}
	for (i = 0; i <= 3; i++)
	{
		gotoxy(27 + i, 10);
		printf(" ");
		gotoxy(35 - i, 10);
		printf(" ");
		Sleep(200);
	}
	gotoxy(31, 10);
	printf(" ");
}
void mm(monster m[20],int mappos)
{
	srand(rand());
	int x, y;
	for (int i = 0; i < 20; i++)
	{
		if (mappos == m[i].mappos)
		{
			monster_move(m[i].mappos, m[i].posx, m[i].posy);
		}
	}
}
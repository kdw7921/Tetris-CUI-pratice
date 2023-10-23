#pragma warning(disable:4996)

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<string.h>

#define mapX 2
#define mapY 2
#define HEIGHT 22
#define WIDTH 11

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define Pause 80
#define pause 112
#define SPACE 32
#define ENTER 13
#define BACKSPACE 8
#define ESC 27

int map[HEIGHT][WIDTH];

int BLOCK[7][5][5] = {
{ { 0,0,0,0,0 },
{ 0,0,0,0,0 },
{ 0,0,1,1,0 },
{ 0,0,1,1,0 },
{ 0,0,0,0,0 } },

{ { 0,0,0,0,0 },
{ 0,0,1,0,0 },
{ 0,0,1,0,0 },
{ 0,0,1,0,0 },
{ 0,0,1,0,0 } },

{ { 0,0,0,0,0 },
{ 0,0,0,0,0 },
{ 0,0,1,1,0 },
{ 0,1,1,0,0 },
{ 0,0,0,0,0 } },

{ { 0,0,0,0,0 },
{ 0,0,0,0,0 },
{ 0,1,1,0,0 },
{ 0,0,1,1,0 },
{ 0,0,0,0,0 } },

{ { 0,0,0,0,0 },
{ 0,0,1,0,0 },
{ 0,1,1,1,0 },
{ 0,0,0,0,0 },
{ 0,0,0,0,0 } },

{ { 0,0,0,0,0 },
{ 0,1,1,0,0 },
{ 0,0,1,0,0 },
{ 0,0,1,0,0 },
{ 0,0,0,0,0 } },

{ { 0,0,0,0,0 },
{ 0,0,1,1,0 },
{ 0,0,1,0,0 },
{ 0,0,1,0,0 },
{ 0,0,0,0,0 } }
};

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void gotoxy(int, int);				// Set the CursorPosition
void setcursortype(CURSOR_TYPE c);	// Set the condition of Cursor

void mapInitialize();				// Initialize the Map
void drawTetris(int, int);
void Intro();
int mainDraw(int);					// Draw the Title 
void records();
void option();
void credits();
void drawMap();
void blockInitialize(int, int(*)[5][5]);
void removeBlock(int, int);
void drawBlock(int, int, int[5][5]);
bool checkBlock(int, int, int[5][5]);
void blockToMap(int, int, int[5][5]);
void checkLine(int, int*);
void removeLine(int);
bool blockRoll(int, int, int(*)[5][5]);
int Playgame();
bool checkOver();
void checkScore(int);
int EndGame();

int main()
{
	char buf[256] = { 0 };
	sprintf(buf, "mode con: lines=%d cols=%d", 25, 80);
	system(buf);
	
	setcursortype(NOCURSOR);		//Set the Cursor Invisible
	FILE* fp = fopen("score.txt", "rt");
	if (!fp) {
		fp = fopen("score.txt", "wt");
		for (int i = 0; i<40; i++)	fprintf(fp, "unknown 0\n");
	}
	fclose(fp);
	int res;
	Intro();
	while (1) {
		res = 0;
		while (1) {
			res = mainDraw(res);
			if (res == 0) break;
			else if (res == 1) records();
			else if (res == 2) option();
			else if (res == 3) credits();
			else if (res == 4) {
				res = 5;
				break;
			}
		}
		if (res == 5) break;

		while (1) {
			res = Playgame();
			if (res == 1) break;
			else if (res == 2) {
				res = 5;
				break;
			}
		}
		if (res == 5) break;
	}
	gotoxy(1, 26);
	return 0;
}
int Playgame() {
	mapInitialize();
	drawMap();
	int x, y;
	int Fx, Fy;
	int TYPE, NextTYPE;
	int ch;
	int score = 0;
	int cblock[5][5];
	bool flag = false;
	srand(time(NULL));
	NextTYPE = rand() % 7;
GAME:
	while (1) {
		if (checkOver() == true) break;
		x = WIDTH / 2 - 2;
		y = -1;
		TYPE = NextTYPE;
		srand(time(NULL) + TYPE);
		NextTYPE = rand() % 7;
		blockInitialize(TYPE, &cblock);
		drawBlock(x, y, cblock);
		for (Fx = 0; Fx<6; Fx++) {
			for (Fy = -1; Fy<6; Fy++) {
				gotoxy(mapX + WIDTH + Fx, mapY + Fy + 1);
				if (Fx == 5 || Fy == -1 || Fy == 5) printf("¢Ë");
				else if (BLOCK[NextTYPE][Fy][Fx] == 1) printf("¢Ã");
				else printf("  ");
			}
		}
		gotoxy(mapX + 1, mapY + HEIGHT); printf("score : %10d", score);
		while (1) {
			Fy = 0;
			Fx = 0;
			if (checkBlock(x, y + 1, cblock) == true) {
				blockToMap(x, y, cblock);
				score += 3;
				checkLine(y, &score);
				break;
			}
			removeBlock(x, y);
			drawBlock(x, ++y, cblock);
			for (int i = 0; i<10; i++) {
				if (kbhit()) {
					ch = getch();
					if (ch != 224) {
						if (ch == pause || ch == Pause) {
							gotoxy(mapX + 11, 12); printf("¦£------------¦¤");
							gotoxy(mapX + 11, 13); printf("¦¢            ¦¢");
							gotoxy(mapX + 11, 14); printf("¦¢  <pause>   ¦¢");
							gotoxy(mapX + 11, 15); printf("¦¢            ¦¢");
							gotoxy(mapX + 11, 16); printf("¦¦------------¦¥");
							ch = 0;
							while (ch != pause && ch != Pause)	ch = getch();
							gotoxy(mapX + 11, 12); printf("                ");
							gotoxy(mapX + 11, 13); printf("                ");
							gotoxy(mapX + 11, 14); printf("                ");
							gotoxy(mapX + 11, 15); printf("                ");
							gotoxy(mapX + 11, 16); printf("                ");
						}
						if (ch == ESC) {
							gotoxy(mapX + 11, mapY + 9);  printf("¦£------------------¦¤");
							gotoxy(mapX + 11, mapY + 10); printf("¦¢      <ESC>       ¦¢");
							gotoxy(mapX + 11, mapY + 11); printf("¦¢   Resume         ¦¢");
							gotoxy(mapX + 11, mapY + 12); printf("¦¢   Restart        ¦¢");
							gotoxy(mapX + 11, mapY + 13); printf("¦¢   Back to main   ¦¢");
							gotoxy(mapX + 11, mapY + 14); printf("¦¦------------------¦¥");
							ch = 0;
							int p;
							while (1) {
								gotoxy(mapX + 12, mapY + ch + 11);
								printf(" >");
								p = getch();
								gotoxy(mapX + 12, mapY + ch + 11);
								printf("  ");
								if (p == 224) {
									p = getch();
									if (p == UP) ch = (ch + 2) % 3;
									if (p == DOWN) ch = (ch + 1) % 3;
								}
								else if (p == ENTER) {
									if (ch == 0) {
										gotoxy(mapX + 11, mapY + 9);  printf("                      ");
										gotoxy(mapX + 11, mapY + 10); printf("                      ");
										gotoxy(mapX + 11, mapY + 11); printf("                      ");
										gotoxy(mapX + 11, mapY + 12); printf("                      ");
										gotoxy(mapX + 11, mapY + 13); printf("                      ");
										gotoxy(mapX + 11, mapY + 14); printf("                      ");
										break;
									}
									else if (ch == 1) {
										return 0;
									}
									else if (ch == 2) {
										return 1;
									}
								}
								else if (p == ESC) {
									gotoxy(mapX + 11, mapY + 10); printf("                      ");
									gotoxy(mapX + 11, mapY + 11); printf("                      ");
									gotoxy(mapX + 11, mapY + 12); printf("                      ");
									gotoxy(mapX + 11, mapY + 13); printf("                      ");
									gotoxy(mapX + 11, mapY + 14); printf("                      ");
									break;
								}
							}
						}
						else if (ch == SPACE) {
							while (checkBlock(x, y + 1 + Fy, cblock) != true) Fy++;
							score += 2;
							flag = true;
						}
					}
					else {
						ch = getch();
						switch (ch) {
						case UP: {
							if (TYPE != 0 && blockRoll(x, y, &cblock) == true) {
								removeBlock(x, y);
								drawBlock(x, y, cblock);
							}
							break;
						}
						case DOWN: {
							if (checkBlock(x, y + 1, cblock) != true) {
								Fy++;
								flag = true;
							}
							break;
						}
						case RIGHT: {
							if (checkBlock(x + 1, y, cblock) != true) {
								Fx++;
								flag = true;
							}
							break;
						}
						case LEFT: {
							if (checkBlock(x - 1, y, cblock) != true) {
								Fx--;
								flag = true;
							}
							break;
						}
						}
					}
					if (flag == true) {
						removeBlock(x, y);
						x += Fx;
						y += Fy;
						drawBlock(x, y, cblock);
						Fx = 0;
						Fy = 0;
						flag = false;
						if (checkBlock(x, y + 1, cblock) == true) break;
					}
				}
				Sleep(20);
			}
		}
	}
	//score set
	checkScore(score);
	//end
	return EndGame();
}
void mapInitialize() {
	int x, y;
	for (y = 0; y<HEIGHT; y++) {
		for (x = 0; x<WIDTH; x++) {
			if (y == 1 && x<WIDTH - 1 && x>0) map[y][x] = 3;
			else if (y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) 	map[y][x] = 2;
			else 								map[y][x] = 0;
		}
	}
	return;
}
void drawTetris(int x, int y) {
	gotoxy(x, y); 	  printf("¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã  ¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã  ¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã  ¢Ã¢Ã¢Ã        ¢Ã¢Ã    ¢Ã¢Ã¢Ã¢Ã¢Ã");
	gotoxy(x, y + 1); printf("    ¢Ã¢Ã      ¢Ã¢Ã              ¢Ã¢Ã      ¢Ã¢Ã ¢Ã¢Ã     ¢Ã¢Ã   ¢Ã¢Ã¢Ã¢Ã   ");
	gotoxy(x, y + 2); printf("    ¢Ã¢Ã      ¢Ã¢Ã              ¢Ã¢Ã      ¢Ã¢Ã    ¢Ã          ¢Ã¢Ã¢Ã      ");
	gotoxy(x, y + 3); printf("    ¢Ã¢Ã      ¢Ã¢Ã              ¢Ã¢Ã      ¢Ã¢Ã     ¢Ã   ¢Ã¢Ã  ¢Ã¢Ã¢Ã¢Ã    ");
	gotoxy(x, y + 4); printf("    ¢Ã¢Ã      ¢Ã¢Ã¢Ã¢Ã¢Ã        ¢Ã¢Ã      ¢Ã¢Ã  ¢Ã¢Ã    ¢Ã¢Ã   ¢Ã¢Ã¢Ã¢Ã   ");
	gotoxy(x, y + 5); printf("    ¢Ã¢Ã      ¢Ã¢Ã              ¢Ã¢Ã      ¢Ã¢Ã¢Ã        ¢Ã¢Ã     ¢Ã¢Ã¢Ã¢Ã ");
	gotoxy(x, y + 6); printf("    ¢Ã¢Ã      ¢Ã¢Ã              ¢Ã¢Ã      ¢Ã¢Ã ¢Ã¢Ã     ¢Ã¢Ã      ¢Ã¢Ã¢Ã¢Ã");
	gotoxy(x, y + 7); printf("    ¢Ã¢Ã      ¢Ã¢Ã              ¢Ã¢Ã      ¢Ã¢Ã   ¢Ã¢Ã   ¢Ã¢Ã     ¢Ã¢Ã¢Ã¢Ã ");
	gotoxy(x, y + 8); printf("    ¢Ã¢Ã      ¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã      ¢Ã¢Ã      ¢Ã¢Ã      ¢Ã  ¢Ã¢Ã  ¢Ã¢Ã¢Ã¢Ã¢Ã  ");
	return;
}
void Intro() {
	system("cls");
	int x = 1, y = 3;
	drawTetris(x, y);
	gotoxy(x + 12, y + 16);	printf("<press any key to start>");
	int count = 0;
	while (1) {
		Sleep(10);
		count++;
		gotoxy(x + 12, y + 16);
		if ((count % 100) == 0)    printf("<press any key to start>");
		else if ((count % 100 - 50) == 0) printf("                        ");

		if (kbhit()) {
			getch();
			gotoxy(x + 12, y + 16);	printf("                        ");
			return;
		}
	}
}
int mainDraw(int ch) {
	system("cls");
	int x = 1, y = 3;
	drawTetris(x, y);
	gotoxy(x + 12, y + 13); printf("GameStart");
	gotoxy(x + 12, y + 14); printf("Records");
	gotoxy(x + 12, y + 15); printf("Option");
	gotoxy(x + 12, y + 16); printf("Credits");
	gotoxy(x + 12, y + 17); printf("Exit");
	int p;
	while (1) {
		gotoxy(x + 11, ch + y + 13);	printf(" >");
		p = getch();
		gotoxy(x + 11, ch + y + 13);	printf("  ");
		if (p == 224) {
			p = getch();
			if (p == UP) ch = (ch + 4) % 5;
			if (p == DOWN) ch = (ch + 1) % 5;
		}
		else if (p == ENTER) {
			gotoxy(x + 11, ch + y + 13);
			printf(" >");
			if (ch == 0) {
				gotoxy(x + 17, y + 13); printf("¦£-------------------- ¦¤");
				gotoxy(x + 17, y + 14); printf("¦¢    Single Play      ¦¢");
				gotoxy(x + 17, y + 15); printf("¦¢    Local MultiPlay  ¦¢");
				gotoxy(x + 17, y + 16); printf("¦¦-------------------- ¦¥");
				while (1) {
					gotoxy(x + 18, y + 14); printf(" >");
					ch = getch();
					gotoxy(x + 18, y + 14); printf("  ");
					if (ch == 224) ch = getch();
					else if (ch == ENTER) return 0;
					else if (ch == BACKSPACE) {
						for (ch = 0; ch<4; ch++) {
							gotoxy(x + 17, y + 13 + ch); printf("                         ");
						}
						ch = 0;
						break;
					}
				}
			}
			else	return ch;
		}
	}
}
void records() {
	system("cls");
	int i;
	char name[10];
	int score;
	FILE* Rbuf = fopen("score.txt", "rt");
	for (i = 0; i<40; i++) {
		gotoxy(2 + i / 20 * 15, 2 + i % 20);
		fscanf(Rbuf, "%s %d", name, &score);
		printf("%2d) %10s %5d", i + 1, name, score);
	}
	for (i = 0; i<22; i++) {
		gotoxy(15, i);
		printf("¦¢");
	}
	for (i = 0; i<40; i++) {
		gotoxy(i, 22);
		printf("--");
	}
	fclose(Rbuf);
	gotoxy(2, 23); printf("back to main");
	int p;
	int count = 50;
	while (kbhit()) getch();
	while (1) {
		if (count % 100 == 0) {
			gotoxy(1, 23); printf("  ");
		}
		else if ((count % 100 - 50) == 0) {
			gotoxy(1, 23); printf(" >");
		}
		count++;
		if (kbhit()) {
			p = getch();
			if (p == 224) p = getch();
			else if (p == ENTER) {
				gotoxy(1, 23); printf("              ");
				return;
			}
		}
	}
}
void option() {
	system("cls");
	gotoxy(2, 3); printf("Coming Soon");
	gotoxy(2, 23); printf("back to main");
	int p;
	int count = 50;
	while (1) {
		if (count % 100 == 0) {
			gotoxy(1, 23); printf("  ");
		}
		else if ((count % 100 - 50) == 0) {
			gotoxy(1, 23); printf(" >");
		}
		count++;
		if (kbhit()) {
			p = getch();
			if (p == 224) p = getch();
			else if (p == ENTER) {
				gotoxy(1, 23); printf("              ");
				return;
			}
		}
	}
}
void credits() {
	system("cls");
	gotoxy(2, 3); printf("made by Dong Uk Kim");
	gotoxy(2, 23); printf("back to main");
	int p;
	int count = 50;
	while (1) {
		if (count % 100 == 0) {
			gotoxy(1, 23); printf("  ");
		}
		else if ((count % 100 - 50) == 0) {
			gotoxy(1, 23); printf(" >");
		}
		count++;
		if (kbhit()) {
			p = getch();
			if (p == 224) p = getch();
			else if (p == ENTER) {
				gotoxy(1, 23); printf("              ");
				return;
			}
		}
	}
}
void drawMap() {
	int x, y;
	const char* MAP[HEIGHT][WIDTH];
	system("cls");
	for (y = 0; y<HEIGHT; y++) {
		gotoxy(mapX, mapY + y);
		for (x = 0; x<WIDTH; x++) {
			if (map[y][x] == 3) MAP[y][x]="--";
			else if (map[y][x] == 2) MAP[y][x]="¢Ë";
			else if (map[y][x] == 0) MAP[y][x]="  ";
			else 			 MAP[y][x]="¢Ã";
		}
	}

	for (y = 0; y<HEIGHT; y++) {
		gotoxy(mapX, mapY + y);
		for (x = 0; x<WIDTH; x++) {
			printf("%s", MAP[y][x]);
		}
	}
	return;
}
void blockInitialize(int TYPE, int(*cblock)[5][5]) {
	int i, j;
	for (i = 0; i<5; i++) {
		for (j = 0; j<5; j++) {
			if (BLOCK[TYPE][i][j]) (*cblock)[i][j] = 1;
			else (*cblock)[i][j] = 0;
		}
	}
}
bool checkBlock(int x, int y, int cblock[5][5]) {
	int i, j;
	for (i = 0; i<5; i++) {
		for (j = 0; j<5; j++) {
			if (y + i<HEIGHT&&y + i>-1 && x + j>-1 && x + j<WIDTH)
				if (cblock[i][j] == 1 && (map[y + i][x + j] == 1 || map[y + i][x + j] == 2))	return true;
		}
	}
	return false;
}
void drawBlock(int x, int y, int cblock[5][5]) {
	int i, j;
	for (i = 0; i<5; i++) {
		for (j = 0; j<5; j++) {
			if (y + i<HEIGHT - 1 && x + j>0 && x + j<WIDTH - 1) {
				if (cblock[i][j]) {
					gotoxy(mapX + x + j, mapY + y + i);
					printf("¢Ã");
				}
			}
		}
	}
}
void removeBlock(int x, int y) {
	int i, j;
	const char* removeSP[5][6];
	for (i = 0; i<5; i++) {
		for (j = 0; j<5; j++) {
			if (y + i<HEIGHT - 1 && y + i>-1 && x + j>0 && x + j<WIDTH - 1) {
				if (map[y + i][x + j] == 3) removeSP[i][j]="--";
				else if (map[y + i][x + j] == 0) removeSP[i][j]="  ";
				else 			 removeSP[i][j]="¢Ã";
			}
		}
	}
	for (i = 0; i<5; i++) {
		for (j = 0; j<5; j++) {
			if (y + i<HEIGHT - 1 && y + i>-1 && x + j>0 && x + j<WIDTH - 1) {
				gotoxy(mapX + x + j, mapY + y + i);
				printf("%s", removeSP[i][j]);
			}
		}
	}
}
void blockToMap(int x, int y, int cblock[5][5]) {
	int i, j;
	for (i = 0; i<5; i++) {
		for (j = 0; j<5; j++) {
			if (cblock[i][j]) map[y + i][x + j] = 1;
		}
	}
}
void checkLine(int y, int* score) {
	int i, j;
	bool flag;
	for (i = y; i<y + 5; i++) {
		flag = true;
		for (j = 1; j<WIDTH - 1; j++) {
			if (map[i][j] != 1) {
				flag = false;
				break;
			}
		}
		if (flag == true) {
			*score += 10;
			removeLine(i);
		}
	}
}
void removeLine(int i) {
	int j;
	for (; i>0; i--) {
		for (j = 1; j<WIDTH - 1; j++) {
			map[i][j] = map[i - 1][j];
		}
	}
	for (j = 1; j<WIDTH - 1; j++)
		if (map[2][j] == 3) map[2][j] = 0;
	for (j = 1; j<WIDTH - 1; j++)
		if (map[1][j] == 0) map[1][j] = 3;
	for (j = 1; j<WIDTH - 1; j++) map[0][j] = 0;
	drawMap();
}
bool blockRoll(int x, int y, int(*cblock)[5][5]) {
	int ri, rj;
	int i, j;
	int rblock[5][5];
	for (i = 0, rj = 4; i<5; rj--, i++) {
		for (j = 0, ri = 0; j<5; ri++, j++) {
			rblock[i][j] = (*cblock)[ri][rj];
		}
	}
	if (checkBlock(x, y, rblock) != true) {
		for (i = 0; i<5; i++) {
			for (j = 0; j<5; j++) {
				(*cblock)[i][j] = rblock[i][j];
			}
		}
		return true;
	}
	return false;
}
bool checkOver() {
	int i;
	for (i = 1; i<WIDTH - 1; i++)
		if (map[1][i] != 3) return true;
	return false;
}
void checkScore(int score) {
	int i, j;
	char name[40][10];
	int Scores[40];
	FILE* Rbuf = fopen("score.txt", "rt");
	for (i = 0; i<40; i++)	fscanf(Rbuf, "%s %d", name[i], &Scores[i]);
	fclose(Rbuf);
	FILE* Wbuf = fopen("score.txt", "wt");
	for (i = 0; i<40; i++) {
		if (score>Scores[i]) {
			for (j = 39; j>i; j--) {
				strcpy(name[j], name[j - 1]);
				Scores[j] = Scores[j - 1];
			}
			gotoxy(mapX + WIDTH + 7, 2); printf("type your name : "); gets(name[i]);
			gotoxy(mapX + WIDTH + 7, 2); printf("                                     ");
			while (kbhit()) getch();
			Scores[i] = score;
			for (i = 0; i<40; i++) {
				fprintf(Wbuf, "%s  %d\n", name[i], Scores[i]);
			}
			break;
		}
	}
	for (i = 0; i<20; i++) {
		gotoxy(mapX + WIDTH + 7, 1 + i);
		printf("%2d. %10s %5d", i + 1, name[i], Scores[i]);
	}
	fclose(Wbuf);
}
int EndGame() {
	int x = -1, y = 10;
	gotoxy(mapX + x, mapY + y);   printf("¦£----------------------- ¦¤");
	gotoxy(mapX + x, mapY + y + 1); printf("¦¢      Game  over!       ¦¢");
	gotoxy(mapX + x, mapY + y + 2); printf("¦¢                        ¦¢");
	gotoxy(mapX + x, mapY + y + 3); printf("¦¢       Retry            ¦¢");
	gotoxy(mapX + x, mapY + y + 4); printf("¦¢       Back to main     ¦¢");
	gotoxy(mapX + x, mapY + y + 5); printf("¦¢       Exit             ¦¢");
	gotoxy(mapX + x, mapY + y + 6); printf("¦¦----------------------- ¦¥");
	int p;
	int ch = 0;
	while (1) {
		gotoxy(mapX + x + 3, mapY + ch + y + 3);
		printf(" >");
		p = getch();
		gotoxy(mapX + x + 3, mapY + ch + y + 3);
		printf("  ");
		if (p == 224) {
			p = getch();
			if (p == UP) ch = (ch + 2) % 3;
			if (p == DOWN) ch = (ch + 1) % 3;
		}
		else if (p == ENTER) {
			gotoxy(mapX + x + 4, mapY + ch + y + 3);
			printf(" >");
			return ch;
		}
	}
}
void gotoxy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcursortype(CURSOR_TYPE c) {
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

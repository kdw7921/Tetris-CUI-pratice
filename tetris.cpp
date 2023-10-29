#include <stdio.h>
#include<windows.h>
#include <conio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

//방향키들 
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
//여기까지
 
#define HEIGHT 22 //게임판 세로 
#define WIDTH 11 //게임판 가로 
#define SPACE 32 //hard drop
#define ESC 27 //게임 종료 
#define pause 112 //일시정지 
#define PAUSE 80 //일시정지
#define main_x 17 //게임판 x좌표
#define main_y 2 //게임판 y좌표

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

//정의하는 함수들 
void gotoxy(int,int);//좌표이동함수 
int maindraw();//메인화면 
void drmap(int);//맵 그리기 
int playgame();//게임함수 
int checkMove(int,int,int,int,int);//옆에 체크 
void DrBlock(int,int,int,int);//실시간 블록 그리기 함수 
void setcursortype(CURSOR_TYPE);//커서 타입
int checkDown(int,int,int,int*);//아래체크 
void DBTM(int,int,int,int);//맵에 블록 고정 
void Brolling(int,int *,int,int);//회전체크 
void BR(int,int);//블록 잔상 제거 
void LineDown(int);//라인 내리기 함수 
void LineCheck(int);//라인 클리어 체크 함수 
int EndCheck();//게임 오버 체크함수 
void mapset();//맵 초기화 함수 
void ShowScore();//점수출력 
void DrScore();//점수의 순위 출력 
int CheckScoreInsert();//점수 순위권 체크 
//끄읏
void f_drmap();
void f_mapset();

//전역 변수들 
char name[HEIGHT][8];
int Score[HEIGHT];
int score;
int map[HEIGHT][WIDTH];
int f_map[HEIGHT][WIDTH];
int flag=0;
int BLOCK[7][4][4][4]={

		//1번째 모양 
		{{
		{0,0,0,0},
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0}
		},	
		{
		{0,0,0,0},
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0}
		},	
		{
		{0,0,0,0},
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0}
		},	
		{
		{0,0,0,0},
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0}
		}},
		
		//2번째 모양 
		{{
		{0,0,0,0},
		{1,1,0,0},
		{0,1,1,0},
		{0,0,0,0}
		},		
		{
		{0,0,1,0},
		{0,1,1,0},
		{0,1,0,0},
		{0,0,0,0}
		},		
		{
		{0,0,0,0},
		{1,1,0,0},
		{0,1,1,0},
		{0,0,0,0}
		},
		{
		{0,0,1,0},
		{0,1,1,0},
		{0,1,0,0},
		{0,0,0,0}
		}},
		
		//3번째 모양 
		{{
		{0,0,0,0},
		{0,0,1,1},
		{0,1,1,0},
		{0,0,0,0}
		},		
		{
		{0,1,0,0},
		{0,1,1,0},
		{0,0,1,0},
		{0,0,0,0}
		},		
		{
		{0,0,0,0},
		{0,0,1,1},
		{0,1,1,0},
		{0,0,0,0}
		},
		{
		{0,1,0,0},
		{0,1,1,0},
		{0,0,1,0},
		{0,0,0,0}
		}},
		
		//4번째 모양 
		{{
		{0,1,0,0},
		{0,1,0,0},
		{0,1,1,0},
		{0,0,0,0}
		},		
		{
		{0,0,0,0},
		{0,0,0,1},
		{0,1,1,1},
		{0,0,0,0}
		},		
		{
		{0,1,1,0},
		{0,0,1,0},
		{0,0,1,0},
		{0,0,0,0}
		},
		{
		{0,0,0,0},
		{0,1,1,1},
		{0,1,0,0},
		{0,0,0,0}
		}},
		
		//5번째 모양 
		{{
		{0,0,1,0},
		{0,0,1,0},
		{0,1,1,0},
		{0,0,0,0}
		},		
		{
		{0,0,0,0},
		{0,1,1,1},
		{0,0,0,1},
		{0,0,0,0}
		},		
		{
		{0,1,1,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,0,0,0}
		},
		{
		{0,0,0,0},
		{0,1,0,0},
		{0,1,1,1},
		{0,0,0,0}
		}},
		
		//6번째 모양 
		{{
		{0,0,1,0},
		{0,1,1,1},
		{0,0,0,0},
		{0,0,0,0}
		},		
		{
		{0,0,1,0},
		{0,1,1,0},
		{0,0,1,0},
		{0,0,0,0}
		},		
		{
		{0,0,0,0},
		{0,1,1,1},
		{0,0,1,0},
		{0,0,0,0}
		},
		{
		{0,0,1,0},
		{0,0,1,1},
		{0,0,1,0},
		{0,0,0,0}
		}},
		
		//7번째 모양 
		{{
		{0,1,0,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,1,0,0}
		},		
		{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{1,1,1,1}
		},		
		{
		{0,1,0,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,1,0,0}
		},
		{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{1,1,1,1}
		}}
	};

void f_mapset(){
		int a,b;
		for(a=0;a<HEIGHT;a++){
			for(b=0;b<WIDTH;b++)
				f_map[a][b]=map[a][b];
		}
}

void f_drmap(){
	
	int i,j;
	
	for(i=0;i<HEIGHT;i++){
		
		gotoxy(32,i+2);
		
		for(j=0;j<WIDTH;j++){
			switch(f_map[i][j]){
				case 1:{
					printf("▣");
					break;
				}
				case 2:{
					printf("▩");
					break;
				}
				case 3:{
					printf("--");
					break;
				}			
				case 0:{
					printf("  ");
					break;
				}
			}
		}
	}
	
}

int DE=0;
	
//메인함수 
int main()
{
	SetConsoleTitle("Tetris");
	
	char buf[256] = { 0 };
	sprintf(buf, "mode con: lines=%d cols=%d", 25, 90);
	system(buf);
	
	int ch=1;
	int i;
	
	//커서 타입 초기화
	setcursortype(NOCURSOR);
	
	
	//파일 포인터 선언 
	FILE* RBuf=fopen("score.txt","rt");
	
	//처음 점수판 설정 
	if(RBuf){
		for(i=0;i<HEIGHT;i++){
			fscanf(RBuf,"%s",name[i]);
			fscanf(RBuf,"%d",Score+i);
		}
	}
	else{
		for(i=0;i<HEIGHT;i++){
			strcpy(name[i],"Unknown");
			Score[i]=0;
		}
	}
	
	//게임의 전체적인 흐름 
	while(1){
		if(ch==1){
			if(maindraw()==0) return 0;
		}
		else if(ch==0){
			gotoxy(0,50);
			return 0;
		}
		mapset(); 
		ch=playgame();
	}
	
	fclose(RBuf);
		
	return 0;
}

//게임 플레이 함수 
int playgame(){
	
	int i,input;
	int x,y;
	int NEXT_TYPE,TYPE,SHAPE;
	
	int a=0;
	score=0;
	
	system("cls");
	
	NEXT_TYPE=rand()%7;
	while(1){
		srand(time(NULL));
		TYPE=NEXT_TYPE;
		NEXT_TYPE=rand()%7;
		SHAPE=0;
		x=3;
		y=0;
		drmap(NEXT_TYPE);
		if(DE==1)
			f_drmap();
		if(EndCheck()) break;			
		while(1){
			ShowScore();
			for(i=0;i<10;i++){
				if(flag){
					BR(x,y);
					DrBlock(TYPE,SHAPE,x,y);
					flag=0;
		    	}
				if(kbhit()){
					input=getch();
					if(input==pause||input==PAUSE){
								gotoxy(main_x,10); printf("┌-----------------┐");
								gotoxy(main_x,11); printf(" |--◈일시 정지◈---| ");
								gotoxy(main_x,12); printf(" |--(계속하려면 p)--| ");
								gotoxy(main_x,13); printf("└-----------------┘");						
						do{	
							input=getch();
						}while(input!=pause&&input!=PAUSE);
						drmap(NEXT_TYPE); 
					}
					else{
						while(kbhit()) input=getch();
						switch(input){						
							case LEFT:{
								if(checkMove(TYPE,SHAPE,x,y,-1))
									x--;
								break;
							}
							case RIGHT:{
								if(checkMove(TYPE,SHAPE,x,y,1))
									x++;
								break;
							}
							case DOWN:{
								checkDown(TYPE,SHAPE,x,&y);				
								break;
							}
							case ESC:{
								gotoxy(main_x+11,10); printf("┌-----------------┐");
								gotoxy(main_x+11,11); printf(" |--ESC:돌아가기----|");
								gotoxy(main_x+11,12); printf(" |--Enter:종료------|");
								gotoxy(main_x+11,13); printf(" |--R:다시하기------|");	
								gotoxy(main_x+11,14); printf(" |--M:메인화면------|");
								gotoxy(main_x+11,15); printf("└-----------------┘");
								while(1){
									input=getch();
									if(input==ESC){
								gotoxy(main_x+11,10); printf("                     ");
								gotoxy(main_x+11,11); printf("                     ");
								gotoxy(main_x+11,12); printf("                     ");
								gotoxy(main_x+11,13); printf("                     ");
								gotoxy(main_x+11,14); printf("                     ");
								gotoxy(main_x+11,15); printf("                     ");
										break;
									}
									else if(input==13) return 0;
									else if(input==82||input==114) return 2;
									else if(input==77||input==109) return 1;
								}
								break;
							}
							case UP:{
								Brolling(TYPE,&SHAPE,x,y);
								break;
							}
							case SPACE:{
								BR(x,y);
								while(checkDown(TYPE,SHAPE,x,&y));
								score+=80;
								goto S;
								break;
							}
						}
					}
				}
				while(kbhit()) getch();
				Sleep(20);
			}
			if(checkDown(TYPE,SHAPE,x,&y)==0){
				score+=50;
				S:
				//
				if(DE==1) f_mapset();
				//	 
				DBTM(TYPE,SHAPE,x,y);
				LineCheck(y);
				break;
			}
		}
	}
	gotoxy(main_x+11,9);  printf("┌-----------------┐");	
	gotoxy(main_x+11,10); printf(" |---Game Over!-----|");
	gotoxy(main_x+11,11); printf(" |--ESC:종료--------|");
	gotoxy(main_x+11,12); printf(" |--R:다시하기------|");
	gotoxy(main_x+11,13); printf(" |--M:메인화면------|");
	gotoxy(main_x+11,14); printf("└-----------------┘");	
	if(CheckScoreInsert()) DrScore();				
	while(1){
		input=getch();
		if(input==ESC) return 0;
		else if(input==82||input==114) return 2;
		else if(input==77||input==109) return 1;
	}
	return 0;
}

//현재 점수 점수판에 포함 
int CheckScoreInsert(){
	
	int i,j;
	int ret=1;//return값 
	
	FILE* WBuf=fopen("score.txt","wt+");
	
	for(i=0;i<HEIGHT;i++){
		if(score>Score[i]){		
			for(j=HEIGHT-1;j>i;j--){	
				Score[j]=Score[j-1];		
				strcpy(name[j],name[j-1]);
			}
			Score[i]=score;
			strcpy(name[i]," ");
			DrScore();
			gotoxy(3,i+3);
			setcursortype(NORMALCURSOR);	
			gets(name[i]);
			while(kbhit()) getch();//버퍼 값을 버림
			setcursortype(NOCURSOR);
			DrScore();
			
			for(i=0;i<HEIGHT;i++){
				fprintf(WBuf,"%s %d\n",name[i],Score[i]);
			}
			ret=0;
		}
	}
	fclose(WBuf);
	return ret;
}

//점수판 출력
void DrScore(){
	
	int i,j;
	
	//전체 점수 출력
	for(i=0;i<HEIGHT;i++){
		gotoxy(1,3+i);
		printf("%2d. %s",i+1,name[i]);
		gotoxy(9,3+i);
		printf(" : %d",Score[i]);
	}
	return;
}

//게임 오버 체크함수
int EndCheck(){
	
	int i;
	
	for(i=1;i<WIDTH-1;i++)
		if(map[2][i]!=3) return 1;
	return 0;
}

//라인 클리어 체크 함수 
void LineCheck(int y){
	
	int i,j,n;

	for(i=4+y;i>=y;i--){
		if(i>=0&&i<HEIGHT){
			n=0;
			for(j=1;j<WIDTH-1;j++)
				if(map[i][j]==1) n++;
			if(n==WIDTH-2){	
				LineDown(i);
				i++;
				score+=500;
			}
		}
	}
	return;
}

//라인 내리기
void LineDown(int i){
	
	int j,index;
	
	for(;i>=0;i--){
		for(j=1;j<WIDTH-1;j++){
			if(i==0) map[0][j]=0;
			else if(i==2) map[2][j]=3;//라인클리어체크줄 
			else if(i==3) map[3][j]=(map[2][j]==1)?1:0;//그 아래줄 
			else	map[i][j]=map[i-1][j];
		}
	}
	return;
}

//블록을 맵에 그리는 함수
void DBTM(int TYPE,int SHAPE,int x,int y){
	
	int i,j;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(BLOCK[TYPE][SHAPE][i][j])
				map[y+i][x+j]=1;
		}
	}
	return;
}

//블록 회전 함수 체크 함수 
void Brolling(int TYPE,int *SHAPE,int x,int y){
	
	int i,j,index;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			index=map[y+i][x+j];
			if((index==2||index==1)&&BLOCK[TYPE][*SHAPE+1][i][j]==1) return;
		}
	}
	flag=1;
	if(*SHAPE==3) *SHAPE=0;
	else *SHAPE+=1;
	return;
}

//움직이는 블록 출력 함수 
void DrBlock(int TYPE,int SHAPE,int x,int y){
	
	int i,j;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			gotoxy(main_x+x+j,y+main_y+i);
			if(BLOCK[TYPE][SHAPE][i][j]==1)
				printf("▣");
		}
	}
	return;
}

//원래의 화면상 블록 제거
void BR(int x,int y){
	
	int i,j;
	
	for(i=0;i<6;i++){
		for(j=0;j<6;j++){
			gotoxy(main_x+x-1+j,main_y+y-1+i);		
			if(y+i-1>=0&&y+i-1<22&&j+x-1<11&&x+j-1>0)
				switch(map[y-1+i][x-1+j]){
					case 1:{
						printf("▣");
						break;
					}
					case 3:{
						printf("--");
						break;
					}
					case 2:{
						printf("▩");
						break;
					}
					case 0:{
						printf("  ");
						break;
					}
				}
		}
	}
	return;
}

//아래 동작 가능 체크함수 
int checkDown(int TYPE,int SHAPE,int x,int *y){
	
	int i,j,index;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			index=map[*y+i+1][x+j];
			if((index==2||index==1)&&BLOCK[TYPE][SHAPE][i][j]==1) return 0;
		}
	}
	*y+=1;
	flag=1;
	
	return 1;
}
 
//양옆 동작 가능 체크함수 
int checkMove(int TYPE,int SHAPE,int x,int y,int d){
	
	int i,j,index;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			index=map[y+i][x+j+d];
			if((index==2||index==1)&&BLOCK[TYPE][SHAPE][i][j]==1) return 0;
		}
	}
	flag=1;
	
	return 1;
} 

//메인 화면 출력 함수 
int maindraw(){
	
	int x,y;
	int a;
	int c=0;
	
	system("cls");
	
	while(1){
		if(kbhit()) break;
		gotoxy(15,20);
		if(c%50==0) printf("press any button");
		gotoxy(15,20);
		if(c%50-25==0) printf("                ");
		if(c%100==0){
			gotoxy(2,5);  printf("★★★★★  ★★★★  ★★★★★  ★★★★★      ★★★★★    ★★★  ");
			gotoxy(2,6);  printf("    ★      ★            ★      ★★    ★★        ★      ★★  ★★");
			gotoxy(2,7);  printf("    ★      ★            ★      ★★      ★        ★      ★      ★");
			gotoxy(2,8);  printf("    ★      ★            ★      ★★      ★        ★        ★      ");
			gotoxy(2,9);  printf("    ★      ★★★★      ★      ★★★★★★        ★        ★★    ");
			gotoxy(2,10); printf("    ★      ★★★★      ★      ★★  ★★          ★          ★★  ");
			gotoxy(2,11); printf("    ★      ★            ★      ★★    ★★        ★            ★★");
			gotoxy(2,12); printf("    ★      ★            ★      ★★    ★★        ★      ★      ★");
			gotoxy(2,13); printf("    ★      ★            ★      ★★      ★★      ★      ★★  ★★");
			gotoxy(2,14); printf("    ★      ★★★★      ★      ★★      ★★  ★★★★★    ★★★  ");
		}
		if(c%100-50==0){
			gotoxy(2,5);  printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,6);  printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,7);  printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,8);  printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,9);  printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,10); printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,11); printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,12); printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,13); printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");
			gotoxy(2,14); printf("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★");			
		}
		Sleep(10);
		c++;
	}
	
	while(kbhit()) a=getch();
	
	if(a==ESC) return 0;
	else return 1;
}

//점수를 보여줌 
void ShowScore(){
	
	gotoxy(main_x+11,6+main_y);
	printf("score:%d",score);
	
	return;
}

//맵+블록 그리기 
void drmap(int NEXT_TYPE){
	
	int i,j;
	
	for(i=0;i<HEIGHT;i++){
		
		gotoxy(main_x,i+main_y);
		
		for(j=0;j<WIDTH;j++){
			switch(map[i][j]){
				case 1:{
					printf("▣");
					break;
				}
				case 2:{
					printf("▩");
					break;
				}
				case 3:{
					printf("--");
					break;
				}			
				case 0:{
					printf("  ");
					break;
				}
			}
		}
	}
	for(i=0;i<6;i++){
		gotoxy(main_x+11,2+i);
		for(j=0;j<5;j++){
			if(i==0||i==5||j==4) printf("▩");
			else
				switch(BLOCK[NEXT_TYPE][0][i-1][j]){
					case 0:{
						printf("  ");
						break;
					}
					case 1:{
					printf("▣");
					break;
					}				
				}
		}
	}
	
	return;
}

//맵 초기화 함수 
void mapset(){
	
	int i,j;
	
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if(i==HEIGHT-1||j==0||j==WIDTH-1)
			map[i][j]=2;
			else{
				if(i==2)	map[i][j]=3;
				else	map[i][j]=0;
			}
		}
	}
	if(DE==1) f_mapset();
	return;
}

//좌표이동함수 
void gotoxy(int x,int y) {

    COORD pos={2*x,y};

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}

//커서 타입 변환 함수 
void setcursortype(CURSOR_TYPE c){
	
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize=1;
		CurInfo.bVisible=FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize=100;
		CurInfo.bVisible=TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize=20;
		CurInfo.bVisible=TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

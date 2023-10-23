#include <stdio.h>
#include<windows.h>
#include <conio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

//����Ű�� 
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
//�������
 
#define HEIGHT 22 //������ ���� 
#define WIDTH 11 //������ ���� 
#define SPACE 32 //hard drop
#define ESC 27 //���� ���� 
#define pause 112 //�Ͻ����� 
#define PAUSE 80 //�Ͻ�����
#define main_x 17 //������ x��ǥ
#define main_y 2 //������ y��ǥ

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

//�����ϴ� �Լ��� 
void gotoxy(int,int);//��ǥ�̵��Լ� 
int maindraw();//����ȭ�� 
void drmap(int);//�� �׸��� 
int playgame();//�����Լ� 
int checkMove(int,int,int,int,int);//���� üũ 
void DrBlock(int,int,int,int);//�ǽð� ��� �׸��� �Լ� 
void setcursortype(CURSOR_TYPE);//Ŀ�� Ÿ��
int checkDown(int,int,int,int*);//�Ʒ�üũ 
void DBTM(int,int,int,int);//�ʿ� ��� ���� 
void Brolling(int,int *,int,int);//ȸ��üũ 
void BR(int,int);//��� �ܻ� ���� 
void LineDown(int);//���� ������ �Լ� 
void LineCheck(int);//���� Ŭ���� üũ �Լ� 
int EndCheck();//���� ���� üũ�Լ� 
void mapset();//�� �ʱ�ȭ �Լ� 
void ShowScore();//������� 
void DrScore();//������ ���� ��� 
int CheckScoreInsert();//���� ������ üũ 
//����
void f_drmap();
void f_mapset();

//���� ������ 
char name[HEIGHT][8];
int Score[HEIGHT];
int score;
int map[HEIGHT][WIDTH];
int f_map[HEIGHT][WIDTH];
int flag=0;
int BLOCK[7][4][4][4]={

		//1��° ��� 
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
		
		//2��° ��� 
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
		
		//3��° ��� 
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
		
		//4��° ��� 
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
		
		//5��° ��� 
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
		
		//6��° ��� 
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
		
		//7��° ��� 
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
					printf("��");
					break;
				}
				case 2:{
					printf("��");
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
	
//�����Լ� 
int main()
{
	char buf[256] = { 0 };
	sprintf(buf, "mode con: lines=%d cols=%d", 25, 90);
	system(buf);
	
	int ch=1;
	int i;
	
	//Ŀ�� Ÿ�� �ʱ�ȭ
	setcursortype(NOCURSOR);
	
	
	//���� ������ ���� 
	FILE* RBuf=fopen("score.txt","rt");
	
	//ó�� ������ ���� 
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
	
	//������ ��ü���� �帧 
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

//���� �÷��� �Լ� 
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
								gotoxy(main_x,10); printf("��-----------------��");
								gotoxy(main_x,11); printf(" |--���Ͻ� ������---| ");
								gotoxy(main_x,12); printf(" |--(����Ϸ��� p)--| ");
								gotoxy(main_x,13); printf("��-----------------��");						
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
								gotoxy(main_x+11,10); printf("��-----------------��");
								gotoxy(main_x+11,11); printf(" |--ESC:���ư���----|");
								gotoxy(main_x+11,12); printf(" |--Enter:����------|");
								gotoxy(main_x+11,13); printf(" |--R:�ٽ��ϱ�------|");	
								gotoxy(main_x+11,14); printf(" |--M:����ȭ��------|");
								gotoxy(main_x+11,15); printf("��-----------------��");
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
	gotoxy(main_x+11,9);  printf("��-----------------��");	
	gotoxy(main_x+11,10); printf(" |---Game Over!-----|");
	gotoxy(main_x+11,11); printf(" |--ESC:����--------|");
	gotoxy(main_x+11,12); printf(" |--R:�ٽ��ϱ�------|");
	gotoxy(main_x+11,13); printf(" |--M:����ȭ��------|");
	gotoxy(main_x+11,14); printf("��-----------------��");	
	if(CheckScoreInsert()) DrScore();				
	while(1){
		input=getch();
		if(input==ESC) return 0;
		else if(input==82||input==114) return 2;
		else if(input==77||input==109) return 1;
	}
	return 0;
}

//���� ���� �����ǿ� ���� 
int CheckScoreInsert(){
	
	int i,j;
	int ret=1;//return�� 
	
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
			while(kbhit()) getch();//���� ���� ����
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

//������ ���
void DrScore(){
	
	int i,j;
	
	//��ü ���� ���
	for(i=0;i<HEIGHT;i++){
		gotoxy(1,3+i);
		printf("%2d. %s",i+1,name[i]);
		gotoxy(9,3+i);
		printf(" : %d",Score[i]);
	}
	return;
}

//���� ���� üũ�Լ�
int EndCheck(){
	
	int i;
	
	for(i=1;i<WIDTH-1;i++)
		if(map[2][i]!=3) return 1;
	return 0;
}

//���� Ŭ���� üũ �Լ� 
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

//���� ������
void LineDown(int i){
	
	int j,index;
	
	for(;i>=0;i--){
		for(j=1;j<WIDTH-1;j++){
			if(i==0) map[0][j]=0;
			else if(i==2) map[2][j]=3;//����Ŭ����üũ�� 
			else if(i==3) map[3][j]=(map[2][j]==1)?1:0;//�� �Ʒ��� 
			else	map[i][j]=map[i-1][j];
		}
	}
	return;
}

//����� �ʿ� �׸��� �Լ�
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

//��� ȸ�� �Լ� üũ �Լ� 
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

//�����̴� ��� ��� �Լ� 
void DrBlock(int TYPE,int SHAPE,int x,int y){
	
	int i,j;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			gotoxy(main_x+x+j,y+main_y+i);
			if(BLOCK[TYPE][SHAPE][i][j]==1)
				printf("��");
		}
	}
	return;
}

//������ ȭ��� ��� ����
void BR(int x,int y){
	
	int i,j;
	
	for(i=0;i<6;i++){
		for(j=0;j<6;j++){
			gotoxy(main_x+x-1+j,main_y+y-1+i);		
			if(y+i-1>=0&&y+i-1<22&&j+x-1<11&&x+j-1>0)
				switch(map[y-1+i][x-1+j]){
					case 1:{
						printf("��");
						break;
					}
					case 3:{
						printf("--");
						break;
					}
					case 2:{
						printf("��");
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

//�Ʒ� ���� ���� üũ�Լ� 
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
 
//�翷 ���� ���� üũ�Լ� 
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

//���� ȭ�� ��� �Լ� 
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
			gotoxy(2,5);  printf("�ڡڡڡڡ�  �ڡڡڡ�  �ڡڡڡڡ�  �ڡڡڡڡ�      �ڡڡڡڡ�    �ڡڡ�  ");
			gotoxy(2,6);  printf("    ��      ��            ��      �ڡ�    �ڡ�        ��      �ڡ�  �ڡ�");
			gotoxy(2,7);  printf("    ��      ��            ��      �ڡ�      ��        ��      ��      ��");
			gotoxy(2,8);  printf("    ��      ��            ��      �ڡ�      ��        ��        ��      ");
			gotoxy(2,9);  printf("    ��      �ڡڡڡ�      ��      �ڡڡڡڡڡ�        ��        �ڡ�    ");
			gotoxy(2,10); printf("    ��      �ڡڡڡ�      ��      �ڡ�  �ڡ�          ��          �ڡ�  ");
			gotoxy(2,11); printf("    ��      ��            ��      �ڡ�    �ڡ�        ��            �ڡ�");
			gotoxy(2,12); printf("    ��      ��            ��      �ڡ�    �ڡ�        ��      ��      ��");
			gotoxy(2,13); printf("    ��      ��            ��      �ڡ�      �ڡ�      ��      �ڡ�  �ڡ�");
			gotoxy(2,14); printf("    ��      �ڡڡڡ�      ��      �ڡ�      �ڡ�  �ڡڡڡڡ�    �ڡڡ�  ");
		}
		if(c%100-50==0){
			gotoxy(2,5);  printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,6);  printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,7);  printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,8);  printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,9);  printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,10); printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,11); printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,12); printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,13); printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");
			gotoxy(2,14); printf("�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�");			
		}
		Sleep(10);
		c++;
	}
	
	while(kbhit()) a=getch();
	
	if(a==ESC) return 0;
	else return 1;
}

//������ ������ 
void ShowScore(){
	
	gotoxy(main_x+11,6+main_y);
	printf("score:%d",score);
	
	return;
}

//��+��� �׸��� 
void drmap(int NEXT_TYPE){
	
	int i,j;
	
	for(i=0;i<HEIGHT;i++){
		
		gotoxy(main_x,i+main_y);
		
		for(j=0;j<WIDTH;j++){
			switch(map[i][j]){
				case 1:{
					printf("��");
					break;
				}
				case 2:{
					printf("��");
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
			if(i==0||i==5||j==4) printf("��");
			else
				switch(BLOCK[NEXT_TYPE][0][i-1][j]){
					case 0:{
						printf("  ");
						break;
					}
					case 1:{
					printf("��");
					break;
					}				
				}
		}
	}
	
	return;
}

//�� �ʱ�ȭ �Լ� 
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

//��ǥ�̵��Լ� 
void gotoxy(int x,int y) {

    COORD pos={2*x,y};

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}

//Ŀ�� Ÿ�� ��ȯ �Լ� 
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

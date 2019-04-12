/************************************************************************
 
 俄罗斯方块重铸版 
 
 Shenzhen Longhua Sunward Electronics Factory
 
 深圳龙华三和电子厂 

************************************************************************/ 

#include <windows.h>
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <math.h>

#define Speed 250 //下落速度 
#define buffer 3 //触底缓冲 

int board[30][30][2];
int sort[7]={0,1,3,5,7,11,15};//俄罗斯方块分为7大类19小类 
int mark=0;
int amend_x=10,amend_y=4,speed=Speed;
time_t seed;
int kind=0,next_kind;
int key;//水平锁 
int win=1;
	
struct before{
	int x[4];
	int y[4];
}before; 

struct now{
	int x[4];
	int y[4];
	int color;
}now[19]={                       //□□
	{{0,2,0,2},{-1,-1,-2,-2},10},//□□系列 num:0

    {{0,6,4,2},{-1,-1,-1,-1},11},{{2,2,2,2},{-1,-2,-3,-4},11},//□□□□系列 num:1-2 

	                                                          //□□
	{{0,4,2,2},{-2,-1,-1,-2},12},{{0,2,0,2},{-1,-2,-2,-3},12},//  □□系列 num:3-4;
                                                
	                                                          //  □□
	{{0,4,2,2},{-1,-2,-1,-2},13},{{0,2,2,0},{-2,-1,-2,-3},13},//□□  系列 num:5-6;
	                                                                                                                    //  □
	{{0,4,2,2},{-1,-1,-1,-2},14},{{0,2,0,0},{-1,-2,-2,-3},14},{{0,4,2,2},{-2,-2,-1,-2},14},{{0,2,2,2},{-2,-1,-2,-3},14},//□□□系列 num:7-10

                                                                                                                    //□
	{{0,4,2,0},{-1,-1,-1,-2},9},{{0,2,0,0},{-1,-3,-2,-3},9},{{0,4,2,4},{-2,-1,-2,-2},9},{{0,2,2,2},{-1,-1,-2,-3},9},//□□□系列 num:11-14
                                                                                             
	                                                                                                                //    □
	{{0,4,2,4},{-1,-1,-1,-2},3},{{0,2,0,0},{-1,-1,-2,-3},3},{{0,4,2,0},{-1,-2,-2,-2},3},{{0,2,2,2},{-3,-1,-2,-3},3},//□□□系列 num:15-18
 };//方块形态结构体;
 
void gotoxy(int x, int y){
    COORD pos;//表示一个字符在控制台屏幕上的坐标
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//是API中定位光标位置的函数。
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);//获取光标信息
	cci.bVisible = FALSE;//隐藏光标
	SetConsoleCursorInfo(hOut,&cci);//设置光标信息
}//光标控制模块

void color(int b){
    HANDLE hConsole = GetStdHandle((STD_OUTPUT_HANDLE)) ;
    SetConsoleTextAttribute(hConsole, b) ;//颜色控制
}//颜色控制模块 

void boundary(){
	color(7);
    int i=0,j=0;
    gotoxy(2,1);
    for(i=0;i<26;i++){
        printf("■");
	}
    gotoxy(2,27);
    for(i=0;i<26;i++){
        printf("■");
	}
    for(i=0;i<25;i++){
        gotoxy(2,2+i);
        printf("■");
        gotoxy(34,2+i);
        if(i==7||i==13){
			printf("■■■■■■■■■");
		}else{
            printf("■");
		}
        gotoxy(52,2+i);
		printf("■");
	}
    int t=465;
    gotoxy(37,11);
    printf("最高分数:%d",t);
    gotoxy(37,13);
    printf("当前得分:%d",mark);
    gotoxy(37,17);
    printf("↑:旋转");
    gotoxy(37,19);
    printf("↓:加速");
    gotoxy(37,21);
    printf("←:左移 ");
    gotoxy(37,23);
    printf("→:右移");
    gotoxy(37,25);
    printf("空格:停止");
    color(12);  
    gotoxy(23,29);
	printf("俄罗斯方块"); 
	gotoxy(14,31);
	printf("     深圳龙华三和电子厂");
	color(7);
    for(int i1=0;i1<30;++i1){
    	board[26][i1][0]=1;
	}
	for(int i2=24;i2>=0;i2--){	
		for(int j=0;j<15;j++){
			if(board[i2][j][0]==0){
				color(8);
            	gotoxy(32-j*2,2+i2);
            	printf("□"); 	
			}
        }		 
	}
}//界面生成模块

void next(){
	srand(time(&seed));
	next_kind=sort[rand()%7];
	gotoxy(37,3);
	color(7);
	printf("下一个");
	color(now[next_kind].color);
	for(int i=0;i<4;++i){
		gotoxy(now[kind].x[i]+37,now[kind].y[i]+7);
		printf("  ");
	}
	for(int i2=0;i2<4;++i2){
		gotoxy(now[next_kind].x[i2]+37,now[next_kind].y[i2]+7);
		printf("■");
	}
	color(7);
}

void left_and_rigth(int dir){
	if(now[kind].x[0]+amend_x>=6 && now[kind].x[1]+amend_x<=30){
		if(dir==3){
			for(int i=0;i<4;++i){
				int x=now[kind].x[i]+amend_x;
				int y=now[kind].y[i]+amend_y;
				if(board[y-1][x/2-3][0]==1){
					key=0;
				}
			}
		}
		if(dir==4 || dir==1){
			for(int i=0;i<4;++i){
				int x=now[kind].x[i]+amend_x;
				int y=now[kind].y[i]+amend_y;
				if(board[y-1][x/2-1][0]==1){
					key=0;
				}
			}
		}
	}
	if(now[kind].x[0]+amend_x<6 && dir==3){
		key=0;
	}
	if(now[kind].x[1]+amend_x>30 && (dir==4 || dir==1)){
		key=0;
	}
}//左右障碍判断 

void game_over(){
	color(3); 
    for(int i=25;i>=0;i--){	
		for(int j=0;j<15;j++){
			if(i<25){
            	gotoxy(32-j*2,2+i);
            	printf("■");
            	Sleep(10);
			}	 
        	board[i][j][0]=0;
        }		 
	}
    for(int j=0;j<25;j++)
	{
        for(int i=0;i<15;i++){
             gotoxy(4+2*i,j+2);
             color(8); 
             printf("□");
             Sleep(10);
         }
	 }
	gotoxy(37,13);
    printf("当前得分:%d  ",mark);
	mark=0;
}//界面清除模块 

int input(){
	int ch1=0;
    int ch2=0;
	if (ch1=getch()){
		ch2=getch();//第一次调用getch()，返回值224
		if(kbhit()){
			ch2=getch();
		}
		switch (ch2)//第二次调用getch()
		{
		//↑↓←→键
		case 72: return 1;  
		case 80: return 2; 
		case 75: return 3;
		case 77: return 4;
		}
	}
}

void move(int fall){
	for(int i=0;i<2;++i){
		for(int j=0;j<4;++j){
			int x=now[kind].x[j]+amend_x;
			int y=now[kind].y[j]+amend_y;
			if(i==0 && y>1){
				gotoxy(x,y);
				color(now[kind].color);
				printf("■");
				gotoxy(x,y+1);
			}
			if(i==1 && fall==1 && y>1){
				gotoxy(x,y);
				color(8);
				printf("□"); 
			}
		}
		color(7); 
		Sleep(speed*(1-i));
	}
	gotoxy(37,13);
	printf("当前得分:%d   ",mark);
}//方块的移动 

bool fall_or_not(){
	for(int i=0;i<4;++i){
		int x=now[kind].x[i]+amend_x;
		int y=now[kind].y[i]+amend_y;
		if(board[y-1][x/2-2][0]==1){
			return false;
		}
	}
	return true;
	
}//垂直障碍模块 

void rotate(){
	if(kind>0 && kind<3){
		kind==1?kind=2:kind=1;
	}
	if(kind>2 && kind<5){
		kind==3?kind=4:kind=3;
	}
	if(kind>4 && kind<7){
		kind==5?kind=6:kind=5;
	}
	if(kind>6 && kind<11){
		kind==10?kind=7:kind++;
	}
	if(kind>10 && kind<15){
		kind==14?kind=11:kind++; 
	}
	if(kind>14){
		kind==18?kind=15:kind++;
	}
}//方块旋转模块 

void line_clear(int n){
	 for(n;n>1;--n){
		 for(int i=0;i<15;++i){
			 gotoxy(4+2*i,n+1);
			 color(8);
			 printf("□");
			 board[n][i][0]=board[n-1][i][0];
			 board[n][i][1]=board[n-1][i][1];
			 if(board[n][i][0]==1){
				 gotoxy(4+2*i,n+1);
				 color(board[n][i][1]);
				 printf("■");
			 }
		 }
	 }
}//行满清除 

void full_or_not(){
	int line=0;
	for(int i=1;i<26;++i){
		int k=0;
		for(int j=0;j<15;++j){
			if(board[i][j][0]==1){
				++k;
			}
		}
		if(k==15){
			++line;
			line_clear(i); 
		}
	}
	
	if(line>0){
		mark+=pow(3,line-1);
		color(7);
	}

}//判断行满 

void game(){
	int fall=1,dir,time=buffer,stop=0;
	amend_x=10,amend_y=3;
	if(kind==1){
		--amend_y;
	} 
	while(fall==1){
		dir=5;
		key=1;
		if(kbhit()){
			dir=input();
			left_and_rigth(dir);
			gotoxy(2,0);
			if(dir==1 && key==1 && amend_y>4 && stop==0){
				rotate();
				time=2;
			}
			if(dir==2){
				speed=30;
			}
			if(dir==3 && key==1){
				amend_x-=2;
				if(stop==0){
					--amend_y;
				}
				speed=30;
				time=2;
			}
			if(dir==4 && key==1){
				amend_x+=2;
				if(stop==0){
					--amend_y;
				}
				speed=30;
				time=2;
			}
		} 
		++amend_y;
		if(fall_or_not()==false){
			stop=1;
			for(int i=0;i<4;++i){
				if(now[kind].y[i]+amend_y<4){
					win=0;
				}
			}
		}
		if(fall_or_not()==true){
			stop=0;
			time=buffer;
		}
		if(stop==1 && time>0){
			--time;
			--amend_y;
		}
		if(time==0){
			fall=0; 
		}
		move(fall);
		speed=Speed;
		gotoxy(10,0);
	}
	for(int i=0;i<4;++i){
		int x=now[kind].x[i]+amend_x;
		int y=now[kind].y[i]+amend_y;
		board[y-1][x/2-2][0]=1;
		board[y-1][x/2-2][1]=now[kind].color;
	}
	full_or_not();

}//控制一个俄罗斯方块的运行 

int main(){
	srand(time(&seed));
	kind=sort[rand()%7];
	boundary();
	next_kind=sort[rand()%7];
	int again=1;
	while(again==1){
		while(win==1){
			next();
			game();
			kind=next_kind;
		}
		game_over();
		boundary();
		gotoxy(60,2);
		color(7);
		printf("输入‘1’继续游戏:");
		scanf("%d",&again);
		gotoxy(60,2);
		printf("                           ");
		win=1;
	}

	gotoxy(0,40);
	return 0;
}

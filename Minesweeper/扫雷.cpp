/**********************************************

 Waysoft Campany	伪软公司

扫雷（鼠标操作）

格子大小：15*15像素
图片来源：world文档插入图片（自制）

左键：扫雷 
右键：标记
已标记的不能扫雷

雷区定义：小窗口
雷区长宽：20<n<40 15<m<50
地雷个数：15<x<m*n

可拓展的：
1.排行榜显示前n名
2.排行榜信息重置
3.排行榜文件被删除，重新生成，并且排行信息初始化
4.排行榜信息加密

***********************************************/

//头文件包------------------------
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <ctime>
#include <cstdio>
#include <math.h>
//--------------------------------

//自定义数据（雷区，选择按钮，显示板）--------
//单位：像素
#define mine_x 15                            //雷区距控制台左边距离（不包括边界）
#define mine_y 105                           //雷区距控制台顶部距离（不包括边界）
#define grid_x 15                            //雷区格子横距
#define grid_y 15                            //雷区格子纵距
#define bou_x 200                            //设置面板横距
#define bou_y 15                             //设置面板纵距
#define m_x 10                               //计数板横距
#define m_y 20                               //计数板纵距
#define f_x 20                               //笑脸横距
#define f_y 20                               //笑脸纵距
#define b_y 30                               //控制板与控制台顶部距离                               
//--------------------------------

//图片元素------------------------
IMAGE mineNum[8];							 //图片数组：格子附近地雷数量。0~7表示附近地雷1~8
IMAGE numBou[11];                            //图片数组：数字输出。0~9代表输出数字0~9，10代表数字溢出
IMAGE grid;                                  //格子图标(未点击)
IMAGE click;                                 //格子图标(已点击)
IMAGE flag;                                  //标记地雷图标
IMAGE mine;                                  //地雷图标
IMAGE mineDie;                               //地雷图标(踩中)
IMAGE mineWrong;                             //地雷图标(判断错误) 
IMAGE bou;                                   //面板图标
IMAGE regame;								 //重新开始按键
IMAGE right;                                 //右界
IMAGE left;                                  //左界
IMAGE up;                                    //上界
IMAGE down;                                  //下界
IMAGE l_u;                                   //左上
IMAGE l_d;                                   //左下
IMAGE r_u;                                   //右上
IMAGE r_d;                                   //右下
IMAGE face;                                  //圆脸（普通）
IMAGE sweep;                                 //圆脸（扫雷）
IMAGE lose;                                  //圆脸（失败）
IMAGE win;                                   //圆脸（胜利）
IMAGE white;                                 //菜单栏
IMAGE key[20];                               //按键数组:0~9表示9个按键（鼠标聚焦）  10~11表示9个按键（非聚焦）
//---------------------------------

//公共变量(雷区数组)---------------
//含有地雷：值%10=1
//标记地雷：值>=10
//已经探索：值=-1;
//即：(1)标记地雷+有雷=11 (2)标记地雷+无雷=10 (3)无雷=0 (4)有雷=1

int mineX=20;                                 //雷区横距
int mineY=20;                                 //雷区纵距
int mines=50;                                 //地雷数量
int rigth;                                    //标记地雷+已扫描格子数
int mark=0;                                   //标记       
int rank=1;                                   //难度级别          
int gameTime;                                 //游戏运行时间
char rankString[240];                         //排行榜数据字符串
int recordRank[4];                            //各难度排行
//---------------------------------

//顺序表 用于存放格子地雷信息
typedef struct SequenceList{
	int board[51][51];
}List;

typedef struct stack{
	int x;
	int y;
	int dir;
	stack *next;
	stack *before;
}Stack;//定义搜索栈,双向链表

//定位器：通过鼠标坐标，确定所属格子
//鼠标点击所在范围 的 格子 的坐标（格子坐标=格子右上角坐标）
//算法：x-=(n-位移量)%格子宽
void getGird(int x,int y,int xy[2]){
	xy[0]=x-=(x-mine_x)%grid_x;
	xy[1]=y-=(y-mine_y)%grid_y;
}

//辅助读取文件中的数字
int readNum(int point){
	int readN=0;
	int i=0;
	while(rankString[point]>='0' && rankString[point]<='9'){
		++i;
		++point;
	}
	while(i>0){
		readN+=((int)rankString[point-i]-48)*pow(10,i-1);
		--i;
	}
	return readN;
}

//扫雷英雄榜读取
void readTXT() 
{
	FILE *fp;
	char mapth[4][50]={"game//save//easy.txt","game//save//standard.txt","game//save//middle.txt","game//save//hard.txt"};
	char reading[4][60];
	for(int g=0;g<4;++g){
		if((fp=fopen(mapth[g],"r"))==NULL){
			printf("打开文件错误!\n");
			exit(0);
		}
	
		//读取原始数据
		while(!feof(fp))
		{
		fscanf(fp,"%s",reading[g]);	
		}
		
	}
	//清空rankString
	for(int i2=0;i2<240;++i2){
		rankString[i2]=NULL;
	}
	//存放到rankString
	int hh=0;
	for(int i1=0;i1<4;++i1){
		int j1=0;
		while(reading[i1][j1]!=NULL){
			rankString[hh]=reading[i1][j1];
			++hh;
			++j1;
		}
	}

	//存储各个难度的最短时间
	int point[1]={0};
	while(rankString[point[0]]!=NULL){
		if(rankString[point[0]]=='!'){//读取到easy
			point[0]+=3;
			recordRank[0]=readNum(point[0]);
		}
		if(rankString[point[0]]=='@'){//读取到standard
			point[0]+=3;
			recordRank[1]=readNum(point[0]);
		}
		if(rankString[point[0]]=='$'){//读取到middle
			point[0]+=3;
			recordRank[2]=readNum(point[0]);
		}
		if(rankString[point[0]]=='^'){//读取到hight
			point[0]+=3;
			recordRank[3]=readNum(point[0]);
		}
		++point[0];
	}

	//格式化数据
	//读取到#：替换成\n(换行)
	//读取到%：替换成\t(TAB)
	int i=0;
	while(rankString[i]!=NULL){
		if(rankString[i]=='#'){
			rankString[i]='\n';
		}
		else if(rankString[i]=='%'){
			rankString[i]='\t';
		}
		else if(rankString[i]=='!' || rankString[i]=='@' || rankString[i]=='$' || rankString[i]=='^'){
			rankString[i]=' ';
		}
		++i;
	}

	if(fclose(fp))
	{
		printf("Can not close the file!\n");
		exit(0);
	}
}

//扫雷英雄榜存储
void writeTXT(char sp[20]){
	char mapth[4][50]={"game//save//easy.txt","game//save//standard.txt","game//save//middle.txt","game//save//hard.txt"};//路径
	FILE *p=fopen(mapth[rank],"w");
	char ch[4][200]= {"简单!%%","标准@%%","中等$%%","困难^%%"};//难度
	char sec[5]="秒%%";//单位
	int r=0;
	int t=gameTime;
	if(gameTime==0){
		++gameTime;
		++t;
	}
	int digit=0;//探测位数
	while(t>0)
	{
		t/=10;++digit;
	}
	r=6+digit+1;//跳过难度和位数
	//存放用时
	while(digit>0){
		ch[rank][6+digit]=(char)(gameTime%10+48);
		gameTime/=10;
		--digit;
	}
	//存放单位
	int i=0;
	while(sec[i]!=NULL){
		ch[rank][r]=sec[i];
		++r;
		++i;
	}

	//存放姓名
	i=0;
	while(sp[i]!=NULL){
		if(sp[i]!='!' && sp[i]!='@' && sp[i]!='#' && sp[i]!='$' && sp[i]!='%' && sp[i]!='^'){
			ch[rank][r]=sp[i];
			++r;
		}
		++i;
	}
	ch[rank][r]='#';//加入空格
	fwrite(ch[rank],1,sizeof(ch[rank]),p);  //写入数据
	fclose(p);
}


//时间显示板
void timeBou(long t){
	long now=(GetTickCount()-t)/1000;
	gameTime=now;
	if(now<9999){
		for(int h=3;h>=0;--h,now/=10){
			putimage(mineX*grid_x+mine_x-m_x*(5-h),m_y+b_y,&numBou[now%10]);
		}
	}else{
		for(int h=3;h>=0;--h,now/=10){
			putimage(mineX*grid_x-m_x*(4-h),m_y+b_y,&numBou[10]);
		}
	}
}

//标记显示板
void markBou(){
	int num=mines-mark;
	if(num>=0){
		for(int h=3;h>=0;--h,num/=10){
			putimage(m_x*(h+2),m_y+b_y,&numBou[num%10]);
		}
	}else{
		for(int h=3;h>=0;--h,num/=10){
			putimage(m_x*(h+2),m_y+b_y,&numBou[10]);
		}
	}
}//标记显示板

//游戏初始化模块
void gameStart(List *list){
	//游戏大界面
    initgraph((mineX+1)*grid_x+mine_x,(mineY+1)*grid_y+mine_y);

	//雷区
	putimage(mine_x-grid_x,mine_y-grid_y, &l_u);
	putimage(mine_x-grid_x,mineY*grid_y+mine_y, &l_d);
	putimage(mineX*grid_x+mine_x,mine_y-grid_y, &r_u);
	putimage(mineX*grid_x+mine_x,mineY*grid_y+mine_y, &r_d);
	for(int i=0;i<mineX+1;++i){
		for(int j=0;j<mineY+1;++j){
			if(i==0 && j!=mineY){
				putimage(mine_x-grid_x, j*grid_y+mine_y, &left);
			}
			if(i!=mineX){
				putimage(i*grid_x+mine_x, j*grid_y+mine_y, &grid);
				list->board[i][j]=0;//地雷标记置空
			}
			if(i==mineX && j!=mineY){
				putimage(mineX*grid_x+mine_x, j*grid_y+mine_y, &right);
			}
			if(j==0 && i!=mineX){
				putimage(i*grid_x+mine_x,mine_y-grid_y, &up);				
			}
			if(j==mineY && i!=mineX){
				putimage(i*grid_x+mine_x,j*grid_y+mine_y, &down);
			}
		}
	}

	//上面板
	putimage(0,b_y,&l_u);
	putimage(0,mine_y-grid_y*2, &l_d);
	putimage(mineX*grid_x+mine_x,mine_y-grid_y*2, &r_d);
	putimage(mineX*grid_x+mine_x,b_y, &r_u);
	for(int h=(mine_y-b_y)/grid_y-3;h>0;--h){
		putimage(0,h*grid_y+b_y,&left);
		putimage(mineX*grid_x+mine_x,h*grid_y+b_y,&right);
		for(int k=0;k<mineX;++k){
			putimage(k*grid_x+mine_x,h*grid_y+b_y, &bou);
			putimage(k*grid_x+mine_x,b_y, &up);
			putimage(k*grid_x+mine_x,mine_y-grid_y*2, &down);
		}
	}
	//笑脸
	putimage(mineX*grid_x/2,grid_y+2+b_y,&face);

	for(int c=2;c<6;++c){
		//标记计数板
		putimage(m_x*c,m_y+b_y,&numBou[0]);

		//时间计数板
		putimage(mineX*grid_x+mine_x-m_x*c,m_y+b_y,&numBou[0]);
	}
	//选择框
	for(int k=0;k<mineX+2;++k){
		putimage((k-1)*grid_x+mine_x,0, &white);
	}

	//菜单栏
	int len=((mineX+1)*grid_x)/8;
	for(int kn=0;kn<8;++kn){
		putimage(len*kn+len/8,5, &key[kn+10]);
	}

	//随机地雷
	time_t seed;
	srand(time(&seed));
	int t=mines;
	while(t>0){
		int x=rand()%mineX;
		int y=rand()%mineY;
		if(list->board[y][x]!=1){
			//putimage(x*grid_x+mine_x,y*grid_y+mine_y,&mine);
			list->board[y][x]=1;
			--t;
		}
	}

	markBou();
}

//格子附近地雷探索器
int mineSearch(int m,int n,List *list){//m，n为数组横纵编号;mm，mn为雷区横纵格子数
	int h=0;
	int dir[8][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};
	//搜索方向数组。8表示8个方向（格子附近8个格子），2表示x，y
	//比如x+dir[0][1],y+dir[0][0]代表x+1,y+1,即格子（x，y）的左上方 
	for(int i=0;i<8;++i){
		int M=m+dir[i][1];
		int	N=n+dir[i][0];
		if(N>=0 && M>=0 && N<100 && M<100){
			if(list->board[N][M]%10==1){
				++h;
			}
		}
	}
	return h;
}//检测方块附近地雷数量

/*深搜函数（递归）
void dfs(int x,int y,List *list){
	int dir[8][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};//搜索方向
	if(x>=0 && y>=0 && x<mineX && y<mineY){
		int n=mineSearch(x,y,list);
		if(list->board[y][x]==0 && n>0){
			list->board[y][x]=-1;
			putimage(x*grid_x+mine_x,y*grid_y+mine_y,&mineNum[n-1]);
			++rigth;
		}
		if(list->board[y][x]==0 && n==0){
			putimage(x*grid_x+mine_x,y*grid_y+mine_y,&click);
			list->board[y][x]=-1;
			++rigth;
			for(int i=0;i<8;++i){
				int X=x+dir[i][1];
				int Y=y+dir[i][0];
				dfs(X,Y,list);
			}
		}
	} 
}
*/

//非递归深搜
void dfs(int x,int y,List *list){
	int dir[8][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};//搜索方向
	stack *s=(stack*)malloc(sizeof(stack));
	stack *q;
	s->before=NULL;
	s->next=NULL;
	s->x=x,s->y=y,s->dir=-1;
	
	while(s!=NULL){
		if(s->x>=0 && s->y>=0 && s->x<mineX && s->y<mineY){
			int n=mineSearch(s->x,s->y,list);
			int h=0;
			if(list->board[s->y][s->x]==0 && n>0){
				list->board[s->y][s->x]=-1;
				putimage(s->x*grid_x+mine_x,s->y*grid_y+mine_y,&mineNum[n-1]);
				++rigth;

				//返回上一级（出栈）
				q=s;
				s=s->before;
				free(q);
				if(s!=NULL){
					list->board[s->y][s->x]=0;--rigth;
				}
			}
			else if(list->board[s->y][s->x]==0 && n==0){
				putimage(s->x*grid_x+mine_x,s->y*grid_y+mine_y,&click);
				list->board[s->y][s->x]=-1;
				++rigth;
				if(s->dir<7){
					++s->dir;
					int X=s->x+dir[s->dir][1];
					int Y=s->y+dir[s->dir][0];
					stack *p=(stack*)malloc(sizeof(stack));
					p->before=s;
					p->next=NULL;
					p->x=X;
					p->y=Y;
					p->dir=-1;
					s->next=p;
					s=s->next;
				}
				
				//返回上一级（出栈）
				else{
					q=s;
					s=s->before;
					free(q);
					if(s!=NULL){
						list->board[s->y][s->x]=0;--rigth;
					}
				}
			}

			//返回上一级（出栈）
			else{
				q=s;
				s=s->before;
				free(q);
				if(s!=NULL){
					list->board[s->y][s->x]=0;--rigth;
				}
			}
		}

		//返回上一级（出栈）
		else{
			q=s;
			s=s->before;
			free(q);
			if(s!=NULL){
				list->board[s->y][s->x]=0;--rigth;
			}
		}
	}
	free(s);
}



//结束游戏
void gameOver(List *list){
	for(int i=0;i<50;++i){
		for(int j=0;j<50;++j){
			if(list->board[i][j]==1){
				putimage(j*grid_x+mine_x,i*grid_y+mine_y,&mine);
			}
			if(list->board[i][j]==10){
				putimage(j*grid_x+mine_x,i*grid_y+mine_y,&mineWrong);
			}
			list->board[i][j]=0;
		}
	}
}

void otherGame(){
	MOUSEMSG mouse;
	int key=0;
	while(key!=2){
		if(MouseHit()){
			mouse=GetMouseMsg();
			switch(mouse.uMsg){
			case WM_LBUTTONDOWN:
				if(mouse.x>=mineX*grid_x/2 && mouse.x<mineX*grid_x/2+f_x  && mouse.y>=grid_y+2+b_y && mouse.y<grid_y+2+f_y+b_y){
					putimage(mineX*grid_x/2,grid_y+2+b_y,&regame);
					key=1;
				}
				break;
			case WM_LBUTTONUP:
				if(key==1 && mouse.x>=mineX*grid_x/2 && mouse.x<mineX*grid_x/2+f_x  && mouse.y>=grid_y+2+b_y && mouse.y<grid_y+2+f_y+b_y){
					key=2;
				}
				break;
			}
		}
	}
}//重开游戏

int operate(long start,int *replay,int *y,List *list){
    MOUSEMSG mouse;		// 定义鼠标消息
	int xy[2];//鼠标点击所在范围 的 格子 的坐标（格子坐标=格子右上角坐标）
				  //xy[0]为横坐标 xy[1]为纵坐标
	bool mouse_in_mine=false;//鼠标是否在雷区（fasle=否）
	//bool replay=false;//用户是否中途重新开始（fasle=否）

	
	mouse = GetMouseMsg();// 获取一条鼠标消息

	getGird(mouse.x,mouse.y,xy);//鼠标点击所在范围 的 格子 的坐标（格子坐标=格子右上角坐标）
	*y=mouse.y;
	int mm=(xy[0]-mine_x)/grid_x;//通过坐标转换对应数组横编号
	int nm=(xy[1]-mine_y)/grid_y;//通过坐标转换对应数组竖编号


	//菜单栏选项
	if(mouse.y<=30){

		//菜单栏
		int len=((mineX+1)*grid_x)/8;//菜单键范围
		int menuNum=(mouse.x-len/8)/len;//获取菜单键编号（从左往右为0~n号）

		//菜单选择键特效
		for(int k=0;k<8;++k){
			if(mouse.x>=len/8+len*k && mouse.x<=len*k+len/8+40 && mouse.y<=30){
				putimage(len*k+len/8,5, &key[k]);
			}else{
				putimage(len*k+len/8,5, &key[k+10]);
			}
		}
		switch(mouse.uMsg){
			case WM_LBUTTONDOWN:
				if(menuNum==0){mineX=20;mineY=10;mines=20;rank=0;}//初级
				if(menuNum==1){mineX=20;mineY=20;mines=50;rank=1;}//标准
				if(menuNum==2){mineX=30;mineY=20;mines=100;rank=2;}//中级
				if(menuNum==3){mineX=40;mineY=30;mines=300;rank=3;}//高级
				if(menuNum==4){
					bool trueInput=false;//输入正确否
					char s[30];
					int sr=0;
					while(trueInput==false){
						InputBox(s,30,"自定义难度\n格式:长度，宽度，地雷数\n自动调整不合规范的数据\n雷区长度m(20<=m<=50)\n雷区宽度n(10<=n<=40)\n地雷数量s(15<=s<m*n)");
					    sr=0;
						trueInput=true;
						while(s[sr]!=NULL){
							if(sr!=2 && sr!=5){
								if(s[sr]<'0' || s[sr]>'9'){
									trueInput=false;
								}
							}else{
								if(s[sr]!=','){
									trueInput=false;
								}
							}
							++sr;
						}
						if(trueInput==true){
							mineX=((int)s[0]-48)*10+((int)s[1]-48);
							mineY=((int)s[3]-48)*10+((int)s[4]-48);
							int ip=0;
							while(s[ip+7]!=NULL){
								++ip;
							}
							int j=ip;
							mines=0;
							while(j>=0){
								mines+=pow(10,j)*((int)s[ip-j+6]-48);
								--j;
							}
						}else{
							MessageBox(GetForegroundWindow(),"输入错误，请重新输入！","1 error(s),0 warning(s)",MB_ICONSTOP);
						}
					}
					if(mineX>50 || mineX<20){mineX=20;}
					if(mineY>40 || mineY<20){mineY=10;}
					if(mines>=mineX*mineY || mines<15){mines=15;}
					rank=5;
				}//自定义
				if(menuNum==5){
					char rule[1000]=
					{"一、鼠标操作\n左键进行扫雷，右键进行标记\n已标记的格子无法进行扫雷操作，再次右键取消标记\n\n二、游戏规则\n1.左击到的格子有地雷，游戏失败\n2.格子没有地雷的情况下附近九宫格有雷，显示地雷数量;\n   附近也没有地雷，则向外拓展\n3.所有没有地雷的格子被扫出，游戏胜利\n\n三、难度\n级别 长度 宽度 地雷\n初级   20   10   20\n标准   20   20   50\n中级   30   20   100\n高级   40   30   300\n"};
					MessageBox(GetForegroundWindow(),rule,"扫雷规则与操作",0);
				}
				if(menuNum==6){
					char us[1000]=
					{"\t\tWindows 10\t\t\t\n———————————————————————————\n\nWaysoft Windows \n\n版本 1402(内部版本 2018.11.1402)\n@2018 Waysoft(伪软 2018) 保留所有权利\nWindows 操作系统及其用户界面受美国和其他国家/地区待颁布或已\n颁布的知识产权法保护。\n\n\tby 一条咸鱼\n\n许可如下用户使用本产品\n\n\twindows 用户"};
					MessageBox(GetForegroundWindow(),us,"关于“扫雷”",0);
				}
				if(menuNum==7){	
					//输出数据
					MessageBox(GetForegroundWindow(),rankString,"只有第一名会被人们记住",0);}
				if(menuNum<5) return 2;
		}
	}

	if(xy[0]>=mine_x && xy[0]<mine_x+grid_x*mineX && xy[1]>=mine_y && xy[1]<mine_y+grid_y*mineY){
		mouse_in_mine=true;
	}//判断鼠标在不在雷区

	//雷区控制
	switch(mouse.uMsg){
		case WM_LBUTTONDOWN://左键按住
			if(mouse_in_mine==true && list->board[nm][mm]<10 && list->board[nm][mm]>=0){
				putimage(mineX*grid_x/2,grid_y+2+b_y,&sweep);
			}
			if(mouse.x>=mineX*grid_x/2 && mouse.x<mineX*grid_x/2+f_x  && mouse.y>=grid_y+2+b_y && mouse.y<grid_y+2+f_y+b_y){
				putimage(mineX*grid_x/2,grid_y+2+b_y,&regame);
				*replay=1;
			}
			break;

		case WM_LBUTTONUP://左键松开
			putimage(mineX*grid_x/2,grid_y+2+b_y,&face);
			if(*replay==1){
				return 2;
			}
			if(mouse_in_mine==true && list->board[nm][mm]<10 && list->board[nm][mm]>=0){//如果鼠标在雷区且这个格子没有被标记或探索
				
				if(list->board[nm][mm]%10!=1){
					int search=mineSearch(mm,nm,list);//格子附近地雷
					if(search==0){
						dfs(mm,nm,list);
						--rigth;
					}else{
						putimage(xy[0],xy[1],&mineNum[search-1]);
					}
					++rigth;
					list->board[nm][mm]=-1;//标记探索
				}else{
					//失败判断
					gameOver(list);
					putimage(mineX*grid_x/2,grid_y+2+b_y,&lose);
					putimage(xy[0],xy[1],&mineDie);
					return 1;
				}
			}
			//
			break;//左键 扫雷

		case WM_RBUTTONUP:
			if(mouse_in_mine==true && list->board[nm][mm]>=0){//鼠标在雷区且没有被探索
				if(list->board[nm][mm]>=10){//已被标记，取消标记
					putimage(xy[0], xy[1], &grid);
					list->board[nm][mm]-=10;
					--mark;
					markBou();
				}else{//未被标记，进行标记
					putimage(xy[0], xy[1], &flag);
					list->board[nm][mm]+=10;
					++mark;
					markBou();
				}
			}markBou();break;

		default:break;
	}

	//胜利判断
	if(rigth>=mineX*mineY-mines){
		gameOver(list);
		putimage(mineX*grid_x/2,grid_y+2+b_y,&win);

		if(gameTime<recordRank[rank] && rank<5){
			char sp[20];
			InputBox(sp,20,"您成为该难度区的扫雷专家，请输入15个字符以内的名字\n\n含有 ‘!’ ‘@’ ‘#’ ‘$’ ‘^’ ‘%’将被剔除");
			if(sp[0]==NULL){
				writeTXT("匿名");
			}else{
				writeTXT(sp);//存取排名
			}
		}
		return 1;
	}
	return 0;
}//鼠标操作

bool game(List *list){
	long start=GetTickCount();
	int replay=0;
	int y=100;
	gameStart(list);
	readTXT();//读取排行
	rigth=0;
	int choose=0;//0：继续游戏	1：要点击图标才能下一局	2：直接开始下一局
	while(choose==0){
		timeBou(start);
		if(MouseHit()){//如果有鼠标信息的话，就获取（不加上这个，慢到死！！）
			choose=operate(start,&replay,&y,list);
		}

	}
	replay=false;
	gameOver(list);
	if(choose==1){
		otherGame();
	}
	mark=0;
	for(int i=0;i<5;++i){
		recordRank[i]=9999;
	}
	closegraph();
	return true;
}

int main(){
	//雷区相关图片-------------------
    loadimage(&flag, "game//image//mineField//flag.jpg");
    loadimage(&grid, "game//image//mineField//grid.jpg");
    loadimage(&click, "game//image//mineField//click.jpg");
	loadimage(&mine, "game//image//mineField//mine.jpg");
	loadimage(&mineDie, "game//image//mineField//broke.jpg");
	loadimage(&mineWrong, "game//image//mineField//wrong.jpg");
	loadimage(&mineNum[0], "game//image//mineField//1.jpg");
	loadimage(&mineNum[1], "game//image//mineField//2.jpg");
	loadimage(&mineNum[2], "game//image//mineField//3.jpg");
	loadimage(&mineNum[3], "game//image//mineField//4.jpg");
	loadimage(&mineNum[4], "game//image//mineField//5.jpg");
	loadimage(&mineNum[5], "game//image//mineField//6.jpg");
	loadimage(&mineNum[6], "game//image//mineField//7.jpg");
	loadimage(&mineNum[7], "game//image//mineField//8.jpg");
	//--------------------------------

	//显示板（时间、标记、圆脸）相关--
	loadimage(&numBou[0], "game//image//displayBoard//n0.jpg");
	loadimage(&numBou[1], "game//image//displayBoard//n1.jpg");
	loadimage(&numBou[2], "game//image//displayBoard//n2.jpg");
	loadimage(&numBou[3], "game//image//displayBoard//n3.jpg");
	loadimage(&numBou[4], "game//image//displayBoard//n4.jpg");
	loadimage(&numBou[5], "game//image//displayBoard//n5.jpg");
	loadimage(&numBou[6], "game//image//displayBoard//n6.jpg");
	loadimage(&numBou[7], "game//image//displayBoard//n7.jpg");
	loadimage(&numBou[8], "game//image//displayBoard//n8.jpg");
	loadimage(&numBou[9], "game//image//displayBoard//n9.jpg");
	loadimage(&numBou[10],"game//image//displayBoard//out.jpg");
	loadimage(&lose, "game//image//displayBoard//lose.jpg");
	loadimage(&win, "game//image//displayBoard//win.jpg");
	loadimage(&bou, "game//image//displayBoard//boundary.jpg");
    loadimage(&regame, "game//image//displayBoard//regame.jpg");	
    loadimage(&face, "game//image//displayBoard//face.jpg");
	loadimage(&sweep, "game//image//displayBoard//sweep.jpg");
	//---------------------------------

    //菜单栏（调节难度，显示规则排名等）---
	loadimage(&white, "game//image//menu//white.jpg");
	loadimage(&key[0], "game//image//menu//easyIn.jpg");
	loadimage(&key[10],"game//image//menu//easyOut.jpg");
	loadimage(&key[1], "game//image//menu//staIn.jpg");
	loadimage(&key[11],"game//image//menu//staOut.jpg");
	loadimage(&key[2], "game//image//menu//midIn.jpg");
	loadimage(&key[12],"game//image//menu//midOut.jpg");
	loadimage(&key[3], "game//image//menu//higIn.jpg");
	loadimage(&key[13],"game//image//menu//higOut.jpg");
	loadimage(&key[4], "game//image//menu//cusIn.jpg");
	loadimage(&key[14],"game//image//menu//cusOut.jpg");
	loadimage(&key[5], "game//image//menu//rulIn.jpg");
	loadimage(&key[15],"game//image//menu//rulOut.jpg");	
	loadimage(&key[6], "game//image//menu//abuIn.jpg");
	loadimage(&key[16],"game//image//menu//abuOut.jpg");
	loadimage(&key[7], "game//image//menu//ranIn.jpg");
	loadimage(&key[17],"game//image//menu//ranOut.jpg");
	//---------------------------------

	//各区域边界-----------------------
	loadimage(&right, "game//image//border//right.jpg");
    loadimage(&left, "game//image//border//left.jpg");
	loadimage(&up, "game//image//border//up.jpg");
    loadimage(&down, "game//image//border//down.jpg");
    loadimage(&l_u, "game//image//border//l-u.jpg");
	loadimage(&l_d, "game//image//border//l-d.jpg");
    loadimage(&r_u, "game//image//border//r-u.jpg");
	loadimage(&r_d, "game//image//border//r-d.jpg");
	//----------------------------------

	//初始化顺序表
	List *list=(List*)malloc(sizeof(List));
	while(game(list)){}
	free(list);
	return 0;
}

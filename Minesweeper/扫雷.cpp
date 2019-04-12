/**********************************************

 Waysoft Campany	α��˾

ɨ�ף���������

���Ӵ�С��15*15����
ͼƬ��Դ��world�ĵ�����ͼƬ�����ƣ�

�����ɨ�� 
�Ҽ������
�ѱ�ǵĲ���ɨ��

�������壺С����
��������20<n<40 15<m<50
���׸�����15<x<m*n

����չ�ģ�
1.���а���ʾǰn��
2.���а���Ϣ����
3.���а��ļ���ɾ�����������ɣ�����������Ϣ��ʼ��
4.���а���Ϣ����

***********************************************/

//ͷ�ļ���------------------------
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <ctime>
#include <cstdio>
#include <math.h>
//--------------------------------

//�Զ������ݣ�������ѡ��ť����ʾ�壩--------
//��λ������
#define mine_x 15                            //���������̨��߾��루�������߽磩
#define mine_y 105                           //���������̨�������루�������߽磩
#define grid_x 15                            //�������Ӻ��
#define grid_y 15                            //���������ݾ�
#define bou_x 200                            //���������
#define bou_y 15                             //��������ݾ�
#define m_x 10                               //��������
#define m_y 20                               //�������ݾ�
#define f_x 20                               //Ц�����
#define f_y 20                               //Ц���ݾ�
#define b_y 30                               //���ư������̨��������                               
//--------------------------------

//ͼƬԪ��------------------------
IMAGE mineNum[8];							 //ͼƬ���飺���Ӹ�������������0~7��ʾ��������1~8
IMAGE numBou[11];                            //ͼƬ���飺���������0~9�����������0~9��10�����������
IMAGE grid;                                  //����ͼ��(δ���)
IMAGE click;                                 //����ͼ��(�ѵ��)
IMAGE flag;                                  //��ǵ���ͼ��
IMAGE mine;                                  //����ͼ��
IMAGE mineDie;                               //����ͼ��(����)
IMAGE mineWrong;                             //����ͼ��(�жϴ���) 
IMAGE bou;                                   //���ͼ��
IMAGE regame;								 //���¿�ʼ����
IMAGE right;                                 //�ҽ�
IMAGE left;                                  //���
IMAGE up;                                    //�Ͻ�
IMAGE down;                                  //�½�
IMAGE l_u;                                   //����
IMAGE l_d;                                   //����
IMAGE r_u;                                   //����
IMAGE r_d;                                   //����
IMAGE face;                                  //Բ������ͨ��
IMAGE sweep;                                 //Բ����ɨ�ף�
IMAGE lose;                                  //Բ����ʧ�ܣ�
IMAGE win;                                   //Բ����ʤ����
IMAGE white;                                 //�˵���
IMAGE key[20];                               //��������:0~9��ʾ9�����������۽���  10~11��ʾ9���������Ǿ۽���
//---------------------------------

//��������(��������)---------------
//���е��ף�ֵ%10=1
//��ǵ��ף�ֵ>=10
//�Ѿ�̽����ֵ=-1;
//����(1)��ǵ���+����=11 (2)��ǵ���+����=10 (3)����=0 (4)����=1

int mineX=20;                                 //�������
int mineY=20;                                 //�����ݾ�
int mines=50;                                 //��������
int rigth;                                    //��ǵ���+��ɨ�������
int mark=0;                                   //���       
int rank=1;                                   //�Ѷȼ���          
int gameTime;                                 //��Ϸ����ʱ��
char rankString[240];                         //���а������ַ���
int recordRank[4];                            //���Ѷ�����
//---------------------------------

//˳��� ���ڴ�Ÿ��ӵ�����Ϣ
typedef struct SequenceList{
	int board[51][51];
}List;

typedef struct stack{
	int x;
	int y;
	int dir;
	stack *next;
	stack *before;
}Stack;//��������ջ,˫������

//��λ����ͨ��������꣬ȷ����������
//��������ڷ�Χ �� ���� �����꣨��������=�������Ͻ����꣩
//�㷨��x-=(n-λ����)%���ӿ�
void getGird(int x,int y,int xy[2]){
	xy[0]=x-=(x-mine_x)%grid_x;
	xy[1]=y-=(y-mine_y)%grid_y;
}

//������ȡ�ļ��е�����
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

//ɨ��Ӣ�۰��ȡ
void readTXT() 
{
	FILE *fp;
	char mapth[4][50]={"game//save//easy.txt","game//save//standard.txt","game//save//middle.txt","game//save//hard.txt"};
	char reading[4][60];
	for(int g=0;g<4;++g){
		if((fp=fopen(mapth[g],"r"))==NULL){
			printf("���ļ�����!\n");
			exit(0);
		}
	
		//��ȡԭʼ����
		while(!feof(fp))
		{
		fscanf(fp,"%s",reading[g]);	
		}
		
	}
	//���rankString
	for(int i2=0;i2<240;++i2){
		rankString[i2]=NULL;
	}
	//��ŵ�rankString
	int hh=0;
	for(int i1=0;i1<4;++i1){
		int j1=0;
		while(reading[i1][j1]!=NULL){
			rankString[hh]=reading[i1][j1];
			++hh;
			++j1;
		}
	}

	//�洢�����Ѷȵ����ʱ��
	int point[1]={0};
	while(rankString[point[0]]!=NULL){
		if(rankString[point[0]]=='!'){//��ȡ��easy
			point[0]+=3;
			recordRank[0]=readNum(point[0]);
		}
		if(rankString[point[0]]=='@'){//��ȡ��standard
			point[0]+=3;
			recordRank[1]=readNum(point[0]);
		}
		if(rankString[point[0]]=='$'){//��ȡ��middle
			point[0]+=3;
			recordRank[2]=readNum(point[0]);
		}
		if(rankString[point[0]]=='^'){//��ȡ��hight
			point[0]+=3;
			recordRank[3]=readNum(point[0]);
		}
		++point[0];
	}

	//��ʽ������
	//��ȡ��#���滻��\n(����)
	//��ȡ��%���滻��\t(TAB)
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

//ɨ��Ӣ�۰�洢
void writeTXT(char sp[20]){
	char mapth[4][50]={"game//save//easy.txt","game//save//standard.txt","game//save//middle.txt","game//save//hard.txt"};//·��
	FILE *p=fopen(mapth[rank],"w");
	char ch[4][200]= {"��!%%","��׼@%%","�е�$%%","����^%%"};//�Ѷ�
	char sec[5]="��%%";//��λ
	int r=0;
	int t=gameTime;
	if(gameTime==0){
		++gameTime;
		++t;
	}
	int digit=0;//̽��λ��
	while(t>0)
	{
		t/=10;++digit;
	}
	r=6+digit+1;//�����ѶȺ�λ��
	//�����ʱ
	while(digit>0){
		ch[rank][6+digit]=(char)(gameTime%10+48);
		gameTime/=10;
		--digit;
	}
	//��ŵ�λ
	int i=0;
	while(sec[i]!=NULL){
		ch[rank][r]=sec[i];
		++r;
		++i;
	}

	//�������
	i=0;
	while(sp[i]!=NULL){
		if(sp[i]!='!' && sp[i]!='@' && sp[i]!='#' && sp[i]!='$' && sp[i]!='%' && sp[i]!='^'){
			ch[rank][r]=sp[i];
			++r;
		}
		++i;
	}
	ch[rank][r]='#';//����ո�
	fwrite(ch[rank],1,sizeof(ch[rank]),p);  //д������
	fclose(p);
}


//ʱ����ʾ��
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

//�����ʾ��
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
}//�����ʾ��

//��Ϸ��ʼ��ģ��
void gameStart(List *list){
	//��Ϸ�����
    initgraph((mineX+1)*grid_x+mine_x,(mineY+1)*grid_y+mine_y);

	//����
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
				list->board[i][j]=0;//���ױ���ÿ�
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

	//�����
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
	//Ц��
	putimage(mineX*grid_x/2,grid_y+2+b_y,&face);

	for(int c=2;c<6;++c){
		//��Ǽ�����
		putimage(m_x*c,m_y+b_y,&numBou[0]);

		//ʱ�������
		putimage(mineX*grid_x+mine_x-m_x*c,m_y+b_y,&numBou[0]);
	}
	//ѡ���
	for(int k=0;k<mineX+2;++k){
		putimage((k-1)*grid_x+mine_x,0, &white);
	}

	//�˵���
	int len=((mineX+1)*grid_x)/8;
	for(int kn=0;kn<8;++kn){
		putimage(len*kn+len/8,5, &key[kn+10]);
	}

	//�������
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

//���Ӹ�������̽����
int mineSearch(int m,int n,List *list){//m��nΪ������ݱ��;mm��mnΪ�������ݸ�����
	int h=0;
	int dir[8][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};
	//�����������顣8��ʾ8�����򣨸��Ӹ���8�����ӣ���2��ʾx��y
	//����x+dir[0][1],y+dir[0][0]����x+1,y+1,�����ӣ�x��y�������Ϸ� 
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
}//��ⷽ�鸽����������

/*���Ѻ������ݹ飩
void dfs(int x,int y,List *list){
	int dir[8][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};//��������
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

//�ǵݹ�����
void dfs(int x,int y,List *list){
	int dir[8][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};//��������
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

				//������һ������ջ��
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
				
				//������һ������ջ��
				else{
					q=s;
					s=s->before;
					free(q);
					if(s!=NULL){
						list->board[s->y][s->x]=0;--rigth;
					}
				}
			}

			//������һ������ջ��
			else{
				q=s;
				s=s->before;
				free(q);
				if(s!=NULL){
					list->board[s->y][s->x]=0;--rigth;
				}
			}
		}

		//������һ������ջ��
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



//������Ϸ
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
}//�ؿ���Ϸ

int operate(long start,int *replay,int *y,List *list){
    MOUSEMSG mouse;		// ���������Ϣ
	int xy[2];//��������ڷ�Χ �� ���� �����꣨��������=�������Ͻ����꣩
				  //xy[0]Ϊ������ xy[1]Ϊ������
	bool mouse_in_mine=false;//����Ƿ���������fasle=��
	//bool replay=false;//�û��Ƿ���;���¿�ʼ��fasle=��

	
	mouse = GetMouseMsg();// ��ȡһ�������Ϣ

	getGird(mouse.x,mouse.y,xy);//��������ڷ�Χ �� ���� �����꣨��������=�������Ͻ����꣩
	*y=mouse.y;
	int mm=(xy[0]-mine_x)/grid_x;//ͨ������ת����Ӧ�������
	int nm=(xy[1]-mine_y)/grid_y;//ͨ������ת����Ӧ���������


	//�˵���ѡ��
	if(mouse.y<=30){

		//�˵���
		int len=((mineX+1)*grid_x)/8;//�˵�����Χ
		int menuNum=(mouse.x-len/8)/len;//��ȡ�˵�����ţ���������Ϊ0~n�ţ�

		//�˵�ѡ�����Ч
		for(int k=0;k<8;++k){
			if(mouse.x>=len/8+len*k && mouse.x<=len*k+len/8+40 && mouse.y<=30){
				putimage(len*k+len/8,5, &key[k]);
			}else{
				putimage(len*k+len/8,5, &key[k+10]);
			}
		}
		switch(mouse.uMsg){
			case WM_LBUTTONDOWN:
				if(menuNum==0){mineX=20;mineY=10;mines=20;rank=0;}//����
				if(menuNum==1){mineX=20;mineY=20;mines=50;rank=1;}//��׼
				if(menuNum==2){mineX=30;mineY=20;mines=100;rank=2;}//�м�
				if(menuNum==3){mineX=40;mineY=30;mines=300;rank=3;}//�߼�
				if(menuNum==4){
					bool trueInput=false;//������ȷ��
					char s[30];
					int sr=0;
					while(trueInput==false){
						InputBox(s,30,"�Զ����Ѷ�\n��ʽ:���ȣ���ȣ�������\n�Զ��������Ϲ淶������\n��������m(20<=m<=50)\n�������n(10<=n<=40)\n��������s(15<=s<m*n)");
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
							MessageBox(GetForegroundWindow(),"����������������룡","1 error(s),0 warning(s)",MB_ICONSTOP);
						}
					}
					if(mineX>50 || mineX<20){mineX=20;}
					if(mineY>40 || mineY<20){mineY=10;}
					if(mines>=mineX*mineY || mines<15){mines=15;}
					rank=5;
				}//�Զ���
				if(menuNum==5){
					char rule[1000]=
					{"һ��������\n�������ɨ�ף��Ҽ����б��\n�ѱ�ǵĸ����޷�����ɨ�ײ������ٴ��Ҽ�ȡ�����\n\n������Ϸ����\n1.������ĸ����е��ף���Ϸʧ��\n2.����û�е��׵�����¸����Ź������ף���ʾ��������;\n   ����Ҳû�е��ף���������չ\n3.����û�е��׵ĸ��ӱ�ɨ������Ϸʤ��\n\n�����Ѷ�\n���� ���� ��� ����\n����   20   10   20\n��׼   20   20   50\n�м�   30   20   100\n�߼�   40   30   300\n"};
					MessageBox(GetForegroundWindow(),rule,"ɨ�׹��������",0);
				}
				if(menuNum==6){
					char us[1000]=
					{"\t\tWindows 10\t\t\t\n������������������������������������������������������\n\nWaysoft Windows \n\n�汾 1402(�ڲ��汾 2018.11.1402)\n@2018 Waysoft(α�� 2018) ��������Ȩ��\nWindows ����ϵͳ�����û���������������������/�������䲼����\n�䲼��֪ʶ��Ȩ��������\n\n\tby һ������\n\n��������û�ʹ�ñ���Ʒ\n\n\twindows �û�"};
					MessageBox(GetForegroundWindow(),us,"���ڡ�ɨ�ס�",0);
				}
				if(menuNum==7){	
					//�������
					MessageBox(GetForegroundWindow(),rankString,"ֻ�е�һ���ᱻ���Ǽ�ס",0);}
				if(menuNum<5) return 2;
		}
	}

	if(xy[0]>=mine_x && xy[0]<mine_x+grid_x*mineX && xy[1]>=mine_y && xy[1]<mine_y+grid_y*mineY){
		mouse_in_mine=true;
	}//�ж�����ڲ�������

	//��������
	switch(mouse.uMsg){
		case WM_LBUTTONDOWN://�����ס
			if(mouse_in_mine==true && list->board[nm][mm]<10 && list->board[nm][mm]>=0){
				putimage(mineX*grid_x/2,grid_y+2+b_y,&sweep);
			}
			if(mouse.x>=mineX*grid_x/2 && mouse.x<mineX*grid_x/2+f_x  && mouse.y>=grid_y+2+b_y && mouse.y<grid_y+2+f_y+b_y){
				putimage(mineX*grid_x/2,grid_y+2+b_y,&regame);
				*replay=1;
			}
			break;

		case WM_LBUTTONUP://����ɿ�
			putimage(mineX*grid_x/2,grid_y+2+b_y,&face);
			if(*replay==1){
				return 2;
			}
			if(mouse_in_mine==true && list->board[nm][mm]<10 && list->board[nm][mm]>=0){//���������������������û�б���ǻ�̽��
				
				if(list->board[nm][mm]%10!=1){
					int search=mineSearch(mm,nm,list);//���Ӹ�������
					if(search==0){
						dfs(mm,nm,list);
						--rigth;
					}else{
						putimage(xy[0],xy[1],&mineNum[search-1]);
					}
					++rigth;
					list->board[nm][mm]=-1;//���̽��
				}else{
					//ʧ���ж�
					gameOver(list);
					putimage(mineX*grid_x/2,grid_y+2+b_y,&lose);
					putimage(xy[0],xy[1],&mineDie);
					return 1;
				}
			}
			//
			break;//��� ɨ��

		case WM_RBUTTONUP:
			if(mouse_in_mine==true && list->board[nm][mm]>=0){//�����������û�б�̽��
				if(list->board[nm][mm]>=10){//�ѱ���ǣ�ȡ�����
					putimage(xy[0], xy[1], &grid);
					list->board[nm][mm]-=10;
					--mark;
					markBou();
				}else{//δ����ǣ����б��
					putimage(xy[0], xy[1], &flag);
					list->board[nm][mm]+=10;
					++mark;
					markBou();
				}
			}markBou();break;

		default:break;
	}

	//ʤ���ж�
	if(rigth>=mineX*mineY-mines){
		gameOver(list);
		putimage(mineX*grid_x/2,grid_y+2+b_y,&win);

		if(gameTime<recordRank[rank] && rank<5){
			char sp[20];
			InputBox(sp,20,"����Ϊ���Ѷ�����ɨ��ר�ң�������15���ַ����ڵ�����\n\n���� ��!�� ��@�� ��#�� ��$�� ��^�� ��%�������޳�");
			if(sp[0]==NULL){
				writeTXT("����");
			}else{
				writeTXT(sp);//��ȡ����
			}
		}
		return 1;
	}
	return 0;
}//������

bool game(List *list){
	long start=GetTickCount();
	int replay=0;
	int y=100;
	gameStart(list);
	readTXT();//��ȡ����
	rigth=0;
	int choose=0;//0��������Ϸ	1��Ҫ���ͼ�������һ��	2��ֱ�ӿ�ʼ��һ��
	while(choose==0){
		timeBou(start);
		if(MouseHit()){//����������Ϣ�Ļ����ͻ�ȡ�������������������������
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
	//�������ͼƬ-------------------
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

	//��ʾ�壨ʱ�䡢��ǡ�Բ�������--
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

    //�˵����������Ѷȣ���ʾ���������ȣ�---
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

	//������߽�-----------------------
	loadimage(&right, "game//image//border//right.jpg");
    loadimage(&left, "game//image//border//left.jpg");
	loadimage(&up, "game//image//border//up.jpg");
    loadimage(&down, "game//image//border//down.jpg");
    loadimage(&l_u, "game//image//border//l-u.jpg");
	loadimage(&l_d, "game//image//border//l-d.jpg");
    loadimage(&r_u, "game//image//border//r-u.jpg");
	loadimage(&r_d, "game//image//border//r-d.jpg");
	//----------------------------------

	//��ʼ��˳���
	List *list=(List*)malloc(sizeof(List));
	while(game(list)){}
	free(list);
	return 0;
}

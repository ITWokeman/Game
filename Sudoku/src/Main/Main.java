package Main;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Random;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextArea;


/*生成随机数独
 * 流程：
 * 1.生成9*9空白数独数组
 * 2.随机往数独数组填数
 * 3.DFS生成数独标准解（即数独数组81个格子都填满数字）
 * 4.先挖n个空，对挖完空的数独进行求解（全部）
 * 5.将所有解与标准解进行对比，将不一样的地方记录下。这些地方不允许被被挖空
 */
class SudokuMaker {
	private int[][] Arr;//临时数组
	private int [][]Sudoku;
	private int [][]Answer;//答案
	private int [][]Game;
	
	public SudokuMaker(){
		this.Arr=new int[9][9];
		this.Sudoku=new int[9][9];
		this.Answer=new int[9][9];
		rand();
		DFS(Arr,0,false);
		diger();
		DevTools.showGame(Game);
		DevTools.showAnswer(Answer);
	}
	
	private void rand(){
		int t=0;
		int x,y,num;
		//先往数组里面随机丢t个数
		while(t<15){//t不宜多，否则运行起来耗费时间；t也不宜少，否则生成的游戏看起来一点也不随机
			x=new Random().nextInt(9);
			y=new Random().nextInt(9);
			num=new Random().nextInt(9)+1;
			if(Arr[y][x]==0){
				if(isTrue(Arr,x,y,num)==true){
					Arr[y][x]=num;++t;
				}
			}
		}
	}
	
	//判断该数字填写的地方是否符合数独规则
	public static boolean isTrue(int arr[][],int x,int y,int num){//数字横坐标；数字纵坐标；数字数值
		//判断中单元格（3*3）
		for(int i=(y/3)*3;i<(y/3+1)*3;++i){
			for(int j=(x/3)*3;j<(x/3+1)*3;++j){
				if(arr[i][j]==num ){return false;}
			}
		}
		//判断横竖
		for(int i=0;i<9;++i){
			if((arr[i][x]==num || arr[y][i]==num)){return false;}	
		}
		return true;
	}
	
	//深度优先搜索寻找
	//绝对有很多种解法，但是我们只需要第一个解出来的
	private boolean flag=false;//判断是否不再求解
	int total=0;
	private void DFS(int arr[][],int n,boolean all){//arr是数独数组，n是探索深度（一共81个格子，深度为81,n为0~80），是否要求全部解
		//n/9为格子的纵坐标，n%9为格子的横坐标
		if(n<81){
			//如果已经求出了一种解，终止递归就行了，不用继续求下去了
			if(flag==true && all==false){return;}
			
			if(arr[n/9][n%9]==0){
				for(int i=1;i<10;++i){
					if(isTrue(arr,n%9,n/9,i)==true){
						arr[n/9][n%9]=i;
						DFS(arr,n+1,all);
						arr[n/9][n%9]=0;
					}
				}
			}else{
				DFS(arr,n+1,all);
			}
			
		}else{
			if(all==false){
				flag=true;
				for(int i=0;i<9;++i){
					for(int j=0;j<9;++j){
						Sudoku[i][j]=arr[i][j];
						Answer[i][j]=arr[i][j];
					}
				}
			}else{
				for(int i=0;i<9;++i){
					for(int j=0;j<9;++j){
						if(arr[i][j]!=Answer[i][j]){
							Game[i][j]=Answer[i][j];
							i=10;j=10;
							break;
						}
					}
				}
			}
		}
	}
	
	//给数独挖空
	//保证仅有一解
	private void diger(){
		int t=55;
		Game=new int[9][9];
		while(t>0){
			int x=new Random().nextInt(9);
			int y=new Random().nextInt(9);
			if(Sudoku[y][x]!=0){
				Sudoku[y][x]=0;--t;
			}
		}
		
		for(int i=0;i<9;++i){
			for(int j=0;j<9;++j){
				Game[i][j]=Sudoku[i][j];
			}
		}
		
		DFS(Sudoku,0,true);
	}
	
	//获取最终数独
	public int[][] getArr(){
		return this.Game;
	}
	
	//获取数独答案
	public int[][] getAnswer(){
		return this.Answer;
	}
}


//游戏界面
class UI{
	private JFrame gameUI;
	private JLabel gameGrid;//数独81宫格
	private SudokuMaker game;//数独
	private JLabel[][] smallGrid;//数独小格子
	private UserEvents [][] mouseListen;//监听器
	

	
	public UI(){
		gameUI=new JFrame();
		
		gameUI.setVisible(true);
		gameUI.setLayout(null);
		gameUI.setSize(600,430);
		gameUI.setResizable(false);//不允许窗口最大化
		gameUI.setLocationRelativeTo(null);
		
		JButton bt1=new JButton("生成新数独");
		gameUI.add(bt1);
		bt1.setBounds(400,10,100,20);
		bt1.addActionListener(new OtherGameEvent());
		
		JButton bt2=new JButton("显示答案");
		gameUI.add(bt2);
		bt2.setBounds(400,110,100,20);
		bt2.addActionListener(new ShowAnswer());
		
		
		gameGrid=new JLabel();
		gameGrid.setBounds(10,10,365,365);
		gameUI.add(gameGrid);
		java.net.URL imgURL = Main.class.getResource("/img/Grid.jpg");
		gameGrid.setIcon(new ImageIcon(imgURL));
		gameGrid.setOpaque(true);
		
		Font font=new Font("宋体",Font.BOLD,25);
		
		smallGrid=new JLabel[9][9];
		mouseListen=new UserEvents[9][9];
		for(int i=0;i<9;++i){
			for(int j=0;j<9;++j){
				smallGrid[i][j]=new JLabel("",JLabel.CENTER);
				gameGrid.add(smallGrid[i][j]);
				mouseListen[i][j]=new UserEvents(smallGrid[i][j],i,j,false);
				smallGrid[i][j].setFont(font);
				smallGrid[i][j].setBounds(j*40+1,i*40+1,40,40);
				smallGrid[i][j].addMouseListener(mouseListen[i][j]);
			}
		}
		newGame();
	}
	
	//新游戏
	private void newGame(){

		game=new SudokuMaker();
		int [][]gameArr=game.getArr();
		for(int i=0;i<9;++i){
			for(int j=0;j<9;++j){
				if(gameArr[i][j]!=0){	
					smallGrid[i][j].setText(gameArr[i][j]+"");
					mouseListen[i][j].setUse(false);
					smallGrid[i][j].setForeground(Color.black);
				}else{
					smallGrid[i][j].setText(null);
					mouseListen[i][j].setUse(true);
				}
			}
		}
	}
	
	private class ShowAnswer implements ActionListener{

		@Override
		public void actionPerformed(ActionEvent arg0) {
			// TODO Auto-generated method stub
			for(int i=0;i<9;++i){
				for(int j=0;j<9;++j){
					if(mouseListen[i][j].getUse()==true){
						smallGrid[i][j].setText(game.getAnswer()[i][j]+"");
						smallGrid[i][j].setForeground(Color.BLUE);
						mouseListen[i][j].setUse(false);
					}
				}
			}
		}
		
	}
	
	private class OtherGameEvent implements ActionListener{
		@Override
		public void actionPerformed(ActionEvent arg0) {
			// TODO Auto-generated method stub
			newGame();
		}
	}
	
	private class UserEvents implements MouseListener{
		
		private JTextArea jta;
		private JLabel focus;//聚焦
		private int gridX;
		private int gridY;
		private boolean isUse;//是否开启事件


		public UserEvents(JLabel f,int y,int x,boolean u){
			focus=f;
			gridX=x;
			gridY=y;
			isUse=u;
			
			jta=new JTextArea();
			jta.setBounds(5,5,30,30);
			jta.setVisible(false);
			jta.setOpaque(false);
			jta.setFont(new Font("宋体",Font.BOLD,25));
			focus.add(jta);
		}
		
		@Override
		public void mouseClicked(MouseEvent arg0) {
			// TODO Auto-generated method stub
			if(isUse==true){

				focus.setBorder(BorderFactory.createLineBorder(Color.red,5));
				jta.setVisible(true);
				focus.setText(null);
			}
		}

		@Override
		public void mouseEntered(MouseEvent arg0) {
			// TODO Auto-generated method stub

		}
		
		@Override
		public void mouseExited(MouseEvent arg0) {
			// TODO Auto-generated method stub
			int X=arg0.getX(),Y=arg0.getY();
			if(isUse==true){
				if(X<=0 || X>=40 || Y<=0 || Y>=40){
					focus.setBorder(BorderFactory.createLineBorder(Color.red,0));
					try{
						int t=new Integer(jta.getText());
						if(t>0 && t<10){
							game.getArr()[gridY][gridX]=0;
							if(SudokuMaker.isTrue(game.getArr(), gridX, gridY, t)==true){
								focus.setForeground(Color.green);
							}else{
								focus.setForeground(Color.red);
							}
							game.getArr()[gridY][gridX]=t;
							}
							focus.setText(jta.getText());
					}catch(Exception e){
						jta.setText(null);
					}
					jta.setVisible(false);
				}
			}
		}

		@Override
		public void mousePressed(MouseEvent arg0) {
			// TODO Auto-generated method stub
			if(this.isUse==true){
				focus.setBorder(BorderFactory.createLineBorder(Color.red,5));
			}
		}

		@Override
		public void mouseReleased(MouseEvent arg0) {
			// TODO Auto-generated method stub
		}
		
		public void setUse(boolean u){
			this.isUse=u;
			if(u==true){
				jta.setText(null);
			}else{
				focus.setBorder(BorderFactory.createLineBorder(Color.red,0));
			}
		}
		
		
		public boolean getUse(){
			return isUse;
		}

	}
	
}

public class Main {

	
	//测试方法
	public static void main(String arg[]){
		new UI();
	}
}

//开发工具包
class DevTools{
	
	public static void showAnswer(int arr[][]){
		System.out.println("\n答案：");
		for(int i[]:arr){
			for(int j:i){
				System.out.print(j);
			}
			System.out.println();
		}
		System.out.println();
	}
	
	public static void showGame(int arr[][]){
		System.out.println("\n题目：");
		int total=0;
		for(int i[]:arr){
			for(int j:i){
				System.out.print(j);
				if(j==0){++total;}
			}
			System.out.println();
		}
		System.out.println("挖空数："+total);
	}
}

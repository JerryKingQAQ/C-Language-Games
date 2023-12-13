#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#define HIGH 23
#define WIDTH 60
#define ENEMYNUMBER 5 //敌机数目

int canvas[HIGH+1][WIDTH+1];//记录二维数组游戏画面对应的元素
						//0输出空格 1输出飞机 2输出子弹 3是敌机 

int position_x, position_y;
int bullet_x, bullet_y;
int enemy_x[ENEMYNUMBER], enemy_y[ENEMYNUMBER];
int BulletWidth;//子弹宽度
int BulletNumber;//子弹数目
int EnemySpeed;
int operationNumber;

int score;
int speed[10];

void GameOver();

void gotoxy(int x, int y)  //光标移动到(x,y)的位置
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void color(short x)	//自定义函根据参数改变颜色 
{
	if (x >= 0 && x <= 15)//参数在0-15的范围颜色
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);	//只有一个参数，改变字体颜色 
	else//默认的颜色白色
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


void startup()  //数据初始化
{
	int k;
	
	position_x = WIDTH / 2;
	position_y = HIGH - 5;
	
	for (k = 0; k < ENEMYNUMBER; k++)//输出多台敌机
	{
		enemy_x[k] = rand() % WIDTH;
		enemy_y[k] = rand() % 4;
		canvas[enemy_y[k]][enemy_x[k]] = 3;
	}
	
	EnemySpeed = 50;

	BulletWidth = 0;
	BulletNumber = 10;

	canvas[position_y][position_x] = 1;
	
	score = 0;

	operationNumber = 3;
}

void show()  //显示画面
{

	int i, j;//i是y轴 j是x轴
	gotoxy(0, 0); //清屏函数 光标移动到左上角

	for (i = 0; i < HIGH; i++)
	{
		//printf("|");
		for (j = 0; j <WIDTH; j++)
		{
			if (canvas[i][j] == 1)
			{
				color(13);
				printf("*");//输出飞机
				color(16);
			}
				
			else if (canvas[i][j] == 0)
				printf(" ");
			else if (canvas[i][j] == 2)//输出子弹
			{
				color(14);
				printf("|");
				color(16);
			}
			else if (canvas[i][j] == 3)//输出敌机
				printf("@");
		}
		printf("\n");
	}
	color(4); printf("得分 = %-2d\n", score);
	color(6); printf("子弹数目 = %-2d \n", BulletNumber);
	color(15); printf("\nWASD移动 空格发射");
	printf("\n分数越高子弹越强！敌军也将变快！\n");
	printf("敌军到达下边界扣分，分数小于0或碰到敌机时游戏结束\n");
	color(16);
}


void updateWithoutInput() //用户输入无关的操作
{
	int i, j, k;
	//srand((unsigned)time(NULL));//随机化种子

	for (i = 0; i < operationNumber; i++)
	{
		speed[i]++;
	}

	//子弹上市
	if (speed[0] == 2)
	{
		for (i = 0; i < HIGH; i++)//遍历查找子弹
		{
			for (j = 0; j < WIDTH; j++)
			{
				if (canvas[i][j] == 2) //使得子弹自动向上移动 如果是子弹
				{
					for (k = 0;k < ENEMYNUMBER; k++)
					{
						if ((i == enemy_y[k] && j == enemy_x[k])||( i == enemy_y[k]+1 && j == enemy_x[k]))//击中敌机的时候
						{
							srand((int)time(NULL));//随机数生成
							canvas[enemy_y[k]][enemy_x[k]] = 0;
							enemy_x[k] = rand() % (WIDTH-1);
							enemy_y[k] = rand() % 1;
							score++;

							if (score % 5 == 0)//子弹变宽 敌机变快 要写在击中敌机的瞬间
							{
								BulletWidth += 1;
								EnemySpeed = EnemySpeed - 5;
							}
						}

					}

					canvas[i][j] = 0;//子弹消失 

					if (i > 0)//子弹向上移动
					{
						canvas[i - 1][j] = 2;
					}
					else
					{
						canvas[i][j] = 0;
					}
				}
			}
		}
		speed[0] = 0;
	}


	//敌机下落
	if(speed[1] == EnemySpeed)
	{
		for (k = 0; k < ENEMYNUMBER; k++)
		{
			if (enemy_y[k] < HIGH)//敌机自动下落
			{
				canvas[enemy_y[k]][enemy_x[k]] = 0;
				enemy_y[k]++;
				canvas[enemy_y[k]][enemy_x[k]] = 3;
			}
			else if(enemy_y[k] >= HIGH)
			{
				score-=1;
				canvas[enemy_y[k]][enemy_x[k]] = 0;
				enemy_x[k] = rand() % WIDTH;
				enemy_y[k] = rand() % 2;
			}
		}
		speed[1] = 0;//减速函数归零
		
	}
	
	//增加子弹
	if (speed[2] == 100)
	{
		BulletNumber++;
		speed[2] = 0;
	}

	GameOver();
}

void GameOver()
{
	int k;
	if (score < 0)
	{
		printf("GameOver!");
		system("pause");
	}
	if(score>50)
	{
		printf("You Win!");
		system("pause");
	}
	for (k = 0; k < ENEMYNUMBER; k++)
	{
		if (position_x == enemy_x[k] && position_y == enemy_y[k])
		{
			printf("GameOver!");
			system("pause");
		}
	}
}

void updateWithInput() //按键操作
{
	char input;
	if (_kbhit())//当按键时执行
	{
		input = _getch();
			if (input == 'a')
			{
				canvas[position_y][position_x] = 0;//将原位置变为空格
				if(position_x>0)
					position_x--;
				canvas[position_y][position_x] = 1;
			}
			if (input == 'd')
			{
				canvas[position_y][position_x] = 0;//将原位置变为空格
				if(position_x<WIDTH-1)
					position_x++;
				canvas[position_y][position_x] = 1;
			}
			if (input == 'w')
			{
				canvas[position_y][position_x] = 0;//将原位置变为空格
				if(position_y>0)
					position_y--;
				canvas[position_y][position_x] = 1;
			}
			if (input == 's')
			{
				canvas[position_y][position_x] = 0;//将原位置变为空格
				if(position_y<HIGH-1)
					position_y++;
				canvas[position_y][position_x] = 1;
			}

			if (BulletNumber > 0)
			{
				if (input == ' ')
				{
					int Bulletleft, BulletRight;
					Bulletleft = position_x - BulletWidth;
					if (Bulletleft < 0)
						Bulletleft = 0;
					BulletRight = position_x + BulletWidth;
					if (BulletRight >= WIDTH - 1)
						BulletRight = WIDTH - 1;
					bullet_y = position_y - 1;
					bullet_x = position_x;

					for (int i = Bulletleft; i <= BulletRight; i++)
					{
						canvas[bullet_y][i] = 2;
					}
					
					BulletNumber--;
				}
			}
				
		
		
		if (input == 27)//ESC暂停
			system("pause");

	}
}

void CleanCursor()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}//去除控制台光标

int main()
{
	srand((unsigned)time(NULL));//随机化种子
	CleanCursor();//去除控制台光标
	startup();//数据初始化
	
	while (1)
	{
		srand((int)time(NULL));//随机数生成
		show();//显示画面
		updateWithoutInput();//用户输入无关的操作
		updateWithInput();//有关的操作
	}
	return 0;
}





/*
颜色函数SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),前景色 | 背景色 | 前景加强 | 背景加强);
	前景色：数字0-15 或 FOREGROUND_XXX 表示	（其中XXX可用BLUE、RED、GREEN表示）
	前景加强：数字8 或 FOREGROUND_INTENSITY 表示
	背景色：数字16 32 64 或 BACKGROUND_XXX 三种颜色表示
	背景加强： 数字128 或 BACKGROUND_INTENSITY 表示
主要应用：改变指定区域字体与背景的颜色
前景颜色对应值：
　　0=黑色                8=灰色　　
  　1=蓝色                9=淡蓝色        十六进制        　　
　　2=绿色                10=淡绿色       0xa        　　
　　3=湖蓝色              11=淡浅绿色     0xb　
　　4=红色                12=淡红色       0xc　　
　　5=紫色                13=淡紫色       0xd        　　
　　6=黄色                14=淡黄色       0xe        　　
　　7=白色                15=亮白色       0xf
　　也可以把这些值设置成常量。
*/

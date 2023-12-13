#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#define HIGH 23
#define WIDTH 60

int canvas[HIGH][WIDTH];//记录二维数组游戏画面对应的元素
						//0输出空格 -1输出边界 1输出蛇头* 大于1输出蛇身* 

int moveDirection;//移动方向 1上 2下 3左 4右

int position_x, position_y;
int operationNumber;
int score;
int speed[10];

int newHead_x;
int newHead_y;

int food_y;
int food_x;

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
	int i,j;
	operationNumber = 5;
	moveDirection = 4;

	/*初始化蛇头蛇身*/
	canvas[HIGH / 2][WIDTH / 2] = 1;
	for (i = 1; i <= 3; i++)
	{
		canvas[HIGH / 2][WIDTH / 2-i] = 1+i;
	}
	newHead_x = 0;
	newHead_y = 0;
	/*边框*/
	for (i = 0; i < HIGH; i++)//左右边界初始化
	{
		canvas[i][0] = -1;
		canvas[i][WIDTH - 1] = -1;
	}
	for (j = 0; j < WIDTH; j++)//上下边界初始化
	{
		canvas[0][j] = -1;
		canvas[HIGH - 1][j] = -1;
	}

	food_y = rand() % (HIGH-10)+1;
	food_x = rand() % (WIDTH-30)+1;
	canvas[food_y][food_x] = -2;
}

void moveSnakeByDirection()
{
	int i, j;
	int max = 0;
	int oldTail_y = 0;//旧的蛇尾
	int oldTail_x = 0;
	int oldHead_y = 0;//旧的蛇头
	int oldHead_x = 0;

	for (i = 1; i < HIGH - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			if (canvas[i][j] > 0)
			{
				//对所有大于0的元素加一
				canvas[i][j]++;

				if (max < canvas[i][j])//查找蛇尾
				{
					max = canvas[i][j];
					oldTail_y = i;
					oldTail_x = j;
				}

				if (canvas[i][j] == 2)//记录旧蛇头位置
				{
					oldHead_y = i;
					oldHead_x = j;
				}
			}
		}
	}
	

	if (moveDirection == 1)
	{
		newHead_x = oldHead_x;
		newHead_y = oldHead_y - 1;
	}
	if (moveDirection == 2)
	{
		newHead_x = oldHead_x;
		newHead_y = oldHead_y + 1;
	}
	if (moveDirection == 3)
	{
		newHead_x = oldHead_x - 1;
		newHead_y = oldHead_y;
	}	
	if (moveDirection == 4)
	{
		newHead_x = oldHead_x + 1;
		newHead_y = oldHead_y;
	}
	
	//判断小蛇蛇头是否碰到食物
	if (canvas[newHead_y][newHead_x] == -2)
	{
		//吃到食物 蛇尾不变为0
		canvas[food_y][food_x] = 0;
		food_y = rand() % (HIGH - 10);
		food_x = rand() % (WIDTH - 30);
		canvas[food_y][food_x] = -2;
	}
	else
	{
		//没有吃到食物长度不变
		canvas[oldTail_y][oldTail_x] = 0;//蛇尾变为空格
	}


	//游戏失败条件
	if (canvas[newHead_y][newHead_x] == -1 || canvas[newHead_y][newHead_x]>0)
	{
		color(4);
		printf("\nGAME OVER!");
		system("pause");
		exit(0);
	}
	else
	{
		//正常情况下蛇头位置改变
		canvas[newHead_y][newHead_x] = 1;
	}

}

void show()  //显示画面
{

	int i, j;//i是y轴 j是x轴
	gotoxy(0, 0); //清屏函数 光标移动到左上角

	for (i = 0; i < HIGH; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (canvas[i][j] == 0)
				printf(" ");
			else if (canvas[i][j] == -1)
			{
				color(6);
				printf("#");//输出边框
				color(16);
			}
			else if (canvas[i][j] == 1)//输出蛇头
			{
				color(10);
				printf("*");
				color(16);
			}
			else if (canvas[i][j] >= 1)//输出蛇身
			{
				color(10);
				printf("*");
				color(16);
			}
			else if (canvas[i][j] == -2)
			{
				color(11);
				printf("*");
				color(16);
			}
				
		}
		printf("\n");
	}
	
}


void updateWithoutInput() //用户输入无关的操作
{
	int i, j, k;
	
	
	for (i = 0; i < operationNumber; i++)
	{
		speed[i]++;
	}
	if (speed[0] == 5)
	{
		moveSnakeByDirection();
		speed[0] = 0;
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
			moveDirection = 3;
		}
		if (input == 'd')
		{
			moveDirection = 4;
		}
		if (input == 'w')
		{
			moveDirection = 1;
		}
		if (input == 's')
		{
			moveDirection = 2;
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

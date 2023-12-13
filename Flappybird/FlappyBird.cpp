#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>


int bird_x, bird_y;//小球坐标
int high, width;//游戏画面尺寸
int bar_x, bar_yTOP, bar_yDOWN;//障碍物坐标
int randPosition ;//随机坐标
int score;
int speed;

void PrintBorder(int _i, int _j);
void GameOver();
void enemydown();

void gotoxy(int x, int y)  //光标移动到(x,y)的位置
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void startup()  //数据初始化
{
	srand((unsigned)time(NULL));//随机化种子
	randPosition = rand() % (high - 3);

	high = 15;
	width = 25;

	bar_x = width-2;
	bar_yTOP = randPosition;
	bar_yDOWN = randPosition + high / 3;
	

	bird_y = high/2;
	bird_x = 5;

	speed = 0;
	score = 0;

}

void show()  //显示画面
{

	int i, j;//i是y轴 j是x轴
	gotoxy(0, 0); //清屏函数 光标移动到左上角

	for (i = 0; i <= high; i++)
	{
		//printf("|");
		for (j = 0; j <= width; j++)
		{
			if (i == bird_y && j == bird_x)
				printf("@");//输出小鸟 @
			else if (i == high && j < width)
				printf("-");
			else if (j == width - 1 && i < high)
				printf("|");
			else if((i<=bar_yTOP||(i>=bar_yDOWN&&i<=high))&&j==bar_x)
				printf("*");
			else
				printf(" ");

		}
		printf("\n");
	}
	printf("score:%d", score/30);//30是除以speed
}

void updateWithoutInput() //用户输入无关的操作
{
	speed++;
	if (speed == 30)//减速函数
	{
		if (bar_x > 0)
		{
			bar_x--;
		}
		else
		{
			bar_x = width-2;

			srand((unsigned)time(NULL));//随机化种子
			randPosition = rand() % (high - 3);
			bar_yTOP = randPosition;
			bar_yDOWN = randPosition + high/3 ;
		}
		bird_y++;
		speed = 0;//减速函数归零
	}
	if (bird_x == bar_x && (bird_y <= bar_yTOP || bird_y >= bar_yDOWN))
	{
		printf("\nGame Over!");
		system("pause");
	}
	else if (bird_y == high)
	{
		printf("\nGame Over!");
		system("pause");
	}
	else if(bird_x == bar_x && (bird_y > bar_yTOP && bird_y < bar_yDOWN))
	{
		score++;
	}
	
}
void updateWithInput() //按键操作
{
	char input;
	if (_kbhit())//当按键时执行
	{
		input = _getch();
		if (bird_y >= 2)
		{
			if (input == ' ')
			{
				bird_y -= 2;
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
}


int main()
{
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
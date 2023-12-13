#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<Windows.h>
#include<time.h>
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"Winmm.lib")

#define HIGH 720
#define WIDTH 1280


char Begininput;
char ASscore[10];
char BSscore[10];

//板子的各种参数
int A_Lx;
int A_Rx;
int A_Ty;
int A_By;

int B_Lx;
int B_Rx;
int B_Ty;
int B_By;

int BarXLength;
int BarYLength;


//小球的各种坐标
int ball_x, ball_y;//小球坐标
int ball_vx, ball_vy;//方向参数
int Ballradius;


int step;

int Ascore;
int Bscore;

int WinnerScore;

long double BallSpeed;
long double BarSpeed;

long double CountTime;

void HideWindow();
void startup();
void show();
void showBegin();
void updateWithoutInput();
void updateWithInput();
void gameover();
int GetRandomInteger(int low, int up);

int main()
{
	//bgm
	mciSendString("open .\\bgm1.mp3 alias bgm", NULL, 0, NULL);
	mciSendString("play bgm repeat", NULL, 0, NULL);
	mciSendString("open .\\DUANG.mp3 alias duang", NULL, 0, NULL);
	HideWindow();
	startup();//数据初始化
	showBegin();
	if (Begininput == ' ')
	{
		HideWindow();
		MessageBox(0, "A玩家 W和S 移动\nB玩家 方向键上下移动！\n5分结束比赛", "游戏规则", MB_OK | MB_SETFOREGROUND);
		HideWindow();

		initgraph(WIDTH, HIGH);
		BeginBatchDraw();
		while (1)
		{
			show();
			updateWithoutInput();
			updateWithInput();
			gameover();
		}
	}
	
	return 0;
}


void startup()
{

	//板子的各种参数
	BarXLength = 10;
	BarYLength = 200;


	A_Lx = 40;
	A_Rx = A_Lx + BarXLength;
	A_Ty = HIGH/2 - BarYLength / 2;
	A_By = A_Ty + BarYLength;

	B_Lx = WIDTH-40;
	B_Rx = B_Lx - BarXLength;
	B_Ty = HIGH / 2- BarYLength / 2;
	B_By = B_Ty + BarYLength;

	Ascore = 0;
	Bscore = 0;


	//小球的各种参数
	ball_x = WIDTH / 2;
	ball_y = HIGH / 2;
	ball_vx = 5;
	ball_vy = 5;
	Ballradius = 5;

	step = 10;
	BallSpeed=20;
	BarSpeed=20;

	WinnerScore = 5;
	CountTime = 0;
}


void show()
{
	//得分
	sprintf_s(ASscore, "%d", Ascore);
	sprintf_s(BSscore, "%d", Bscore);
	


	//画中间的虚线
	setcolor(WHITE);
	setlinestyle(PS_DASH, 5);
	line(WIDTH / 2, 0, WIDTH/2, HIGH);
	setlinestyle(PS_SOLID, 1);

	//画小球
	setcolor(YELLOW);
	setfillcolor(YELLOW);
	fillcircle(ball_x, ball_y, Ballradius);

	//画板子
	setcolor(WHITE);
	setfillcolor(WHITE);
	fillrectangle(A_Lx, A_Ty, A_Rx, A_By);
	fillrectangle(B_Lx, B_Ty, B_Rx, B_By);


	FlushBatchDraw();

	//擦除原来的图案
	setcolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(ball_x, ball_y, Ballradius);
	fillrectangle(A_Lx, A_Ty, A_Rx, A_By);
	fillrectangle(B_Lx, B_Ty, B_Rx, B_By);
	setcolor(WHITE);
}

void showBegin()
{
	initgraph(WIDTH, HIGH);
	settextstyle(200, 0,"DigitalRegular");
	outtextxy(WIDTH / 3+40, HIGH / 3, "PONG");
	settextstyle(30, 0, "STXINGKA");
	outtextxy(WIDTH / 2-60, HIGH -250, "按下空格开始");
	while (Begininput != ' ')
	{
		Begininput = _getch();
	}
	closegraph();
}


void updateWithoutInput()
{
	settextstyle(100, 70, _T("Small Fonts"));
	setcolor(WHITE);
	outtextxy(WIDTH / 2 - 300 , 40, ASscore);
	outtextxy(WIDTH / 2 + 230, 40, BSscore);

	static int SleepTime = 0;
	SleepTime++;
	if (BallSpeed) {
		CountTime += 0.00000001;
		BallSpeed -= CountTime;
	}
	

	if (SleepTime >= BallSpeed)
	{
		//小球自动运行轨迹
		ball_x = ball_x + ball_vx;
		ball_y = ball_y + ball_vy;

		//小球反弹
		if (ball_y + Ballradius == HIGH || ball_y - Ballradius == 0)
		{
			ball_vy = -ball_vy;
		}
		if (ball_x - Ballradius == A_Rx && ball_y + Ballradius >= A_Ty && ball_y + Ballradius <= A_By)
		{
			ball_vx = -ball_vx;
			mciSendString("play duang", NULL, 0, NULL);
		}
		if (ball_x - Ballradius == B_Rx && ball_y + Ballradius >= B_Ty && ball_y + Ballradius <= B_By)
		{
			ball_vx = -ball_vx;
			mciSendString("play duang", NULL, 0, NULL);
		}
		SleepTime = 0;
	}
	//得分判断
	if (ball_x - Ballradius == 0)
	{
		Bscore++;
		ball_x = WIDTH / 2;
		ball_y = HIGH / 2;
		ball_vx = -ball_vx;
		BallSpeed = 20;
	}
	if (ball_x + Ballradius == WIDTH)
	{
		Ascore++;
		ball_x = WIDTH / 2;
		ball_y = HIGH / 2;
		ball_vx = -ball_vx;
		BallSpeed = 20;
	}
	
	
}

void updateWithInput()
{
	static int SleepTime = 0;
	SleepTime++;

	if (SleepTime == BarSpeed)
	{
		//A的操作
		if (A_Ty >= 0)
		{
			if (GetAsyncKeyState(87) & 0x8000)//w
			{
				A_Ty -= step;
				A_By -= step;
			}
		}

		if (A_By <= HIGH)
		{
			if (GetAsyncKeyState(83) & 0x8000)//s
			{
				A_Ty += step;
				A_By += step;
			}
		}

		//B的操作
		if (B_Ty >= 0)
		{
			if (GetAsyncKeyState(VK_UP) & 0x8000)//上w
			{
				B_Ty -= step;
				B_By -= step;
			}
		}
		if (B_By <= HIGH)
		{
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)//上
			{
				B_Ty += step;
				B_By += step;
			}
		}
		SleepTime = 0;
	}
	
}

void gameover()
{
	if (Ascore == WinnerScore)
	{
		MessageBox(NULL, "A玩家获胜", "GAME OVER!", MB_OK);
		exit(0);
	}
	if (Bscore == WinnerScore)
	{
		MessageBox(NULL, "B玩家获胜", "GAME OVER!", MB_OK);
		exit(0);
	}
}

void HideWindow() {
	HWND hwnd = GetForegroundWindow();	//获取程序启动时的窗口
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);				//设置指定窗口的显示状态
	}
}



int RandomInt()//随机生成数
{
	int result = 0;
	srand((unsigned)time(NULL));
	while (result != 0)
	{
		result = rand() % 2;
	}
	return result;
}
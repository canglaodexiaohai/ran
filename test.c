#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>

#define FOOD  "■"
typedef struct paihang {
	char name[10];
	char score[6];
}paihang;
paihang people[10];
int count = 0;
void SetPos(int x, int y)
{
	//设置光标位置
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0 };
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

//蛇身结点
typedef struct node {
	int x;
	int y;
	struct node* next;
}SnakeNode,*pSnakeNode;
//行走方向
enum DIRECTION {UP,DOWN,LEFT,RIGHT};
//蛇的状态
enum Status{OK,KILL_BY_SELF,KILL_BY_WALL};
//蛇
typedef struct snake {
	pSnakeNode _pSnake;//蛇头指针
	pSnakeNode _pFood;//食物
	enum DIRECTION _Dir;//方向
	enum Status _Status;//蛇的状态
	int _SleepTime;//每走一步停留时间
	//等级 加分 变速
	int _level;//等级
	int _score;//分数
}Snake,*pSnake;
#define INIT_X 24
#define INIT_Y 4
void InitSnake(pSnake ps)
{
	pSnakeNode cur = NULL;
	int i = 0;

	cur = (pSnakeNode)malloc(sizeof(SnakeNode));
	memset(cur, 0x00, sizeof(SnakeNode));
	cur->x = INIT_X;
	cur->y = INIT_Y;
	cur->next = NULL;
	for (i = 1; i <= 4; ++i)
	{
		ps->_pSnake = malloc(sizeof(SnakeNode));
		ps->_pSnake->next = cur;
		ps->_pSnake->x = INIT_X + i * 2;
		ps->_pSnake->y = INIT_Y;
		cur = ps->_pSnake;
	}
	while (cur!=NULL)
	{
		SetPos(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}
	ps->_SleepTime = 400;
	ps->_Status = OK;
	ps->_Dir = RIGHT;
	ps->_level = 1;
	ps->_score = 0;
}

void DrawMap(pSnake ps)
{
	int i;
	for (i = 0; i < 58; i += 2)
	{
		SetPos(i, 0);
		printf(FOOD);
	}
	for (i = 0; i < 58; i += 2)
	{
		SetPos(i, 26);
		printf(FOOD);
	}
	for (i = 0; i < 26; i++)
	{
		SetPos(0, i);
		printf(FOOD);
	}
	for (i = 0; i <= 26; i++)
	{
		SetPos(58, i);
		printf(FOOD);
	}
	SetPos(66, 12);
	printf("分数：%d", ps->_score);
	SetPos(66, 14);
	printf("等级：%d", ps->_level); 
	
}
void CreatFood(pSnake ps)
{
	pSnakeNode cur = NULL;
	pSnakeNode food = NULL;
	food = malloc(sizeof(SnakeNode));
again:
	memset(food, 0x00, sizeof(SnakeNode));
	do
		food->x = rand() % 55 + 2;
	while (food->x % 2 != 0);
	food->y = rand() % 24 + 1;

	cur = ps->_pSnake;
	while (cur)
	{
		if (cur->x == food->x&&cur->y == food->y)
		{
			goto again;
		}
		cur = cur->next;

	}
	ps->_pFood = food;
	SetPos(food->x, food->y);
	//printf("\033[%d;%dH", food->y, food->x);
	printf("\033[32m");
	printf(FOOD); printf("\n");
	printf("\033[0m");

}
void SnakeStart(pSnake ps)
{
	srand((unsigned int)time(NULL));

	system("cls");
	InitSnake(ps);
	DrawMap(ps);


	CreatFood(ps);
}
void Welcome(void)
{
	//system("color 70");
	system("mode con cols=100 lines=30");
	SetPos(40, 10);
	printf("欢迎来到贪吃蛇游戏\n");
	SetPos(38, 12);
	printf("↑  ↓  ←  → 控制方向\n");
	SetPos(40, 14);
	printf("贾聪阳@版权所有\n");
	getchar();
	system("cls");
}
int NextHasFood(pSnake ps, pSnakeNode nNode)
{
	return (ps->_pFood->x == nNode->x)&&
		(ps->_pFood->y == nNode->y);
}
void EatFood(pSnake ps, pSnakeNode nNode)
{
	pSnakeNode cur = ps->_pSnake;
	nNode->next = cur;
	ps->_pSnake = nNode;


	cur = ps->_pSnake;

	while (cur)
	{
		SetPos(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}
	SetPos(66, 12);
	printf("分数：%d", ++ps->_score);
	SetPos(66, 14);
	printf("等级：%d", ps->_level);
	if (ps->_score % 6 == 0)
	{
		ps->_level++;
		if(ps->_SleepTime>100)
		    ps->_SleepTime -= 100;
		else if(ps->_SleepTime>40)
			ps->_SleepTime -= 10;
	}
	CreatFood(ps);
}
void NoFood(pSnake ps, pSnakeNode nNode)
{
	pSnakeNode cur = ps->_pSnake;
	nNode->next = cur;
	ps->_pSnake = nNode;

	cur = ps->_pSnake;
	while(cur->next->next)
	{
		SetPos(cur->x, cur->y);
		printf(FOOD);
		cur = cur->next;
	}
	//SetPos(cur->x, cur->y);
	//printf(FOOD);
	SetPos(cur->next->x, cur->next->y);
	printf(" \n");
	free(cur->next);
	cur->next = NULL;
}
void SnakeMove(pSnake ps)
{
	pSnakeNode nNode = malloc(sizeof(SnakeNode));
	memset(nNode, 0x00, sizeof(SnakeNode));
	nNode->x = ps->_pSnake->x;
	nNode->y = ps->_pSnake->y;
	switch (ps->_Dir)
	{
	case UP:
		nNode->y--;
		break;
	case DOWN:
		nNode->y++;
		break;
	case LEFT:
		nNode->x -= 2;
		break;
	case RIGHT:
		nNode->x += 2;
		break;
	default:
		break;
	}
	if (NextHasFood(ps,nNode))
	{
		EatFood(ps,nNode);
	}
	else
	{
		NoFood(ps, nNode);
	}
}
int KillBySelf(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake->next;
	while (cur != NULL)
	{
		if (cur->x==ps->_pSnake->x&&cur->y==ps->_pSnake->y)
		{
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}
int KillByWall(pSnake ps)
{
	if (ps->_pSnake->x == 0 || ps->_pSnake->x == 58 ||
		ps->_pSnake->y == 0 || ps->_pSnake->y == 26)
		return 1;
	return 0;
}
void SnakeRun(pSnake ps)
{
	do
	{
		if (GetAsyncKeyState(VK_DOWN)&&ps->_Dir!=UP) {
			ps->_Dir = DOWN;
		}
		else if (GetAsyncKeyState(VK_UP) && ps->_Dir != DOWN) {
			ps->_Dir = UP;
		}
		else if (GetAsyncKeyState(VK_LEFT) && ps->_Dir != RIGHT) {
			ps->_Dir = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && ps->_Dir != LEFT) {
			ps->_Dir = RIGHT;
		}
		SnakeMove(ps);
		if (KillBySelf(ps))
			ps->_Status = KILL_BY_SELF;
		if (KillByWall(ps))
			ps->_Status = KILL_BY_WALL;

		Sleep(ps->_SleepTime);
	} while (ps->_Status==OK);
	SetPos(66, 8);
	if (ps->_Status == KILL_BY_WALL)
	{
		printf("\033[31m");
		printf("撞墙而死\n");
		printf("\033[0m");
	}
	else
	{
		printf("\033[31m");
		printf("被自己咬死\n");
		printf("\033[0m");	
	}
}
int main()
{
	
	Snake s;
	memset(&s, 0x00, sizeof(Snake));

	Welcome();

	SnakeStart(&s);
	
	SnakeRun(&s);
	//getchar();
	//system("pause");
	//paihangbang();
	return 0;
}

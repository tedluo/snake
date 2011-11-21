#include "SDL.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_SIZE 32


SDL_Surface *screen;                         	 
SDL_Surface *pBmp_head;			     		
SDL_Surface *pBmp_tail;
SDL_Surface *pBmp_body;
SDL_Rect rect_head,rect_body,rect_tail;

SDL_Rect food_place; //记录刷出果子的坐标
//int a;

typedef struct struct_type {
	SDL_Surface * pBmp;
	SDL_Rect rect;} snake_element; //每一节蛇定义为一个结构，包含有载入图片信息和载入位置的信息

snake_element snake_main[40];//蛇身定义在数组，蛇最长40, 可以修改

int length= 2 ;//记录蛇身的长度，初始化为2
int event_PreMark = 4;//记录在当前键位按下前，蛇的运动方向
int event_mark = 4;//记录当前方向键标志
int  food_flag = 0 ; //食物标记，如果屏幕刷出了食物，则置位1

int delay_time = 200 ; //关卡参数
int pass_length = 20 ;

//////////////////////////////初始化//////////////////////////////////
void Load_first_snake(void)  //初始化第一张位图位置和图片,并装入内存
{
	int i;
	snake_main[0].rect.x = 325;
	snake_main[0].rect.y = 235;
	if((snake_main[0].pBmp = SDL_LoadBMP("body.bmp"))==NULL)
			{
				fprintf(stderr,"Can't load BMP %s : %s\n","body.bmp",SDL_GetError());
				exit(1);
			}
	for(i =1;i < length; i++ )
	{
		if((snake_main[i].pBmp = SDL_LoadBMP("body.bmp"))==NULL)
			{
				fprintf(stderr,"Can't load BMP %s : %s\n","body.bmp",SDL_GetError());
				exit(1);
			}
		snake_main[i].rect.x = snake_main[i-1].rect.x - 5;
		snake_main[i].rect.y = snake_main[i-1].rect.y;
	}
	//return snake_main;
}

void Load_bmp_show(void)// 显示位图函数
{
	int i;
	for(i=0;i<length;i++)
	{
		if(SDL_BlitSurface(snake_main[i].pBmp,NULL,screen,&snake_main[i].rect)!=0)
			{
				fprintf(stderr,"blit Error!: %s\n",SDL_GetError());
				exit(1);
			}
	}
	SDL_Flip(screen);
	for(i=0;i<length;i++)
	{
		SDL_FreeSurface(snake_main[i].pBmp);
	}
}
void Creat_screen(void)       //初始化屏幕
{
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1)
		{
			fprintf(stderr,"Can't init SDL: %s\n",SDL_GetError());
			//exit(1);
		}
	atexit(SDL_Quit);
	SDL_WM_SetCaption("snake","snake");
	if((screen = SDL_SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_SIZE, 0 ))==NULL)
		{
			fprintf(stderr,"Can't create screen: %s\n",SDL_GetError());
			exit(1);
		}	
}
//////////////////////////////////触发事件/////////////////////////////////////////////////
int check_event(void)//事件处理函数，当遇不同的控制键时返回一个相应的整数
{
	int KeyDown_mark = 0;
	SDL_Event event;
	while(SDL_PollEvent(&event)) 
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) 
					{
						case SDLK_ESCAPE:
							KeyDown_mark = 6;
						return KeyDown_mark;	//按下了ESC退出程序。
						case SDLK_UP:
							KeyDown_mark = 1;
						return KeyDown_mark;
						case SDLK_DOWN:
							KeyDown_mark = 3;
						return KeyDown_mark;
						case SDLK_LEFT:
							KeyDown_mark = 2;
						return KeyDown_mark;
						case SDLK_RIGHT:
							KeyDown_mark = 4;
						return KeyDown_mark;
						case SDLK_SPACE:
							KeyDown_mark = 7;
						return KeyDown_mark;
						default:
						return 5;
					}
				break;
				case SDL_QUIT:
					KeyDown_mark = 6 ;
				return KeyDown_mark;
				break;
				}
			}
	return 0;		 
}

int PressQuit(void)//用于提供暂停和等待子程序
{	
	int flag = 0 ;
	SDL_Event event ;
	while(!flag)
	{
		if(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
			return 1;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return 1;
				default:
					return 2; 
				}
			}
		}
		return 0;
	}
}

///////////////////////////////键盘控制键操作函数//////////////////////////////////////////

snake_element* key_UpMove(snake_element* snake_main)
{
	int i;
	snake_element* temp;
	for(i = 0;i < length-1;i++)
		{	
			snake_main[length-1-i].rect.x = snake_main[length-i-2].rect.x;
			snake_main[length-1-i].rect.y = snake_main[length-i-2].rect.y;
		}
	snake_main[0].rect.y = snake_main[0].rect.y - 5;
	temp = snake_main ;
	event_PreMark = 1 ;
	SDL_Delay(delay_time);
	return temp;
}

snake_element* key_DownMove(snake_element* snake_main)
{
	int i;
	snake_element* temp ;
	for(i = 0;i < length -1; i++)
		{
			snake_main[length-1-i].rect.x = snake_main[length-i-2].rect.x;
			snake_main[length-1-i].rect.y = snake_main[length-i-2].rect.y;
		}
	snake_main[0].rect.y =  snake_main[0].rect.y +5;
	temp = snake_main ;
	event_PreMark = 3 ;
	SDL_Delay(delay_time);
	return temp;

}
snake_element* key_LeftMove(snake_element* snake_main)
{
	int i;
	snake_element* temp ;
	for(i = 0;i < length -1; i++)
		{
			snake_main[length-1-i].rect.x = snake_main[length-i-2].rect.x;
			snake_main[length-1-i].rect.y = snake_main[length-i-2].rect.y;
		}
	snake_main[0].rect.x =  snake_main[0].rect.x -5;
	temp = snake_main ;
	event_PreMark = 2 ;
	SDL_Delay(delay_time);
	return temp;

}
snake_element* key_RightMove(snake_element* snake_mark)
{
	int i;
	snake_element* temp ;
	for(i = 0;i < length -1; i++)
		{
			snake_main[length-1-i].rect.x = snake_main[length-i-2].rect.x;
			snake_main[length-1-i].rect.y = snake_main[length-i-2].rect.y;
		}
	snake_main[0].rect.x =  snake_main[0].rect.x +5;
	temp = snake_main ;
	event_PreMark = 4 ;
	SDL_Delay(delay_time);
	return temp;

}
//////////////////////////////////////////////////////////////////////////////////

void Load_snake(int length,snake_element* snake_main)// 载入位图函数
{
	int i;
	for(i = 0;i < length; i++)
	{
		if(i==0)
			snake_main[i].pBmp = SDL_LoadBMP("head.bmp");
		else
			snake_main[i].pBmp = SDL_LoadBMP("body.bmp");
	}
}

void Clear_bmp(int length)// 在载入下一张位图之前，清除前面载入位图
	
{	
	int i;
	for(i=0;i<length;i++)
		{				
			SDL_FillRect(screen,&snake_main[i].rect,0);
		}
}
///////////////////////////////////////////////////////////////
int Check_over(snake_element* snake_main)//检测游戏是否结束，当蛇碰到自己身体，或越界时，游戏结束

{
	int flag = 0;
	int i ;
	if(length > 4)
	{	
		for(i = 4;i <=length ;i ++)
		{	
			if((snake_main[0].rect.x == snake_main[i].rect.x)&&(snake_main[0].rect.y == snake_main[i].rect.y))
				flag = 1;
		}
	}	
	if(snake_main[1].rect.x == 0||snake_main[0].rect.x == 640||snake_main[1].rect.y == 0||snake_main[0].rect.y == 480||flag ==1)
	{	
		SDL_Rect game_over_rect;
		game_over_rect.x = 0;
		game_over_rect.y = 120;
		Clear_bmp(length);
		SDL_Surface* game_over;
		game_over = SDL_LoadBMP("gameover.bmp");
		SDL_BlitSurface(game_over,NULL,screen,&game_over_rect);
		SDL_Flip(screen);
		return 1 ;
	}
	return 0 ;
}

int Space_pause(void) //暂停功能
{
	int i;
	if(event_mark == 7)
			while(!(i = (PressQuit())));
	return i;
}
////////////////////////////////////////////////////////////////

void Rand_food(SDL_Rect *food_place)//在屏幕随机坐标刷出一个苹果
{
	if(food_flag == 0)
	{
		srand(time(NULL));
		SDL_Surface* pfood;
		food_place->x = (rand()%128)*5 ;
		food_place->y = (rand()%96)*5 ;
		pfood = SDL_LoadBMP("food.bmp");
		SDL_BlitSurface(pfood,NULL,screen,food_place);
		SDL_Flip(screen);
		food_flag = 1 ;
	}
}
int Eat_food(void)
{	
	if(snake_main[0].rect.x == food_place.x && snake_main[0].rect.y == food_place .y)
		{	
			food_flag = 0;
			length = length + 1;
			return 1;
		}
	return 0;
}
////////////////////////主程序//////////////////////////////
void main(int argc,char* argv[])
{	
	int key;
	snake_element* snake_mark;
	Creat_screen();
	Load_first_snake();
	Load_bmp_show();
	while(!(key=PressQuit())); // 没有按键时等待..
	if(key == 1) // 按下退出键时退出程序，否则执行下一条语句
		exit(1);
	event_mark=check_event();
	while((event_mark == 0) || (event_mark == 5)||(event_mark % 2 == event_PreMark % 2)||event_mark == 7)
		{
			Clear_bmp(length);
			snake_mark = key_RightMove(snake_main);//开始游戏，初始化向右行驶，当查询到左右键或者无操作时循环，退出键时退出程序	
			Load_snake(length,snake_main);         //查询到上下键时，退出本循环，并在下一个循环对此次按键进行处理
			Load_bmp_show();
			event_mark = check_event() ;
			Rand_food(&food_place);
			Eat_food();
			if(event_mark == 6)
				exit(1);
			if((event_mark % 2 == event_PreMark % 2)|| event_mark == 5|| event_mark == 0||event_mark == 7)//如果当前方向与前一次的移动方向相同,或相反，则不做处理，装载下一位				
				continue ;
			break;
		}

	while(event_mark!=6)//主循环，不断扫描键盘，并根据扫描到的相应的值，进行相应的操作
		{
			switch(event_mark)
			{
				case 1:
					while(event_mark!= 6)
					{
						Clear_bmp(length);
						key_UpMove(snake_main);
						Load_snake(length,snake_main);
						Load_bmp_show();
						Rand_food(&food_place);//当检测到果子标志位为0时，随机刷果子
						Eat_food();//检测到是否吃到果子？如果吃到了，清零果子标志位
						if(Space_pause() ==1)//暂停功能
							exit(1);
						if(Check_over(snake_main) == 1)//游戏失败？，失败则退出游戏
							{		
								SDL_Delay(5000);
								exit(1);
							}
						event_mark = check_event() ;//查询事件
						if((event_mark % 2 == event_PreMark % 2)||event_mark == 0)//根据不同事件采取不同操作
							continue ;
						if(event_mark %2 != event_PreMark %2)
							break ;				
					}
				break ;	
				case 2:
					while(event_mark!= 6 )
					{
						Clear_bmp(length);
						key_LeftMove(snake_main);
						Load_snake(length,snake_main);
						Load_bmp_show();
						Rand_food(&food_place);
						Eat_food();
						if(Space_pause() ==1)//暂停功能
							exit(1);
						if(Check_over(snake_main) == 1)
							{		
								SDL_Delay(5000);
								exit(1);
							}

						event_mark = check_event() ;
						if((event_mark % 2 == event_PreMark % 2)||event_mark == 5||event_mark == 7)
							continue ;
						if(event_mark %2 != event_PreMark %2)
							break ;						
					}
				break;
				case 3:
					while(event_mark!= 6 )
					{
						Clear_bmp(length);
						key_DownMove(snake_main);
						Load_snake(length,snake_main);
						Load_bmp_show();
						Rand_food(&food_place);
						Eat_food();
						if(Space_pause() ==1)
							exit(1);
						if(Check_over(snake_main) == 1)
							{		
								SDL_Delay(5000);
								exit(1);
							}
						event_mark = check_event() ;
						if((event_mark % 2 == event_PreMark % 2)||event_mark == 0)
							continue ;
						if(event_mark %2 != event_PreMark %2)
							break ;	
			

					}
				break;
				case 4:
					while(event_mark!= 6)
					{
						Clear_bmp(length);
						key_RightMove(snake_main);
						Load_snake(length,snake_main);
						Load_bmp_show();
						Rand_food(&food_place);
						Eat_food();
						if(Space_pause() ==1)
							exit(1);
						if(Check_over(snake_main) == 1)
							{		
								SDL_Delay(5000);
								exit(1);
							}
						event_mark = check_event() ;
						if((event_mark % 2 == event_PreMark % 2)||event_mark == 5||event_mark == 7)
							continue ;
						if(event_mark %2 != event_PreMark %2)
							break ;	
					}
				break;
			}
		}	
}




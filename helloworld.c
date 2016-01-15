#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <random.h>
#include <stdbool.h>

//Ascii codes
#define KEY_ESC			27
#define KEY_LEFT		75
#define KEY_RIGHT		77
#define KEY_UP		 	72
#define KEY_DOWN		80

//Road length
#define LENGTH			48

//Minimum and maximum values of the center of the road at screen that
//guaranties at least one grass unit at both sides of the screen
//MIN = -123........o =  1+3+8+1 = 13
//MAX = o........321- = 80-3-8-1 = 68
#define MIN				13
#define MAX				68

//Basic structure functions
void Initialize(void);
void Loop(void);
void Finalize(void);

void Input(void);
void Update(void);
void Draw(void);

//Auxiliar functions
int  Random(void);
void DrawRoadLine(int i);

//Player variables
int x,x_prev,	//current and previous player position
	speed,
	lifes,
	meters;

//Road structure: an array of lines, where the integer value corresponds to
//                the offset from left screen limit (road has a fixed width)
int	road[LENGTH];

//Control variables
bool begin,
	 end;

//------------------------------------------
//---------------- MAIN --------------------
//------------------------------------------
int main()
{
	//-- TODO:
	Initialize();

	Loop();

		//Input();
		//Update();
		//Draw(); //Render();


	Finalize();

	//--

	return 0;
}

//Returns a random value {-1,0,1}
int Random()
{
	return (genrand_int32()%3)-1;
}

//Setup game: resources, variables and data structure
void Initialize()
{
	int i;

	//Initialize random numbers
	mt_init_genrand(13);
	//Text mode 80x50
	textmode(C4350);
	//Hide cursor
	_setcursortype(_NOCURSOR);

	// Player initial position at screen = (80/2=40,42)
	//
	// -	 1st line
	// o 2nd -> c[0]
	// o
	// ...
	// o 42th -> c[40]
	// ...
	// o 49th
	// -	 50th line

	x =        40;
	x_prev =    x;
	road[40] = 40;
	meters =    0;
	speed =     1;
	lifes =     3;

	//Setup road
	for(i=39;i>=0;i--)
	{
		road[i] = road[i+1] + Random();

		if(road[i]<MIN)		 road[i] = MIN;
		else if(road[i]>MAX) road[i] = MAX;
	}
	//-- TODO:BOTTOM PART OF the road
	for(i=49;i>40;i--)
	{
		road[i] = road[i+1]+ Random();
		if(road[i]<MIN)		 road[i] = MIN;
		else if(road[i]>MAX) road[i] = MAX;
	}

	//--

	begin = true;
}

//Game loop with frame rate synchronization based on player speed
//speed 1 => 10 fps, speed 9 =>
void Loop()
{

	int t1,t2;


	while(!end)
	{
		t1 = GetTickCount();

		//-- TODO:
		//x=x+speed;

		Input();
		Update();
		//DrawRoadLine(x);
		Draw();
		//--

		do {
			t2 = GetTickCount();
		} while((t2-t1)<(100/speed));
	}
}

//Release
void Finalize()
{
	//Setup initial text mode
	textmode(LASTMODE);
}

//Get user input
void Input()
{
	//kbhit: Returns a non-zero integer if a key is in the keyboard buffer.
	//       It will not wait for a key to be pressed.
	if(kbhit())
	{
		x_prev = x;
		//Get key pressed
		switch(getch())
		{
			case KEY_ESC:	end = true;				break;

			//-- TODO:
			case KEY_LEFT:  x=x-1; 					break;

			case KEY_RIGHT: x=x+1; 					break;

			case KEY_UP:    speed=speed+1;			break;

			case KEY_DOWN:
				speed=speed-1;

				if(speed-1<0)
					speed=1;
				break;
			//--
		}
	}
}

//Update game state: move the road and detect collisions
void Update()
{
	int i;
	bool collision = false;

	if(!begin)
	{
		//Move the road (the scroll)
		meters++;
		memcpy(&road[1],&road[0],(LENGTH-1)*sizeof(int));

		//-- TODO:
		//New piece of road

		road[0] = road[0+1]+ Random();



		//--
	}

	//Collisions
	if(x<(road[40]-8))		collision = true;
	else if(x>(road[40]+8))	collision = true;  //TODO

	if(collision)
	{
		//Wait 500ms
		i = GetTickCount();
		while((GetTickCount()-i)<500) {}

		//-- TODO:

		lifes=lifes-1;
		//--
		if(lifes == 0)
		{
			end = true;
		}
		else
		{
			//Reset speed value and place player in the center of the road
			speed = 1;
			x = road[40];
		}
	}
}

//Draw the GUI (lifes, meters, speed), the road and the player
void Draw()
{
	//gotoxy(x,y): set cursor at (x,y) position
	//Screen resolution = 80x50 => x=1..80, y=1..50

	int i;

	if(!end)
	{
		//GUI
		//Speed
		gotoxy(5,1);
		textcolor(CYAN);
		printf("Speed: ");
		textcolor(LIGHTCYAN);
		for(i=0;i<speed;i++)
			printf("%c",0x04);
		textcolor(CYAN);
		for(i=0;i<(10-speed);i++)
			printf("%c",0x04);
		//Meters
		gotoxy(40,1);
		textcolor(YELLOW);
		printf("%d Meters",meters);
		//Lifes
		gotoxy(70,1);
		textcolor(RED);
		printf("Life: ");
		textcolor(LIGHTRED);
		for(i=0;i<lifes;i++)
			printf("%c",0x03);
		textcolor(RED);
		for(i=0;i<(3-lifes);i++)
			printf("%c",0x03);

		//ROAD
		if(begin)
		{
			//Clear screen
			clrscr();
			//Draw the road
			for(i=0;i<LENGTH;i++)
			{
				gotoxy(1,i+2);
				DrawRoadLine(i);
			}
			begin = false;
		}
		else
		{
			//Delete player
			textcolor(LIGHTGRAY);
			gotoxy(x_prev,42);
			printf("%c",0xDB);
			if(x_prev!=x) x_prev = x;

			//-- TODO:
			//Road scrolling

			//New piece of road

			gotoxy(1,2);
			DrawRoadLine(2);

			movetext(1,2,80,48,1,3);




			//--
		}

		//PLAYER
		textcolor(BLUE);
		textbackground(LIGHTGRAY);
		gotoxy(x,42);
		printf("%c",0x02);
		textbackground(BLACK);
	}
	else
	{
		textcolor(BLUE);
		gotoxy(34,20); printf("-------------");
		gotoxy(34,21); printf("| GAME OVER |");
		gotoxy(34,22); printf("-------------");

		//Wait 500ms showing game over message
		i=GetTickCount();
		while((GetTickCount()-i)<500) {}
		//Wait for a user key down to continue
		while(!kbhit()) {}
	}
}

//Draw the i-th segment line of the road
void DrawRoadLine(int i)
{
	//Road line = grass kerb tarmac kerb grass
	//kerb: 3 unit spaces, left and right
	//tarmac: 8 + 1 + 8 = 17 unit spaces
	//...123--------o--------321...
	int j;
	int num_left,num_right;

	num_left  = road[i]-12;
	num_right = road[i]+12;

	//Grass
	textcolor(GREEN);
	for(j=0;j<num_left;j++){
		printf("%c",0xDB);
		if(j%5==0){
			textcolor(BROWN);


		}else
			textcolor(GREEN);



	}

	//Left limit road (kerb)
	textcolor(RED);
	printf("%c",0xB1);
	textcolor(WHITE);
	printf("%c",0xB1);
	textcolor(RED);
	printf("%c",0xB1);

	//Tarmac
	textcolor(LIGHTGRAY);
	for(i=0;i<17;i++)
		printf("%c",0xDB);

	//Right limit road (kerb)
	textcolor(RED);
	printf("%c",0xB1);
	textcolor(WHITE);
	printf("%c",0xB1);
	textcolor(RED);
	printf("%c",0xB1);

	//Grass

	textcolor(BLUE);
	for(j=num_right;j<=80;j++){
		cprintf("%c",0xDB);
		if(j%5==0){
			textcolor(BROWN);


		}else
			textcolor(GREEN);
	}
}


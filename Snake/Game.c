#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

//Currently the game can(?) is being drawn at 60Hz. Final product should perserve this if possible.

//Major TODO: Snake length; Movement; Timer; Eating the food; Collisions
//Minor TODO: !!!!!!!!! FIND A WAY TO NOT TIE THE GAME SPEED WITH THE DRAW REFRESH RATE !!!!!!!!!

int row;
int col;
int headSnakeRow;
int headSnakeCol;
int boardArray[25][100];

//moves the console cursor to the specified coordinates
void goToXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setup()
{
	//fill the game board with empty values
	for (int row = 0; row < 25; row++)
	{
		for (int col = 0; col < 100; col++)
		{
			boardArray[row][col] = 0;
		}
	}

	//generate two random numbers for the food location
	srand(time(NULL));   
	int foodRow = rand() % 25;
	int foodCol = rand() % 100;
	boardArray[foodRow][foodCol] = 2;

	//generate two random numbers for the snake's starting location
	int headSnakeRow = rand() % 25;
	int headSnakeCol = rand() % 100;
	boardArray[headSnakeRow][headSnakeCol] = 1;
}

//Directions can be 1(up) 2(down) 3(right) 4(left)
int Move()
{
	return 1;
}

void refreshBoard()
{
	//loops though all rows and starts with -1 because when its -1 its a border draw space, that way it's assured that the game area is 25x100 inside the border
	//same applies for the columns
	for (int row = -1; row <= 26; row++)
	{
		for (int col = -1; col <= 101; col++)
		{
			if ((row == -1 || row == 26)) 
			{
				printf("#");
				continue;
			}

			if ((col == -1 || col == 101))
			{
				printf("#");
				continue;
			}

			//check the values of the game board 1=snake 2=food 0=empty
			switch (boardArray[row][col])
			{
			case 0:
				printf(" ");
				break;
			case 2:
				printf("@");
				break;
			case 1:
				printf("o");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}

void mainLoop()
{
}

//TODO: Rewrite. Currently the program crashes possibly caused by an out of bounds exception. Also I'm not liking this way of movement, 
//possible collisions with the queue way of storing the snake. Find a better way of movement. Adjust prirority: Queue or Movement first?
void drawSnake(int boardArray[25][100], int direction)
{
	if (direction <= 2)
	{
		switch (direction)
		{
		case 1:
			boardArray[headSnakeRow][headSnakeCol] = 0;
			boardArray[headSnakeRow - 1][headSnakeCol] = 1;
			headSnakeRow -= 1;
			break;
		default:
			break;
		}
	}
}

//hides the cursor from flashing around on the screen and allowes for 60Hz refresh rate that is needed
void hideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int main()
{
	hideCursor();
	while(1)
	{
		setup();
		refreshBoard();
		//drawSnake(boardArray, Move());
		//Sleep(16);
		goToXY(0,0);
	}
	return 0;
}
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>


//TODO: Snake length; Movement; Timer; Eating the food; Collisions

int row;
int col;
int boardArray[25][100];

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
	int snakeRow = rand() % 25;
	int snakeCol = rand() % 100;
	boardArray[snakeRow][snakeCol] = 1;
}

void drawSnake()
{
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

int main()
{
	while(1)
	{
		//basic mainLoop prototype
		int a;
		setup();
		refreshBoard();
		scanf_s("%d", &a);
		system("CLS");
	}
	return 0;
}
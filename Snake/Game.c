#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

int row;
int col;
int headSnakeRow;
int headSnakeCol;
int boardArray[25][25];
int startingMove;
int scoreTracker;
int difficulty;

struct snake
{
	int x;
	int y;

	struct snake *next;
	struct snake *prev;
};

//this link always point to first Link
struct snake *head = NULL;

//this link always point to last Link
struct snake *last = NULL;

struct snake *current = NULL;

//is list empty
boolean isEmpty()
{
	return head == NULL;
}

//moves the console cursor to the specified coordinates
void goToXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void endGameCheck(int x, int y)
{
	if (boardArray[x][y] == 1)
	{
		system("cls");
		printf("GAME OVER \n");
		printf("Score: %d \n", scoreTracker);
		Sleep(100000);
		exit(0);
	}
}

//insert link at the first location
void insertFirst(int x, int y)
{
	//create a link
	struct snake *link = (struct snake*) malloc(sizeof(struct snake));
	link->x = x;
	link->y = y;

	endGameCheck(x, y);

	if (isEmpty())
	{
		//make it the last link
		last = link;
	}
	else
	{
		//update first prev link
		head->prev = link;
	}

	//point it to old first link
	link->next = head;

	//point first to new first link
	head = link;
}

//insert link at the last location
//Although this function is not used in the program it is critical for debugging
void insertLast(int x, int y)
{
	//create a link
	struct snake *link = (struct snake*) malloc(sizeof(struct snake));
	link->x = x;
	link->y = y;

	if (isEmpty())
	{
		//make it the last link
		last = link;
	}
	else
	{
		//make link a new last link
		last->next = link;

		//mark old last node as prev of new link
		link->prev = last;
	}

	//point last to new last node
	last = link;
}

//delete link at the last location
struct snake* deleteLast()
{
	//save reference to last link
	struct snake *tempLink = last;

	//if only one link
	if (head->next == NULL)
	{
		head = NULL;
	}
	else
	{
		boardArray[last->x][last->y] = 0;
		last->prev->next = NULL;
	}

	last = last->prev;
	//return the deleted link
	return tempLink;
}

struct snake* draw()
{
	//start from the first link
	struct snake* current = head;
	struct snake* previous = NULL;
	struct snake *tempLink = last;

	//if list is empty
	if (head == NULL)
	{
		return NULL;
	}

	//navigate through list
	boardArray[current->x][current->y] = 1;
	while (current != tempLink)
	{
		current = current->next;
		boardArray[current->x][current->y] = 1;
	}

	return current;
}

void setup()
{
	//fill the game board with empty values
	for (int row = 0; row < 24; row++)
	{
		for (int col = 0; col < 24; col++)
		{
			boardArray[row][col] = 0;
		}
	}

	//generate two random numbers for the food location
	srand(time(NULL));
	int foodRow = rand() % 24;
	int foodCol = rand() % 24;
	boardArray[foodRow][foodCol] = 2;

	//generate two random numbers for the snake's starting location
	int headSnakeRow = rand() % 24;
	int headSnakeCol = rand() % 24;
	insertFirst(headSnakeRow, headSnakeCol);
	//insertFirst(5, 0);

	startingMove = rand() % 4;
}

void generateFood()
{
	int isGeneratedSuccessfully = 0;

	while (isGeneratedSuccessfully == 0) {
		srand(time(NULL));
		int foodRow = rand() % 25;
		int foodCol = rand() % 25;
		if (boardArray[foodRow][foodCol] == 1) {
			continue;
		}
		boardArray[foodRow][foodCol] = 2;
		scoreTracker += 10;
		isGeneratedSuccessfully++;
	}
}

void food(int x, int y)
{
	if (boardArray[x][y] == 2)
	{
		insertFirst(x, y);
		boardArray[x][y] = 0;
		generateFood();
	}
}

//Directions can be 1(up) 2(down) 3(right) 4(left)
void Move(int direction)
{
	struct snake* current = head;
	switch (direction)
	{
	case 1:
		//Checks for wall collision and loops the snake if it detects such
		//Also checks for collision with food
		if ((current->x - 1) == -1)
		{
			insertFirst(24, current->y);
			food(24, current->y);
			deleteLast();
			break;
		}

		insertFirst((current->x) - 1, current->y);
		food((current->x) - 1, current->y);
		deleteLast();
		break;
	case 2:
		if ((current->x + 1) == 25)
		{
			insertFirst(0, current->y);
			food(0, current->y);
			deleteLast();
			break;
		}

		insertFirst((current->x) + 1, current->y);
		food((current->x) + 1, current->y);
		deleteLast();
		break;
	case 3:
		if ((current->y + 1) == 25)
		{
			insertFirst(current->x, 0);
			food(current->x, 0);
			deleteLast();
			break;
		}

		insertFirst(current->x, (current->y) + 1);
		food(current->x, (current->y) + 1);
		deleteLast();
		break;
	case 4:
		if ((current->y - 1) == -1)
		{
			insertFirst(current->x, 24);
			food(current->x, 24);
			deleteLast();
			break;
		}

		insertFirst(current->x, (current->y) - 1);
		food(current->x, (current->y) - 1);
		deleteLast();
		break;
	default:
		break;
	}
}

void refreshBoard()
{
	for (int row = -1; row <= 25; row++)
	{
		for (int col = -1; col <= 25; col++)
		{
			if ((row == -1 || row == 25))
			{
				printf("#");
				continue;
			}

			if ((col == -1 || col == 25))
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
	printf("Score: %d", scoreTracker);
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

//Gets the arrow
//Directions can be 1(up) 2(down) 3(right) 4(left)
int getKeyboardInput()
{
	/*int a = _getch();
	if (a == 0 || a == 0xE0) a = _getch();*/

	switch (getch())
	{
	case 'w':
	case 'W':
		//Checks if the entered direction is not the complete opposite of the current one
		if (startingMove == 2) return startingMove;
		return 1;
	case 's':
	case 'S':
		if (startingMove == 1) return startingMove;
		return 2;
	case 'd':
	case 'D':
		if (startingMove == 4) return startingMove;
		return 3;
	case 'a':
	case 'A':
		if (startingMove == 3) return startingMove;
		return 4;
	default:
		return startingMove;
	}
}

void menuScreen() {
	while (1) {
		system("cls");
		printf("Wellcome to the Snake Game ver. 1.1 \n");
		printf("Please pick a difficulty: \n");
		printf("1. Easy \n");
		printf("2. Medium \n");
		printf("3. Hard \n");
		scanf("%d", &difficulty);
		switch (difficulty)
		{
		case 1:
			difficulty = 300;
			return;
		case 2: 
			difficulty = 200;
			return;
		case 3: 
			difficulty = 100;
			return;
		default:
			break;
		}
	}
}

int main()
{
	menuScreen();
	system("cls");

	hideCursor();
	setup();

	while (1)
	{
		draw();
		refreshBoard();
		//Directions can be 1(up) 2(down) 3(right) 4(left)
		if (_kbhit())
		{
			Move(startingMove = getKeyboardInput());
		}
		else
		{
			Move(startingMove);
		}
		goToXY(0, 0);
		//The difficulty property is an int between 100 and 300 (hard/normal/easy) that controlls the speed of the game
		Sleep(difficulty);
	}
	return 0;
}
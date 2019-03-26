#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

//Currently the game is theoretically being drawn at 60Hz. Final product should perserve this if possible.

//Major TODO: Snake length; Movement; Timer; Eating the food; Collisions
//Minor TODO: !!!!!!!!! FIND A WAY TO NOT TIE THE GAME SPEED WITH THE DRAW REFRESH RATE !!!!!!!!!

int row;
int col;
int headSnakeRow;
int headSnakeCol;
int boardArray[25][100];
int startingMove;

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
boolean isEmpty() {
	return head == NULL;
}

//insert link at the first location
void insertFirst(int x, int y) {
	//create a link
	struct snake *link = (struct snake*) malloc(sizeof(struct snake));
	link->x = x;
	link->y = y;

	if (isEmpty()) {
		//make it the last link
		last = link;
	}
	else {
		//update first prev link
		head->prev = link;
	}

	//point it to old first link
	link->next = head;

	//point first to new first link
	head = link;
}

//insert link at the last location
void insertLast(int x, int y) {
	//create a link
	struct snake *link = (struct snake*) malloc(sizeof(struct snake));
	link->x = x;
	link->y = y;

	if (isEmpty()) {
		//make it the last link
		last = link;
	}
	else {
		//make link a new last link
		last->next = link;

		//mark old last node as prev of new link
		link->prev = last;
	}

	//point last to new last node
	last = link;
}

//delete first item
struct snake* deleteFirst() {
	//save reference to first link
	struct snake *tempLink = head;

	//if only one link
	if (head->next == NULL) {
		last = NULL;
	}
	else {
		head->next->prev = NULL;
	}

	head = head->next;
	//return the deleted link
	return tempLink;
}

//delete link at the last location

struct snake* deleteLast() {
	//save reference to last link
	struct snake *tempLink = last;

	//if only one link
	if (head->next == NULL) {
		head = NULL;
	}
	else {
		boardArray[last->x][last->y] = 0;
		last->prev->next = NULL;
	}

	last = last->prev;
	//return the deleted link
	return tempLink;
}

struct snake* draw() {
	//start from the first link
	struct snake* current = head;
	struct snake* previous = NULL;
	struct snake *tempLink = last;

	//if list is empty
	if (head == NULL) {
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
	//insertFirst(headSnakeRow, headSnakeCol);
	insertFirst(12, 12);
	insertLast(12, 13);
	insertLast(12, 13);
	insertLast(12, 13);
	insertLast(12, 13);
	insertLast(12, 13);

	startingMove = rand() % 4;
}

//Directions can be 1(up) 2(down) 3(right) 4(left)
void Move(int direction)
{
	struct snake* current = head;
	switch (direction)
	{
	case 1:
		insertFirst((current->x) - 1, current->y);
		deleteLast();
		break;
	case 2:
		insertFirst((current->x) + 1, current->y);
		deleteLast();
		break;
	case 3:
		insertFirst(current->x, (current->y) + 1);
		deleteLast();
		break;
	case 4:
		insertFirst(current->x, (current->y) - 1);
		deleteLast();
		break;
	default:
		break;
	}
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

//hides the cursor from flashing around on the screen and allowes for 60Hz refresh rate that is needed
void hideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int getKeyboardInput()
{
	int a = _getch();
	if (a == 0 || a == 0xE0) a = _getch();

	if (a == 72)
		return 1;
	else if (a == 80)
		return 2;
	else if (a == 75)
		return 4;
	else if (a == 77)
		return 3;
}

int main()
{
	hideCursor();
	setup();

	while (1)
	{
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
		draw();
		goToXY(0, 0);
		if (startingMove < 3) Sleep(100);
		Sleep(300);
	}
	return 0;
}
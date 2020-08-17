/*
Written by: Bello Abarchi, Fred Zeder, Leslie Recendiz
Date: October 2017
Purpose: Battleship, Exam 2
Professor: Hunchuck
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#define PAUSE system("pause")
#define CLS system("cls")
#define FLUSH fflush(stdin)
#define SIZE 10
#define COLS 10
#define ROWS 10
// COLS and ROWS are for the 10x10 game grid; SIZE is for scores

void baseGameboard(char gameboard[ROWS][COLS]);
void bubbleSort(int[], int);
char checkCoordinates(char[][COLS], int, int);
int convertChar(char);
void displayMenu();
void displayHighscores(int[], int);
char getWinCondition(int, int);
void hitMissSunk(char gameBoard[][COLS], char shipBoard[][COLS],
	int i, int j, int hitCount[], int *sunkCount);
int missiles(int[], int, int);
void placeRandomShipBoard(char[][COLS], int);
void rulesOfGame();
void resetBoard(char gameboard[ROWS][COLS]);
void resetHitCount(int[], int);
void saveHighscores(int[], int);
void sunkShipLetters(char[][COLS], char[][COLS], int, int, char);
void userChoiceCoordinates(char *row, int *column);
char userChoice();


/*typedef struct {
char nameInitial[3];
}NAME;*/


main() {
	char row;
	int column;
	char gameBoard[ROWS][COLS];
	char choice;
	int counter = 0;
	int highscoreCount = 0, missileCount = 0, rowNum = 0;
	int highscore[SIZE] = { 0 };
	char shipBoard[ROWS][COLS];
	char win, repeatFlag;
	int hitCount[SIZE] = { 0 };
	int sunkCount = 0;

	srand((unsigned)time(NULL));

	FILE *ptr;

	// function to read saved highscores
	if (ptr = fopen("highscoresTopTen.bin", "rb")) { // open highscoresTopTen.bin 
		if (ptr == NULL) {
			printf("\nError! Could not open file!\n\n");
			PAUSE;
			exit(-1);
		}

		// read highscores from bin file
		fread(&highscoreCount, sizeof(int), 1, ptr);
		fread(highscore, sizeof(int), highscoreCount, ptr);

		fclose(ptr);
	} // end if

	do {
		choice = userChoice();

		switch (choice)
		{
		case 'A':
			//start game
			resetBoard(gameBoard);// Clear the board

								  // put $'s in shipBoard array
			resetBoard(shipBoard);

			// reset hitcount array after every game
			resetHitCount(hitCount, counter);

			// This is where the ships are randomly placed
			placeRandomShipBoard(shipBoard, counter);

			win = 'N'; sunkCount = 0; missileCount = 0;
			do {
				// do loop keeps looping to enter coordinates
				repeatFlag = 'N';// works unless all ships sunk
				do {
					// This is the base gameboard function
					baseGameboard(gameBoard);

					// Display missile counter
					printf("\n\t\t\t\t\t\t\t\t      Missiles Fired: %i\n\n", missileCount);


					// gets coordinates
					userChoiceCoordinates(&row, &column);

					// Had to decrement column to match subindex element in gameBoard array
					column--;

					// This converts char from row into int and puts it in rowNum
					rowNum = convertChar(row);

					// error Check for coordinates
					repeatFlag = checkCoordinates(gameBoard, rowNum, column);
				} while (repeatFlag != 'N'); // end do while

											 // This changes $'s on gameboard and displays sunk ship
				hitMissSunk(gameBoard, shipBoard, rowNum, column, hitCount, &sunkCount);

				// Increments missileCount
				missileCount++;

				// This changes win to Y when all ships are sunk and displays win messages
				win = getWinCondition(sunkCount, missileCount);
			} while (win != 'Y'); // end do while loop for the game

								  // Saves missileCount into highscore array
			highscoreCount = missiles(highscore, highscoreCount, missileCount);
			break;
		case 'B'://see rules
			rulesOfGame();
			PAUSE;
			break;
		case 'C'://see top 10 scores
				 // this displays the current highscores			
			displayHighscores(highscore, highscoreCount);
			PAUSE;
			break;
		case 'S'://Quit game

				 // It will save the scores, sort them with a bubble sort and save the top 10.
			saveHighscores(highscore, highscoreCount);

			CLS;
			system("color F9");
			printf("\t\t\t========================================================\n");
			printf("\t\t\t\t  You have quit playing Battleship!\n\n\t\t\t\t      Thank you come again!\n");
			printf("\t\t\t========================================================\n\n");
			PAUSE;
			break;
		default:
			printf("\n\t\tPlease pick a valid option.\n\n");
			PAUSE;
			break;
		}
	} while (choice != 'S'); // end do while
} //end main

void baseGameboard(char gameboard[ROWS][COLS]) {
	CLS;
	system("color 8E");
	int i, j;

	printf("\t\t\t\t\t====================\n\t\t\t\t\t  == BATTLESHIP ==\n\t\t\t\t\t====================\n");
	printf("\n\t\t1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t\n");
	for (i = 0; i < ROWS; i++) {
		printf("\t  *************************************************************************************\n");
		if (i == 0)
			printf("\tA|");
		else if (i == 1)
			printf("\tB|");
		else if (i == 2)
			printf("\tC|");
		else if (i == 3)
			printf("\tD|");
		else if (i == 4)
			printf("\tE|");
		else if (i == 5)
			printf("\tF|");
		else if (i == 6)
			printf("\tG|");
		else if (i == 7)
			printf("\tH|");
		else if (i == 8)
			printf("\tI|");
		else if (i == 9)
			printf("\tJ|");
		for (j = 0; j < COLS; j++) {
			printf("\t%c", gameboard[i][j]);
		}
		printf("\n");
	}

} // end baseGameboard

void bubbleSort(int scores[], int counter) {
	int i, bottom, temp;
	char flag;

	bottom = counter - 1;

	do {
		flag = 'N';
		for (i = 0; i < bottom; i++) {
			if (scores[i] > scores[i + 1]) {
				flag = 'Y';
				temp = scores[i];
				scores[i] = scores[i + 1];
				scores[i + 1] = temp;
			} // end if
		} // end for
		bottom--;
	} while (flag == 'Y'); // end do while

	return;
} // end bubbleSort

void displayMenu() {
	CLS;
	system("color B5");
	printf("\t\t\t\t\t=======================================\n");
	printf("\t\t\t\t\t=======================================\n");
	printf("\t\t\t\t\t==         M A I N   M E N U         ==\n");
	printf("\t\t\t\t\t=======================================\n");
	printf("\t\t\t\t\t=======================================\n\n");
	printf("\t\t\t\t\t[A] New game\n");
	printf("\t\t\t\t\t[B] Rules\n");
	printf("\t\t\t\t\t[C] High scores\n");
	printf("\t\t\t\t\t[S] Save and Quit\n");
	printf("\t\t\t\t\tEnter choice: ");
}//end displayMenu

 // check if user has already entered those coordinates
char checkCoordinates(char gameBoard[][COLS], int row, int column) {
	char result;

	if (gameBoard[row][column] == '$') {
		result = 'N';
	}
	else {
		result = 'Y';
		printf("\nYou have already entered those coordinates. Please choose new coordinates!\n\n");
		PAUSE;
	}

	return result;
} // end checkCoordinates

  // convert row's char to an int so that the compiler can read the coordinates
int convertChar(char row) {
	int result;

	switch (row) {
	case 'A':
		result = 0;
		break;
	case 'B':
		result = 1;
		break;
	case 'C':
		result = 2;
		break;
	case 'D':
		result = 3;
		break;
	case 'E':
		result = 4;
		break;
	case 'F':
		result = 5;
		break;
	case 'G':
		result = 6;
		break;
	case 'H':
		result = 7;
		break;
	case 'I':
		result = 8;
		break;
	case 'J':
		result = 9;
		break;
	} // end switch

	return result;
} // end convertChar

  // display top 10 highscores
void displayHighscores(int scores[], int counter) {
	int i;

	CLS;
	printf("\t\t\t\t\t");
	for (i = 0; i < 20; i++) {
		printf("=");
	} // end for
	printf("\n\t\t\t\t\t  == HIGHSCORES ==\n\t\t\t\t\t");
	for (i = 0; i < 20; i++) {
		printf("=");
	} // end for
	printf("\n\n\n");

	bubbleSort(scores, counter);

	if (counter == 0) {
		printf("There are currently no saved highscores!\n");
	}
	else {
		for (i = 0; i < counter; i++) {
			printf("\t\t\t    Highscore #%i:\t\t%i Missiles Fired", i + 1, scores[i]);
		} // end for
	} // end if else
	printf("\n");

	return;
} // end of displayHighscores

  // checks for win condition after every missile fired
char getWinCondition(int sunkCount, int missiles) {
	char result = 'N';

	if (sunkCount == 5) {
		printf("\nCongratulations! You've sunk all the ships!\n\n");
		printf("Your highscore is %i missiles!\n\n", missiles);
		//We tried to make this work:
		//printf("Enter your name initials: ");
		//scanf("%s",name->nameInitial);
		result = 'Y'; PAUSE;
	}

	return result;
} // end getWinCondition

void hitMissSunk(char gameBoard[][COLS], char shipBoard[][COLS], int i, int j, int hitCount[], int *sunkCount)
{
	int airCraft = 5, battleShip = 4, cruiser = 2, sub = 3, destroyer = 3;
	char shipLetter = '\0';

	//ship names

	if (shipBoard[i][j] == '$')
	{
		gameBoard[i][j] = 'M';
		printf("\nMISS! There was no ship there!\n\n"); PAUSE;
	}
	else
	{
		gameBoard[i][j] = 'H';
		printf("\nHIT! You've hit a ship!\n\n"); PAUSE;

		switch (shipBoard[i][j])
		{
		case 'A':
			hitCount[0]++;
			//Aircraft Carrier has been hit
			if (hitCount[0] >= airCraft) {
				shipLetter = 'A';
				sunkShipLetters(gameBoard, shipBoard, i, j, shipLetter);
				printf("\nYou sunk the Aircraft Carrier!\n\n");
				(*sunkCount)++; PAUSE;
			}
			break;
		case 'B':
			hitCount[1]++;
			//battleShip has been hit
			if (hitCount[1] >= battleShip) {
				shipLetter = 'B';
				sunkShipLetters(gameBoard, shipBoard, i, j, shipLetter);
				printf("\nYou sunk the Battleship!\n\n");
				(*sunkCount)++; PAUSE;
			}
			break;
		case 'S':
			hitCount[2]++;
			//submarine has been hit
			if (hitCount[2] >= sub) {
				shipLetter = 'S';
				sunkShipLetters(gameBoard, shipBoard, i, j, shipLetter);
				printf("\nYou sunk the Submarine!\n\n");
				(*sunkCount)++; PAUSE;
			}
			break;
		case 'D':
			hitCount[3]++;
			//destroyer has been hit
			if (hitCount[3] >= destroyer) {
				shipLetter = 'D';
				sunkShipLetters(gameBoard, shipBoard, i, j, shipLetter);
				printf("\nYou sunk the Destroyer!\n\n");
				(*sunkCount)++; PAUSE;
			}
			break;
		case 'C':
			hitCount[4]++;
			//cruiser has been hit
			if (hitCount[4] >= cruiser) {
				shipLetter = 'C';
				sunkShipLetters(gameBoard, shipBoard, i, j, shipLetter);
				printf("\nYou sunk the the Cruiser!\n\n");
				(*sunkCount)++; PAUSE;
			}
			break;
		}

	}

} // end hitMissSunk

  // replaces lowest score with new highscore if applicable
  // then sorts scores into correct order
int missiles(int scores[], int counter, int missiles) {
	int result;
	int i;

	if (counter < 10) { // fill up highscore arrray
		scores[counter] = missiles;
		counter++; // will only count up until counter reaches 10
	}
	else {
		for (i = 0; i < counter; i++) {
			if (missiles < scores[i]) {
				// replace last highscore with new highscore
				scores[counter - 1] = missiles;
			}
		} // end for
	} // end if else
	  // second bubbleSort is necessary incase new highscore is not last
	bubbleSort(scores, counter);

	result = counter;
	return result;
} // end missiles

  // randomly place ships into shipboard array without overlapping
void placeRandomShipBoard(char ship[][COLS], int counter) {
	char flag = 'N'; // flag to make sure ship is placed
	char full = 'N'; // flag to make sure there isn't another ship in that position
	int LB, UB;
	int randomRows = 0, randomCols = 0, position = 0, temp = 0;


	do { // loop until AIRCRAFT CARRIER is placed
		 // get random coordinates
		flag = 'N'; counter = 0;
		LB = 0, UB = 9;
		randomRows = LB + rand() % (UB - LB + 1);
		randomCols = LB + rand() % (UB - LB + 1);
		// get random decision for horizontal(1) or vertical(2) positioning
		LB = 1, UB = 2;
		position = LB + rand() % (UB - LB + 1);

		if (position == 1) {
			counter = randomCols + 4;
			// check to see if coordinates are too near the boundary
			if (counter < 10) {
				for (randomCols; randomCols <= counter; randomCols++) {
					// place ship letters in array
					ship[randomRows][randomCols] = 'A';
				} // end for
				flag = 'Y';
			}
		}
		else {
			counter = randomRows + 4;
			if (counter < 10) {
				for (randomRows; randomRows <= counter; randomRows++) {
					ship[randomRows][randomCols] = 'A';
				} // end for
				flag = 'Y';
			}
		}
	} while (flag != 'Y'); // end do while

	do { // loop until BATTLESHIP is placed
		 // get random coordinates
		flag = 'N'; full = 'N'; counter = 0;
		LB = 0, UB = 9;
		randomRows = LB + rand() % (UB - LB + 1);
		randomCols = LB + rand() % (UB - LB + 1);
		// get random decision for horizontal(1) or vertical(2) positioning
		LB = 1, UB = 2;
		position = LB + rand() % (UB - LB + 1);

		if (position == 1) {
			temp = randomCols;
			counter = randomCols + 3;
			// check to see if there is another ship there
			for (randomCols = temp; randomCols <= counter; randomCols++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomCols = temp; randomCols <= counter; randomCols++) {
					ship[randomRows][randomCols] = 'B';
				} // end for
				flag = 'Y';
			}
		}
		else {
			temp = randomRows;
			counter = randomRows + 3;
			for (randomRows = temp; randomRows <= counter; randomRows++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomRows = temp; randomRows <= counter; randomRows++) {
					ship[randomRows][randomCols] = 'B';
				} // end for
				flag = 'Y';
			}
		}
	} while (flag != 'Y'); // end do while

	do { // loop until SUBMARINE is placed
		 // get random coordinates
		flag = 'N'; full = 'N'; counter = 0;
		LB = 0, UB = 9;
		randomRows = LB + rand() % (UB - LB + 1);
		randomCols = LB + rand() % (UB - LB + 1);
		// get random decision for horizontal(1) or vertical(2) positioning
		LB = 1, UB = 2;
		position = LB + rand() % (UB - LB + 1);

		if (position == 1) {
			temp = randomCols;
			counter = randomCols + 2;
			// check to see if there is another ship there
			for (randomCols = temp; randomCols <= counter; randomCols++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomCols = temp; randomCols <= counter; randomCols++) {
					ship[randomRows][randomCols] = 'S';
				} // end for
				flag = 'Y';
			}
		}
		else {
			temp = randomRows;
			counter = randomRows + 2;
			for (randomRows = temp; randomRows <= counter; randomRows++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomRows = temp; randomRows <= counter; randomRows++) {
					ship[randomRows][randomCols] = 'S';
				} // end for
				flag = 'Y';
			}
		}
	} while (flag != 'Y'); // end do while

	do { // loop until DESTROYER is placed
		 // get random coordinates
		flag = 'N'; full = 'N'; counter = 0;
		LB = 0, UB = 9;
		randomRows = LB + rand() % (UB - LB + 1);
		randomCols = LB + rand() % (UB - LB + 1);
		// get random decision for horizontal(1) or vertical(2) positioning
		LB = 1, UB = 2;
		position = LB + rand() % (UB - LB + 1);

		if (position == 1) {
			temp = randomCols;
			counter = randomCols + 2;
			// check to see if there is another ship there
			for (randomCols = temp; randomCols <= counter; randomCols++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomCols = temp; randomCols <= counter; randomCols++) {
					ship[randomRows][randomCols] = 'D';
				} // end for
				flag = 'Y';
			}
		}
		else {
			temp = randomRows;
			counter = randomRows + 2;
			for (randomRows = temp; randomRows <= counter; randomRows++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomRows = temp; randomRows <= counter; randomRows++) {
					ship[randomRows][randomCols] = 'D';
				} // end for
				flag = 'Y';
			}
		}
	} while (flag != 'Y'); // end do while

	do { // loop until CRUISER is placed
		 // get random coordinates
		flag = 'N'; full = 'N'; counter = 0;
		LB = 0, UB = 9;
		randomRows = LB + rand() % (UB - LB + 1);
		randomCols = LB + rand() % (UB - LB + 1);
		// get random decision for horizontal(1) or vertical(2) positioning
		LB = 1, UB = 2;
		position = LB + rand() % (UB - LB + 1);

		if (position == 1) {
			temp = randomCols;
			counter = randomCols + 1;
			// check to see if there is another ship there
			for (randomCols = temp; randomCols <= counter; randomCols++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomCols = temp; randomCols <= counter; randomCols++) {
					ship[randomRows][randomCols] = 'C';
				} // end for
				flag = 'Y';
			}
		}
		else {
			temp = randomRows;
			counter = randomRows + 1;
			for (randomRows = temp; randomRows <= counter; randomRows++) {
				if (ship[randomRows][randomCols] != '$') {
					full = 'Y';
				}
			} // end for
			if (counter < 10 && full == 'N') {
				for (randomRows = temp; randomRows <= counter; randomRows++) {
					ship[randomRows][randomCols] = 'C';
				} // end for
				flag = 'Y';
			}
		}
	} while (flag != 'Y'); // end do while
	return;
} // end placeRandomShipBoard

  //Resets for every new game
void resetBoard(char gameboard[ROWS][COLS]) {
	int i, j;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			gameboard[i][j] = '$';
		}
	}
} // end resetBoard

  // reset hitcount array for every new game
void resetHitCount(int hitCount[], int i) {

	for (i = 0; i < SIZE; i++) {
		hitCount[i] = 0;
	} // end for

	return;
} // end resetHitCount

void rulesOfGame() {
	CLS;
	CLS;
	system("color 3F");
	printf("\t\t\t\t============================\n\t\t\t\t == RULES OF BATTLESHIP ==\n\t\t\t\t============================\n\n");
	printf("\t\t\t\t1. The objective is to sink the ships the computer has placed.\n");
	printf("\t\t\t\t2. When you have a hit you will see a 'H' in that square and when"
		" you miss\n   \t\t\t\tyou will see a 'M'.\n");
	printf("\t\t\t\t3. When a ship has been sunk, the 'H' will change to its respective"
		" letter\n   \t\t\t\tto signify that it has sunk.\n");
	printf("\t\t\t\t4. The highest score that can be achieved is 17 points.\n\n");
	printf("\t\t\t\t============================\n\t\t\t\t============================\n\t\t\t\t============================\n\n");
	system("pause");

} // end rulesOfGame

  // save highscores into bin file
void saveHighscores(int scores[], int counter) {
	FILE *ptr;

	ptr = fopen("highscoresTopTen.bin", "wb"); // create highscoresTopTen.bin file
	if (ptr == NULL) {
		printf("\nError! Could not open file!\n\n");
		PAUSE;
		exit(-1);
	}

	// sort the highscores array before saving
	bubbleSort(scores, counter);
	// save highscores into the bin file
	fwrite(&counter, sizeof(int), 1, ptr);
	fwrite(scores, sizeof(int), counter, ptr);

	fclose(ptr);

	return;
} // end saveHighscores

  // changes H's to ship's letter when a ship is sunk
void sunkShipLetters(char gameBoard[][COLS], char shipBoard[][COLS], int i, int j, char letter) {

	// loop through the entire ship board
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) { // replace game board letter with ship letter
			if (shipBoard[i][j] == letter) {
				gameBoard[i][j] = letter;
			}
		} // end inner for
	} // end outer for

	return;
} // end sunkShipLetters

char userChoice()
{
	char result;
	displayMenu();
	scanf("%c", &result); FLUSH;
	return toupper(result);
} // end userChoice 

void userChoiceCoordinates(char *row, int *column) {

	do { // loop until valid variable
		printf("\nChoose your row(A-J): ");
		scanf("%c", row); FLUSH;
		*row = toupper(*row);
		if ((*row) < 'A' || (*row) > 'J') {
			printf("\nThat is not a valid coordinate. Please re-enter!\n");
		}
	} while ((*row) < 'A' || (*row) > 'J'); // end do while

	do { // loop until valid variable
		printf("\nChoose your column(1-10): ");
		scanf("%i", column); FLUSH; 
		if (*row == 'S' || *column == 'S')
			if ((*column) < 1 || (*column) > 10) {
				printf("\nThat is not a valid coordinate. Please re-enter!\n");
			}
	} while ((*column) < 1 || (*column) > 10); // end do while	

} // end userChoiceCoordinates
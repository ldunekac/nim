/*
 * NIM coded in C. This was only worked on after midnight
 * but it should be bullet proof. 
 */

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/*
 * Global variables
 * The board is global because most functions use it and 
 * the program cannot generat multiple boards

 * The playerType and computer Type are global because
 * many functions use them and there is only one of each
 * in the program
 */
struct {
	int*  	rowLengths;
	int 	numberOfRows;
} nimBoard;

typedef enum {HUMAN, COMPUTER} 	playerType;
typedef enum {SMART, RANDOM  } 	computerType;
playerType player = HUMAN;
computerType computer= SMART;

/*
 * Free up memory that the board is using
 */
void freeBoard() 
{
	free(nimBoard.rowLengths);
}

/*  
 * Prints the board
 */
void printBoard() 
{
	int i,j;
	// loops throught he board printing the number of X's
	for(i = 0; i < nimBoard.numberOfRows; i++) 
	{
		printf("ROW %d:(%d) ", i + 1, nimBoard.rowLengths[i]);
		for(j = 0; j < nimBoard.rowLengths[i]; j++) 
		{
			printf("X");
		}
		printf("\n");
	}
	printf("\n");
}


void initializeBoard(int whichBoard)
{
	/*
	 * This will load the board to
	 * row 1: X 
	 * row 2: XXX
	 * row 3: XXXXX
	 * row 4: XXXXXXX
	 */
	if(whichBoard == 1)
	{
		nimBoard.numberOfRows = 4;
		nimBoard.rowLengths = calloc(4, sizeof(int));
		nimBoard.rowLengths[0] = 1;
		nimBoard.rowLengths[1] = 3;
		nimBoard.rowLengths[2] = 5;
		nimBoard.rowLengths[3] = 7;
	}
	/*
	 * This will load the board to
	 * row 1: XXXX
	 * row 2: XXX
	 * row 3: XXXXXXX
	 */
	else
	{
		nimBoard.numberOfRows = 3;
		nimBoard.rowLengths = calloc(3, sizeof(int));
		nimBoard.rowLengths[0] = 4;
		nimBoard.rowLengths[1] = 3;
		nimBoard.rowLengths[2] = 7;
	}
}

/*
 * Checks to see if the game is over by checking if 
 * each row in the borad is empty
 */
int gameOver() 
{
	int i; 
	for(i = 0; i < nimBoard.numberOfRows; i++) 
	{
		if(nimBoard.rowLengths[i] > 0) return FALSE;
	}
	return TRUE;
}

/*
 * Computer chooses a random row and number of sticks to take
 * Becuase the computer is choosing a random row that might 
 * not contain any sticks it is possible for the computer
 * to never choose a valid row. This is exreamly unlikely
 * 
 * If many rows are being used the computer should get a list
 * of which rows he can choose from and chose a random row
 * from those 
 */ 
void makeRandomMove() 
{
	int row, many;
	do 
	{
		row = rand() % (nimBoard.numberOfRows);
	} while(nimBoard.rowLengths[row] == 0);

	many = (rand() % nimBoard.rowLengths[row]) + 1 ;
	printf("Computer takes %d from row %d.\n\n", many, row + 1);

	nimBoard.rowLengths[row] -= many;
}

/*
 * Uses XOR magic to find the best move
 */
void makeSmartMove() 
{
	int i, xorTotal, amoutToSubtract;
	xorTotal = 0;
	for(i = 0; i < nimBoard.numberOfRows; i++) 
	{
		xorTotal = xorTotal ^ nimBoard.rowLengths[i];
	}

	if(xorTotal == 0) return makeRandomMove();
	
	for(i = 0; i < nimBoard.numberOfRows; i++)
	{
		if(nimBoard.rowLengths[i] > (xorTotal ^ nimBoard.rowLengths[i]))
		{
			amoutToSubtract = nimBoard.rowLengths[i] - (xorTotal ^ nimBoard.rowLengths[i]);
			break;
		}
	}
	if(i == nimBoard.numberOfRows) 
	{
		perror("incorrect calculation was made!");
		return makeRandomMove();
	}
	printf("Computer takes %d for row %d\n\n", amoutToSubtract, i+1);
	nimBoard.rowLengths[i] -= amoutToSubtract;
}

/*
 * Deternimes if the smart or random player should play
 */
void makeComputerMove() {
	if(computer == SMART) makeSmartMove();
	else makeRandomMove();
}

/*
 * Player pics a row and number of sticks to take
 */
void getHumanMove() {
	int row, many;
	char str[10];
	int validRow = FALSE;
	int validNumberOfSticks = FALSE;
	while(!validRow)
	{

		printf("Pick a row <1-%d>: ", nimBoard.numberOfRows);
		row = getUserInput();
		// makes sure that the input is in range
		if(row < 1 || row > nimBoard.numberOfRows)
		{
			printf("Row out of range.\n");
		}
		// makes sure the row is not an empty row
		else if( nimBoard.rowLengths[row - 1] <= 0)
		{
			printf("No sticks in that row. \n");
		} 
		else
		{
			validRow = TRUE;
		}
	}

	while(!validNumberOfSticks) 
	{
		printf("Select number of Sticks <1-%d>: ", nimBoard.rowLengths[row - 1]);
		many = getUserInput();
		// makes sure input is in the correct range
		if(many <= 0 || many > nimBoard.rowLengths[row - 1])
		{
			printf("The number of sticks must be between 1 and %d.\n", nimBoard.rowLengths[row -1]);
		}
		else
		{
			validNumberOfSticks = TRUE;
		}
	} 
	printf("\n");
	nimBoard.rowLengths[row - 1] -= many;	
}

/*
 * Decides which player should move
 */ 
void makeMove() 
{
	if(player == COMPUTER) 
	{
		printf("Commputer is making move \n");
		makeComputerMove();
	}
	else 
	{
		getHumanMove();
	}
}

/*
 * alternates players
 */
void changePlayer() 
{
	if(player == COMPUTER) player = HUMAN;
	else player = COMPUTER;
}

/*
 * Prints the winner of nim!!!
 * If it currently the human turns that means the computer won
 * the with the prevoius move
 */
void printVictor() {
	if (player == HUMAN) printf("Computer Wins!\n");
	else printf("You win!!!\n");
}


/*
 * Reads input from the command line. The buffer only 
 * accepts valid numbers to be added into the buffer
 * 
 * The zero int is used to keep track if the input was 
 * all zeros. This has to be checked because the atoi 
 * function returns zero if it fails or gets a zero number
 *
 * UPDATE: The zero is no longer needed sense row 0 is no
 * longer a valid row
 */
int getUserInput() {
	int bufferSize = 10; // reads a max of 10 characters
	char c;
	char buffer[10] = {0}; // initial all buffers to zero
	int i, returnValue;
	int zero = TRUE;
	
	// Reads the input one character at a time
	// If that character is a valid number it is added 
	// to the buffer
	for(i = 0; i < bufferSize; i++)
	{
		// check for end of input
		if(!(c = getchar()) || c == '\n') break; 
		// only load buffer with valid numbers
		if(c >= '0' || c <= '9') 
		{
			if(c != '0') zero = FALSE;
			buffer[i] = c;
		} 
		else 
		{
			i--; // buffer did not add anything
		}
	}
	// clear input stream
	if(i == bufferSize && buffer[bufferSize-1] != '\n')
		while ((c = getchar()) != '\n' && c != EOF);

	if(zero) return 0; // If the number was all zeros
	if(buffer[0] == 0) return -1; // if no numbers were read
	if(atoi(buffer) <= 0) return -1; // if atoi function failed
	return atoi(buffer); // returns the number inputed by the user
}

/*
 * Gets the first character from the input and returns it
 * and then clears the input stream
 */
char getUserOption() 
{
	int bufferSize = 1;
	char buffer[1] = {0};
	char c;
	int i;
	// dont need to check for end of input sense our buffer is 1 in length
	for(i = 0; i < bufferSize; i++)
	{
		c = getchar();
		buffer[i] = c;
	}
	if(i == bufferSize && buffer[bufferSize-1] != '\n')
		while ((c = getchar()) != '\n' && c != EOF);

	return buffer[0];
}

/*
 * User picks which board he/she wants to play with
 */
void selectBoardConfiguration()
{
	char c;
	do
	{
		printf("Board Configurations: \n");
		printf("1: [1, 3, 5, 7]\n");
		printf("2: [4, 3, 7]\n");
		printf("Select board configuration <1-2>: ");
		c = getUserOption();
		printf("\n");
	} while(c != '1' && c != '2');

	if(c == '1') initializeBoard(1);
	else initializeBoard(2);
}

/*
 * Selects the computer to be random or smart
 */
void selectComputerType()
{
	char c;
	do
	{
		printf("Play against a random player (r) or a smart player (s)? ");
		c = getUserOption();
	} while(c != 's' && c != 'r');
	printf("\n");
	if(c == 's') computer = SMART;
	else computer = RANDOM;
}

/*
 * Initialize the current player to computer
 */
void makeComputerStart()
{
	player = COMPUTER;
}

/*
 * Main game loop. 
 */
void playGame() 
{
	char c;
	do {
		selectBoardConfiguration();
		selectComputerType();
		makeComputerStart();
		printBoard();
		while(!gameOver()) 
		{
			makeMove();
			printBoard();
			changePlayer();
		}
		printVictor();
		freeBoard();
		printf("Play again? yes or no: ");
		c = getUserOption();
	} while(c != 'n' && c != 'N');
}


/*
 * initializes random
 * see if the smart player or random plyaer will play
 * starts the game
 */
int main(int argc, char const *argv[])
{
	srand(time(NULL));
	printf("Lets play nim!!\n");

	playGame();
	return 0;
}
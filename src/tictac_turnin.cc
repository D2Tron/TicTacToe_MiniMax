#include <tictac_support.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>

using namespace std;
/**
	make_move: takes a board state and makes a legal 
	(hopefully optimal) move

	args:
		int [][3] board: 3x3 array of ints representing the 
		board state. The values of board are altered based
		on the move
			0: empty
			1: x
		 	-1: o
		
	returns (int):
		the number of steps it took to choose the best move
		(current implementation returns 1 by default, 0 if no move made)
**/

int make_move( int board[][3] )
{
	int steps = 1;
	int x = -1;
	int y = -1;
	int bestVal = 0;

	// figure out what move it is
	int state = 0;
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			state += board[i][j];

	state = -state;
	if( state == 0 )
		state = 1;

	if (state == 1) {
		bestVal = -1000;
	} else {
		bestVal = 1000;
	}

	//See if game is already done
	if ( (gameOver(board)) || (isDraw(board)) ) {
		cout << "Game already concluded!" << endl;
		return 0;
	}

	// Make the best move
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			// find an empty square
			if( board[i][j] == 0 )
			{
				//cout << "First space---------" << i << " " << j << endl;
				//Set the empty square to the current state
				board[i][j] = state;

				//Run minimax to find the optimal value for this empty square
				bool turn = bool(state);
				int moveVal = minimax(!turn, steps++, board);

				//Reverse the state
				board[i][j] = 0;
				
				//cout << "Moveval: " << moveVal << endl;
				//Based on the state, check if the moveVal is optimal
				if (state == 1) {
					if (moveVal > bestVal)
					{
					    x = i;
					    y = j;
					    bestVal = moveVal;
					}	
				} else {
					if (moveVal < bestVal)
					{
					    x = i;
					    y = j;
					    bestVal = moveVal;
					}
				}
			}
		}
	}

	// that's the move
	board[x][y] = state;
	printf( "Player [%d] made move: [%d,%d]\n", state, x, y );

	//State the winner, if there is one
	if (gameOver(board)) {
		cout << "Player [" << state << "] wins the game!" << endl;
		return 1;
	}
	
	else if (isDraw(board)) {
	// no move was made (board was full)
		cout << "Game ends in a draw!" << endl;
		return 1;
	}
	
	else {
		//cout << steps << endl;
		return steps;
	}
}

int minimax(bool turn, int steps, int board[][3]) {
	//Assign a score to the moves
	int score = evaluate(board);
	//cout << "Score: " << score << endl;
	if (score == 10) {
		return score;
	}
	if (score == -10) {
		return score;
	}

	if (isDraw(board)==true)
        	return 0;

	//If it is the maximizer's turn
	if (turn)
	    {
		int best = -1000;
	 
		// Traverse all cells
		for (int i = 0; i<3; i++)
		{
		    for (int j = 0; j<3; j++)
		    {
		        // Check if cell is empty
		        if (board[i][j]==0)
		        {
				//cout << "point: " << i << " " << j << endl;
		            // Make the move
		            board[i][j] = 1;
	 
		            // Call minimax recursively and choose
		            // the maximum value
			
				//cout << "Turn: " << !turn << endl;
		            best = max( best, minimax( !turn, steps++, board ) );
	 
		            // Undo the move
		            board[i][j] = 0;
		        }
		    }
		}
		//cout << "Best: " << best << endl;
		return best;
	    }
	 
	    // If this minimizer's move
	    else
	    {
		int best = 1000;
	 
		// Traverse all cells
		for (int i = 0; i<3; i++)
		{
		    for (int j = 0; j<3; j++)
		    {
		        // Check if cell is empty
		        if (board[i][j]==0)
		        {
				//cout << "point: " << i << " " << j << endl;
		            // Make the move
		            board[i][j] = -1;
	 
		            // Call minimax recursively and choose
		            // the minimum value
				//cout << "Turn: " << !turn << endl;
		            best = min(best, minimax(!turn, steps++, board));
	 
		            // Undo the move
		            board[i][j] = 0;
		        }
		    }
		}
		//cout << "Best: " << best << endl;
		return best;
	    }
}

int evaluate(int b[][3])
{
    // Checking for Rows for X or O victory.
    for (int row = 0; row<3; row++)
    {
        if (b[row][0]==b[row][1] &&
            b[row][1]==b[row][2])
        {
            if (b[row][0]== 1)
                return +10;
            else if (b[row][0]== -1)
                return -10;
        }
    }
 
    // Checking for Columns for X or O victory.
    for (int col = 0; col<3; col++)
    {
        if (b[0][col]==b[1][col] &&
            b[1][col]==b[2][col])
        {
            if (b[0][col]== 1)
                return +10;
 
            else if (b[0][col]== -1)
                return -10;
        }
    }
 
    // Checking for Diagonals for X or O victory.
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2])
    {
        if (b[0][0]== 1)
            return +10;
        else if (b[0][0]== -1)
            return -10;
    }
 
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0])
    {
        if (b[0][2]== 1)
            return +10;
        else if (b[0][2]== -1)
            return -10;
    }
 
    // Else if none of them have won then return 0
    return 0;
}

//Check if game is over
bool gameOver(int board[][3]) 
{ 
	return(rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board) ); 
}

//Check if a row is crossed
bool rowCrossed(int board[][3]) 
{ 
	for (int i=0; i<3; i++) 
	{ 
		if (board[i][0] == board[i][1] && 
			board[i][1] == board[i][2] && 
			board[i][0] != 0) 
			return (true); 
	} 
	return(false); 
} 

//Check if column is crossed
bool columnCrossed(int board[][3]) 
{ 
	for (int i=0; i<3; i++) 
	{ 
		if (board[0][i] == board[1][i] && 
			board[1][i] == board[2][i] && 
			board[0][i] != 0) 
			return (true); 
	} 
	return(false); 
} 

//Check if diagonal is crossed
bool diagonalCrossed(int board[][3]) 
{ 
	if (board[0][0] == board[1][1] && 
		board[1][1] == board[2][2] && 
		board[0][0] != 0) 
		return(true); 
		
	if (board[0][2] == board[1][1] && 
		board[1][1] == board[2][0] && 
		board[0][2] != 0) 
		return(true); 

	return(false); 
} 

//Check if game is draw
bool isDraw(int board[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0) {
				return (false);
			}
		}
	}
	return (true);
}

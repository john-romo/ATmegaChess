// chess.c

#include "headers/chess.h"

uint8_t board[64];
uint8_t previous_positions[9][64];

uint8_t start;
uint8_t end;

uint8_t turn = 1;
uint8_t turn_count = 0;

uint8_t check = 2;
uint8_t checkmate = 0;

uint64_t white_move_map = 0;
uint64_t black_move_map = 0;


/////////////////////////////////////////////////////////////////////////////////////////

// -------------- TESTS -------------------- //
#include "headers/tests.h"

int8_t getCoords(char ch){
	char c; 
	uint8_t r;
	printf("Enter %c Collumn: ", ch);
	scanf(" %c", &c);
	if(c == 'x') return 'x';
	if(c == '+') return '+';
	if(c == '-') return '-';
	printf("Enter %c Row: ", ch);
	scanf(" %" SCNu8, &r);
	return (c - 97) + (8 * (8 - r));
}

void testMoves(){
	board[12] = PAWN + MOD;
	board[11] = PAWN;
	board[13] = PAWN;
//	board[13] = PAWN;
//  board[60] = PAWN;
//	board[61] = PAWN;
//	board[62] = PAWN;
//	board[63] = PAWN;

	board[52] = IS_WHITE + PAWN; //	board[9] = IS_WHITE + PAWN + MOD;
	board[51] = IS_WHITE + PAWN;
	board[53] = IS_WHITE + PAWN;
//	board[12] = IS_WHITE + BISHOP;
//	board[13] = IS_WHITE + BISHOP;
//	board[14] = IS_WHITE + BISHOP;
//	board[15] = IS_WHITE + BISHOP;

//	board[35] = KNIGHT;
//	board[26] = KNIGHT;

//	board[] = IS_WHITE + KNIGHT;
//	board[31] = IS_WHITE + KNIGHT;

//	board[0] = BISHOP;
//	board[0] = BISHOP;
//	board[] = IS_WHITE + BISHOP;
//	board[34] = IS_WHITE + BISHOP;

//	board[0] = ROOK + MOD;
//	board[7] = ROOK;
//	board[56] = IS_WHITE + ROOK + MOD;
//	board[5] = IS_WHITE + ROOK;

//	board[61] = IS_WHITE + QUEEN;

//	board[3] = QUEEN;
	
	board[4] = KING + MOD;

	board[60] = IS_WHITE + KING + MOD;
}

void testLoop(){
	turn = 1; // white = 1, black = 0

//	initBoard();	
	testMoves();
	getAttacks();

	while((start != 'x') && !endGame()){
		
		printBoard();
		printAttackBoard();
		printMoveMap(1);
		printMoveMap(0);

		start = getCoords('S');

		if(start == 'x') printf("\nPLAYER %d RESIGNED\n", turn);
		else if((start == '+' ) || (start == '-')){
			
			if(check == turn) printf("Illegal move\n");
			else{
				uint8_t result = 0;
				if(start == '+') result = castle(1);
				else result = castle(-1);

				if(result){
					getAttacks();
					turn = !turn;
					addPrevPos();
					++turn_count;
				}
				else printf("Illegal move\n");
				if((check != 2) && !checkmate) printf("\nYOU ARE IN CHECK\n");
			}
		}
		else {
			end = getCoords('E');

			if(((board[start] & IS_WHITE) == 8) == turn){
				if(move()){
					turn = !turn;
					addPrevPos();
					++turn_count;
				}
				else printf("Illegal move\n");
			}
			else printf("Illegal move\n");
			if((check != 2) && !checkmate) printf("\nYOU ARE IN CHECK\n");
 		}
	}
	printBoard();
}
//---------------------------------------------*/


// checkmate is harder than you'd think

uint8_t endGame(){
	if(checkmate){
		printf("\nCHECKMATE\n");
		return 1;
	}
	if(stalemate()){
		printf("\nSTALEMATE\n");
		return 1;
	}
	if(threefold()){
		printf("\nTHREEFOLD REPITITION\n");
		return 1;
	}
	if(insufficient()){
		printf("\nINSUFFICIENT MATERIAL\n");
		return 1;
	}
	if(turn_count >= 100) printf("\nDRAW BY 50 MOVE RULE");
	return 0;
}


// two knights cannot checkmate on thier own

uint8_t insufficient(){
	uint8_t wbCount = 0;
	uint8_t bbCount = 0;
	uint8_t wnCount = 0;
	uint8_t bnCount = 0;

	for(uint8_t pos = 0; pos < 64; ++pos){
		switch(board[pos] & TYPE){
			case 1: return 0;
			case 2:	
				if(board[pos] & IS_WHITE) ++wnCount;
				else ++bnCount;
				break;
			case 3:
				if(board[pos] & IS_WHITE) ++wbCount;
				else ++bbCount;
				break;
			case 4: return 0;
			case 5: return 0;
		}
	}
	if((wbCount == 2) || (bbCount == 2)	|| (wbCount && wnCount) || (bbCount && bnCount)){
		return 0;
	}
	return 1;
}


// part of why i used move maps.  part. 

uint8_t stalemate(){
	if((white_move_map == 0) && (black_move_map == 0)) return 1;
	return 0;
}


// uses addPrevPos(), checks last 9 positions for repeats

uint8_t threefold(){
	uint8_t count = 1;
	
	for(uint8_t first = 0; first < 9; ++first){
		uint8_t samePos = 0;
		for(uint8_t second = 0; second < 64; ++second){
			if((previous_positions[first][second] & REMOVE_ATTACKS) != (board[second] & REMOVE_ATTACKS)){
				break;
			}
			else ++samePos;
		}
		count += (samePos == 64);
	}
	if(count > 3) return 1;
	
	return 0;
}


// add previous position to array, used for threefold repition

void addPrevPos(){
	for(uint8_t pre = 8; pre > 0; --pre){
		for(uint8_t pos = 0; pos < 64; ++pos){
			previous_positions[pre][pos] = previous_positions[pre-1][pos];
		}
	}
	for(uint8_t post = 0; post < 64; ++post){
		previous_positions[0][post] = board[post];
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]){
	testLoop();
//	testMoves();
//	initBoard();
	initDangerMap();
//	printDangerMap();
	getAttacks();
	printDangerMap();
}

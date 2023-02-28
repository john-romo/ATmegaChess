// engine.c

#include "headers/engine.h"


int8_t dangerMap[64];

uint8_t engine = 0;

int8_t good; // if engine plays white: good == 1; else: good == -1


void adjustDangerMap(uint8_t pos, uint8_t dest){
	if((board[pos] & IS_WHITE) != (engine & IS_WHITE)) good = (-good);
	int8_t pieceValue = getPieceValue(board[pos]);
	uint8_t add = (((dangerMap[dest] + pieceValue) < 128) && ((dangerMap[dest] + pieceValue) > -128));	
		
	dangerMap[dest] += (add * pieceValue);

	// last thing before return:
	if(engine & IS_WHITE) good = 1;
	else good = -1;
}


int8_t getPieceValue(uint8_t piece){
	switch(piece & TYPE){
		case PAWN: return 5 * good;
		case KNIGHT: return 15 * good;
		case BISHOP: return 16 * good;
		case ROOK: return 25 * good;
		case QUEEN: return 45 * good;
		case KING: return 127 * good;
	}
	return 0;
}


void printDangerMap(){
	printf("\nin\n");
	for(uint8_t pos = 0; pos < 64; ++pos){
		printf("|%d",dangerMap[pos]);
		if((pos%8) == 7) printf("\n");

	}
}


void initDangerMap(){
	engine += (IS_WHITE + ACTIVATED); // will eventually be chosen by user input

	if(engine & IS_WHITE) good = 1;
	else good = -1;


	dangerMap[27] = 5 * good;
	dangerMap[28] = 5 * good;
	dangerMap[35] = 5 * good;
	dangerMap[36] = 5 * good;

	dangerMap[18] = 3 * good;
	dangerMap[19] = 3 * good;
	dangerMap[20] = 3 * good;
	dangerMap[21] = 3 * good;

	dangerMap[26] = 3 * good;
	dangerMap[29] = 3 * good;
	dangerMap[34] = 3 * good;
	dangerMap[37] = 3 * good;

	dangerMap[42] = 3 * good;
	dangerMap[43] = 3 * good;
	dangerMap[44] = 3 * good;
	dangerMap[45] = 3 * good;

	dangerMap[49] = 1 * good;
	dangerMap[50] = 1 * good;
	dangerMap[51] = 1 * good;
	dangerMap[52] = 1 * good;
	dangerMap[53] = 1 * good;
	dangerMap[54] = 1 * good;

	dangerMap[9] = 1 * good;
	dangerMap[10] = 1 * good;
	dangerMap[11] = 1 * good;
	dangerMap[12] = 1 * good;
	dangerMap[13] = 1 * good;
	dangerMap[14] = 1 * good;

	dangerMap[17] = 1 * good;
	dangerMap[25] = 1 * good;
	dangerMap[33] = 1 * good;
	dangerMap[41] = 1 * good;

	dangerMap[22] = 1 * good;
	dangerMap[30] = 1 * good;
	dangerMap[38] = 1 * good;
	dangerMap[46] = 1 * good;

	for(uint8_t i = 0; i < 8; ++i){
		dangerMap[i] = 0;
		dangerMap[i+56] = 0;
		dangerMap[i*8] = 0;
		dangerMap[(i*8)+7] = 0;
	}
	
}

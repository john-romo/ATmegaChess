// board.c

#include<stdio.h>
#include<stdint.h>
#include "headers/defines.h"


void initBoard(){
	for(uint8_t  pos = 0; pos < 64; ++pos){
		board[pos] = SQUARE;
		if((pos < 24) || (pos > 39)){
			if(((pos < 16) || (pos > 47))){
				if(pos > 16)board[pos] += IS_WHITE;
				
				if(((pos > 7) && (pos < 16)) || ((pos < 56) && (pos > 47))) board[pos] += (PAWN + MOD);
				else if(((pos % 8) == 0) || ((pos % 8) == 7)) board[pos] += (ROOK + MOD);
				else if(((pos % 8) == 1) || ((pos % 8) == 6)) board[pos] += KNIGHT;
				else if(((pos % 8) == 2) || ((pos % 8) == 5)) board[pos] += BISHOP;
				else if((pos % 8) == 3) board[pos] += QUEEN;
				else board[pos] += (KING + MOD);
			}
		}
	}
}



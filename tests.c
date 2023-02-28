// test.c

#include<stdio.h>
#include<inttypes.h>
#include "headers/defines.h"



void printBoard(){
	printf("\n");
	
	if(turn) printf("    WHITE TO PLAY\n\n");
	else printf("    BLACK TO PLAY\n\n");

	for(uint8_t  pos = 0; pos < 64; ++pos){
		if(pos % 8 == 0){
			for(uint8_t i = 0; i < 8; ++i){
				if(i == 0) printf("  ");
				printf("|-");
				if(i == 7) printf("|\n");
			}
		}

		if(pos %8 == 0) printf("%d ", 8-(pos/8));
		//printf("|%d", ((board[pos] & IS_WHITE) == 8) + (board[pos] & TYPE));
		//printf("|%d", board[pos]);
		switch (board[pos] & TYPE){
			case 0:
				printf("| ");
				break;
			case 1: 
				if(board[pos] & IS_WHITE) printf("|P");
				else printf("|p");
				break;
			case 2: 
				if(board[pos] & IS_WHITE) printf("|N");
				else printf("|n");
				break;
			case 3: 
				if(board[pos] & IS_WHITE) printf("|B");
				else printf("|b");
				break;
			case 4: 
				if(board[pos] & IS_WHITE) printf("|R");
				else printf("|r");
				break;
			case 5: 
				if(board[pos] & IS_WHITE) printf("|Q");
				else printf("|q");
				break;
			case 6: 
				if(board[pos] & IS_WHITE) printf("|K");
				else printf("|k");
				break;
		}

		if((pos % 8 == 7) && pos) {
			printf("|\n");
		}
	}
	printf("\n   A B C D E F G H\n\n");
}


void printAttackBoard(){
	printf("\n");
	for(int i = 0; i < 64; ++i){
		if(i % 8 == 0){
			for(uint8_t j = 0; j < 8; ++j){
				printf("|-");
				if(j == 7) printf("|\n");
			}
		}
		uint8_t value = ((board[i] & ATTACKED_BY_WHITE) != 0) + (2*(((board[i]) & ATTACKED_BY_BLACK) != 0));
		if(value) printf("|%d", value);
		else printf("| ");
		if((i % 8) == 7) printf("|\n");
	}
}


void printMoveMap(uint8_t color){
	printf("\n");
	for(int i = 0; i < 64; ++i){
		if(i % 8 == 0){
			for(uint8_t j = 0; j < 8; ++j){
				printf("|-");
				if(j == 7) printf("|\n");
			}
		}
		uint64_t map = white_move_map;
		if(!color) map = black_move_map;

		//printf("%" PRIu64 "\n", map);

		uint8_t value = ((map & (1ull << i)) != 0);	
		if(value) printf("|%d", value);
		
		else printf("| ");
		if((i % 8) == 7) printf("|\n");
	}
}


void testLegalMove(uint8_t valid){
	printf("Test Legal Move: %d\n\n", valid);
}

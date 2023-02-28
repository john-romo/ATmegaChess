// moves.c

#include "headers/moves.h"

uint8_t ranged_check[7];
uint8_t en_passant = 0;
uint8_t white_king_moves = 0;
uint8_t black_king_moves = 0;


///////////////////////////////////////////////////  MOVEMENT HANDLERS  /////////////////////////////////////////////


// move() is the primary entrance point from chess.c
// returns to chess.c true if move is valid

uint8_t move(){
	uint8_t startPiece = board[start];
	uint8_t endPiece = board[end];
	
	// if move not in legal move list, return 0;
	if(!getAttacks()) return 0;
	board[end] = (board[start]);
	board[start] = 0;
	
	// promote pawn if applicable
	if(((board[end] & TYPE) == PAWN)
		&& (((board[end] & IS_WHITE) && ((end / 8) == 0 ))  
		|| (!(board[end] & IS_WHITE) && ((end / 8) == 7)))){
			promote();
		}
	
	// if en passant move played, perform en passant move
	if(en_passant) enPassant();

	// make sure move doesn't leave king in check
	getAttacks();

	// undo moves if king still in check
	if(check ==  turn){
		if(en_passant) undoEnPassant(startPiece, endPiece);
		else{
			board[start] = startPiece;
			board[end] = endPiece;
			return 0;
		}
	}

	// check if checkmate deliverd
	if((check == !turn) && checkCheckmate()){
		checkmate = 1;
	}
	
	// if rook or king moved, remove castling option
	if(((board[end] & TYPE) == KING)
		|| ((board[end] & TYPE) == ROOK)) board[end] &= ~MOD;
	
	if((board[end] & PAWN) || (endPiece & TYPE)) turn_count = 0;
	return 1;
}


// uses attack() to cycle through all pieces to get all possible moves
// returns sendMove to move(); 

uint8_t getAttacks(){
	removeAttacks();
	uint8_t whiteKing;
	uint8_t blackKing;

	uint8_t sendMove = 0;
	
	for(uint8_t pos = 0; pos < 64; ++pos){
		// get king attaks
		if(board[pos] & TYPE) sendMove |= attack(pos);
	
		// get king positions
		if((board[pos] & TYPE) == KING){
			if(board[pos] & IS_WHITE) whiteKing = pos;
			else blackKing = pos;
		}
	}
	
	// get king moves
	// put in conditional so testing doesnt show weird results
	if((whiteKing != 0 ) && (blackKing != 0)){
		sendMove |= kingAttack(whiteKing, blackKing);	
	}	

	return sendMove;
}


// remove attack modifiers from squares / pieces

void removeAttacks(){
	white_move_map = 0;
	black_move_map = 0;
	white_king_moves = 0;
	black_king_moves = 0;
	check = 2;

	for(uint8_t pos = 0; pos < 64; ++pos){
		board[pos] &= REMOVE_ATTACKS;
	}
}


// gets all possible moves from the given piece (not kings)
// returns sendMove to getAttacks();

uint8_t  attack(uint8_t pos){
	uint8_t sendMove = 0;
	switch(board[pos] & TYPE){
		case PAWN: 
			sendMove = pawnAttack(pos);
			break; 
		case KNIGHT: 
			sendMove = knightAttack(pos);
			break; 
		case BISHOP:	
			sendMove = bishopAttack(pos);
			break;
		case ROOK: 
			sendMove = rookAttack(pos);
			break;
		case QUEEN: 
			sendMove = rookAttack(pos);
			sendMove |= bishopAttack(pos);
			break;
	}		
	return sendMove;
}


///////////////////////////////////////////////////  PIECE ATTACKS  /////////////////////////////////////////////


////////  PAWN  ////////

// check move handled internally, goes directly to getMove() 
// en_passant handled partially internally, partially in getMove();
// sendMove -> true if move checked is equal to start and end
// returns sendMove to attack();;

uint8_t pawnAttack(uint8_t pos){
	if((board[pos] & IS_WHITE) == turn) board[pos] &= ~MOD;

	uint8_t sendMove = 0;
	
	int8_t direction = 1;
	if(board[pos] & IS_WHITE) direction = -1;
	
	// single jump
	if(!(board[pos + (direction * 8)] & TYPE)){
		sendMove |= getMove(pos, pos + (direction * 8));		
		
		// double jump
		if(!((board[pos + (direction * 16)]) & TYPE) && (((board[pos] & IS_WHITE) && ((pos/8) == 6))
			|| (!(board[pos] & IS_WHITE) && ((pos/8) == 1)))){
			board[pos] |= MOD;
			sendMove |= getMove(pos, pos + (direction * 16));
		}
	}
		
		
	// attacking moves
	if((pos % 8) != 0){

		if(((board[pos-1] & TYPE) == PAWN) && (board[pos-1] & MOD) 
			&& ((board[pos-1] & IS_WHITE) != (board[pos] & IS_WHITE))){
			// enP -
			en_passant = 1;
			attackSquare(pos, pos - 1);
			if(board[pos] & IS_WHITE) sendMove |= getMove(pos, pos - 9);
			else sendMove |= getMove(pos, pos + 7);
		}
		
		if(board[pos] & IS_WHITE){
			// white cap
			attackSquare(pos, pos - 9);	
			if((board[pos-9] & TYPE) && !(board[pos-9] & IS_WHITE)) sendMove |= getMove(pos, pos - 9);
		}
		
		if(!(board[pos] & IS_WHITE)){
			// black cap
			attackSquare(pos, pos + 7);	
			if((board[pos+7] & TYPE) && (board[pos+7] & IS_WHITE)) sendMove |= getMove(pos, pos + 7);
		}
	}
	if((pos % 8) != 7){
		
		if(((board[pos+1] & TYPE) == PAWN) && (board[pos+1] & MOD) 
			&& ((board[pos+1] & IS_WHITE) != (board[pos] & IS_WHITE))){
			// enp +
			en_passant = 1;
			attackSquare(pos, pos + 1);	
			if(board[pos] & IS_WHITE) sendMove |= getMove(pos, pos - 7);
			else sendMove |= getMove(pos, pos + 9);
		}
		
		if(board[pos] & IS_WHITE){
			// white cap
			attackSquare(pos, pos - 7);	
			if((board[pos-7] & TYPE) && !(board[pos-7] & IS_WHITE)) sendMove |= getMove(pos, pos - 7);
		}
		
		if(!(board[pos] & IS_WHITE)){
			// black cap
			attackSquare(pos, pos + 9);	
			if((board[pos+9] & TYPE) && (board[pos+9] & IS_WHITE)) sendMove |= getMove(pos, pos + 9);
		}
	}
	
	return sendMove;
}


////////  KNIGHT  ////////

// sends possible moves to checkMove(); returns sendMove to attack();

uint8_t  knightAttack(uint8_t pos){
	uint8_t sendMove = 0;
				
	int8_t lesserJumps[] = {6,15,-10,-17};
	int8_t greaterJumps[] = {-6,-15,10,17};

	for(uint8_t j = 0; j < 4; ++j){
		if(((pos + lesserJumps[j]) > -1) && ((pos + lesserJumps[j]) < 64)){
			if(((pos + lesserJumps[j]) % 8) < (pos % 8)){
				sendMove |= checkMove(pos, pos + lesserJumps[j]);
			}
		}
		if(((pos + greaterJumps[j]) > -1) && ((pos + greaterJumps[j]) < 64)){
			if(((pos + greaterJumps[j]) % 8) > (pos % 8)){
				attackSquare(pos, pos + greaterJumps[j]);
				sendMove |= checkMove(pos, pos + greaterJumps[j]);
			}
		}
		
	}
	return sendMove;
}


////////  BISHOP  ////////

// sends possible moves to checkMove() and if attacks enemy king, to setRangedCheck();
// returns sendMove to attack();

uint8_t bishopAttack(uint8_t pos){
	uint8_t sendMove = 0;

	int8_t direction[] = {-7, 9, 7, -9};
	
	for(uint8_t d = 0; d < 4; ++d){
		int8_t select = pos + direction[d];
		uint8_t bounds = ((select % 8) < (pos % 8));
		uint8_t checkingLine[] = {pos,0,0,0,0,0,0};
		uint8_t index = 1;

		while((select > -1) && (select < 64)){
			if((((pos%8) == 7) || (select%8 == 0)) && (d < 2)) break;
			if((((pos%8) == 0) || (select%8 == 7)) && (d > 1)) break;
						
			sendMove |= checkMove(pos, select);	

			if(board[select] & TYPE){
				if(((board[select] & TYPE) == KING) && ((board[select] & IS_WHITE) != ((board[pos] & IS_WHITE)))){
					setRangedCheck(checkingLine);
				}
				break;
			}
			
			if(index < 7) checkingLine[index++] = select;
			select += direction[d];
		}
		
	}
	return sendMove;
}


////////  ROOK  ////////

// sends possible moves to checkMove() and if attacks enemy king, to setRangedCheck(); 
// returns sendMove to attack();

uint8_t  rookAttack(uint8_t pos){
	uint8_t sendMove = 0;

	int8_t direction[] = {1, -1, 8, -8};

	for(uint8_t d = 0; d < 4; ++d){
		uint8_t index = 1;
		uint8_t checkingLine[] = {pos,0,0,0,0,0,0};
		int8_t  select = pos + direction[d];

        uint8_t  mod = pos%8;

		while((select > -1) && (select < 64)){
			if(d < 2){
				mod += (direction[d]*((pos % 8) != (select % 8)));
			}
			if((mod < 0) || (mod > 7)) break;

			sendMove |= checkMove(pos, select);
			
			if(board[select] & TYPE){
				if(((board[select] & TYPE) == KING) && ((board[select] & IS_WHITE) != ((board[pos] & IS_WHITE)))){
					setRangedCheck(checkingLine);
				}
				break;
			}
			if(index < 7) checkingLine[index++] = select;
			select += direction[d];
		}
	}
	return sendMove;
}


////////  KING  ////////

// checks both kings 'simultaniously', so that kings
// cant't move to squares attacked by enemy kings;
// bypasses move validation,  returns sendMove to getAttacks();

uint8_t  kingAttack(uint8_t whitePos, uint8_t blackPos){
	uint8_t sendMove = 0;
	
	uint8_t tempWhite = 0;	// temporory king move maps
	uint8_t tempBlack = 0;
	
	int8_t direction[] = {-9, -1, 7, -8, 8, -7, 1, 9}; // directions king cam move

	// get legal moves for both kings, ignoring the enemy king's attacked squares
	for(uint8_t king = 0; king < 2; ++king){
		uint8_t* tempMoves;
		uint8_t pos;
		if(king){
			tempMoves = &tempWhite;
			pos = whitePos;
		}
		else{
			tempMoves = &tempBlack;
			pos = blackPos;
		}

		for(uint8_t  d = 0; d < 8; ++d){
			if((pos + direction[d]) < 0) continue;
			if((pos + direction[d]) > 63) continue;
			if(((pos%8) == 0) && (d < 3)) continue;
			if(((pos%8) == 7) && (d > 4)) continue;

			attackSquare(pos, pos+direction[d]);
				
			if(((board[pos] & IS_WHITE) && !(board[pos + direction[d]] & ATTACKED_BY_BLACK))
				|| (!(board[pos] & IS_WHITE) && !(board[pos + direction[d]] & ATTACKED_BY_WHITE))){
	
				if(!(board[pos + direction[d]] & TYPE) || ((board[pos] & IS_WHITE) != (board[pos + direction[d]] & IS_WHITE))){
					*tempMoves |= (1u << d);
				}
			}
		}
	}
	
	// remove from king's move maps any identical moves
	for(uint8_t white = 0; white < 8; ++white){
		if(!(tempWhite & (1u << white))) continue;
	
		for(uint8_t black = 0; black < 8; ++black){
			if(!(tempBlack & (1u << black))) continue;
			
			if((whitePos + direction[white]) == (blackPos + direction[black])){
				tempWhite &= ~(1u << white);
				tempBlack &= ~(1u << black);
			}
		}
	}
	
	// add legal king moves to attack/move maps 
	for(uint8_t king = 0; king < 2; ++king){
		uint8_t tempMoves;
		uint8_t pos;
		if(king){
			tempMoves = tempWhite;
			pos = whitePos;
		}
		else{
			tempMoves = tempBlack;
			pos = blackPos;
		}
		for(uint8_t d = 0; d < 8; ++d){
			if(tempMoves & (1u << d)){
				sendMove |=	getMove(pos, pos + direction[d]); 
				if (board[pos] & IS_WHITE) white_king_moves |= (1u << d);
				else black_king_moves |= (1u << d);
			}
		}
	}
	return sendMove;
}


///////////////////////////////////////////////////  MOVE VALIDATION  /////////////////////////////////////////////


// sets the attacked square/piece's attack modifiers. if king attacked, king in check

void  attackSquare(uint8_t pos, uint8_t dest){
	if(board[pos] & IS_WHITE) board[dest] |= ATTACKED_BY_WHITE;
	else board[dest] |= ATTACKED_BY_BLACK;
	
	if(((board[dest] & TYPE) == KING) && ((board[pos] & IS_WHITE) != (board[dest] & IS_WHITE))){
		board[pos] |= ATTACKS_KING;
		board[dest] |= ATTACKS_KING;
		if(board[dest] & IS_WHITE) check = 1;
		if(!(board[dest] & IS_WHITE)) check = 0;
	}
	if(engine & ACTIVATED){
		if((board[pos] & TYPE) == KING) adjustDangerMap(pos,dest);
	}
}


// runs attackSquare(), checks if move is legal
// if move is legal, returns getMove();
// returns to piece's move handler

uint8_t checkMove(uint8_t pos, uint8_t dest){
	
	attackSquare(pos, dest);
		
	if((!(board[dest] & TYPE)
	    || ((board[pos] & IS_WHITE) != (board[dest] & IS_WHITE)))){
		return getMove(pos,dest);
	}
	return 0;
}


// create move maps, handles 'en passant cancel' for pawns
// return's true if start and end is equal to sent positions
// returns to checkMove();

uint8_t getMove(uint8_t pos, uint8_t dest){
	if(board[pos] & IS_WHITE) white_move_map |= (1ull << dest);
	else black_move_map |= (1ull << dest);
	
	if(en_passant && ((start == pos) && (end == dest))){
		en_passant = 1;
		return 1;
	}
	
	return ((start == pos) && (end == dest));

}


///////////////////////////////////////////////////  SPECIAL MOVES  /////////////////////////////////////////////


// performs castle move and returns 1 if legal, 0 if illegal. 
// direction = 1: kingside, direction = -1: queenside;

uint8_t castle(int8_t d){
	uint8_t king = (4 + (turn * 56));
	uint8_t rook;
	if(d == 1) rook = (7 + (turn * 56));
	else rook = (turn * 56);
	
	if(!((board[king] & MOD) && (board[rook] & MOD))) return 0;

	for(int8_t i = ((king * d) + 1); i < (rook * d); ++i){ // <----- nice
		if(board[i*d] & TYPE) return 0;
		if((board[king] & IS_WHITE) && (board[i*d] & ATTACKED_BY_BLACK)) return 0;
		if(!(board[king] & IS_WHITE) && (board[i*d] & ATTACKED_BY_WHITE)) return 0;
	}
	
	uint8_t color = 0;
	if(board[king] & IS_WHITE) color = IS_WHITE;

	board[king] = 0;
	board[rook] = 0;
	board[king+d] = ROOK + color;
	board[king+(2*d)] = KING + color;

	return 1;
}


// if en passant move was made and is legal, removes captured pawn 

void enPassant(){
	en_passant = 0;
	if(board[end] & IS_WHITE) board[end+8] = 0;
	else board[end-8] = 0;
}


// if after en passant still in check, resets positions

void undoEnPassant(uint8_t startPiece, uint8_t endPiece){
	board[start] = startPiece;
	if(board[end] & IS_WHITE) board[end + 8] = endPiece;
	else board[end-8] = endPiece;
	board[end] = 0;
}

// promotion move

void promote(){
	// full implementation depends on mcu interface code
	board[end] = QUEEN + (board[end] & IS_WHITE);
}



///////////////////////////////////////////////////   CHECK CHECKMATE  /////////////////////////////////////////////


// king move order: -9,-1,7,-8,8,-7,1,9
// check -> white in check: 1, black: 0, none: 2
// uses _move_map, _king_moves, and  ranged_check[]

int8_t checkCheckmate(){
	if(!check && turn && (black_king_moves != 0)) return 0;
	if((check == 1) && !turn && (white_king_moves != 0)) return 0;

	uint64_t moveMap = white_move_map;
	if(turn) moveMap = black_move_map;

	for(uint8_t pos = 0; pos < 64; ++pos){
		if(board[pos] & ATTACKS_KING){
			if((moveMap >> pos) & 1ull) return 0;
			for(int i = 0; i < 7; ++i){
				if((moveMap >> ranged_check[i]) & 1ull) return 0;
			}
		}
	}
	return 1;
}


// if ranging piece attacks king, sets rangedAttack
// for use with checkCheckmate();

void setRangedCheck(uint8_t checkingLine[7]){
	for(uint8_t i = 0; i < 7; ++i){
		ranged_check[i] = checkingLine[i];
	}
}

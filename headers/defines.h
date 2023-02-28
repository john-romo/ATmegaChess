// defines.h 

// pieces/squares:  0    0    0    0     0    000  
//                 mod  attK  attw attb color  type

// mod: (pawn -> en-passant),(k+r -> castle)
// attK (king -> check), (other pieces -> attacks the enemy king)


// engine:	0	0	 0	0	 0	  0	 0	 0
//                         color		activated  


#ifndef DEFINES_t

#define DEFINES_H_

// pieces 
#define SQUARE 0
#define PAWN 0b00000001
#define KNIGHT 0b00000010
#define BISHOP 0b00000011
#define ROOK 0b00000100
#define QUEEN 0b00000101
#define KING 0b00000110

//mods(can be treated like masks) 
#define MOD 0b10000000
#define ATTACKS_KING 0b01000000
#define ATTACKED_BY_BLACK 0b00100000
#define ATTACKED_BY_WHITE 0b00010000
#define IS_WHITE 0b00001000

// masks
#define REMOVE_ATTACKS 0b10001111
#define TYPE 0b00000111
#define ACTIVATED 00000001


extern uint8_t board[64];
extern uint8_t previous_positions[9][64];

extern uint8_t start;
extern uint8_t end;

extern uint8_t turn;
extern uint8_t turn_count;

extern uint8_t checkmate;
extern uint8_t check;

extern uint64_t white_move_map;
extern uint64_t black_move_map;

#endif

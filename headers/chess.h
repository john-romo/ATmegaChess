// chess.h

#include <stdio.h>
#include <inttypes.h>
#include "defines.h"
#include "board.h"
#include "moves.h"
#include "engine.h"


extern uint8_t engine;

uint8_t endGame();

uint8_t insufficient();

uint8_t stalemate();

uint8_t threefold();

void addPrevPos();

void adjustDangerMap(uint8_t pos, uint8_t dest);

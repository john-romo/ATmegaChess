// engine.h

#ifndef ENGINE_H_
#define EINGE_H_

#include <stdio.h>
#include <inttypes.h>
#include "defines.h"
#include "board.h"

extern int8_t dangerMap[64];

void adjustDangerMap(uint8_t pos, uint8_t dest);

void initDangerMap();

void printDangerMap();

int8_t getPieceValue(uint8_t piece);

extern uint8_t engine;

#endif

// moves.h

#ifndef MOVES_H_
#define MOVES_H_

#include <stdio.h>
#include <stdint.h>
#include "defines.h"
#include "engine.h"


// movement handlers

uint8_t move();

uint8_t getAttacks();

void removeAttacks();

uint8_t attack(uint8_t pos);


// piece attacks

uint8_t pawnAttack(uint8_t pos);

uint8_t knightAttack(uint8_t pos);

uint8_t bishopAttack(uint8_t pos);

uint8_t rookAttack(uint8_t pos);

uint8_t kingAttack(uint8_t whitePos, uint8_t blackPos);


// movement validation

void attackSquare(uint8_t pos, uint8_t dest);

uint8_t checkMove(uint8_t pos, uint8_t dest);

uint8_t getMove(uint8_t pos, uint8_t dest);


// special moves

uint8_t castle(int8_t d);

void enPassant();

void undoEnPassant(uint8_t startPiece, uint8_t endPiece);

void promote();


// check checkmate

int8_t checkCheckmate();

void setRangedCheck(uint8_t checkingLine[7]);


#endif

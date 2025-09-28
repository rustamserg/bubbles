#ifndef _BOARD_H_
#define _BOARD_H_

#include "raylib.h"

#define BOARD_SIZE_WIDTH 8
#define BOARD_SIZE_HEIGHT 8
#define BOARD_CELL_SIZE 80
#define BOARD_CELL_TYPES 8

struct Bubble;
struct Game;

typedef struct Board
{
	Color colors[BOARD_CELL_TYPES];
	struct Bubble* cells[BOARD_SIZE_WIDTH][BOARD_SIZE_HEIGHT];

	int last_update_h;
	int last_update_w;

	void (*fnDraw)(struct Board*, int, int);
	bool (*fnUpdate)(struct Board*, struct Game*);

	bool (*fnAddBubble)(struct Board*);
	void (*fnMoveBubble)(struct Board*, Vector2, Vector2);
	struct Bubble* (*fnTryGetBubble)(struct Board*, Vector2);
} Board;

Board* BoardCreate();
void BoardDestroy(Board* board);

#endif // _BOARD_H_
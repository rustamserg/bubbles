#ifndef _BOARD_H_
#define _BOARD_H_

#include "game.h"
#include "raylib.h"

struct Bubble;
struct Game;

typedef struct Board
{
	Color colors[BOARD_CELL_TYPES];
	struct Bubble* cells[BOARD_SIZE_WIDTH][BOARD_SIZE_HEIGHT];

	int added_bubbles;
	int last_update_h[NUM_BUBBLES_TO_ADD];
	int last_update_w[NUM_BUBBLES_TO_ADD];
	int free_cells_count;

	void (*fnDraw)(struct Board*, int, int);
	bool (*fnUpdate)(struct Board*, struct Game*);

	bool (*fnAddBubble)(struct Board*);
	void (*fnMoveBubble)(struct Board*, Vector2, Vector2);
	struct Bubble* (*fnTryGetBubble)(struct Board*, Vector2);
} Board;

Board* BoardCreate();
void BoardDestroy(Board* board);

#endif // _BOARD_H_
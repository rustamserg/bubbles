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

	int pathfind_current_step;
	float pathfind_step_time;
	Vector2 pathfind_path[BOARD_SIZE_WIDTH * BOARD_SIZE_HEIGHT];
	int pathfind_cells[BOARD_SIZE_WIDTH + 2][BOARD_SIZE_HEIGHT + 2];

	int added_bubbles;
	int destroyed_bubbles;
	int last_update_h[NUM_BUBBLES_TO_ADD];
	int last_update_w[NUM_BUBBLES_TO_ADD];
	int free_cells_count;

	void (*fnDraw)(struct Board*);
	bool (*fnUpdate)(struct Board*, struct Game*);

	Color (*fnGetNextColor)(struct Board*);
	bool (*fnAddBubble)(struct Board*, Color);
	bool (*fnTryMoveBubble)(struct Board*, Vector2, Vector2);
	bool (*fnTrySwapBubbles)(struct Board*, Vector2, Vector2);
	struct Bubble* (*fnTryGetBubble)(struct Board*, Vector2);
} Board;

Board* BoardCreate();
void BoardDestroy(Board* board);

#endif // _BOARD_H_
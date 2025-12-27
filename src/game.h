#ifndef _GAME_H_
#define _GAME_H_

#include "raylib.h"

#define DRAW_OFFSET_X 80
#define DRAW_OFFSET_Y 200

#define BOARD_SIZE_WIDTH 14
#define BOARD_SIZE_HEIGHT 8
#define BOARD_CELL_SIZE 80
#define BOARD_CELL_TYPES 8

#define MIN_MATCHES_IN_ROW 5
#define NUM_BUBBLES_TO_ADD 3
#define ADD_ANIMATION_TIME 0.3f

struct Board;
struct Dummy;

enum Turn
{
	TURN_AI,
	TURN_PLAYER
};

typedef struct Game
{
	enum Turn turn;
	struct Board* board;
	struct Dummy* ai;
	struct Dummy* player;

	int min_matches;
	int score;

	Color next_colors[NUM_BUBBLES_TO_ADD];

	void (*fnUpdate)(struct Game*);
	void (*fnDraw)(struct Game*);
} Game;

void GameInit(Game* game);


#endif // _GAME_H_
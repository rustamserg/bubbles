#ifndef _GAME_H_
#define _GAME_H_

#include "raylib.h"

#define VIRTUAL_SCREEN_WIDTH 1600
#define VIRTUAL_SCREEN_HEIGHT 900

#define DRAW_OFFSET_X 80
#define DRAW_OFFSET_Y 200

#define BOARD_SIZE_WIDTH 14
#define BOARD_SIZE_HEIGHT 8
#define BOARD_CELL_SIZE 80
#define BOARD_CELL_TYPES 8

#define BASE_SCORE 5
#define MIN_MATCHES_IN_ROW 5
#define NUM_BUBBLES_TO_ADD 3

#define BUBBLE_ADD_ANIMATION_TIME 0.3f
#define BUBBLE_MOVE_ANIMATION_TIME 0.01f

struct Board;
struct AI;
struct Player;
struct UI;

enum Turn
{
	TURN_AI,
	TURN_PLAYER
};

enum State
{
	STATE_GAME,
	STATE_END
};

typedef struct ScoreDef
{
	int base_score;
	int multiplayer;
	const char* message;
} ScoreDef;

typedef struct Game
{
	enum State state;
	enum Turn turn;
	struct Board* board;
	struct AI* ai;
	struct Player* player;
	struct UI* ui;

	int min_matches;
	int current_strike;
	int total_score;
	ScoreDef score_ladder[5];

	Color next_colors[NUM_BUBBLES_TO_ADD];

	void (*fnNewGame)(struct Game*);
	void (*fnUpdate)(struct Game*);
	void (*fnDraw)(struct Game*);
} Game;

void GameInit(Game* game);


#endif // _GAME_H_
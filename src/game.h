#ifndef _GAME_H_
#define _GAME_H_

#define DRAW_OFFSET_X 50
#define DRAW_OFFSET_Y 50

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

	void (*fnUpdate)(struct Game*);
	void (*fnDraw)(struct Game*);
} Game;

void GameInit(Game* game);


#endif // _GAME_H_
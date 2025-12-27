#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "raylib.h"
#include <stdbool.h>

struct Game;

typedef struct Player
{
	Vector2 from;
	bool is_turn_end;

	bool (*fnUpdate)(struct Player*, struct Game*);
} Player;

Player* PlayerCreate();

#endif // _PLAYER_H_
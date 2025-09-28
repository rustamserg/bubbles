#include "dummy.h"
#include "board.h"
#include "game.h"

#include "raylib.h"

#include <stdlib.h>


static bool Update(Dummy* dummy, Game* game)
{
	const bool is_board_filled = !game->board->fnAddBubble(game->board);
	if (is_board_filled)
	{
		BoardDestroy(game->board);
		game->board = BoardCreate();
	}
	return true;
}

Dummy* DummyCreateAI()
{
	Dummy* ai = (Dummy*)malloc(sizeof(Dummy));

	if (ai != NULL)
	{
		ai->fnUpdate = Update;
	}
	return ai;
}

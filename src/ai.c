#include "dummy.h"
#include "board.h"
#include "game.h"

#include <malloc.h>


static bool Update(Dummy* dummy, Game* game)
{
	int to_add = NUM_BUBBLES_TO_ADD;
	bool is_board_filled = false;

	while (to_add-- && !is_board_filled)
	{
		is_board_filled = !game->board->fnAddBubble(game->board);
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

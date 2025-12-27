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
		Color clr = game->next_colors[to_add];
		is_board_filled = !game->board->fnAddBubble(game->board, clr);
	}

	if (!is_board_filled)
	{
		for (to_add = 0; to_add < NUM_BUBBLES_TO_ADD; ++to_add)
		{
			Color clr = game->board->fnGetNextColor(game->board);
			game->next_colors[to_add] = clr;
		}
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

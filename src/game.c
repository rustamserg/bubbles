#include "game.h"
#include "board.h"
#include "dummy.h"

#include "raylib.h"

#define MIN_MATCHES_IN_ROW 5


static void Update(Game* game)
{
	bool end_turn = game->board->fnUpdate(game->board, game);
	if (end_turn)
	{
		if (TURN_AI == game->turn)
		{
			end_turn = game->ai->fnUpdate(game->ai, game);
			if (end_turn)
			{
				if (0 == game->board->free_cells_count)
				{
					// TODO: game end here
					BoardDestroy(game->board);
					game->board = BoardCreate();
				}
				else
				{
					game->turn = TURN_PLAYER;
				}
			}
		}
		else if (TURN_PLAYER == game->turn)
		{
			end_turn = game->player->fnUpdate(game->player, game);
			if (end_turn)
			{
				game->turn = TURN_AI;
			}
		}
	}
}

static void Draw(Game* game)
{
	game->board->fnDraw(game->board, DRAW_OFFSET_X, DRAW_OFFSET_Y);
}

void GameInit(Game* game)
{
	game->turn = TURN_AI;
	game->min_matches = MIN_MATCHES_IN_ROW;

	game->board = BoardCreate();
	game->ai = DummyCreateAI();
	game->player = DummyCreatePlayer();

	game->fnDraw = Draw;
	game->fnUpdate = Update;
}
#include "game.h"
#include "board.h"
#include "dummy.h"


static void Update(Game* game)
{
	bool end_turn = game->board->fnUpdate(game->board, game);
	if (end_turn)
	{
		if (game->board->destroyed_bubbles > 0)
		{
			if (game->board->destroyed_bubbles == game->min_matches)
			{
				game->score += 10;
			}
			else if (game->board->destroyed_bubbles > game->min_matches)
			{
				game->score += 15;
			}

			if (game->board->free_cells_count < BOARD_SIZE_WIDTH * BOARD_SIZE_HEIGHT)
			{
				game->turn = TURN_PLAYER;
			}
		}

		if (TURN_AI == game->turn)
		{
			end_turn = game->ai->fnUpdate(game->ai, game);
			if (end_turn)
			{
				if (0 == game->board->free_cells_count)
				{
					// TODO: game end here
					game->score = 0;
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

	// draw hud
	DrawText(TextFormat("Score: %06i", game->score), 800, 110, 30, RED);

	int x_pos = 260;
	float radius = BOARD_CELL_SIZE / 2 - 10;
	for (int i = 0; i < sizeof(game->next_colors) / sizeof(game->next_colors[0]); ++i)
	{
		DrawCircle((int)(x_pos + i * 2 * (radius + 10)), (int)(BOARD_CELL_SIZE / 2 + 20), radius, game->next_colors[i]);
	}
	DrawText("Next bubbles", 240, 110, 30, BLUE);
}

static void Draw(Game* game)
{
	game->board->fnDraw(game->board, DRAW_OFFSET_X, DRAW_OFFSET_Y);
}

void GameInit(Game* game)
{
	game->score = 0;

	game->turn = TURN_AI;
	game->min_matches = MIN_MATCHES_IN_ROW;

	game->board = BoardCreate();
	game->ai = DummyCreateAI();
	game->player = DummyCreatePlayer();

	for (int i = 0; i < sizeof(game->next_colors) / sizeof(game->next_colors[0]); ++i)
	{
		game->next_colors[i] = game->board->fnGetNextColor(game->board);
	}

	game->fnDraw = Draw;
	game->fnUpdate = Update;
}
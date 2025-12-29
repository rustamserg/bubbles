#include "game.h"
#include "board.h"
#include "player.h"
#include "ai.h"
#include "ui.h"


static void ProcessScore(Game* game)
{
	int ladder_id = 0;
	const int max_ladder_id = sizeof(game->score_ladder) / sizeof(game->score_ladder[0]) - 1;

	while (ladder_id <= max_ladder_id)
	{
		if (game->board->destroyed_bubbles <= game->score_ladder[ladder_id].destroyed_bubbles)
		{
			break;
		}
		++ladder_id;
	}
	ladder_id = (ladder_id <= max_ladder_id) ? ladder_id : max_ladder_id;

	game->total_score += game->score_ladder[ladder_id].base_score * game->score_ladder[ladder_id].multiplayer;

	game->ui->fnAddScoreMessage(game->ui, &game->score_ladder[ladder_id]);
}

static void Update(Game* game)
{
	bool end_turn = game->board->fnUpdate(game->board, game);
	if (end_turn)
	{
		if (game->board->destroyed_bubbles > 0)
		{
			ProcessScore(game);

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
					game->total_score = 0;
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

	game->ui->fnUpdate(game->ui, game);
}

static void Draw(Game* game)
{
	game->board->fnDraw(game->board);
	game->ui->fnDraw(game->ui, game);
}

void GameInit(Game* game)
{
	game->total_score = 0;
	game->score_ladder[0] = (ScoreDef){ MIN_MATCHES_IN_ROW, BASE_SCORE, 1, "GOOD" };
	game->score_ladder[1] = (ScoreDef){ MIN_MATCHES_IN_ROW + 1, (int)(BASE_SCORE + 0.5f * BASE_SCORE), 1, "VERY GOOD" };
	game->score_ladder[2] = (ScoreDef){ 2 * MIN_MATCHES_IN_ROW, BASE_SCORE, 2, "AWESOME" };
	game->score_ladder[3] = (ScoreDef){ 3 * MIN_MATCHES_IN_ROW, BASE_SCORE, 10, "EXCELLENT" };
	game->score_ladder[4] = (ScoreDef){ 4 * MIN_MATCHES_IN_ROW, BASE_SCORE, 100, "AMAZING" };

	game->turn = TURN_AI;
	game->min_matches = MIN_MATCHES_IN_ROW;

	game->board = BoardCreate();
	game->ai = AICreate();
	game->player = PlayerCreate();
	game->ui = UICreate();

	for (int i = 0; i < sizeof(game->next_colors) / sizeof(game->next_colors[0]); ++i)
	{
		game->next_colors[i] = game->board->fnGetNextColor(game->board);
	}

	game->fnDraw = Draw;
	game->fnUpdate = Update;
}
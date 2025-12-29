#include "game.h"
#include "board.h"
#include "player.h"
#include "ai.h"
#include "ui.h"


static void ProcessScore(Game* game)
{
	int ladder_id = game->current_strike;
	const int max_ladder_id = sizeof(game->score_ladder) / sizeof(game->score_ladder[0]) - 1;

	ladder_id = (ladder_id <= max_ladder_id) ? ladder_id : max_ladder_id;

	int added_score = (game->board->destroyed_bubbles * game->score_ladder[ladder_id].base_score) * game->score_ladder[ladder_id].multiplayer;
	game->total_score += added_score;

	game->ui->fnAddScoreMessage(game->ui, &game->score_ladder[ladder_id], added_score);
}

static void NewGame(Game* game)
{
	game->state = STATE_GAME;
	game->turn = TURN_AI;
	game->total_score = 0;
	game->current_strike = 0;
	BoardDestroy(game->board);
	game->board = BoardCreate();
}

static void Update(Game* game)
{
	if (game->state == STATE_GAME)
	{
		bool end_turn = game->board->fnUpdate(game->board, game);
		if (end_turn)
		{
			if (game->board->destroyed_bubbles > 0)
			{
				ProcessScore(game);

				if (game->board->free_cells_count < BOARD_SIZE_WIDTH * BOARD_SIZE_HEIGHT)
				{
					game->current_strike++;
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
						game->state = STATE_END;
					}
					else
					{
						game->current_strike = 0;
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
	else
	{
		// update our player in end game coz it is our player controller and input here
		game->player->fnUpdate(game->player, game);
	}
	game->ui->fnUpdate(game->ui, game);
}

static void Draw(Game* game)
{
	if (game->state == STATE_GAME)
	{
		game->board->fnDraw(game->board);
	}
	game->ui->fnDraw(game->ui, game);
}

void GameInit(Game* game)
{
	game->total_score = 0;
	game->current_strike = 0;
	game->score_ladder[0] = (ScoreDef){ BASE_SCORE, 1, "GOOD" };
	game->score_ladder[1] = (ScoreDef){ BASE_SCORE, 2, "VERY GOOD" };
	game->score_ladder[2] = (ScoreDef){ BASE_SCORE * 2, 5, "AWESOME" };
	game->score_ladder[3] = (ScoreDef){ BASE_SCORE * 3, 10, "EXCELLENT" };
	game->score_ladder[4] = (ScoreDef){ BASE_SCORE * 4, 100, "AMAZING" };

	game->state = STATE_GAME;
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

	game->fnNewGame = NewGame;
	game->fnDraw = Draw;
	game->fnUpdate = Update;
}
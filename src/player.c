#include "player.h"
#include "board.h"
#include "bubble.h"
#include "game.h"

#include <malloc.h>


static bool Update(Player* player, Game* game)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();

		mousePos.x = VIRTUAL_SCREEN_WIDTH * (mousePos.x / GetScreenWidth());
		mousePos.y = VIRTUAL_SCREEN_HEIGHT * (mousePos.y / GetScreenHeight());

		if (0.f == player->from.x && 0.f == player->from.y)
		{
			Bubble* bubble = game->board->fnTryGetBubble(game->board, (Vector2) { mousePos.x - DRAW_OFFSET_X, mousePos.y - DRAW_OFFSET_Y });
			if (bubble != NULL)
			{
				bubble->is_selected = true;
				player->from = (Vector2){ mousePos.x - DRAW_OFFSET_X, mousePos.y - DRAW_OFFSET_Y };
			}
		}
		else
		{
			Bubble* bubble = game->board->fnTryGetBubble(game->board, (Vector2) { mousePos.x - DRAW_OFFSET_X, mousePos.y - DRAW_OFFSET_Y });
			if (NULL == bubble)
			{
				// TODO check if we can move here
				game->board->fnMoveBubble(game->board, player->from, (Vector2) { mousePos.x - DRAW_OFFSET_X, mousePos.y - DRAW_OFFSET_Y });

				Bubble* bubble = game->board->fnTryGetBubble(game->board, (Vector2) { mousePos.x - DRAW_OFFSET_X, mousePos.y - DRAW_OFFSET_Y });
				if (bubble)
				{
					bubble->is_selected = false;

					player->from = (Vector2){ 0.f, 0.f };
					player->is_turn_end = true;
				}
			}
			else
			{
				Bubble* old_bubble = game->board->fnTryGetBubble(game->board, player->from);
				old_bubble->is_selected = false;

				bubble->is_selected = true;
				player->from = (Vector2){ mousePos.x - DRAW_OFFSET_X, mousePos.y - DRAW_OFFSET_Y };
			}
		}
	}

	if (player->is_turn_end)
	{
		player->is_turn_end = false;
		return true;
	}
	return false;
}

Player* PlayerCreate()
{
	Player* player = (Player*)malloc(sizeof(Player));

	if (player != NULL)
	{
		player->from = (Vector2) { 0.f, 0.f };
		player->is_turn_end = false;
		player->fnUpdate = Update;
		return player;
	}
	return NULL;
}
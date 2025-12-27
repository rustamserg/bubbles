#include "dummy.h"
#include "board.h"
#include "bubble.h"
#include "game.h"

#include "raylib.h"

#include <malloc.h>


typedef struct Player
{
	Dummy base; // must be first to enable cast to player

	Vector2 from;
	bool is_turn_end;
} Player;


static bool Update(Dummy* dummy, Game* game)
{
	Player* player = (Player*)dummy;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();

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

Dummy* DummyCreatePlayer()
{
	Player* player = (Player*)malloc(sizeof(Player));

	if (player != NULL)
	{
		player->from = (Vector2) { 0.f, 0.f };
		player->is_turn_end = false;
		player->base.fnUpdate = Update;
		return &player->base;
	}
	return NULL;
}
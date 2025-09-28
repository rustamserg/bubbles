#include "board.h"
#include "bubble.h"
#include "game.h"

#include <stdlib.h>

static bool IsBubbleMatchColor(Bubble* bubble, Color color)
{
	return bubble->color.r == color.r && bubble->color.g == color.g && bubble->color.b == color.b;
}

static void FindMatches(Board* board, Game* game)
{
	const Bubble* bubble = board->cells[board->last_update_w][board->last_update_h];
	if (!bubble)
		return;

	const Color clr_to_match = bubble->color;

	int w_matches = 0;
	int w_from = board->last_update_w, w_to = board->last_update_w, w = board->last_update_w;

	while (w < BOARD_SIZE_WIDTH)
	{
		Bubble* bubble = board->cells[w][board->last_update_h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			w_to = w;
			++w_matches;
		}
		else
			break;
		++w;
	}
	w = board->last_update_w - 1;
	while (w >= 0)
	{
		Bubble* bubble = board->cells[w][board->last_update_h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			w_from = w;
			++w_matches;
		}
		else
			break;
		--w;
	}

	int h_matches = 0;
	int h_from = board->last_update_h, h_to = board->last_update_h, h = board->last_update_h;

	while (h < BOARD_SIZE_HEIGHT)
	{
		Bubble* bubble = board->cells[board->last_update_w][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_to = h;
			++h_matches;
		}
		else
			break;
		++h;
	}
	h = board->last_update_h - 1;
	while (h >= 0)
	{
		Bubble* bubble = board->cells[board->last_update_w][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_from = h;
			++h_matches;
		}
		else
			break;
		--h;
	}

	if (w_matches >= game->min_matches)
	{
		for (int w = w_from; w <= w_to; ++w)
		{
			Bubble* bubble = board->cells[w][board->last_update_h];
			if (bubble)
			{
				BubbleDestroy(bubble);
				board->cells[w][board->last_update_h] = NULL;
			}
		}
	}
	if (h_matches >= game->min_matches)
	{
		for (int h = h_from; h <= h_to; ++h)
		{
			Bubble* bubble = board->cells[board->last_update_w][h];
			if (bubble)
			{
				BubbleDestroy(bubble);
				board->cells[board->last_update_w][h] = NULL;
			}
		}
	}
}

static bool Update(Board* board, Game* game)
{
	FindMatches(board, game);

	bool is_bubbles_updated = false;
	bool is_board_emptry = true;

	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			Bubble* bubble = board->cells[w][h];
			if (bubble)
			{
				is_board_emptry = false;
				bool is_updated = bubble->fnUpdate(bubble);
				is_bubbles_updated = is_bubbles_updated || is_updated;
			}
		}
	}
	return is_bubbles_updated || is_board_emptry;
}

static Bubble* TryGetBubble(Board* board, Vector2 hitPos)
{
	int w = (int)(hitPos.x / BOARD_CELL_SIZE);
	int h = (int)(hitPos.y / BOARD_CELL_SIZE);

	if (w < BOARD_SIZE_WIDTH && h < BOARD_SIZE_HEIGHT)
	{
		return board->cells[w][h];
	}
	return NULL;
}

static void MoveBubble(Board* board, Vector2 fromPos, Vector2 toPos)
{
	int w_from = (int)(fromPos.x / BOARD_CELL_SIZE);
	int h_from = (int)(fromPos.y / BOARD_CELL_SIZE);

	int w_to = (int)(toPos.x / BOARD_CELL_SIZE);
	int h_to = (int)(toPos.y / BOARD_CELL_SIZE);

	if (w_from < BOARD_SIZE_WIDTH && h_from < BOARD_SIZE_HEIGHT)
	{
		if (w_to < BOARD_SIZE_WIDTH && h_to < BOARD_SIZE_HEIGHT)
		{
			board->cells[w_to][h_to] = board->cells[w_from][h_from];
			board->cells[w_from][h_from] = NULL;

			board->last_update_h = h_to;
			board->last_update_w = w_to;
		}
	}
}

static void DrawBoard(Board* board, int board_pos_x, int board_pos_y)
{
	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			DrawRectangleLines(board_pos_x + w * BOARD_CELL_SIZE, board_pos_y + h * BOARD_CELL_SIZE, BOARD_CELL_SIZE, BOARD_CELL_SIZE, BLACK);

			Bubble* bubble = board->cells[w][h];
			if (bubble)
			{
				bubble->fnDraw(bubble, board_pos_x + w * BOARD_CELL_SIZE, board_pos_y + h * BOARD_CELL_SIZE);
			}
		}
	}
}

static bool AddBubble(Board* board)
{
	int free_cells_count = 0;

	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			if (NULL == board->cells[w][h])
			{
				++free_cells_count;
			}
		}
	}

	if (0 == free_cells_count)
	{
		return false;
	}

	int occupy_cell_index = GetRandomValue(0, free_cells_count - 1);

	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			if (NULL == board->cells[w][h])
			{
				if (0 == occupy_cell_index)
				{
					Color clr = board->colors[GetRandomValue(0, BOARD_CELL_TYPES - 1)];
					board->cells[w][h] = BubbleCreate(clr);

					board->last_update_h = h;
					board->last_update_w = w;
				}
				--occupy_cell_index;
			}
		}
	}
	return true;
}

Board* BoardCreate()
{
	Board* board = (Board*)malloc(sizeof(Board));

	if (board != NULL)
	{
		for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
		{
			for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
			{
				board->cells[w][h] = NULL;
			}
		}

		board->colors[0] = GREEN;
		board->colors[1] = RED;
		board->colors[2] = BLUE;
		board->colors[3] = YELLOW;
		board->colors[4] = VIOLET;
		board->colors[5] = ORANGE;
		board->colors[6] = BEIGE;
		board->colors[7] = MAGENTA;

		board->last_update_h = 0;
		board->last_update_w = 0;

		board->fnDraw = DrawBoard;
		board->fnAddBubble = AddBubble;
		board->fnTryGetBubble = TryGetBubble;
		board->fnMoveBubble = MoveBubble;
		board->fnUpdate = Update;
	}

	return board;
}

void BoardDestroy(Board* board)
{
	if (board != NULL)
	{
		for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
		{
			for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
			{
				if (board->cells[w][h] != NULL)
				{
					BubbleDestroy(board->cells[w][h]);
					board->cells[w][h] = NULL;
				}
			}
		}
		free((void*)board);
	}
}


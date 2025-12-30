#include "board.h"
#include "bubble.h"

#include <malloc.h>
#include <string.h>


static bool IsBubbleMatchColor(Bubble* bubble, Color color)
{
	return bubble->color.r == color.r && bubble->color.g == color.g && bubble->color.b == color.b;
}

static void FindMatches(Board* board, Game* game, int h_added, int w_added)
{
	const Bubble* bubble = board->cells[w_added][h_added];
	if (!bubble)
		return;

	const Color clr_to_match = bubble->color;

	// scan row
	int matches = 0;
	int w_from = w_added, w_to = w_added, w = w_added;

	while (w < BOARD_SIZE_WIDTH)
	{
		Bubble* bubble = board->cells[w][h_added];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			w_to = w;
			++matches;
		}
		else
			break;
		++w;
	}
	w = w_added - 1;
	while (w >= 0)
	{
		Bubble* bubble = board->cells[w][h_added];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			w_from = w;
			++matches;
		}
		else
			break;
		--w;
	}

	if (matches >= game->min_matches)
	{
		for (w = w_from; w <= w_to; ++w)
		{
			Bubble* bubble = board->cells[w][h_added];
			bubble->fnMarkForDestroy(bubble);
		}
	}

	// scan height
	matches = 0;
	int h_from = h_added, h_to = h_added, h = h_added;

	while (h < BOARD_SIZE_HEIGHT)
	{
		Bubble* bubble = board->cells[w_added][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_to = h;
			++matches;
		}
		else
			break;
		++h;
	}
	h = h_added - 1;
	while (h >= 0)
	{
		Bubble* bubble = board->cells[w_added][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_from = h;
			++matches;
		}
		else
			break;
		--h;
	}

	if (matches >= game->min_matches)
	{
		for (h = h_from; h <= h_to; ++h)
		{
			Bubble* bubble = board->cells[w_added][h];
			bubble->fnMarkForDestroy(bubble);
		}
	}

	// scan up-left to down-rigth
	matches = 0;
	w_from = w_added, w_to = w_added, w = w_added;
	h_from = h_added, h_to = h_added, h = h_added;

	// going up-left
	while (w >= 0 && h >= 0)
	{
		Bubble* bubble = board->cells[w][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_from = h;
			w_from = w;
			++matches;
		}
		else
			break;
		--w;
		--h;
	}

	// going down-right
	w = w_added + 1;
	h = h_added + 1;

	while (w < BOARD_SIZE_WIDTH && h < BOARD_SIZE_HEIGHT)
	{
		Bubble* bubble = board->cells[w][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_to = h;
			w_to = w;
			++matches;
		}
		else
			break;
		++w;
		++h;
	}

	if (matches >= game->min_matches)
	{
		w = w_from;
		for (h = h_from; h <= h_to; ++h)
		{
			Bubble* bubble = board->cells[w][h];
			bubble->fnMarkForDestroy(bubble);
			++w;
		}
	}

	// scan up-right to down-left
	matches = 0;
	w_from = w_added, w_to = w_added, w = w_added;
	h_from = h_added, h_to = h_added, h = h_added;

	// going up-right
	while (w < BOARD_SIZE_WIDTH && h >= 0)
	{
		Bubble* bubble = board->cells[w][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_from = h;
			w_from = w;
			++matches;
		}
		else
			break;
		++w;
		--h;
	}

	// going down-left
	w = w_added - 1;
	h = h_added + 1;

	while (w >= 0 && h < BOARD_SIZE_HEIGHT)
	{
		Bubble* bubble = board->cells[w][h];
		if (bubble && IsBubbleMatchColor(bubble, clr_to_match))
		{
			h_to = h;
			w_to = w;
			++matches;
		}
		else
			break;
		--w;
		++h;
	}

	if (matches >= game->min_matches)
	{
		w = w_from;
		for (h = h_from; h <= h_to; ++h)
		{
			Bubble* bubble = board->cells[w][h];
			bubble->fnMarkForDestroy(bubble);
			--w;
		}
	}
}

static bool Update(Board* board, Game* game)
{
	while (board->added_bubbles)
	{
		board->added_bubbles--;
		int h_added = board->last_update_h[board->added_bubbles];
		int w_added = board->last_update_w[board->added_bubbles];
		FindMatches(board, game, h_added, w_added);
	}

	bool is_bubbles_updated = true;
	board->destroyed_bubbles = 0;

	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			Bubble* bubble = board->cells[w][h];
			if (bubble)
			{
				bool is_updated = bubble->fnUpdate(bubble);
				is_bubbles_updated = is_bubbles_updated && is_updated;

				if (bubble->mark_for_destroy && is_updated)
				{
					BubbleDestroy(bubble);
					board->cells[w][h] = NULL;
					board->free_cells_count++;
					++board->destroyed_bubbles;
				}
			}
		}
	}

	bool is_bubble_moved = true;

	if (board->pathfind_path[board->pathfind_current_step].x > 0)
	{
		is_bubble_moved = false;
		board->pathfind_step_time += GetFrameTime();

		if (board->pathfind_step_time > BUBBLE_MOVE_ANIMATION_TIME)
		{
			board->pathfind_step_time -= BUBBLE_MOVE_ANIMATION_TIME;
			int w_from = board->pathfind_path[board->pathfind_current_step].x - 1;
			int h_from = board->pathfind_path[board->pathfind_current_step].y - 1;

			board->pathfind_current_step++;
			int w_to = board->pathfind_path[board->pathfind_current_step].x - 1;
			int h_to = board->pathfind_path[board->pathfind_current_step].y - 1;

			if (board->pathfind_path[board->pathfind_current_step].x == 0)
			{
				board->pathfind_current_step = 0;
				board->pathfind_step_time = 0;
				memset(board->pathfind_path, 0, sizeof(board->pathfind_path));

				board->last_update_h[0] = h_from;
				board->last_update_w[0] = w_from;
				board->added_bubbles = 1;
			}
			else
			{
				board->cells[w_to][h_to] = board->cells[w_from][h_from];
				board->cells[w_from][h_from] = NULL;
			}
		}
	}
	return is_bubbles_updated && is_bubble_moved;
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

static bool TryMoveBubble(Board* board, Vector2 fromPos, Vector2 toPos)
{
	int w_from = (int)(fromPos.x / BOARD_CELL_SIZE);
	int h_from = (int)(fromPos.y / BOARD_CELL_SIZE);

	int w_to = (int)(toPos.x / BOARD_CELL_SIZE);
	int h_to = (int)(toPos.y / BOARD_CELL_SIZE);

	if (w_from < BOARD_SIZE_WIDTH && h_from < BOARD_SIZE_HEIGHT)
	{
		if (w_to < BOARD_SIZE_WIDTH && h_to < BOARD_SIZE_HEIGHT)
		{
			int step = 1;
			bool path_found = false;
			bool stop_search = true;

			// pathfinding board is wider than origin to simplify borders validations
			memset(board->pathfind_cells, 0, sizeof(board->pathfind_cells));
			memset(board->pathfind_path, 0, sizeof(board->pathfind_path));
			board->pathfind_cells[w_from + 1][h_from + 1] = step;
			board->pathfind_current_step = 0;
			board->pathfind_step_time = 0;

			// wave algorithm to avoid heavy recursion
			do
			{
				++step;
				stop_search = true;

				for (int w = 1; w <= BOARD_SIZE_WIDTH; ++w)
				{
					for (int h = 1; h <= BOARD_SIZE_HEIGHT; ++h)
					{
						if (board->cells[w - 1][h - 1] != NULL || board->pathfind_cells[w][h] != 0)
						{
							continue;
						}
						if ((board->pathfind_cells[w - 1][h] == step - 1) ||
							(board->pathfind_cells[w - 1][h - 1] == step - 1) ||
							(board->pathfind_cells[w][h - 1] == step - 1) ||
							(board->pathfind_cells[w + 1][h - 1] == step - 1) ||
							(board->pathfind_cells[w + 1][h] == step - 1) ||
							(board->pathfind_cells[w + 1][h + 1] == step - 1) ||
							(board->pathfind_cells[w][h + 1] == step - 1) ||
							(board->pathfind_cells[w - 1][h + 1] == step - 1))
						{
							stop_search = false;
							board->pathfind_cells[w][h] = step;

							// reach destination point, path is found
							if (w == (w_to + 1) && h == (h_to + 1))
							{
								path_found = true;
								break;
							}
						}
					}
				}
			} while (!stop_search && !path_found);

			if (path_found)
			{
				// trace back to "from" position to build move path
				int w_back = w_to + 1;
				int h_back = h_to + 1;
				board->pathfind_path[step - 1] = (Vector2){ w_back, h_back };

				while (--step > 0)
				{
					if (board->pathfind_cells[w_back - 1][h_back] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back - 1, h_back };
						w_back = w_back - 1;
						continue;
					}
					if (board->pathfind_cells[w_back - 1][h_back - 1] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back - 1, h_back - 1 };
						w_back = w_back - 1;
						h_back = h_back - 1;
						continue;
					}
					if (board->pathfind_cells[w_back][h_back - 1] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back, h_back - 1 };
						h_back = h_back - 1;
						continue;
					}
					if (board->pathfind_cells[w_back + 1][h_back - 1] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back + 1, h_back - 1 };
						w_back = w_back + 1;
						h_back = h_back - 1;
						continue;
					}
					if (board->pathfind_cells[w_back + 1][h_back] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back + 1, h_back };
						w_back = w_back + 1;
						continue;
					}
					if (board->pathfind_cells[w_back + 1][h_back + 1] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back + 1, h_back + 1 };
						w_back = w_back + 1;
						h_back = h_back + 1;
						continue;
					}
					if (board->pathfind_cells[w_back][h_back + 1] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back, h_back + 1 };
						h_back = h_back + 1;
						continue;
					}
					if (board->pathfind_cells[w_back - 1][h_back + 1] == step)
					{
						board->pathfind_path[step - 1] = (Vector2){ w_back - 1, h_back + 1 };
						w_back = w_back - 1;
						h_back = h_back + 1;
						continue;
					}
					break;
				}
			}
			return path_found;
		}
	}
	return false;
}

static void DrawBoard(Board* board)
{
	int board_pos_x = DRAW_OFFSET_X;
	int board_pos_y = DRAW_OFFSET_Y;

	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			DrawRectangle(board_pos_x + w * BOARD_CELL_SIZE, board_pos_y + h * BOARD_CELL_SIZE, BOARD_CELL_SIZE, BOARD_CELL_SIZE, BEIGE);
			DrawRectangleLines(board_pos_x + w * BOARD_CELL_SIZE, board_pos_y + h * BOARD_CELL_SIZE, BOARD_CELL_SIZE, BOARD_CELL_SIZE, BLACK);

			Bubble* bubble = board->cells[w][h];
			if (bubble)
			{
				bubble->fnDraw(bubble, board_pos_x + w * BOARD_CELL_SIZE, board_pos_y + h * BOARD_CELL_SIZE);
			}
		}
	}

	// pathfinding debug draw
#if 0
	for (int w = 1; w <= BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 1; h <= BOARD_SIZE_HEIGHT; ++h)
		{
			if (board->pathfind_cells[w][h] > 0)
			{
				DrawText(TextFormat("%i", board->pathfind_cells[w][h]),
					board_pos_x + (w - 1) * BOARD_CELL_SIZE + 30, board_pos_y + (h - 1) * BOARD_CELL_SIZE + 30, 30, BLACK);
			}
		}
	}
#endif

#if 0
	for (int step_idx = 0; step_idx < sizeof(board->pathfind_path) / sizeof(board->pathfind_path[0]); ++step_idx)
	{
		if (board->pathfind_path[step_idx].x == 0)
		{
			break;
		}
		int w = board->pathfind_path[step_idx].x;
		int h = board->pathfind_path[step_idx].y;
		DrawText(TextFormat("%i", step_idx + 1),
			board_pos_x + (w - 1) * BOARD_CELL_SIZE + 30, board_pos_y + (h - 1) * BOARD_CELL_SIZE + 30, 30, BLACK);

	}
#endif
}

static Color GetNextColor(Board* board)
{
	Color clr = board->colors[GetRandomValue(0, BOARD_CELL_TYPES - 1)];
	return clr;
}

static bool AddBubble(Board* board, Color clr)
{
	if (0 == board->free_cells_count)
	{
		return false;
	}

	int occupy_cell_index = GetRandomValue(0, board->free_cells_count - 1);

	for (int w = 0; w < BOARD_SIZE_WIDTH; ++w)
	{
		for (int h = 0; h < BOARD_SIZE_HEIGHT; ++h)
		{
			if (NULL == board->cells[w][h])
			{
				if (0 == occupy_cell_index)
				{
					board->cells[w][h] = BubbleCreate(clr);

					board->last_update_h[board->added_bubbles] = h;
					board->last_update_w[board->added_bubbles] = w;
					board->free_cells_count--;
					board->added_bubbles++;
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
		memset(board->cells, 0, sizeof(board->cells));
		memset(board->pathfind_cells, 0, sizeof(board->pathfind_cells));
		memset(board->pathfind_path, 0, sizeof(board->pathfind_path));

		board->pathfind_current_step = 0;
		board->pathfind_step_time = 0;

		board->colors[0] = GREEN;
		board->colors[1] = BROWN;
		board->colors[2] = BLUE;
		board->colors[3] = YELLOW;
		board->colors[4] = VIOLET;
		board->colors[5] = ORANGE;
		board->colors[6] = PINK;
		board->colors[7] = RED;

		board->added_bubbles = 0;
		board->destroyed_bubbles = 0;
		board->free_cells_count = BOARD_SIZE_WIDTH * BOARD_SIZE_HEIGHT;

		board->fnGetNextColor = GetNextColor;
		board->fnDraw = DrawBoard;
		board->fnAddBubble = AddBubble;
		board->fnTryGetBubble = TryGetBubble;
		board->fnTryMoveBubble = TryMoveBubble;
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


#include "bubble.h"
#include "board.h"

#include <stdlib.h>


static bool UpdateBubble(Bubble* bubble)
{
	const float max_radius = BOARD_CELL_SIZE / 2 - 10;
	bubble->radius = max_radius;

	if (bubble->add_time_animation > 0.f)
	{
		bubble->radius -= max_radius * (bubble->add_time_animation / ADD_ANIMATION_TIME);
		bubble->add_time_animation = max(0.f, bubble->add_time_animation - GetFrameTime());
		return false;
	}
	else if (bubble->destroy_time_animation > 0.f)
	{
		bubble->radius = max_radius * (bubble->destroy_time_animation / ADD_ANIMATION_TIME);
		bubble->destroy_time_animation = max(0.f, bubble->destroy_time_animation - GetFrameTime());
		return false;
	}
	return true;
}

static void DrawBubble(Bubble* bubble, int bubble_pos_x, int bubble_pos_y)
{
	DrawCircle(bubble_pos_x + BOARD_CELL_SIZE / 2, bubble_pos_y + BOARD_CELL_SIZE / 2, bubble->radius, bubble->color);

	if (bubble->is_selected)
	{
		for (float step = 0.f; step < 2.f; ++step)
		{
			DrawCircleLines(bubble_pos_x + BOARD_CELL_SIZE / 2, bubble_pos_y + BOARD_CELL_SIZE / 2, bubble->radius - step, BLACK);
		}
	}
}

Bubble* BubbleCreate(Color color)
{
	Bubble* bubble = (Bubble*)malloc(sizeof(Bubble));

	if (bubble != NULL)
	{
		bubble->is_selected = false;
		bubble->mark_for_destroy = false;
		bubble->color = color;
		bubble->destroy_time_animation = 0.f;
		bubble->add_time_animation = ADD_ANIMATION_TIME;
		bubble->fnDraw = DrawBubble;
		bubble->fnUpdate = UpdateBubble;
	}
	return bubble;
}

void BubbleMarkForDestroy(Bubble* bubble)
{
	if (bubble != NULL)
	{
		bubble->mark_for_destroy = true;
		bubble->destroy_time_animation = ADD_ANIMATION_TIME;
	}
}

void BubbleDestroy(Bubble* bubble)
{
	if (bubble != NULL)
	{
		free((void*)bubble);
	}
}

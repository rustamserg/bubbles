#include "ui.h"
#include "board.h"
#include "game.h"

#include "raylib.h"
#include <malloc.h>


static bool Update(UI* ui, Game* game)
{
	return true;
}

static void Draw(UI* ui, Game* game)
{
	DrawText(TextFormat("Score: %06i", game->score), 800, 110, 30, RED);

	int x_pos = 260;
	float radius = BOARD_CELL_SIZE / 2 - 10;
	for (int i = 0; i < sizeof(game->next_colors) / sizeof(game->next_colors[0]); ++i)
	{
		DrawCircleGradient(
			(int)(x_pos + i * 2 * (radius + 10)),
			(int)(BOARD_CELL_SIZE / 2 + 20),
			radius,
			WHITE,
			game->next_colors[i]
		);
	}
	DrawText("Next bubbles", 240, 110, 30, BLUE);
}

UI* UICreate()
{
	UI* ui = (UI*)malloc(sizeof(UI));

	if (ui != NULL)
	{
		ui->fnDraw = Draw;
		ui->fnUpdate = Update;
	}
	return ui;
}

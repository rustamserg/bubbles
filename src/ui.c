#include "ui.h"
#include "board.h"
#include "game.h"

#include "raylib.h"

#include <math.h>
#include <malloc.h>


static bool Update(UI* ui, Game* game)
{
	return true;
}

static void DrawGlowText(Font font, const char* text, Vector2 pos, float size, Color color)
{
	// Glow
	for (int i = 6; i > 0; --i)
	{
		DrawTextEx(font, text, (Vector2) { pos.x - i, pos.y }, size, 2, Fade(color, 0.05f));
	}

	// Outline
	DrawTextEx(font, text, (Vector2) { pos.x - 2, pos.y }, size, 2, BLACK);
	DrawTextEx(font, text, (Vector2) { pos.x + 2, pos.y }, size, 2, BLACK);
	DrawTextEx(font, text, (Vector2) { pos.x, pos.y - 2 }, size, 2, BLACK);
	DrawTextEx(font, text, (Vector2) { pos.x, pos.y + 2 }, size, 2, BLACK);

	// Main text
	DrawTextEx(font, text, pos, size, 2, color);
}

static void DrawEffectText(Font font, const char* text, Vector2 pos, float size, Color c1, Color c2)
{
	float t = (float)GetTime();
	float scale = 1.0f + sinf(t * 4.0f) * 0.05f;

	DrawTextPro(font, text, pos, (Vector2) { MeasureTextEx(font, text, 48, 2).x / 2, 0 }, sinf(t) * 2.0f, size * scale, 2, ColorLerp(c1, c2, (sinf(t) + 1) * 0.5f));
}

static void Draw(UI* ui, Game* game)
{
	Font font = GetFontDefault();

	DrawGlowText(font, TextFormat("Score: %06i", game->score), (Vector2) { 800, 110 }, 40, RED);

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
	DrawGlowText(font, "Next bubbles", (Vector2) { 240, 110 }, 40, BLUE);

	DrawTexture(ui->spikes, 1250, 700, WHITE);

	float spiderY = 700;
	if (game->board->free_cells_count > 0)
	{
		spiderY = 100.f + 500.f * (1.f - ((float)game->board->free_cells_count / (BOARD_SIZE_WIDTH * BOARD_SIZE_HEIGHT))); // 100 -> 700
	}
	DrawTextureRec(ui->web, (Rectangle) { 0, 0, 300.f, spiderY - 100.f + 100.f }, (Vector2) { 1250, 100 }, BLACK);
	DrawTexture(ui->spider, 1250, (int)spiderY, WHITE);
}

UI* UICreate()
{
	UI* ui = (UI*)malloc(sizeof(UI));

	if (ui != NULL)
	{
		ui->spikes = LoadTexture("spikes.png");
		ui->spider = LoadTexture("spider.png");
		ui->web = LoadTexture("web.png");

		ui->fnDraw = Draw;
		ui->fnUpdate = Update;
	}
	return ui;
}

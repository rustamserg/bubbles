#include "ui.h"
#include "board.h"
#include "game.h"

#include "raylib.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>


static void AddScoreMessage(UI* ui, const ScoreDef* score)
{
	if (score->multiplayer > 1)
	{
		strcpy(ui->score_msgs[ui->push_score_msg_id].message,
			TextFormat("+%i %s X%i", score->base_score * score->multiplayer, score->message, score->multiplayer));
	}
	else
	{
		strcpy(ui->score_msgs[ui->push_score_msg_id].message,
			TextFormat("+%i %s", score->base_score, score->message));
	}
	ui->score_msgs[ui->push_score_msg_id].display_time = 3;

	ui->push_score_msg_id++;
	if (ui->push_score_msg_id == sizeof(ui->score_msgs) / sizeof(ui->score_msgs[0]))
	{
		ui->push_score_msg_id = 0;
	}
}

static bool Update(UI* ui, Game* game)
{
	if (ui->pop_score_msg_id != ui->push_score_msg_id)
	{
		ui->score_msgs[ui->pop_score_msg_id].display_time = max(0.f, ui->score_msgs[ui->pop_score_msg_id].display_time - GetFrameTime());
		if (0 == ui->score_msgs[ui->pop_score_msg_id].display_time)
		{
			ui->pop_score_msg_id++;
			if (ui->pop_score_msg_id == sizeof(ui->score_msgs) / sizeof(ui->score_msgs[0]))
			{
				ui->pop_score_msg_id = 0;
			}
		}
	}
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

static void Draw(UI* ui, Game* game)
{
	Font font = GetFontDefault();

	DrawGlowText(font, TextFormat("Score: %06i", game->total_score), (Vector2) { 800, 110 }, 40, RED);

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

	if (ui->pop_score_msg_id != ui->push_score_msg_id)
	{
		if (ui->score_msgs[ui->pop_score_msg_id].display_time > 0)
		{
			DrawGlowText(font, ui->score_msgs[ui->pop_score_msg_id].message, (Vector2) { 800, 50 }, 40, GOLD);
		}
	}

	DrawTexture(ui->branch, 1200, -50, WHITE);
	DrawTexture(ui->spikes, 1250, 730, WHITE);

	float spiderY = 700;
	if (game->board->free_cells_count > 0)
	{
		spiderY = 100.f + 500.f * (1.f - ((float)game->board->free_cells_count / (BOARD_SIZE_WIDTH * BOARD_SIZE_HEIGHT))); // 100 -> 700
	}
	DrawTextureRec(ui->web, (Rectangle) { 0, 0, 300.f, spiderY + 100.f }, (Vector2) { 1250, 40 }, BLACK);
	DrawTexture(ui->spider, 1250, (int)spiderY, WHITE);
}

UI* UICreate()
{
	UI* ui = (UI*)malloc(sizeof(UI));

	if (ui != NULL)
	{
		ui->push_score_msg_id = 0;
		ui->pop_score_msg_id = 0;

		ui->branch = LoadTexture("branch.png");
		ui->spikes = LoadTexture("spikes.png");
		ui->spider = LoadTexture("spider.png");
		ui->web = LoadTexture("web.png");

		ui->fnAddScoreMessage = AddScoreMessage;
		ui->fnDraw = Draw;
		ui->fnUpdate = Update;
	}
	return ui;
}

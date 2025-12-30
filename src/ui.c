#include "ui.h"
#include "board.h"
#include "game.h"

#include "raylib.h"

#include <stdlib.h>
#include <string.h>
#include <malloc.h>


static void AddSoundMessage(UI* ui, Sound* sound, float play_time)
{
	ui->sound_msgs[ui->push_sound_msg_id].play_time = play_time;
	ui->sound_msgs[ui->push_sound_msg_id].message = sound;

	ui->push_sound_msg_id++;
	if (ui->push_sound_msg_id == sizeof(ui->sound_msgs) / sizeof(ui->sound_msgs[0]))
	{
		ui->push_sound_msg_id = 0;
	}
}

static void AddScoreMessage(UI* ui, const ScoreDef* score, int added_score)
{
	if (score->multiplayer > 1)
	{
		strcpy(ui->score_msgs[ui->push_score_msg_id].message,
			TextFormat("+%i %s X%i", added_score, score->message, score->multiplayer));
	}
	else
	{
		strcpy(ui->score_msgs[ui->push_score_msg_id].message,
			TextFormat("+%i %s", added_score, score->message));
	}
	ui->score_msgs[ui->push_score_msg_id].display_time = UI_SCORE_MESSAGE_DISPLAY_TIME;

	ui->push_score_msg_id++;
	if (ui->push_score_msg_id == sizeof(ui->score_msgs) / sizeof(ui->score_msgs[0]))
	{
		ui->push_score_msg_id = 0;
	}
}

static bool Update(UI* ui, Game* game)
{
	if (ui->score_msgs[ui->pop_score_msg_id].display_time > 0)
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

	if (ui->sound_msgs[ui->pop_sound_msg_id].play_time > 0)
	{
		if (ui->sound_msgs[ui->pop_sound_msg_id].message != NULL)
		{
			PlaySound(*ui->sound_msgs[ui->pop_sound_msg_id].message);
			ui->sound_msgs[ui->pop_sound_msg_id].message = NULL;
		}
		ui->sound_msgs[ui->pop_sound_msg_id].play_time = max(0.f, ui->sound_msgs[ui->pop_sound_msg_id].play_time - GetFrameTime());

		if (0 == ui->sound_msgs[ui->pop_sound_msg_id].play_time)
		{
			ui->pop_sound_msg_id++;
			if (ui->pop_sound_msg_id == sizeof(ui->sound_msgs) / sizeof(ui->sound_msgs[0]))
			{
				ui->pop_sound_msg_id = 0;
			}
		}
	}
	return true;
}

static void NewGame(UI* ui)
{
	ui->push_sound_msg_id = 0;
	ui->pop_sound_msg_id = 0;
	memset(ui->sound_msgs, 0, sizeof(ui->sound_msgs));

	ui->push_score_msg_id = 0;
	ui->pop_score_msg_id = 0;
	memset(ui->score_msgs, 0, sizeof(ui->score_msgs));
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

	if (game->state == STATE_GAME)
	{
		DrawGlowText(font, TextFormat("Score: %08i", game->total_score), (Vector2) { 800, 110 }, 40, RED);

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

		if (ui->score_msgs[ui->pop_score_msg_id].display_time > 0)
		{
			DrawGlowText(font, ui->score_msgs[ui->pop_score_msg_id].message, (Vector2) { 800, 50 }, 40, GOLD);
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
	else
	{
		const char* endMsg = "GAME OVER";
		DrawGlowText(font, endMsg,
			(Vector2) { VIRTUAL_SCREEN_WIDTH / 2 - (float)MeasureText(endMsg, 60) / 2, VIRTUAL_SCREEN_HEIGHT / 2 - 100 }, 60, RED);

		char scoreMsg[100] = { 0 };
		strcpy(scoreMsg, TextFormat("YOUR SCORE: %08i", game->total_score));
		DrawGlowText(font, scoreMsg,
			(Vector2) {
			VIRTUAL_SCREEN_WIDTH / 2 - (float)MeasureText(scoreMsg, 60) / 2, VIRTUAL_SCREEN_HEIGHT / 2 }, 60, BLUE);
	}
}

UI* UICreate()
{
	UI* ui = (UI*)malloc(sizeof(UI));

	if (ui != NULL)
	{
		ui->sound_bubble_cannot_move = LoadSound("bubble_cannot_move.wav");
		ui->sound_bubble_destroy = LoadSound("bubble_destroy.wav");

		ui->push_sound_msg_id = 0;
		ui->pop_sound_msg_id = 0;
		memset(ui->sound_msgs, 0, sizeof(ui->sound_msgs));

		ui->push_score_msg_id = 0;
		ui->pop_score_msg_id = 0;
		memset(ui->score_msgs, 0, sizeof(ui->score_msgs));

		ui->branch = LoadTexture("branch.png");
		ui->spikes = LoadTexture("spikes.png");
		ui->spider = LoadTexture("spider.png");
		ui->web = LoadTexture("web.png");

		ui->fnNewGame = NewGame;
		ui->fnAddScoreMessage = AddScoreMessage;
		ui->fnDraw = Draw;
		ui->fnUpdate = Update;
		ui->fnAddSoundMessage = AddSoundMessage;
	}
	return ui;
}

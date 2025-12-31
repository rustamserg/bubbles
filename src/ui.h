#ifndef _UI_H_
#define _UI_H_

#include "raylib.h"
#include <stdbool.h>

struct Game;
struct ScoreDef;

typedef struct ScoreMessage
{
	char message[100];
	float display_time;
} ScoreMessage;

typedef struct SoundMessage
{
	Sound* message;
	float play_time;
} SoundMessage;

typedef struct UI
{
	Sound sound_bubble_cannot_move;
	Sound sound_bubble_destroy;
	Sound sound_new_king;

	int push_sound_msg_id;
	int pop_sound_msg_id;
	SoundMessage sound_msgs[20];

	int push_score_msg_id;
	int pop_score_msg_id;
	ScoreMessage score_msgs[3];

	Texture tex_king_no_crown;
	Texture tex_king_crown;
	Texture tex_colum;
	bool celebrate_new_king;

	bool (*fnUpdate)(struct UI*, struct Game*);
	void (*fnDraw)(struct UI*, struct Game*);

	void (*fnNewGame)(struct UI*);
	void (*fnAddSoundMessage)(struct UI*, struct Sound*, float);
	void (*fnAddScoreMessage)(struct UI*, const struct ScoreDef*, int);
} UI;

UI* UICreate();

#endif // _UI_H_
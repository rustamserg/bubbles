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

typedef struct UI
{
	int push_score_msg_id;
	int pop_score_msg_id;
	ScoreMessage score_msgs[3];

	Texture branch;
	Texture spikes;
	Texture web;
	Texture spider;

	bool (*fnUpdate)(struct UI*, struct Game*);
	void (*fnDraw)(struct UI*, struct Game*);

	void (*fnAddScoreMessage)(struct UI*, const struct ScoreDef*, int);
} UI;

UI* UICreate();

#endif // _UI_H_
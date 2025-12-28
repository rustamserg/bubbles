#ifndef _UI_H_
#define _UI_H_

#include "raylib.h"
#include <stdbool.h>

struct Game;

typedef struct UI
{
	Texture spikes;
	Texture web;
	Texture spider;

	bool (*fnUpdate)(struct UI*, struct Game*);
	void (*fnDraw)(struct UI*, struct Game*);
} UI;

UI* UICreate();

#endif // _UI_H_
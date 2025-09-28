#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#include "raylib.h"


typedef struct Bubble
{
	Color color;
	bool is_selected;

	float radius;
	float add_time_animation;

	bool (*fnUpdate)(struct Bubble*);
	void (*fnDraw)(struct Bubble*, int, int);
} Bubble;

Bubble* BubbleCreate(Color color);
void BubbleDestroy(Bubble* bubble);

#endif // _BUBBLE_H_
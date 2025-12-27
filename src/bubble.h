#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#include "raylib.h"

typedef struct Bubble
{
	Color color;
	bool is_selected;
	bool mark_for_destroy;

	float radius;
	float add_time_animation;
	float destroy_time_animation;

	bool (*fnUpdate)(struct Bubble*);
	void (*fnDraw)(struct Bubble*, int, int);
	void (*fnMarkForDestroy)(struct Bubble*);
} Bubble;

Bubble* BubbleCreate(Color color);
void BubbleDestroy(Bubble* bubble);

#endif // _BUBBLE_H_
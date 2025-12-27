#ifndef _AI_H_
#define _AI_H_

#include <stdbool.h>

struct Game;

typedef struct AI
{
	bool (*fnUpdate)(struct AI*, struct Game*);
} AI;

AI* AICreate();

#endif // _AI_H_
#ifndef _DUMMY_H_
#define _DUMMY_H_

#include <stdbool.h>

struct Game;

typedef struct Dummy
{
	bool (*fnUpdate)(struct Dummy*, struct Game*);
} Dummy;

Dummy* DummyCreateAI();
Dummy* DummyCreatePlayer();

#endif // _DUMMY_H_
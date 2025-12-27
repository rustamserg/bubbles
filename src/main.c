#include "raylib.h"
#include <time.h> // For time()

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "game.h"


int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 960, "Bubbles");

	SetTargetFPS(60);
	SetRandomSeed((unsigned int)time(NULL));

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Game game;
	GameInit(&game);
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		game.fnUpdate(&game);

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(RAYWHITE);

		game.fnDraw(&game);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

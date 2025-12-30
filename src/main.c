#include "raylib.h"
#include <time.h> // For time()

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "game.h"


int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, "Bubbles");

	InitAudioDevice();

	SetTargetFPS(60);
	SetRandomSeed((unsigned int)time(NULL));

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);

	Shader crt = LoadShader(0, "crt.fs");

	// Uniform locations
	int locResolution = GetShaderLocation(crt, "resolution");
	int locTime = GetShaderLocation(crt, "time");
	int locCurvature = GetShaderLocation(crt, "curvature");
	int locScanline = GetShaderLocation(crt, "scanline");
	int locVignette = GetShaderLocation(crt, "vignette");
	int locAberration = GetShaderLocation(crt, "aberration");
	int locGrille = GetShaderLocation(crt, "grille");

	// Set static-ish params once
	float curvature = 0.02f;
	float scanline = 0.35f;
	float vignette = 0.35f;
	float aberr = 0.90f;
	float grille = 0.10f;

	SetShaderValue(crt, locCurvature, &curvature, SHADER_UNIFORM_FLOAT);
	SetShaderValue(crt, locScanline, &scanline, SHADER_UNIFORM_FLOAT);
	SetShaderValue(crt, locVignette, &vignette, SHADER_UNIFORM_FLOAT);
	SetShaderValue(crt, locAberration, &aberr, SHADER_UNIFORM_FLOAT);
	SetShaderValue(crt, locGrille, &grille, SHADER_UNIFORM_FLOAT);

	Game game;
	GameInit(&game);
	
	Music back_music = LoadMusicStream("background.wav");
	SetMusicVolume(back_music, 0.5f);
	PlayMusicStream(back_music);

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateMusicStream(back_music);

		// Check for Alt + Enter key press to toggle fullscreen
		if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
		{
			int display = GetCurrentMonitor();

			if (IsWindowFullscreen())
			{
				// If we are full screen, then go back to the windowed size
				SetWindowSize(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
			}
			else
			{
				// If we are not full screen, set the window size to match the monitor we are on
				SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
			}

			// Toggle the state
			ToggleFullscreen();
		}

		game.fnUpdate(&game);

		// 1) draw your world to target
		BeginTextureMode(target);
		ClearBackground(RAYWHITE);
		game.fnDraw(&game);
		EndTextureMode();

		// 2) draw target to screen with shader
		// Update dynamic uniforms
		float t = (float)GetTime();
		SetShaderValue(crt, locTime, &t, SHADER_UNIFORM_FLOAT);

		// Use the *output* size you are drawing to (screen or letterboxed dst size)
		Vector2 res = { (float)GetScreenWidth(), (float)GetScreenHeight() };
		SetShaderValue(crt, locResolution, &res, SHADER_UNIFORM_VEC2);

		// Draw
		BeginDrawing();
		ClearBackground(BLACK);

		BeginShaderMode(crt);

		// If you do pixel-perfect scaling/letterbox, draw your target scaled here.
		// Example: direct full-screen draw:
		Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
		Rectangle dst = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };

		DrawTexturePro(target.texture, src, dst, (Vector2) { 0, 0 }, 0.0f, BLACK);

		EndShaderMode();

		EndDrawing();
	}

	UnloadMusicStream(back_music);
	CloseAudioDevice();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

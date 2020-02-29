#pragma once

#include "SDL.h"
#undef main

#include "imgui.h"
#include "imgui_sdl.h"
#include "Graph.h"

// Window settings.
const char* WIN_TITLE = "SDL ImGui Plot - Demo";
const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 768;

// SDL gloabal vars.
SDL_Window* window;
SDL_Renderer* renderer;

void Start()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow
	(
		WIN_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH,
		WIN_HEIGHT,
		SDL_WINDOW_RESIZABLE
	);

	renderer = SDL_CreateRenderer
	(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);
}

void Exit()
{
	ImGuiSDL::Deinitialize();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	ImGui::DestroyContext();
}
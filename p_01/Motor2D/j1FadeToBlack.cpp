#include "j1FadeToBlack.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Module.h"
#include "j1Scene.h"



j1FadeToBlack::j1FadeToBlack() {}

j1FadeToBlack::~j1FadeToBlack() {}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	uint width, height, scale;
	App->win->GetWindowSize(width, height);
	scale = App->win->GetScale();
	screen = { 0, 0, (int)(width * scale), (int)(height * scale) };

	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	bool ret = true;
	if (current_step == fade_step::none)
		return ret;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			off->CleanUp();
			on->Start();
			//off->Disable();
			//on->Enable();
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;

	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;
	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		if (start_time > total_time) {
			start_time = total_time;
		}
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		on = module_on;
		off = module_off;
		ret = true;
	}
	return ret;
}

bool j1FadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}
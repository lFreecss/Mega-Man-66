#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	p2SString map_name("%s%s", App->map_name.GetString(), ".tmx");
	App->map->Load(map_name.GetString());
	current_map = map_name;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		App->render->camera.x += 1;

	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());

	//Scroll
	
	if (App->player->pos.x > App->render->camera.x + 200 && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->render->camera.x != -1070) {

		App->render->camera.x -= 1;
		gate = false;
	}

	if (App->player->pos.x > App->render->camera.x - 200 && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->render->camera.x != 0) {

		App->render->camera.x += 1;
		gate = false;
	}

	if (App->player->pos.x >= 740) {
		App->player->pos.x = 60.0;
		App->render->camera.x = 0;
		
		App->map->CleanUp();
		if (current_map == "untitled.tmx") {
			App->map->Load("JAIL.tmx");
			current_map = "JAIL.tmx";
		}
		else {
			App->map->Load("untitled.tmx");
			current_map = "untitled.tmx";
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			App->map->CleanUp();
		if (current_map == "untitled.tmx") {
			App->map->Load("JAIL.tmx");
			current_map = "JAIL.tmx";
		}
		else {
			App->map->Load("untitled.tmx");
			current_map = "untitled.tmx";
		}
		//App->fade->FadeToBlack(this, this, 1);
	}
		

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

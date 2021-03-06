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
	first_map = ("%s%s", App->map_name.GetString(), ".tmx");
	App->map->Load(first_map.GetString());
	current_map = first_map;
	InitializeMap();
	App->audio->PlayMusic("audio/music/Armored Armadillo.ogg");
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
	if (map_num == 0 && current_map != "rock_level.tmx")
		ChangeMaps("rock_level.tmx");
	if (map_num == 1 && current_map != "JAIL.tmx")
		ChangeMaps("JAIL.tmx");

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Restart();

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		MapStart();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
		App->render->camera.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		App->render->camera.x += 1;

	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());

	//Scroll, TODO Valdivia
	if (App->player->pos.x < 560)
		App->render->camera.x = -(2 * App->player->pos.x);

	//Debug
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		App->player->pos.x = 540;
	/*if (App->player->pos.x > App->render->camera.x + 200 && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->render->camera.x != -1070) {

	App->render->camera.x -= 1;
	gate = false;
	}

	if (App->player->pos.x > App->render->camera.x - 200 && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->render->camera.x != 0) {

	App->render->camera.x += 1;
	gate = false;
	}
	}*/
	//When player gets at the end of the level, change to the next, TODO Valdivia
	if (App->player->pos.x >= App->map->data.tile_width*App->map->data.width - 30) {
		if (current_map == "rock_level.tmx") {
			ChangeMaps("JAIL.tmx");
			MapStart();
		}

		else {
			ChangeMaps("rock_level.tmx");
			MapStart();
		}

	}
	//Change level, for debuging 
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		if (current_map == "rock_level.tmx") {
			ChangeMaps("JAIL.tmx");
		}

		else {
			ChangeMaps("rock_level.tmx");
		}

	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
//Change from one map to the other, TODO Valdivia
void j1Scene::ChangeMaps(const char* map_name) {
	App->map->CleanUp();
	App->map->Load(map_name);
	current_map = map_name;
	InitializeMap();
	//MapStart();
}
//For starting form the same map, TODO Varela
void j1Scene::InitializeMap() {
	if (current_map == "rock_level.tmx") {
		map_num = 0;
	}
	if (current_map == "JAIL.tmx") {
		map_num = 1;
	}
}
//To Start form the very first level, TODO Varela
void j1Scene::Restart() {
	ChangeMaps("rock_level.tmx");
	MapStart();
	map_num = 0;
}

void j1Scene::MapStart() {
	App->render->camera.x = 0;
	App->render->camera.x = 0;
	App->player->Init();
}
//Load map
bool j1Scene::Load(pugi::xml_node& data)
{
	map_num = data.child("map").attribute("name").as_int();
	return true;
}

// Save map
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node map = data.append_child("map");

	map.append_attribute("name") = map_num;

	return true;
}
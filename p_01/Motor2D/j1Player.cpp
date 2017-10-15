#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1map.h"


j1Player::j1Player() : j1Module() {
	name.create("player");

	//Mega Man
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 133, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.speed = 0.04f;

	right.PushBack({ 188, 10, 24, 24 });
	right.PushBack({ 218, 10, 21, 24 });
	right.PushBack({ 239, 10, 21, 24 });
	right.speed = 0.04f;

	left.PushBack({ 281, 86, 24, 24 });
	left.PushBack({ 259, 86, 21, 24 });
	left.PushBack({ 233, 86, 21, 24 });
	left.speed = 0.04f;

	jumpR.PushBack({ 265, 4, 27, 30 });
	jumpR.speed = 0.001f;
	//Mega Man X
	/*idle.PushBack({ 323, 17, 30, 34 });
	idle.PushBack({ 357, 17, 30, 34 });
	idle.PushBack({ 391, 17, 30, 34 });
	idle.PushBack({ 323, 17, 30, 34 });
	idle.PushBack({ 323, 17, 30, 34 });
	idle.PushBack({ 323, 17, 30, 34 });
	idle.speed = 0.04f;

	right.PushBack({ 106, 107, 30, 34 });
	right.PushBack({ 132, 107, 30, 34 });
	right.PushBack({ 157, 107, 30, 34 });
	right.PushBack({ 181, 107, 30, 34 });
	right.PushBack({ 213, 107, 30, 34 });
	right.PushBack({ 247, 107, 30, 34 });
	right.PushBack({ 274, 107, 30, 34 });
	right.PushBack({ 298, 107, 30, 34 });
	right.PushBack({ 326, 107, 30, 34 });
	right.PushBack({ 391, 107, 30, 34 });
	right.speed = 0.11f;*/

}

j1Player::~j1Player()
{}


bool j1Player::Start() {
	bool ret = true;

	//graphics = App->tex->Load("textures/mmx_xsheet.png");
	graphics = App->tex->Load("textures/8bitmegaman.png");
	if (LoadPlayer() == false)
		ret = false;

	Init();

	return ret;
}

void j1Player::Init() {
	pos = startPos;

	jumping = false;
	actualJumpframes = 0;
}


bool j1Player::Update(float dt) {
	bool ret = true;

	current_animation = &idle;

	move(dt);
	jump(dt);

	if (jumping)
		current_animation = &jumpR;


	SDL_Rect r = current_animation->GetCurrentFrame();
	App->render->Blit(graphics, pos.x, pos.y, &r);

	
	return ret;
}

bool j1Player::LoadPlayer() {
	bool ret = true;
	pugi::xml_parse_result result = player_file.load_file("player.xml");
	if (result == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'player_file' file.");
		ret = false;
	}
	pugi::xml_node player = player_file.child("player");
	if (player == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'player' tag.");
		ret = false;
	}
	else {
		//p2List_item<map_object*>* object = App->map->data.objects.start;
		//startPos.x = object->data->x; 
		//startPos.y = object->data->y;

		vel.x = player.child("physics").attribute("velocityx").as_float();
		vel.y = player.child("physics").attribute("velocityy").as_float();
		gravity = player.child("physics").attribute("gravity").as_float();

		jumpframes = player.child("movement").attribute("jumpframes").as_uint();

		size.x = 22;
		size.y = 22;
	}

	return ret;
}

void j1Player::jump(float dt) {
		
	if (actualJumpframes > 0) {
		++actualJumpframes;
		if (actualJumpframes > jumpframes) {
			jumping = false;
			actualJumpframes = 0;
		}
		else {
			pos.y -= vel.y*dt;
			iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
			iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
			if (App->map->CollisionY(posWorld.x, endPosWorld.x, posWorld.y)) {
				pos.y += gravity*dt;
			}
		}
	}

	pos.y += gravity*dt;
	iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
	iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
	if (App->map->CollisionY(posWorld.x, endPosWorld.x, endPosWorld.y)) {
		pos.y -= gravity*dt;
		jumping = false;
	}
	else {
		jumping = true;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (!jumping) {
			jumping = true;
			actualJumpframes = 1;
		}
	}
	
}

void j1Player::move(float dt) {


	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		current_animation = &left;
		pos.x -= vel.x*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
		if (App->map->CollisionX(posWorld.x, posWorld.y, endPosWorld.y)) {
			pos.x += vel.x*dt;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		current_animation = &right;
		pos.x += vel.x*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
		if (App->map->CollisionX(endPosWorld.x, posWorld.y, endPosWorld.y)) {
			pos.x -= vel.x*dt;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		current_animation = &idle;
}

// Load player position
bool j1Player::Load(pugi::xml_node& data)
{
	pos.x = data.child("position").attribute("x").as_float();
	pos.y = data.child("position").attribute("y").as_float();

	return true;
}

// Save player position
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = pos.x;
	position.append_attribute("y") = pos.y;

	return true;
}
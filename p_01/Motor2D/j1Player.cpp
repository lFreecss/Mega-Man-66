#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1map.h"


j1Player::j1Player() {

	idle.PushBack({ 323, 17, 30, 34 });
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
	right.speed = 0.11f;

}

j1Player::~j1Player()
{}


bool j1Player::Start() {
	bool ret = true;

	graphics = App->tex->Load("textures/mmx_xsheet.png");
	if (LoadPlayer() == false)
		ret = false;

	return ret;
}


bool j1Player::Update(float dt) {
	bool ret = true;

	current_animation = &idle;
	
	vel.y += g * dt;
	pos.y += vel.y * dt;
	pos.x += vel.x * dt;

	move(dt);
	jump(dt);

	if (pos.y >= floor)
		pos.y = floor;

	if (pos.y == floor)
		jumping = true;
	else
		jumping = false;
	

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
		pos.x = player.child("positions").attribute("positionx").as_float();
		pos.y = player.child("positions").attribute("positiony").as_float();
		floor = player.child("positions").attribute("floor").as_int();

		vel.x = player.child("physics").attribute("velocityx").as_float();
		vel.y = player.child("physics").attribute("velocityy").as_float();
		g = player.child("physics").attribute("gravity").as_float();

		jumping = player.child("movement").attribute("jumping").as_bool();
		jumpVelY = player.child("movement").attribute("jumpvel").as_float();

	}

	return ret;
}

bool j1Player::jump(float dt) {
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (jumping) {
			vel.y = jumpVelY * dt;
			ret = true;
		}
		else
			ret = false;
	}
	
	return ret;
}

void j1Player::move(float dt) {
	float speed = 3.8;

	/*if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		position.y += speed;
		if (position.y > floor) {
			position.y = floor;
		}
	}
*/
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		pos.x -= speed*dt;
		if (pos.x < 0) {
			pos.x = 0;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		current_animation = &right;
		pos.x += speed*dt;
		if (pos.x > 1470)
			pos.x = 1470;
	}

}
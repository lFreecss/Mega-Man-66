#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"

struct SDL_Texture;

class j1Player : public j1Module  {

public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update(float dt);

	bool LoadPlayer();
	bool jump(float dt);
	void move(float dt);


	fPoint pos;
private:
	
	Animation* current_animation;
	Animation idle;
	Animation right;
	SDL_Texture* graphics = nullptr;

	int floor;
	fPoint vel;
	float g;

	bool jumping;
	float jumpVelY;

	pugi::xml_document	player_file;

};

#endif
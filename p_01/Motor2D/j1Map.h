#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Textures.h"
typedef unsigned int uint;

struct map_layer {
	p2SString name = nullptr;
	uint width = 0;
	uint height = 0;
	uint* data = nullptr;
	uint size_data = 0;
	~map_layer() {
		RELEASE_ARRAY(data);
	}
	inline uint Get(int x, int y)const;
};

struct map_object {
	p2SString name = nullptr;
	uint id = 0;
	uint x = 0;
	uint y = 0;
	uint width = 0;
	uint height = 0;
};

struct TileSet
{
	
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	~TileSet() {
		App->tex->UnLoad(texture);
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<map_layer*> layers;
	p2List<map_layer*> collisions;
	p2List<map_object*> objects;
};


class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// Coordinates map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	// Coordinates world positions to map positions
	iPoint WorldToMap(int x, int y) const;

	//Collision Detector
	bool CollisionX(uint x, uint y_up, uint y_down);
	bool CollisionY(uint x_left, uint x_right, uint y);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);

	bool LoadLayer(pugi::xml_node& node, map_layer* layer);
	bool LoadObject(pugi::xml_node& node, map_object* layer);



public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__
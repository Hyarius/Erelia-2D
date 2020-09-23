#ifndef ERELIA_2D_H
#define ERELIA_2D_H

#include "jgl.h"

//#include "vld.h"

#include "erelia_define.h"

extern GLuint programID;
extern GLuint delta_pos_uniform;

jgl::Vector2 tile_to_screen(jgl::Vector2 tile_pos, jgl::Vector2 base_pos = -1);
jgl::Vector2 screen_to_tile(jgl::Vector2 coord, jgl::Vector2 base_pos = -1);

#include "Erelia_game_engine.h"

extern Game_engine* engine;

extern jgl::Array<Item*> node_item_list;
extern jgl::Array<Item*> env_item_list;
extern jgl::Array<Item*> trainer_item_list;
extern jgl::Array<Item*> interact_item_list;
extern jgl::Array<Item*> prefab_item_list;
extern jgl::Array<Item*> npc_item_list;

void generate_creature_data();

#endif
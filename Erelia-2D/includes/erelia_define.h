#ifndef ERELIA_DEFINE_H
#define ERELIA_DEFINE_H


#define chunk_size 9
#define node_size 32

#define OBSTACLE		0b00000000
#define WALKABLE		0b00011111
#define DOWN_WALKABLE	0b00000010
#define LEFT_WALKABLE	0b00000100
#define RIGHT_WALKABLE	0b00001000
#define UP_WALKABLE		0b00010000
#define SWIMABLE		0b00100000

using node_type = char;

#define NODE_TAB		0
#define ENV_TAB			5
#define TRAINER_TAB		6
#define INTERACT_TAB	7
#define PREFAB_TAB		8
#define NPC_TAB			9

#endif
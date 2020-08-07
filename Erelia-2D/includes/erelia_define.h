#ifndef ERELIA_DEFINE_H
#define ERELIA_DEFINE_H


#define chunk_size 9
#define node_size 32

#define OBSTACLE		0b000000000
#define WALKABLE		0b000011111
#define DOWN_WALKABLE	0b000000010
#define LEFT_WALKABLE	0b000000100
#define RIGHT_WALKABLE	0b000001000
#define UP_WALKABLE		0b000010000
#define SWIMABLE		0b000100000
#define JUMPING			0b001000000
#define CASCADE			0b010000000

using node_type = short;

#define NODE_TAB		0
#define ENV_TAB			5
#define TRAINER_TAB		6
#define INTERACT_TAB	7
#define PREFAB_TAB		8
#define NPC_TAB			9

#endif
#ifndef ERELIA_PREFAB_H
#define ERELIA_PREFAB_H

class Prefab
{
private:
	jgl::String _name;
	jgl::Vector2 _size;
	std::map<jgl::Vector2, Node*> _content;

public:
	Prefab();

	jgl::String name() { return (_name); }
	void save(jgl::String p_name, Board* board, jgl::Vector2 _first, jgl::Vector2 _second);
	void use(Board* board, jgl::Vector2 start);
};

extern jgl::Array<Prefab*> prefab_array;

#endif
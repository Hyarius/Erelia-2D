#include "erelia-2D.h"

#define chunk_size 9
#define node_size 32

std::string custom_texture_shader_vert = {
	"#version 330 core\n"
	"layout(location = 0) in vec3 model_space;"
	"layout(location = 1) in vec2 vertexUV;"
	"uniform vec3 delta_pos;"
	"out vec2 UV;"
	"void main()"
	"{"
		"vec3 tmp = model_space - delta_pos;"// - vec3(-1, 1, 0);"
		"gl_Position = vec4(tmp, 1.0f);"
		"UV = vertexUV;"
	"}"
};

std::string custom_texture_shader_frag = {
	"#version 330 core\n"
	"in vec2 UV;"
	"out vec4 color;"
	"uniform sampler2D textureID;"
	"void main()"
	"{"
		"color = texture(textureID, UV).rgba;"
	"}"
};

GLuint programID;
GLuint delta_pos_uniform;

#define OBSTACLE		0b00000000
#define WALKABLE		0b00000001
#define SWIMABLE		0b00000010
#define DOWN_WALKABLE	0b00000100
#define LEFT_WALKABLE	0b00001000
#define RIGHT_WALKABLE	0b00010000
#define UP_WALKABLE		0b00100000

using node_type = char;

struct Node
{
	int index;
	jgl::Vector2 sprite;
	node_type type;

	Node(int p_index, jgl::Vector2 p_sprite, node_type p_type)
	{
		index = p_index;
		sprite = p_sprite;
		type = p_type;
	}
};

jgl::Array<Node*> node_array = {
	new Node(0, jgl::Vector2(0, 0), WALKABLE),
	new Node(1, jgl::Vector2(1, 1), OBSTACLE)
};

class Chunk
{
private:
	jgl::Vector2 _pos;
	Node* _content[chunk_size][chunk_size];

	jgl::Array<jgl::Vector3> _points;
	jgl::Array<jgl::Vector2> _uvs;

	GLuint _vertex_buffer;
	GLuint _uvs_buffer;

public:
	bool is_border(int x, int y)
	{
		if (x == 0 || y == 0 || x == chunk_size - 1 || y == chunk_size - 1)
			return (true);
		return (false);
	}
	Chunk(jgl::Vector2 p_pos)
	{
		_pos = p_pos;
		glGenBuffers(1, &_vertex_buffer);
		glGenBuffers(1, &_uvs_buffer);

		for (size_t i = 0; i < chunk_size; i++)
			for (size_t j = 0; j < chunk_size; j++)
				_content[i][j] = nullptr;
	}
	void place(jgl::Vector2 coord, Node* p_node, bool need_bake = false)
	{
		int x = jgl::floor(coord.x);
		int y = jgl::floor(coord.y);
		_content[x][y] = p_node;
	}
	void bake(jgl::Sprite_sheet* tileset)
	{
		_points.clear();
		_uvs.clear();
		for (int i = 0; i < chunk_size; i++)
			for (int j = 0; j < chunk_size; j++)
				if (_content[i][j] != nullptr)
				{
					jgl::Vector2 tmp = (jgl::Vector2(i, j) + _pos * chunk_size) * node_size;

					_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(0, 0) * node_size));
					_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(0, 1) * node_size));
					_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(1, 0) * node_size));
					_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(0, 1) * node_size));
					_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(1, 1) * node_size));
					_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(1, 0) * node_size));

					jgl::Vector2 tmp_sprite = tileset->sprite(_content[i][j]->sprite);

					_uvs.push_back(tmp_sprite + jgl::Vector2(0.0f, 0.0f));
					_uvs.push_back(tmp_sprite + jgl::Vector2(0.0f, tileset->unit().y));
					_uvs.push_back(tmp_sprite + jgl::Vector2(tileset->unit().x, 0.0f));
					_uvs.push_back(tmp_sprite + jgl::Vector2(0.0f, tileset->unit().y));
					_uvs.push_back(tmp_sprite + tileset->unit());
					_uvs.push_back(tmp_sprite + jgl::Vector2(tileset->unit().x, 0.0f));
				}

		const jgl::Vector3* tmp = _points.all();
		const jgl::Vector2* tmp2 = _uvs.all();

		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _points.size() * 3, static_cast<const float*>(&(tmp[0].x)), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _points.size() * 2, static_cast<const float*>(&(tmp2[0].x)), GL_STATIC_DRAW);
	}
	void render(jgl::Sprite_sheet* tileset, jgl::Vector2 center, jgl::Viewport* viewport)
	{
		glBindVertexArray(g_application->vertex_array());
		
		glUseProgram(programID);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tileset->image()->texture_id());

		glUniform3f(delta_pos_uniform, center.x, center.y, 0.0f);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_points.size()));
	}
};

class Board
{
private:
	std::map<jgl::Vector2, Chunk*> _chunks;

	jgl::Sprite_sheet* _tileset = nullptr;

public:
	Chunk* chunk(jgl::Vector2 pos)
	{
		if (_chunks.contains(pos) == 0)
			return (nullptr);
		return (_chunks[pos]);
	}
	Board(jgl::Sprite_sheet* p_tileset, jgl::String path)
	{
		_tileset = p_tileset;
		bool in_chunk = false;
		jgl::Vector2 chunk_pos;
		int chunk_line;

		std::fstream file = jgl::open_file(path);
		while (file.eof() == false)
		{
			jgl::String line = jgl::get_str(file);
			if (line.size() == 0)
				break;
			jgl::Array<jgl::String> tab = line.split(";");
			if (in_chunk == false && tab.size() == 2)
			{
				chunk_pos = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));
				chunk_line = 0;
				in_chunk = true;
			}
			else if (in_chunk == true && tab.size() == chunk_size)
			{
				for (int i = 0; i < chunk_size; i++)
				{
					int type = jgl::stoi(tab[i]);
					if (type != -1 && type < static_cast<int>(node_array.size()))
					{
						jgl::Vector2 node_pos = jgl::Vector2(i, chunk_line) + chunk_pos * chunk_size;
						place(node_pos, node_array[type]);
					}
				}
				chunk_line++;
				if (chunk_line == chunk_size)
					in_chunk = false;
			}
			else
				jgl::error_exit(1, "Error in map file - Line [" + line + "]");
		}
		for (auto tmp : _chunks)
		{
			tmp.second->bake(_tileset);
		}
	}
	jgl::Vector2 tile_on_screen(jgl::Vector2 player_pos, jgl::Vector2 tile_pos)
	{
		jgl::Vector2 result = (tile_pos - player_pos) * node_size + g_application->size() /2;
		return (result);
	}
	jgl::Vector2 tile_coord(jgl::Vector2 player_pos, jgl::Vector2 coord)
	{
		jgl::Vector2 pos = coord - g_application->size() / 2;
		jgl::Vector2 rel = pos / node_size;

		return ((player_pos + rel).floor());
	}
	jgl::Vector2 chunk_pos(jgl::Vector2 node_pos)
	{
		return ((node_pos / chunk_size).floor());
	}
	jgl::Vector2 rel_node_pos(jgl::Vector2 node_pos)
	{
		jgl::Vector2 tmp = chunk_pos(node_pos);
		return (node_pos - tmp * chunk_size);
	}
	void place(jgl::Vector2 node_pos, size_t index, bool need_bake = false)
	{
		if (index >= node_array.size())
			place(node_pos, nullptr);
		else
		{
			Node* node = node_array[index];
			place(node_pos, node);
		}
		jgl::Vector2 tmp_chunk = chunk_pos(node_pos);

		if (need_bake == true)
			_chunks[tmp_chunk]->bake(_tileset);

	}
	void place(jgl::Vector2 node_pos, Node* p_node, bool need_bake = false)
	{
		jgl::Vector2 tmp_chunk = chunk_pos(node_pos);

		if (_chunks.contains(tmp_chunk) == 0)
			_chunks[tmp_chunk] = new Chunk(tmp_chunk);

		Chunk* tmp = _chunks[tmp_chunk];
		tmp->place(rel_node_pos(node_pos), p_node);

		if (need_bake == true)
			tmp->bake(_tileset);
	}
	void bake_chunk(jgl::Vector2 pos)
	{
		if (_chunks.contains(pos) == 0)
			return;
		g_application->viewport()->use();
		_chunks[pos]->bake(_tileset);
	}
	void render(jgl::Vector2 player_pos, jgl::Viewport* viewport)
	{
		jgl::Vector2 delta = jgl::convert_screenV2_to_opengl(player_pos * node_size);
		jgl::Vector2 start = (tile_coord(player_pos, 0) / chunk_size).floor();
		jgl::Vector2 end = (tile_coord(player_pos, g_application->size()) / chunk_size).floor();
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				jgl::Vector2 tmp = jgl::Vector2(i, j);
				if (_chunks.contains(tmp) != 0)
				{
					_chunks[tmp]->render(_tileset, delta, viewport);
				}
			}
	}
};

class Player
{
private:
	jgl::Vector2 _pos;
	jgl::Vector2 _direction;
	float distance;

	Uint32 _total_tick;
	Uint32 _last_tick;
	Uint32 _actual_tick;
	Uint32 _move_speed;
public:
	Player()
	{
		_pos = chunk_size / 2.0f;
		_direction = 0;
		_move_speed = 75;
	}
	void set_move_speed(Uint32 p_state) { _move_speed = p_state; }
	Uint32 move_speed() { return (_move_speed); }
	jgl::Vector2 pos() { return (_pos); }
	bool is_active() { return (_direction != 0); }
	void move(jgl::Vector2 delta)
	{
		if (is_active() == true)
			return;

		_total_tick = 0;
		_last_tick = g_time;
		_actual_tick = g_time;
		_direction = delta / _move_speed;
	}
	void update()
	{
		if (_direction == 0)
			return;

		_actual_tick = g_time;
		Uint32 delta = _actual_tick - _last_tick;

		if (_total_tick + delta > _move_speed)
		{
			delta = _move_speed - _total_tick;
		}
		_total_tick += delta;
		_last_tick = _actual_tick;
		jgl::Vector2 delta_pos = _direction * delta;
		_pos += delta_pos;
		if (_total_tick >= _move_speed)
			_direction = 0;
	}
};

class Renderer : public jgl::Widget
{
private:
	Board* _board;
	Player* _player;

public:
	Renderer(Board* p_board, Player *p_player, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_board = p_board;
		_player = p_player;
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{
		_board->render(_player->pos(), _viewport);

		jgl::fill_centred_rectangle(g_application->size() / 2, 2, jgl::Color(255, 0, 0), _viewport);
		jgl::draw_text("Fps : " + jgl::itoa(print_fps), 50, 16, 1, jgl::text_color::white, jgl::text_style::normal, _viewport);
	}
};

class Selecter : public jgl::Widget
{
private:
	jgl::Sprite_sheet* _tileset;
	int _tile_select;

public:
	Selecter(jgl::Sprite_sheet* p_tileset, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_tileset = p_tileset;
		_tile_select = 0;
	}

	int tile_select() { return (_tile_select); }

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{

	}
};

class Player_controller : public jgl::Widget
{
private:
	Player* _player;
public:
	Player_controller(Player* p_player, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_player = p_player;
	}

	bool handle_keyboard()
	{
		static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
		static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 0.5f), jgl::Vector2(0.0f, -0.5f), jgl::Vector2(-0.5f, 0.0f), jgl::Vector2(0.5f, 0.0f) };

		for (size_t i = 0; i < 4; i++)
		{
			if (jgl::get_key(key_value[i]) == jgl::key_state::down)
			{
				_player->move(move_delta[i]);
				return (true);
			}
		}
		return (false);
	}

	void update()
	{
		_player->update();
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{

	}
};

enum class Item_type
{
	node = 0,
	count
};

class Item
{
private:
	Item_type _item_type;
	jgl::Sprite_sheet* _tileset;
	jgl::Vector2 _sprite;
public:
	Item(Item_type p_item_type, jgl::Sprite_sheet* p_tileset, jgl::Vector2 p_sprite)
	{
		_tileset = p_tileset;
		_item_type = p_item_type;
		_sprite = p_sprite;
	}

	Item_type item_type() { return (_item_type); }
	jgl::Sprite_sheet* tileset() { return (_tileset); }
	jgl::Vector2 sprite() { return (_sprite); }

	void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, jgl::Viewport* p_viewport)
	{
		_tileset->draw(_sprite, p_anchor, p_area, 1.0f, p_viewport);
	}
	virtual void use(jgl::Data param) = 0;
};

class Item_slot : public jgl::Widget
{
private:
	bool _select;
	jgl::w_box_component _box;
	Item* _item;

public:
	Item_slot(Item* p_item, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_select = false;
		_box = jgl::w_box_component(this);
		_item = p_item;
	}

	jgl::w_box_component& box() { return (_box); }

	void select()
	{
		if (_select == true)
			return;

		_box.set_anchor(_box.anchor() - 4);
		_box.set_area(_box.area() + 8);
		_box.set_back(jgl::Color(171, 163, 9));
		_box.set_front(jgl::Color(230, 221, 62));
		_box.set_border(8);
		_select = true;
	}

	void unselect()
	{
		if (_select == false)
			return;

		_box.set_anchor(_box.anchor() + 4);
		_box.set_area(area());
		_box.set_back(jgl::Color(90, 90, 90));
		_box.set_front(jgl::Color(120, 120, 120));
		_box.set_border(5);
		_select = false;
	}

	void move(jgl::Vector2 delta)
	{
		_box.set_anchor(_box.anchor() + delta);
		_viewport->set_anchor(anchor() + delta);
	}

	bool handle_keyboard()
	{
		return (false);
	}

	bool handle_mouse()
	{
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_box.set_area(p_area);
		_box.set_anchor(p_anchor);
	}

	void render()
	{
		if (is_active() == false)
			return;

		_box.render(_viewport);
		draw_rectangle(_box.anchor(), _box.area(), 1, jgl::Color(0, 0, 0));
		draw_rectangle(_box.anchor() + _box.border(), _box.area() - _box.border() * 2, 1, jgl::Color(0, 0, 0));
		if (_item != nullptr)
			_item->draw(_box.border(), _box.area() - _box.border() * 2, _viewport);
	}
};

class Editor_inventory_tab : public jgl::Widget
{
private:
	size_t _nb_slot;
	jgl::Sprite_sheet* _icon_tileset;

	jgl::Frame* _background;

	jgl::Array<Item_slot*> _item_slots;
	jgl::Vscroll_bar* _scroll_bar;

public:
	Editor_inventory_tab(size_t p_nb_slot, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_nb_slot = p_nb_slot;
		_background = new jgl::Frame(this);
		_background->activate();
		_background->send_back();

		_scroll_bar = new jgl::Vscroll_bar(_background);
		_scroll_bar->activate();
	}

	Item_slot* add_item_slot(Item *p_item)
	{
		Item_slot* result = new Item_slot(p_item, _background);
		result->activate();
		_item_slots.push_back(result);
		return (result);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_background->set_geometry(0, p_area);
		_scroll_bar->set_geometry(jgl::Vector2(p_area.x - 20, 0.0f), jgl::Vector2(20.0f, p_area.y));
		float size = (p_area.x - _nb_slot * 10) / _nb_slot;
		for (size_t i = 0; i < _item_slots.size(); i++)
		{
			jgl::Vector2 pos = jgl::Vector2(10 + ((i % _nb_slot) * (size + 5)), 10 + ((i / _nb_slot) * (size + 5)));
			_item_slots[i]->set_geometry(pos, size);
		}
	}

	void render()
	{
		if (area() == -1)
			return;
	}
};

void activate_tab(jgl::Data param)
{
	Editor_inventory_tab* tab = param.acces< Editor_inventory_tab*>(0);
	Editor_inventory_tab** tab2 = param.acces< Editor_inventory_tab**>(1);

	if (tab != nullptr)
		tab->activate();
	if (*tab2 != nullptr)
		(*tab2)->desactivate();
	*tab2 = tab;
}

class Editor_shortcut_bar : public jgl::Widget
{
private:
	int _index;
	jgl::Frame* _background = nullptr;
	Item_slot* _selected = nullptr;
	jgl::Array<Item_slot*> _slots;
public:
	Editor_shortcut_bar(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_index = 0;
		_background = new jgl::Frame(this);
		_background->activate();
		for (size_t i = 0; i < 10; i++)
		{
			Item_slot* result = new Item_slot(nullptr, _background);
			result->activate();

			result->box().set_back(jgl::Color(90, 90, 90));
			result->box().set_front(jgl::Color(120, 120, 120));
			_slots.push_back(result);
		}
	}

	bool handle_mouse()
	{
		if (g_mouse->wheel != 0)
		{
			if (_selected != nullptr)
				_selected->unselect();
			_index = _index + static_cast<int>(-g_mouse->wheel);
			while (_index < 0)
				_index += _slots.size();
			while (_index >= _slots.size())
				_index -= _slots.size();
			_slots[_index]->select();
			_selected = _slots[_index];
		}
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_background->set_geometry(0, p_area);
		
		size_t nb = _slots.size();
		float offset = 10;
		float size = p_area.y - offset * 2;
		float space = ((p_area.x - offset * 2) - (nb * size)) / (nb - 1);
		jgl::Vector2 delta = (p_area - jgl::Vector2(space * (nb - 1) + size * nb, size)) / 2;
		for (size_t i = 0; i < nb; i++)
		{
			jgl::Vector2 pos = delta + jgl::Vector2((size + space) * i, 0.0f);
			_slots[i]->set_geometry(pos, size);
		}
		_slots[_index]->select();
		_selected = _slots[_index];
	}

	void render()
	{
		if (area() == -1)
			return;
	}
};


class Node_item : public Item
{
private:
	size_t _node_index;

public:
	Node_item(size_t p_node_index, jgl::Sprite_sheet* p_tileset, jgl::Vector2 p_sprite) : Item(Item_type::node, p_tileset, p_sprite)
	{
		_node_index = p_node_index;
	}

	void use(jgl::Data param)
	{
		Board* board = param.acces<Board*>(0);
		jgl::Vector2* pos = param.acces<jgl::Vector2*>(1);
		board->place(*pos, _node_index);
	}
};

struct tmp_struct {
	jgl::String name;
	jgl::Array<Item*> items;
};

jgl::Array<tmp_struct> item_array = {
	{"Node", {

	}},
	{"Env.", {
	
	}},
	{"Trainer", {

	}},
	{"Interact", {

	}}
};

class Editor_inventory : public jgl::Widget
{
private:
	bool _activated = false;
	jgl::Frame* _background = nullptr;
	Editor_shortcut_bar* _shortcut = nullptr;
	Editor_inventory_tab* _active_tab = nullptr;
	jgl::Array<Editor_inventory_tab*> _tab;
	jgl::Array<jgl::Button*> _tab_button;

public:
	Editor_inventory(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_background = new jgl::Frame(this);

		_shortcut = new Editor_shortcut_bar(this);
		_shortcut->activate();

		for (size_t i = 0; i < item_array.size(); i++)
		{
			Editor_inventory_tab* result = new Editor_inventory_tab(10, _background);
			for (size_t j = 0; j < (i + 1) * 3; j++)
				result->add_item_slot(nullptr);
			_tab.push_back(result);
		}

		for (size_t i = 0; i < item_array.size(); i++)
		{
			jgl::Button* result = new jgl::Button(activate_tab, jgl::Data(2, _tab[i], &_active_tab), _background);
			result->set_text(item_array[i].name);
			result->activate();
			_tab_button.push_back(result);
		}

		activate_tab(jgl::Data(2, _tab[0], &_active_tab));
	}

	bool status() { return (_activated); }

	void enable()
	{
		_activated = true;
		_background->activate();
		_shortcut->set_frozen(true);
	}

	void disable()
	{
		_activated = false;
		_background->desactivate();
		_shortcut->set_frozen(false);
	}

	bool handle_keyboard()
	{
		return (false);
	}

	bool handle_mouse()
	{
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		float tmp_size = node_size * 2.5f + 5;
		jgl::Vector2 shortcut_size = jgl::Vector2(tmp_size * 10, tmp_size);
		jgl::Vector2 tab_size = jgl::Vector2(shortcut_size.x, p_area.y - shortcut_size.y - 30);
		float tmp_value = (p_area.x - tab_size.x - 40) / 2.0f;
		jgl::Vector2 button_size = jgl::Vector2(tmp_value > 100 ? 100 : tmp_value, 30.0f);
		jgl::Vector2 frame_size = jgl::Vector2(tab_size.x + button_size.x * 2 + 40, p_area.y - 10 - shortcut_size.y);

		jgl::Vector2 frame_pos = ((p_area - 10 - jgl::Vector2(10.0f, shortcut_size.y)) - frame_size) / 2;
		jgl::Vector2 tab_pos = 10;
		jgl::Vector2 button_pos = jgl::Vector2(frame_size.x - (button_size.x + 10) * 2, 10.0f);

		_background->set_geometry(frame_pos, frame_size);

		_shortcut->set_geometry(jgl::Vector2((p_area.x - shortcut_size.x) / 2, frame_size.y + 10.0f), shortcut_size);

		for (size_t i = 0; i < _tab.size(); i++)
			_tab[i]->set_geometry(10, tab_size);

		for (size_t i = 0; i < _tab_button.size(); i++)
			_tab_button[i]->set_geometry(button_pos + jgl::Vector2((i % 2) * (button_size.x + 10), (i / 2) * (button_size.y + 10)), button_size);
	}

	void render()
	{
		if (area() == -1)
			return;
	}
};

class Editor_interact : public jgl::Widget
{
private:
	Editor_inventory* _inventory;
	Board* _board;
	Player* _player;

	bool _state = false;
	jgl::Vector2 _first = -1;
	jgl::Vector2 _second = -1;

public:
	Editor_interact(Editor_inventory* p_inventory, Board* p_board, Player* p_player, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_inventory = p_inventory;
		_board = p_board;
		_player = p_player;
	}

	void handle_multi_pos()
	{
		if (_state == false)
		{
			_first = _board->tile_coord(_player->pos(), g_mouse->pos);
			_state = true;
		}
		else
		{
			_second = _board->tile_coord(_player->pos(), g_mouse->pos);
			jgl::Vector2 start = jgl::Vector2((_first.x < _second.x ? _first.x : _second.x), (_first.y < _second.y ? _first.y : _second.y));
			jgl::Vector2 end = jgl::Vector2((_first.x > _second.x ? _first.x : _second.x), (_first.y > _second.y ? _first.y : _second.y));
			for (float i = start.x; i <= end.x; i++)
				for (float j = start.y; j <= end.y; j++)
				{
					_board->place(jgl::Vector2(i, j), size_t(0));
				}
			start = (start / chunk_size).floor();
			end = (end / chunk_size).floor();
			for (float i = start.x; i <= end.x; i++)
				for (float j = start.y; j <= end.y; j++)
				{
					_board->bake_chunk(jgl::Vector2(i, j));
				}

			_state = false;
		}
	}

	bool handle_mouse()
	{
		if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::pressed ||
			jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
			handle_multi_pos();
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{
		if (_state == true)
		{
			jgl::Vector2 _third = _board->tile_coord(_player->pos(), g_mouse->pos);
			jgl::Vector2 start = jgl::Vector2((_first.x < _third.x ? _first.x : _third.x), (_first.y < _third.y ? _first.y : _third.y));
			jgl::Vector2 end = jgl::Vector2((_first.x > _third.x ? _first.x : _third.x), (_first.y > _third.y ? _first.y : _third.y));
			for (float i = start.x; i <= end.x; i++)
				for (float j = start.y; j <= end.y; j++)
				{
					jgl::Vector2 coord = _board->tile_on_screen(_player->pos(), jgl::Vector2(i, j));
					jgl::draw_rectangle(coord, node_size, 1, jgl::Color(0, 0, 0), _viewport);
				}
		}
	}
};

class Game_engine : public jgl::Widget
{
private:
	jgl::Sprite_sheet* _tileset;
	Board* _board;
	Player* _player;

	Renderer* _renderer;
	Selecter* _selecter;
	Player_controller* _player_controller;
	Editor_inventory* _editor_inventory;
	Editor_interact* _editor_interacter;

public:
	Game_engine(jgl::Widget *p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_tileset = new jgl::Sprite_sheet("ressources/texture/base_tileset.png", jgl::Vector2(38, 41));
		_board = new Board(_tileset, "ressources/maps/world.map");
		_player = new Player();

		_renderer = new Renderer(_board, _player, this);
		_renderer->activate();

		_player_controller = new Player_controller(_player, this);
		_player_controller->activate();

		_editor_interacter = new Editor_interact(_editor_inventory, _board, _player, this);
		_editor_interacter->activate();

		_selecter = new Selecter(_tileset, this);

		_editor_inventory = new Editor_inventory(this);
		_editor_inventory->activate();

		_editor_inventory->send_front();
		_renderer->send_back();
	}

	bool handle_keyboard()
	{
		if (jgl::get_key(jgl::key::tab) == jgl::key_state::release)
		{
			if (_editor_inventory->status() == false)
			{
				_player_controller->set_frozen(true);
				_editor_interacter->set_frozen(true);
				_editor_inventory->enable();
			}
			else
			{
				_player_controller->set_frozen(false);
				_editor_interacter->set_frozen(false);
				_editor_inventory->disable();
			}
		}
		return (false);
	}
	
	
	bool handle_mouse()
	{
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_renderer->set_geometry(0, p_area);
		_selecter->set_geometry(0, p_area);
		_editor_inventory->set_geometry(50, p_area - 100);
		_editor_interacter->set_geometry(0, p_area);
	}

	void render()
	{

	}
};

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D", 0.8f);
	jgl::set_font_path("ressources/font/karma suture.ttf");

	//creation du shader pour les chunks
	programID = g_application->add_custom_shader(custom_texture_shader_vert, custom_texture_shader_frag);
	delta_pos_uniform = g_application->get_custom_uniform(programID, "delta_pos");

	Game_engine* engine = new Game_engine();
	engine->set_geometry(0, app.size());
	engine->activate();

	return (app.run());
}
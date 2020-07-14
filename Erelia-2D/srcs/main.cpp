#include "erelia-2D.h"

#define chunk_size 15
#define node_size 30

class Node
{
private:
	jgl::String _symbol;
	jgl::Color _color_back;
	jgl::Color _color_front;

public:
	Node(jgl::String p_symbol = ' ', jgl::Color p_color_back = jgl::Color(), jgl::Color p_color_front = jgl::Color()) :
		_symbol(p_symbol),
		_color_back(p_color_back),
		_color_front(p_color_front)
	{

	}

	static jgl::Vector2 calc_node_size(float zoom)
	{
		return (node_size * zoom);
	}
	jgl::Vector2 calc_screen_pos(jgl::Vector2 pos, float zoom)
	{
		return (pos * calc_node_size(zoom) + g_application->size() / 2);
	}
	void draw(jgl::Vector2 pos, float zoom, jgl::Viewport* viewport)
	{
		jgl::Vector2 tmp_pos = calc_screen_pos(pos, zoom);
		jgl::Vector2 tmp_size = calc_node_size(zoom);

		jgl::fill_centred_rectangle(tmp_pos, tmp_size, _color_back, viewport);
		jgl::fill_centred_rectangle(tmp_pos, tmp_size * 0.85f, _color_front, viewport);
		if (_symbol.size() != 0)
			jgl::draw_centred_text(_symbol, tmp_pos, 10 * zoom, 1);
	}
};

jgl::Array<Node> node_array = {
	{"", jgl::Color(200, 200, 200), jgl::Color(230, 230, 230)},
	{"X", jgl::Color(50, 50, 50), jgl::Color(75, 75, 75)},
};

class Chunk
{
private:
	jgl::Vector2 _pos;
	int _content[chunk_size][chunk_size];

	jgl::Array<jgl::Vector2> _points;
	jgl::Array<jgl::Color> _colors;

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
		for (size_t i = 0; i < chunk_size; i++)
			for (size_t j = 0; j < chunk_size; j++)
				_content[i][j] = (is_border(i, j) ? 1 : 0);
	}
	void bake(jgl::Vector2 center)
	{
		for (size_t i = 0; i < chunk_size; i++)
			for (size_t j = 0; j < chunk_size; j++)
				if (_content[i][j] >= 0)
				{
					jgl::Vector2 tmp = jgl::Vector2(i, j) + delta;
					node_array[_content[i][j]].draw(tmp, zoom, nullptr);
				}
	}
	void render(jgl::Vector2 center, float zoom, jgl::Viewport* viewport)
	{
		jgl::Vector2 delta = _pos * chunk_size - center;
		for (size_t i = 0; i < chunk_size; i++)
			for (size_t j = 0; j < chunk_size; j++)
				if (_content[i][j] >= 0)
				{
					jgl::Vector2 tmp = jgl::Vector2(i, j) + delta;
					node_array[_content[i][j]].draw(tmp, zoom, nullptr);
				}
	}
};

class Board
{
private:
	std::map<jgl::Vector2, Chunk*> _chunks;

public:
	Board()
	{

	}
	void render(jgl::Vector2 center, jgl::Vector2 start, jgl::Vector2 end, float zoom, jgl::Viewport* viewport)
	{
		int start_chunk[2] = { std::floor(start.x / chunk_size), std::floor(start.y / chunk_size)};
		int end_chunk[2] = { std::ceil(end.x / chunk_size), std::ceil(end.y / chunk_size)};

		for (int i = start_chunk[0]; i < end_chunk[0]; i++)
			for (int j = start_chunk[1]; j < end_chunk[1]; j++)
			{
				jgl::Vector2 pos = jgl::Vector2(i, j);
				if (_chunks.count(pos) == 0)
					_chunks[pos] = new Chunk(pos);
				_chunks[pos]->render(center, zoom, viewport);

			}
	}
};

class Entity
{
private:
	Node* _node;

	float _move_speed;
	jgl::Vector2 _pos;
	jgl::Vector2 _destination;

public:
	Entity(jgl::Vector2 p_pos, float p_move_speed = 5.0f, jgl::String p_symbol = " ", jgl::Color p_color_back = jgl::Color(), jgl::Color p_color_front = jgl::Color())
	{
		_pos = p_pos;
		_destination = p_pos;
		_move_speed = p_move_speed;
		_node = new Node(p_symbol, p_color_back, p_color_front);
	}
	void move(jgl::Vector2 p_destination)
	{
		_destination = p_destination;
	}
	void update()
	{
		if (_destination != _pos)
		{
			jgl::Vector2 delta = ((_destination - _pos)).normalize() * (_move_speed / g_application->max_fps());
			if (delta.length() < (_destination - _pos).length())
				_pos += delta;
			else
				_pos = _destination;
		}
	}
	void render(jgl::Vector2 center, float zoom, jgl::Viewport* viewport)
	{
		_node->draw(_pos - center, zoom, viewport);
	}
};

class Renderer : public jgl::Widget
{
private:
	jgl::Vector2 _camera_pos;
	float _zoom;

	Board* _board;
	Entity* _player;

public:
	float zoom() { return (_zoom); }
	jgl::Vector2 camera_pos() { return (_camera_pos); }

	Renderer(Board* p_board, Entity* p_player, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent), _board(p_board), _player(p_player)
	{
		_camera_pos = chunk_size / 2;
		_zoom = 1;
	}

	jgl::Vector2 node_pos(jgl::Vector2 mouse_pos)
	{
		return ((mouse_pos - g_application->size() / 2) / Node::calc_node_size(_zoom) + _camera_pos);
	}

	void move(jgl::Vector2 delta)
	{
		_camera_pos += delta;
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

	}
	void render()
	{
		jgl::Vector2 start = node_pos(0);
		jgl::Vector2 end = node_pos(g_application->size());
		_board->render(_camera_pos, start, end, _zoom, _viewport);
		_player->render(_camera_pos, _zoom, _viewport);
		jgl::draw_text("Fps : " + jgl::itoa(print_fps), 50);
	}
};

class Game_engine : public jgl::Widget
{
private:
	Board* _board;
	Entity* _player;

	Renderer* _renderer;

public:
	Game_engine(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_board = new Board();
		_player = new Entity(0, 4, "P", jgl::Color(57, 126, 184), jgl::Color(66, 164, 245));
		_renderer = new Renderer(_board, _player, this);
		_renderer->activate();
	}

	void update()
	{
		_player->update();
	}

	bool handle_keyboard()
	{
		return (false);
	}

	bool handle_mouse()
	{
		static jgl::Vector2 border_size = jgl::Vector2(g_application->size().x / 12.0f, g_application->size().x / 8.0f);

		if (jgl::get_mouse_pos().x >= 0 && jgl::get_mouse_pos().x <= border_size.x)
			_renderer->move(jgl::Vector2(-0.25f, 0.0f));
		if (jgl::get_mouse_pos().x >= g_application->size().x - border_size.x && jgl::get_mouse_pos().x <= g_application->size().x)
			_renderer->move(jgl::Vector2(0.25f, 0.0f));
		if (jgl::get_mouse_pos().y >= 0 && jgl::get_mouse_pos().y <= border_size.y)
			_renderer->move(jgl::Vector2(0.0f, -0.25f));
		if (jgl::get_mouse_pos().y >= g_application->size().y - border_size.y && jgl::get_mouse_pos().y <= g_application->size().y)
			_renderer->move(jgl::Vector2(0.0f, 0.25f));

		if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
		{
			jgl::Vector2 pos = _renderer->node_pos(g_mouse->pos);
			_player->move(pos);
			return (true);
		}
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_renderer->set_geometry(0, p_area);
	}
	void render()
	{

	}
};

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D");
	app.set_max_fps(6000);
	jgl::set_font_path("ressources/font/karma suture.ttf");

	Game_engine* engine = new Game_engine();
	engine->set_geometry(0, app.size());
	engine->activate();

	return (app.run());
}
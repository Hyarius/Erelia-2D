#include "erelia-2D.h"

#define chunk_size 15
#define node_size 30

class Node
{
private:
	jgl::Glyph _symbol;
	jgl::Color _color_back;
	jgl::Color _color_front;

public:
	Node(jgl::Glyph p_symbol = ' ', jgl::Color p_color_back = jgl::Color(), jgl::Color p_color_front = jgl::Color()) :
		_symbol(p_symbol),
		_color_back(p_color_back),
		_color_front(p_color_front)
	{

	}

	jgl::Vector2 calc_node_size(float zoom)
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
	}
};

jgl::Array<Node> node_array = {
	{' ', jgl::Color(200, 200, 200), jgl::Color(230, 230, 230)},
	{'X', jgl::Color(50, 50, 50), jgl::Color(75, 75, 75)},
};

class Chunk
{
private:
	jgl::Vector2 _pos;
	int _content[chunk_size][chunk_size];
public:
	Chunk(jgl::Vector2 p_pos)
	{
		_pos = p_pos;
		for (size_t i = 0; i < chunk_size; i++)
			for (size_t j = 0; j < chunk_size; j++)
				_content[i][j] = 0;
	}
	void render(float zoom, jgl::Viewport* viewport)
	{
		for (size_t i = 0; i < chunk_size; i++)
			for (size_t j = 0; j < chunk_size; j++)
				if (_content[i][j] >= 0)
				{
					jgl::Vector2 tmp = jgl::Vector2(i, j) + _pos * chunk_size;
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
	void render(jgl::Vector2 center, float zoom, jgl::Viewport* viewport)
	{
		int distance = 2;
		int start[2] = { (center.x / chunk_size) - distance, (center.y / chunk_size) - distance };
		int end[2] = { (center.x / chunk_size) + distance, (center.y / chunk_size) + distance };

		for (int i = start[0]; i <= end[0]; i++)
			for (int j = start[1]; j <= end[1]; j++)
			{
				jgl::Vector2 pos = jgl::Vector2(i, j);
				if (_chunks.count(pos) == 0)
					_chunks[pos] = new Chunk(pos);
				_chunks[pos]->render(zoom, viewport);

			}
	}
};

class Entity
{
private:
	jgl::String _symbol;
public:
	Entity(jgl::String p_symbol) : _symbol(p_symbol)
	{

	}
};

class Renderer : public jgl::Widget
{
private:
	jgl::Vector2 _camera_pos;
	float _zoom;

	Board* _board;

public:
	Renderer(Board* p_board, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent), _board(p_board)
	{
		_camera_pos = chunk_size / 2;
		_zoom = 1;
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
		_board->render(_camera_pos, _zoom, _viewport);
	}
};

class Game_engine : public jgl::Widget
{
private:
	Board* _board;

	Renderer* _renderer;

public:
	Game_engine(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_board = new Board();
		_renderer = new Renderer(_board, this);
		_renderer->activate();
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
		_renderer->set_geometry(0, p_area);
	}
	void render()
	{

	}
};

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D");
	jgl::set_font_path("ressources/font/karma suture.ttf");

	Game_engine* engine = new Game_engine();
	engine->set_geometry(0, app.size());
	engine->activate();

	return (app.run());
}
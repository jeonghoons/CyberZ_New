#pragma once
#include "pch.h"

class Object
{
public:
	Object() = default;
	Object(sf::Texture& t, int x, int y, int x2, int y2) {

		m_sprite.setTexture(t);
		m_sprite.setTextureRect(sf::IntRect(x, y, x2, y2));
		set_name("NONAME");
		m_mess_end_time = chrono::system_clock::now();
	}

	void a_move(int x, int y) {
		m_sprite.setPosition((float)x, (float)y);
	}
	void SetPosition(int x, int y) { m_x = x; m_y = y; }
	void SetScale(float x) { m_sprite.setScale(x, x); }

	pair<int, int> GetPosition() { return { m_x, m_y }; }

	void a_draw() {
		g_window->draw(m_sprite);
	}

	void draw() {
		float rx = (m_x - g_left_x) * TILE_WIDTH + 1;
		float ry = (m_y - g_top_y) * TILE_WIDTH + 1;
		m_sprite.setPosition(rx, ry);
		g_window->draw(m_sprite);
		// auto size = m_name.getGlobalBounds();
		// m_hp.setPosition(rx + 32 - size.width / 2, ry - 30);
		// g_window->draw(m_hp);
		/*if (m_mess_end_time < chrono::system_clock::now()) {
			m_name.setPosition(rx + 32 - size.width / 2, ry - 10);
			g_window->draw(m_name);
		}
		else {
			m_chat.setPosition(rx + 32 - size.width / 2, ry - 10);
			g_window->draw(m_chat);
		} */

	}
	void set_name(const char str[]) {

		/*string slevel = "LV";
		slevel += static_cast<char>(level + '0');
		slevel += str;*/

		m_name.setFont(*g_font);
		// m_name.setString(slevel.c_str());

		/*if (_id < MAX_USER) m_name.setFillColor(sf::Color(255, 255, 255));
		else m_name.setFillColor(sf::Color(255, 255, 0));*/

		m_name.setFillColor(sf::Color(255, 255, 0));
		m_name.setFillColor(sf::Color(255, 255, 0));
		m_name.setStyle(sf::Text::Bold);
	}
	int GetId() const { return _id; }
	void SetId(int id) { _id = id; }

private:
	sf::Sprite m_sprite;
	sf::Text m_name;
	sf::Text m_hp;
	sf::Text m_chat;
	chrono::system_clock::time_point m_mess_end_time;

	int		_id;
	int m_x, m_y;
	char name[20];
};

extern Object myPlayer;
extern std::unordered_map<int, Object> players;


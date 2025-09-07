
#include "pch.h"
#include "Network.h"
#include <string>
#include "Object.h"

//void HandleError(const char* cause)
//{
//	int errCode = ::WSAGetLastError();
//	cout << cause << " ErrorCode : " << errCode << endl;
//}

static const int   TARGET_FPS = 60;           // 목표 FPS






Network network;

vector<vector<Object>> tileMap(32);

void client_initialize()
{
	board = new sf::Texture;
	pieces = new sf::Texture;
	board->loadFromFile("gamemap.bmp");

	pieces->loadFromFile("charactersheet.bmp");
	// pieces->loadFromFile("chess2.png");
	
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			int tile_x = i * (512 / 32);
			int tile_y = j * (512 / 32);
			Object tile(*board, tile_x, tile_y, 16, 16);
			tile.SetPosition(i, j);
			tile.SetScale(float(TILE_WIDTH) / 16.f);
			tileMap[i].emplace_back(tile);
		}
	}

	g_font = new sf::Font();
	if (false == g_font->loadFromFile("cour.ttf")) {
		cout << "Font Loading Error!\n";
		exit(-1);
	}
	
	
	myPlayer = Object{ *pieces, 0, 0, TILE_WIDTH-1, TILE_WIDTH-1 };
	myPlayer.SetPosition(0, 0);
		
}

void client_finish()
{
	players.clear();
	delete g_font;
	delete board;
	delete pieces;
}

void client_main()
{
	network.RecvPacket();

	for (auto tiles : tileMap)
		for (auto tile : tiles)
			tile.draw();
	

	myPlayer.draw();
	for (auto& pl : players) pl.second.draw();
	sf::Text text;
	text.setFont(*g_font);
	char buf[100];
	sprintf_s(buf, "(%d, %d)", myPlayer.GetPosition().first, myPlayer.GetPosition().second);
	// sprintf_s(buf, "(%d, %d) - LV : %d   HP[%d/%d] ", avatar.GetPosition().first, avatar.GetPosition().second, avatar.level, avatar.hp, avatar.max_hp);
	text.setString(buf);
	g_window->draw(text);
}


int main()
{
	client_initialize();
	
	if (network.Connect2Server() == false)
		exit(-1);

	CS_LOGIN_PACKET loginPkt;
	loginPkt.header.type = CS_PACKET_LIST::CS_LOGIN;
	loginPkt.header.size = sizeof(loginPkt);
	network.Send_packet(&loginPkt);
	
	cout << "Connected to Server" << endl;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT");
	g_window = &window;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				int direction = -1;
				switch (event.key.code) {
				
				case sf::Keyboard::Left:
					direction = 2;
					break;
				case sf::Keyboard::Right:
					direction = 3;
					break;
				case sf::Keyboard::Up:
					direction = 0;
					break;
				case sf::Keyboard::Down:
					direction = 1;
					break;
				
				case sf::Keyboard::C: {
					cout << "메세지 입력 : ";
					/*char chat[CHAT_SIZE];
					cin.getline(chat, CHAT_SIZE - 1);
					CS_CHAT_PACKET p;
					p.size = static_cast<int>(strlen(chat)) + 3;
					memcpy(p.mess, chat, p.size);
					p.type = CS_CHAT;
					cout << p.size << "바이트,	" << p.mess << endl;
					send_packet(&p);*/
					break;

				}
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
				/*if (-1 != direction) {
					CS_MOVE_PACKET p;
					p.size = sizeof(p);
					p.type = CS_MOVE;
					p.direction = direction;
					send_packet(&p);
				}*/

			}
		}

		window.clear();
		client_main();
		window.display();
	}
	client_finish();

}
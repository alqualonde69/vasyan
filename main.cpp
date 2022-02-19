#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <utility>
#include <algorithm>

const int							x = 36;
const int							y = 21;
const int							cell = 32;
const int 							score_bar_height = 64;
const int							width = x * cell;
const int							height = y * cell + score_bar_height;

int									score;
bool								gameover;
bool								pause;
bool								game_started;
bool								greetings = true;
bool								campaign_done = false;
enum								e_menu {MainMenu, SettingsMenu, ControlMenu, GameTypeMenu, LvlMenu};
e_menu								current_menu = MainMenu;
std::vector<std::string>			main_menu = {"New Game", "Settings", "Control", "Quit"};
std::vector<std::string>			settings_menu = {"Speed", "Game Type", "Volume"};
std::vector<std::string>			game_type_menu = {"Classic", "New", "Campaign"};
enum								e_game {Classic, Box, Tunnel, Mill, Railway, Room, Campaign};
e_game								game_type = Classic;
std::vector<std::string>			lvl = {"Box", "Tunnel", "Mill", "Railway", "Room"};
int									main_item = 0;
int									volume = 50;
int									sleep_time = 300;
int 								speed = 5;
int									current_lvl = 0;
int 								total_score = 0;
int 								hearts = 3;
int 								heartbreak_timer = 0;
int									menu_char_size = 100;
std::vector<std::pair<int, int>>	walls;
std::pair<int, int>					port = {-1, -1};
enum								e_dir {UP, LEFT, DOWN, RIGHT};
e_dir								dir;
std::vector<std::pair<int, int>>	snake;
std::pair<int, int>					sweet;
int									random_sweet;
std::vector<std::pair<int, int>>	sweets;
bool								sweets_exist;
int									sweet_counter;
int									sweets_timer;
bool								rainbow;
int 								rainbow_timer;

sf::Texture	none_texture;
sf::Sprite	none;

sf::Texture	wall_texture;
sf::Sprite	wall;

sf::Texture	portal_texture;
sf::Sprite	portal;

sf::Texture	heart_texture;
sf::Sprite	heart;

sf::Texture	body_texture;
sf::Sprite	body;

sf::Texture	vasyan_texture;
sf::Sprite	vasyan;

sf::Texture	head_texture_u[9];
sf::Texture	head_texture_l[9];
sf::Texture	head_texture_d[9];
sf::Texture	head_texture_r[9];

sf::Sprite	head;

sf::Texture	candy_texture[5];

sf::Sprite	candy;

sf::Texture	candies_texture[4];

sf::Sprite	candies;

sf::SoundBuffer	chavk_sb;
sf::Sound		chavk;

sf::SoundBuffer	oof_sb;
sf::Sound		oof;

sf::SoundBuffer	boyyy_sb;
sf::Sound		boyyy;

sf::SoundBuffer	nebo_sb;
sf::Sound		nebo;

sf::SoundBuffer	sweets_sb;
sf::Sound		swts;

sf::SoundBuffer	menu1_sb;
sf::Sound		menu1;

sf::SoundBuffer	menu2_sb;
sf::Sound		menu2;

sf::SoundBuffer	tsok_sb;
sf::Sound		tsok;

sf::SoundBuffer	krutoi_sb;
sf::Sound		krutoi;

sf::SoundBuffer	pobeda_sb;
sf::Sound		pobeda;

sf::SoundBuffer	trash_sb;
sf::Sound		trash;

sf::SoundBuffer	lvl_up_sb;
sf::Sound		lvl_up;

sf::SoundBuffer	quit_sb;
sf::Sound		quit;

sf::SoundBuffer	pause_sb;
sf::Sound		ps;

sf::SoundBuffer	unpause_sb;
sf::Sound		unps;

sf::Font		score_font;
sf::Text		score_text;

sf::Font		gameover_font;
sf::Text		gameover_text;

sf::Text		total_score_text;

sf::Font		greetings_font;
sf::Text		greetings_text;

sf::Font		menu_font;
std::vector<sf::Text>	menu_text;
sf::Text		control_menu[5];
std::vector<sf::Text>	settings_text;
std::vector<sf::Text>	game_type_text;
std::vector<sf::Text>	lvl_text;

void	set_box() {
	for (int i = 0; i < x; ++i) {
		walls.emplace_back(i, 0);
		walls.emplace_back(i, y - 1);
	}
	for (int i = 1; i < y - 1; ++i) {
		walls.emplace_back(0, i);
		walls.emplace_back(x - 1, i);
	}
}

void	set_tunnel() {
	for (int i = 0; i < 6; ++i) {
		walls.emplace_back(i, 0);
		walls.emplace_back(x - 1 - i, 0);
		walls.emplace_back(i, y - 1);
		walls.emplace_back(x - 1 - i, y - 1);
	}
	for (int i = 0; i < 22; ++i) {
		walls.emplace_back(7 + i, 6);
		walls.emplace_back(7 + i, y - 7);
	}
	for (int i = 1; i < 6; ++i) {
		walls.emplace_back(0, i);
		walls.emplace_back(0, y - 1 - i);
		walls.emplace_back(x - 1, i);
		walls.emplace_back(x - 1, y - 1 - i);
	}
}

void	set_mill() {
	for (int i = 0; i < 10; ++i) {
		walls.emplace_back(i, y - 3);
		walls.emplace_back(i, y - 4);
		walls.emplace_back(x - 1 - i, 2);
		walls.emplace_back(x - 1 - i, 3);
	}
	for (int i = 0; i < 14; ++i) {
		walls.emplace_back(8, i);
		walls.emplace_back(9, i);
		walls.emplace_back(x - 9, y - 1 - i);
		walls.emplace_back(x - 10, y - 1 - i);
	}
}

void	set_railway() {
	for (int i = 0; i < x; ++i) {
		walls.emplace_back(i, 0);
		walls.emplace_back(i, y - 1);
	}
	for (int i = 0; i < 26; ++i) {
		walls.emplace_back(5 + i, 5);
		walls.emplace_back(5 + i, y - 6);
	}
	for (int i = 1; i < 6; ++i) {
		walls.emplace_back(0, i);
		walls.emplace_back(0, y - 1 - i);
		walls.emplace_back(x - 1, i);
		walls.emplace_back(x - 1, y - 1 - i);
	}
}

void	set_room() {
	for (int i = 0; i < x; ++i) {
		walls.emplace_back(i, 4);
		walls.emplace_back(i, 5);
	}
	for (int i = 0; i < 16; ++i) {
		walls.emplace_back(i, 11);
		walls.emplace_back(i, 12);
		walls.emplace_back(x - 1 - i, 11);
		walls.emplace_back(x - 1 - i, 12);
	}
	for (int i = 0; i < 22; ++i) {
		walls.emplace_back(5 + i, y - 2);
		walls.emplace_back(5 + i, y - 1);
	}
	for (int i = 0; i < 5; ++i) {
		walls.emplace_back(x - 1 - i, y - 2);
		walls.emplace_back(x - 1 - i, y - 1);
	}
	for (int i = 0; i < 4; ++i) {
		walls.emplace_back(14, i);
		walls.emplace_back(15, i);
	}
	for (int i = 0; i < 3; ++i) {
		walls.emplace_back(x - 2, y - 3 - i);
		walls.emplace_back(x - 1, y - 3 - i);
	}
	for (int i = 0; i < 6; ++i) {
		walls.emplace_back(20, y - 3 - i);
		walls.emplace_back(21, y - 3 - i);
	}
}

void	init_game() {
	walls.clear();
	score = 0;
	gameover = false;
	pause = true;
	game_started = false;
	dir = RIGHT;
	snake.clear();
	random_sweet = rand() % 5;
	sweets_exist = false;
	sweet_counter = 0;
	sweets_timer = 30;
	rainbow = false;
	rainbow_timer = 18;
	if (game_type == Classic) {
		snake.emplace_back(x / 2, y / 2);
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end()) sweet = {rand() % x, rand() % y};
	} else if (game_type == Box) {
		set_box();
		snake.emplace_back(x / 2, y / 2);
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || std::find(walls.begin(), walls.end(), sweet) != walls.end()) sweet = {rand() % x, rand() % y};
	} else if (game_type == Tunnel) {
		set_tunnel();
		snake.emplace_back(x / 2, y / 2);
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || std::find(walls.begin(), walls.end(), sweet) != walls.end()) sweet = {rand() % x, rand() % y};
	} else if (game_type == Mill) {
		set_mill();
		snake.emplace_back(x / 2, y / 2);
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || std::find(walls.begin(), walls.end(), sweet) != walls.end()) sweet = {rand() % x, rand() % y};
	} else if (game_type == Railway) {
		set_railway();
		snake.emplace_back(x / 2, y / 2);
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || std::find(walls.begin(), walls.end(), sweet) != walls.end()) sweet = {rand() % x, rand() % y};
	} else if (game_type == Room) {
		set_room();
		snake.emplace_back(x / 2, y / 2 - 2);
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || std::find(walls.begin(), walls.end(), sweet) != walls.end()) sweet = {rand() % x, rand() % y};
	} else if (game_type == Campaign) {
		port = {-1, -1};
		switch (current_lvl) {
			case 0:
				set_box();
				snake.emplace_back(x / 2, y / 2);
				break;
			case 1:
				set_tunnel();
				snake.emplace_back(x / 2, y / 2);
				break;
			case 2:
				set_mill();
				snake.emplace_back(x / 2, y / 2);
				break;
			case 3:
				set_railway();
				snake.emplace_back(x / 2, y / 2);
				break;
			case 4:
				set_room();
				snake.emplace_back(x / 2, y / 2 - 2);
				break;
		}
		sweet = {rand() % x, rand() % y};
		while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || std::find(walls.begin(), walls.end(), sweet) != walls.end()) sweet = {rand() % x, rand() % y};

	}
}

void	set_volume() {
	chavk.setVolume(volume);
	oof.setVolume(volume);
	boyyy.setVolume(volume);
	nebo.setVolume(volume);
	boyyy.setVolume(volume);
	swts.setVolume(volume);
	menu1.setVolume(volume);
	menu2.setVolume(volume);
	tsok.setVolume(volume);
	krutoi.setVolume(volume);
	pobeda.setVolume(volume);
	trash.setVolume(volume);
	lvl_up.setVolume(volume);
	quit.setVolume(volume);
	ps.setVolume(volume);
	unps.setVolume(volume);
}

void	setup() {
	none_texture.loadFromFile("images/none.png");
	none.setTexture(none_texture);

	wall_texture.loadFromFile("images/leaves.png");
	wall.setTexture(wall_texture);

	portal_texture.loadFromFile("images/portal1.png");
	portal.setTexture(portal_texture);

	heart_texture.loadFromFile("images/life.png");
	heart.setTexture(heart_texture);

	body_texture.loadFromFile("images/body.png");
	body.setTexture(body_texture);

	vasyan_texture.loadFromFile("images/vasyan.png");
	vasyan.setTexture(vasyan_texture);

	head_texture_u[0].loadFromFile("images/vasyan-u.png");
	head_texture_l[0].loadFromFile("images/vasyan-l.png");
	head_texture_d[0].loadFromFile("images/vasyan-d.png");
	head_texture_r[0].loadFromFile("images/vasyan-r.png");

	head_texture_u[1].loadFromFile("images/vasyan-u1.png");
	head_texture_l[1].loadFromFile("images/vasyan-l1.png");
	head_texture_d[1].loadFromFile("images/vasyan-d1.png");
	head_texture_r[1].loadFromFile("images/vasyan-r1.png");

	head_texture_u[2].loadFromFile("images/vasyan-u2.png");
	head_texture_l[2].loadFromFile("images/vasyan-l2.png");
	head_texture_d[2].loadFromFile("images/vasyan-d2.png");
	head_texture_r[2].loadFromFile("images/vasyan-r2.png");

	head_texture_u[3].loadFromFile("images/vasyan-u3.png");
	head_texture_l[3].loadFromFile("images/vasyan-l3.png");
	head_texture_d[3].loadFromFile("images/vasyan-d3.png");
	head_texture_r[3].loadFromFile("images/vasyan-r3.png");

	head_texture_u[4].loadFromFile("images/vasyan-u4.png");
	head_texture_l[4].loadFromFile("images/vasyan-l4.png");
	head_texture_d[4].loadFromFile("images/vasyan-d4.png");
	head_texture_r[4].loadFromFile("images/vasyan-r4.png");

	head_texture_u[5].loadFromFile("images/vasyan-u5.png");
	head_texture_l[5].loadFromFile("images/vasyan-l5.png");
	head_texture_d[5].loadFromFile("images/vasyan-d5.png");
	head_texture_r[5].loadFromFile("images/vasyan-r5.png");

	head_texture_u[6].loadFromFile("images/vasyan-u6.png");
	head_texture_l[6].loadFromFile("images/vasyan-l6.png");
	head_texture_d[6].loadFromFile("images/vasyan-d6.png");
	head_texture_r[6].loadFromFile("images/vasyan-r6.png");

	head_texture_u[7].loadFromFile("images/vasyan-u7.png");
	head_texture_l[7].loadFromFile("images/vasyan-l7.png");
	head_texture_d[7].loadFromFile("images/vasyan-d7.png");
	head_texture_r[7].loadFromFile("images/vasyan-r7.png");

	head_texture_u[8].loadFromFile("images/vasyan-u8.png");
	head_texture_l[8].loadFromFile("images/vasyan-l8.png");
	head_texture_d[8].loadFromFile("images/vasyan-d8.png");
	head_texture_r[8].loadFromFile("images/vasyan-r8.png");

	candy_texture[0].loadFromFile("images/candy1.png");
	candy_texture[1].loadFromFile("images/candy2.png");
	candy_texture[2].loadFromFile("images/candy3.png");
	candy_texture[3].loadFromFile("images/candy4.png");
	candy_texture[4].loadFromFile("images/candy5.png");

	candies_texture[0].loadFromFile("images/candies1.png");
	candies_texture[1].loadFromFile("images/candies2.png");
	candies_texture[2].loadFromFile("images/candies3.png");
	candies_texture[3].loadFromFile("images/candies4.png");

	chavk_sb.loadFromFile("sounds/chavk.wav");
	chavk.setBuffer(chavk_sb);

	oof_sb.loadFromFile("sounds/oof.wav");
	oof.setBuffer(oof_sb);

	boyyy_sb.loadFromFile("sounds/american boy.wav");
	boyyy.setBuffer(boyyy_sb);

	nebo_sb.loadFromFile("sounds/nebo v oblakah.wav");
	nebo.setBuffer(nebo_sb);

	sweets_sb.loadFromFile("sounds/tun tun.wav");
	swts.setBuffer(sweets_sb);

	menu1_sb.loadFromFile("sounds/menu1.wav");
	menu1.setBuffer(menu1_sb);

	menu2_sb.loadFromFile("sounds/menu2.wav");
	menu2.setBuffer(menu2_sb);

	tsok_sb.loadFromFile("sounds/tsok.wav");
	tsok.setBuffer(tsok_sb);

	krutoi_sb.loadFromFile("sounds/ya krutoi.wav");
	krutoi.setBuffer(krutoi_sb);

	pobeda_sb.loadFromFile("sounds/pobeda.wav");
	pobeda.setBuffer(pobeda_sb);

	trash_sb.loadFromFile("sounds/trasssshhh.wav");
	trash.setBuffer(trash_sb);

	lvl_up_sb.loadFromFile("sounds/lvl up.wav");
	lvl_up.setBuffer(lvl_up_sb);

	quit_sb.loadFromFile("sounds/quit.wav");
	quit.setBuffer(quit_sb);

	pause_sb.loadFromFile("sounds/pause.wav");
	ps.setBuffer(pause_sb);

	unpause_sb.loadFromFile("sounds/unpause.wav");
	unps.setBuffer(unpause_sb);

	set_volume();

	score_font.loadFromFile("fonts/a_lcdnova.ttf");
	score_text.setFont(score_font);
	score_text.setCharacterSize(50);
	score_text.setFillColor(sf::Color(0, 0, 0));

	gameover_font.loadFromFile("fonts/Gouranga-Pixel.ttf");
	gameover_text.setFont(gameover_font);
	total_score_text.setFont(gameover_font);

	gameover_text.setString("GAMEOVER");
	gameover_text.setCharacterSize(100);
	gameover_text.setFillColor(sf::Color(255, 0, 0));
	gameover_text.setOutlineThickness(2);
	gameover_text.setOutlineColor(sf::Color(0, 0, 0));
	gameover_text.setPosition((width - gameover_text.getLocalBounds().width) / 2, (height - gameover_text.getLocalBounds().height) / 2);

	total_score_text.setCharacterSize(100);
	total_score_text.setFillColor(sf::Color(255, 0, 0));
	total_score_text.setOutlineThickness(2);
	total_score_text.setOutlineColor(sf::Color(0, 0, 0));

	greetings_font.loadFromFile("fonts/mostwaztedrusbylyajka.otf");
	greetings_text.setFont(greetings_font);

	greetings_text.setString("VASYAN");
	greetings_text.setCharacterSize(200);
	greetings_text.setFillColor(sf::Color(135, 21, 173));
	greetings_text.setOutlineThickness(4);
	greetings_text.setOutlineColor(sf::Color(255, 255, 255));
	greetings_text.setPosition((width - greetings_text.getLocalBounds().width) / 2 - 80, (height - greetings_text.getLocalBounds().height) / 2);
	greetings_text.setRotation(340);

	menu_font.loadFromFile("fonts/Gouranga-Pixel.ttf");
	for (int i = 0; i < main_menu.size(); ++i) {
		menu_text.emplace_back(sf::Text());
		menu_text.back().setFont(menu_font);
		menu_text.back().setString(main_menu[i]);
		menu_text.back().setCharacterSize(menu_char_size);
	}

	control_menu[0].setString("W - Up");
	control_menu[1].setString("A - Left");
	control_menu[2].setString("S - Down");
	control_menu[3].setString("D - Right");
	control_menu[4].setString("Esc - Pause/Play");

	for (int i = 0; i < 5; ++i) {
		control_menu[i].setFont(menu_font);
		control_menu[i].setCharacterSize(menu_char_size);
	}

	for (int i = 0; i < settings_menu.size(); ++i) {
		settings_text.emplace_back(sf::Text());
		settings_text.back().setFont(menu_font);
		settings_text.back().setString(settings_menu[i]);
		settings_text.back().setCharacterSize(menu_char_size);
	}

	for (int i = 0; i < game_type_menu.size(); ++i) {
		game_type_text.emplace_back(sf::Text());
		game_type_text.back().setFont(menu_font);
		game_type_text.back().setString(game_type_menu[i]);
		game_type_text.back().setCharacterSize(menu_char_size);
	}

	for (int i = 0; i < lvl.size(); ++i) {
		lvl_text.emplace_back(sf::Text());
		lvl_text.back().setFont(menu_font);
		lvl_text.back().setString(lvl[i]);
		lvl_text.back().setCharacterSize(menu_char_size);
	}

	init_game();
	for (int i = 0; i < 4; ++i) sweets.emplace_back(-1, -1);
}

void	draw_field(sf::RenderWindow	&window) {
	std::pair<int, int>	xy;
	for (int j = 0; j < y; ++j) {
		for (int i = 0; i < x; ++i) {
			xy = {i, j};
			if (snake[0] == xy) {
				if (dir == UP) {
					if (rainbow) {
						head.setTexture(head_texture_u[rainbow_timer % 9]);
					} else head.setTexture(head_texture_u[0]);
				} else if (dir == LEFT) {
					if (rainbow) {
						head.setTexture(head_texture_l[rainbow_timer % 9]);
					} else head.setTexture(head_texture_l[0]);
				} else if (dir == DOWN) {
					if (rainbow) {
						head.setTexture(head_texture_d[rainbow_timer % 9]);
					} else head.setTexture(head_texture_d[0]);
				} else {
					if (rainbow) {
						head.setTexture(head_texture_r[rainbow_timer % 9]);
					} else head.setTexture(head_texture_r[0]);
				}
				head.setPosition((float)i * cell, (float)j * cell + score_bar_height);
				window.draw(head);
			} else if (std::find(snake.begin(), snake.end(), xy) != snake.end()) {
				body.setPosition((float)i * cell, (float)j * cell + score_bar_height);
				window.draw(body);
			} else if (sweet == xy) {
				candy.setTexture(candy_texture[random_sweet]);
				candy.setPosition((float)i * cell, (float)j * cell + score_bar_height);
				window.draw(candy);
			} else if (sweets_exist && std::find(sweets.begin(), sweets.end(), xy) != sweets.end()) {
				if (xy == sweets[0]) candies.setTexture(candies_texture[0]);
				else if (xy == sweets[1]) candies.setTexture(candies_texture[1]);
				else if (xy == sweets[2]) candies.setTexture(candies_texture[2]);
				else if (xy == sweets[3]) candies.setTexture(candies_texture[3]);
				candies.setPosition((float)i * cell, (float)j * cell + score_bar_height);
				window.draw(candies);
			} else if (std::find(walls.begin(), walls.end(), xy) != walls.end()) {
				wall.setPosition((float)i * cell, (float)j * cell + score_bar_height);
				window.draw(wall);
			} else if (xy == port) {
				portal.setPosition((float )i * cell, (float)j * cell + score_bar_height);
				window.draw(portal);
			} else {
				none.setPosition((float)i * cell, (float)j * cell + score_bar_height);
				window.draw(none);
			}
		}
	}
}

void	draw_score_bar(sf::RenderWindow &window) {
	if (game_type == Campaign) {
		score_text.setString("Score: " + std::to_string(total_score + score));
		sf::RectangleShape	candies_outline1(sf::Vector2f(12 * cell, 1));
		sf::RectangleShape	candies_outline2(sf::Vector2f(1, 32));
		sf::RectangleShape	candies_bar(sf::Vector2f(sweet_counter * cell, 32));
		candies_bar.setPosition(0, 0);
		candies_bar.setFillColor(sf::Color(209, 17, 110));
		window.draw(candies_bar);
		candies_outline1.setFillColor(sf::Color(0, 0, 0));
		candies_outline2.setFillColor(sf::Color(0, 0, 0));
		candies_outline1.setPosition(0, 0);
		candies_outline2.setPosition(0, 0);
		window.draw(candies_outline1);
		window.draw(candies_outline2);
		candies_outline1.setPosition(0, 31);
		candies_outline2.setPosition(12 * cell - 1, 0);
		window.draw(candies_outline1);
		window.draw(candies_outline2);
		for (int i = 0; i < hearts; ++i) {
			heart.setPosition((12 + i) * cell + 10 + (5 * i), 0);
			window.draw(heart);
		}
		if (heartbreak_timer > 0 && heartbreak_timer % 2 == 0) {
			heart.setPosition((12 + hearts) * cell + 10 + (5 * (hearts)), 0);
			window.draw(heart);
		}
	} else score_text.setString("Score: " + std::to_string(score));
	score_text.setPosition(width - score_text.getLocalBounds().width - 20, 2);
	window.draw(score_text);
	sf::RectangleShape	del(sf::Vector2f(width, 1));
	del.setPosition(0, 64);
	del.setFillColor(sf::Color(0, 0, 0));
	window.draw(del);
	if (sweets_exist) {
		sf::RectangleShape sweets_bar(sf::Vector2f(sweets_timer * 9.6, 32));
		sweets_bar.setPosition(0, cell);
		sweets_bar.setFillColor(sf::Color(69, 141, 230));
		window.draw(sweets_bar);
	}
}

void	draw_menu(sf::RenderWindow &window) {
	float				interval = 20;
	float				menu_items_x = 0;
	float				menu_items_y = 0;

	for (int i = 0; i < menu_text.size(); ++i) {
		menu_text[i].setPosition(0, menu_items_y);
		menu_text[i].setFillColor(main_item == i ? sf::Color(0, 0, 0, 255) : sf::Color(0, 0, 0, 150));
		menu_items_x = std::max(menu_items_x, menu_text[i].getLocalBounds().width);
		menu_items_y += menu_text[i].getLocalBounds().height + interval;
	}

	const float			menu_w = menu_items_x + interval * 2;
	const float			menu_h = menu_items_y + interval * 2;
	const float			menu_x = (width - menu_w) / 2;
	const float			menu_y = (height - menu_h) / 2;
	sf::RectangleShape	menu(sf::Vector2f(menu_w, menu_h));

	menu.setPosition(menu_x, menu_y);
	menu.setFillColor(sf::Color(255, 255, 255, 150));
	window.draw(menu);

	for (int i = 0; i < menu_text.size(); ++i) {
		menu_text[i].move(menu_x + 10, menu_y);
		window.draw(menu_text[i]);
	}
}

void	draw_control_menu(sf::RenderWindow &window) {
	float				interval = 20;
	float				menu_items_x = 0;
	float				menu_items_y = 0;

	for (int i = 0; i < 5; ++i) {
		control_menu[i].setPosition(0, menu_items_y);
		control_menu[i].setFillColor(sf::Color(0, 0, 0, 255));
		menu_items_x = std::max(menu_items_x, control_menu[i].getLocalBounds().width);
		menu_items_y += control_menu[i].getLocalBounds().height + interval;
	}

	const float			menu_w = menu_items_x + interval * 2;
	const float			menu_h = menu_items_y + interval * 2;
	const float			menu_x = (width - menu_w) / 2;
	const float			menu_y = (height - menu_h) / 2;
	sf::RectangleShape	menu(sf::Vector2f(menu_w, menu_h));

	menu.setPosition(menu_x, menu_y);
	menu.setFillColor(sf::Color(255, 255, 255, 150));
	window.draw(menu);

	for (int i = 0; i < 5; ++i) {
		control_menu[i].move(menu_x + 10, menu_y);
		window.draw(control_menu[i]);
	}
}

void	draw_settings_menu(sf::RenderWindow &window) {
	float				interval = 20;
	float				menu_items_x = 0;
	float				menu_items_y = 0;


	if (settings_menu[main_item] == "Volume") {
		settings_text[main_item].setString(settings_menu[main_item] + ": " + std::to_string(volume));
	} else if (settings_menu[main_item] == "Speed") {
		settings_text[main_item].setString(settings_menu[main_item] + ": " + std::to_string(speed));
	}
	for (int i = 0; i < settings_text.size(); ++i) {
		settings_text[i].setPosition(0, menu_items_y);
		settings_text[i].setFillColor(main_item == i ? sf::Color(0, 0, 0, 255) : sf::Color(0, 0, 0, 150));
		menu_items_x = std::max(menu_items_x, settings_text[i].getLocalBounds().width);
		menu_items_y += settings_text[i].getLocalBounds().height + interval;
	}

	const float			menu_w = menu_items_x + interval * 2;
	const float			menu_h = menu_items_y + interval * 2;
	const float			menu_x = (width - menu_w) / 2;
	const float			menu_y = (height - menu_h) / 2;
	sf::RectangleShape	menu(sf::Vector2f(menu_w, menu_h));

	menu.setPosition(menu_x, menu_y);
	menu.setFillColor(sf::Color(255, 255, 255, 150));
	window.draw(menu);

	for (int i = 0; i < settings_text.size(); ++i) {
		settings_text[i].move(menu_x + 10, menu_y);
		window.draw(settings_text[i]);
	}
	if (settings_menu[main_item] == "Volume") {
		settings_text[main_item].setString(settings_menu[main_item]);
	} else if (settings_menu[main_item] == "Speed") {
		settings_text[main_item].setString(settings_menu[main_item]);
	}
}

void	draw_game_type_menu(sf::RenderWindow &window) {
	float				interval = 20;
	float				menu_items_x = 0;
	float				menu_items_y = 0;

	for (int i = 0; i < game_type_text.size(); ++i) {
		game_type_text[i].setPosition(0, menu_items_y);
		game_type_text[i].setFillColor(main_item == i ? sf::Color(0, 0, 0, 255) : sf::Color(0, 0, 0, 150));
		menu_items_x = std::max(menu_items_x, game_type_text[i].getLocalBounds().width);
		menu_items_y += game_type_text[i].getLocalBounds().height + interval;
	}

	const float			menu_w = menu_items_x + interval * 2;
	const float			menu_h = menu_items_y + interval * 2;
	const float			menu_x = (width - menu_w) / 2;
	const float			menu_y = (height - menu_h) / 2;
	sf::RectangleShape	menu(sf::Vector2f(menu_w, menu_h));

	menu.setPosition(menu_x, menu_y);
	menu.setFillColor(sf::Color(255, 255, 255, 150));
	window.draw(menu);

	for (int i = 0; i < game_type_text.size(); ++i) {
		game_type_text[i].move(menu_x + 10, menu_y);
		window.draw(game_type_text[i]);
	}
}

void	draw_lvl_menu(sf::RenderWindow &window) {
	float				interval = 20;
	float				menu_items_x = 0;
	float				menu_items_y = 0;

	for (int i = 0; i < lvl_text.size(); ++i) {
		lvl_text[i].setPosition(0, menu_items_y);
		lvl_text[i].setFillColor(main_item == i ? sf::Color(0, 0, 0, 255) : sf::Color(0, 0, 0, 150));
		menu_items_x = std::max(menu_items_x, lvl_text[i].getLocalBounds().width);
		menu_items_y += lvl_text[i].getLocalBounds().height + interval;
	}

	const float			menu_w = menu_items_x + interval * 2;
	const float			menu_h = menu_items_y + interval * 2;
	const float			menu_x = (width - menu_w) / 2;
	const float			menu_y = (height - menu_h) / 2;
	sf::RectangleShape	menu(sf::Vector2f(menu_w, menu_h));

	menu.setPosition(menu_x, menu_y);
	menu.setFillColor(sf::Color(255, 255, 255, 150));
	window.draw(menu);

	for (int i = 0; i < lvl_text.size(); ++i) {
		lvl_text[i].move(menu_x + 10, menu_y);
		window.draw(lvl_text[i]);
	}
}

void	set_sweets() {
	sweets[1] = {sweets[0].first + 1, sweets[0].second};
	sweets[2] = {sweets[0].first, sweets[0].second + 1};
	sweets[3] = {sweets[0].first + 1, sweets[0].second + 1};
}

void	move(bool pop_back) {
	switch (dir) {
		case UP:
			if (snake[0].second == 0) snake.insert(snake.begin(), {snake[0].first, y - 1});
			else snake.insert(snake.begin(), {snake[0].first, snake[0].second - 1});
			break;
		case LEFT:
			if (snake[0].first == 0) snake.insert(snake.begin(), {x - 1, snake[0].second});
			else snake.insert(snake.begin(), {snake[0].first - 1, snake[0].second});
			break;
		case DOWN:
			if (snake[0].second == y - 1) snake.insert(snake.begin(), {snake[0].first, 0});
			else snake.insert(snake.begin(), {snake[0].first, snake[0].second + 1});
			break;
		case RIGHT:
			if (snake[0].first == x - 1) snake.insert(snake.begin(), {0, snake[0].second});
			else snake.insert(snake.begin(), {snake[0].first + 1, snake[0].second});
			break;
	}
	if (pop_back) snake.pop_back();
}

void	logic() {
	if (sweets_exist && --sweets_timer == 0) sweets_exist = false;
	if (rainbow && --rainbow_timer == 0) rainbow = false;
	if (snake[0] == sweet) {
		chavk.play();
		score += speed;
		sweet_counter++;
		if (game_type == Campaign && sweet_counter == 12) {
			sweet = {-1, -1};
			port = {rand() % x, rand() % y};
			while (std::find(snake.begin(), snake.end(), port) != snake.end() || std::find(walls.begin(), walls.end(), port) != walls.end()) port = {rand() % x, rand() % y};
		} else {
			sweet = {rand() % x, rand() % y};
			while (std::find(snake.begin(), snake.end(), sweet) != snake.end() || (game_type != Classic && std::find(walls.begin(), walls.end(), sweet) != walls.end())) sweet = {rand() % x, rand() % y};
			random_sweet = rand() % 5;
			move(false);
			if (sweet_counter % 5 == 0) {
				sweets[0] = {rand() % (x - 1), rand() % (y - 1)};
				set_sweets();
				while (std::find(sweets.begin(), sweets.end(), sweet) != sweets.end() || std::find(snake.begin(), snake.end(), sweets[0]) != snake.end() || std::find(snake.begin(), snake.end(), sweets[1]) != snake.end() || std::find(snake.begin(), snake.end(), sweets[2]) != snake.end() || std::find(snake.begin(), snake.end(), sweets[3]) != snake.end() || (game_type != Classic && (std::find(walls.begin(), walls.end(), sweets[0]) != walls.end() || std::find(walls.begin(), walls.end(), sweets[1]) != walls.end() || std::find(walls.begin(), walls.end(), sweets[2]) != walls.end() || std::find(walls.begin(), walls.end(), sweets[3]) != walls.end()))) {
					sweets[0] = {rand() % (x - 1), rand() % (y - 1)};
					set_sweets();
				}
				sweets_exist = true;
				sweets_timer = 40;
			}
		}
	} else if (sweets_exist && std::find(sweets.begin(), sweets.end(), snake[0]) != sweets.end()) {
		krutoi.play();
		score += sweets_timer * speed;
		sweets_exist = false;
		move(false);
		rainbow = true;
		rainbow_timer = 18;
	} else if (snake[0] == port) {
		total_score += score;
		if (++current_lvl == 5) {
			campaign_done = true;
			pause = true;
		} else {
			lvl_up.play();
			port = {-1, -1};
			init_game();
			pause = false;
			game_started = true;
		}

	} else if (std::find(snake.begin() + 1, snake.end(), snake[0]) != snake.end() || (game_type != Classic && std::find(walls.begin(), walls.end(), snake[0]) != walls.end())) {
		oof.play();
		if (game_type == Campaign) {
			if (--hearts == -1) gameover = true;
			else heartbreak_timer = 10;
		}
		else gameover = true;
	} else { move(true); }
}

int		main() {
	srand(time(nullptr));

	sf::RenderWindow	window(sf::VideoMode(width, height), "VASYAN", sf::Style::Close);

	std::vector<e_dir> dir_queue;

	setup();
	while (window.isOpen()) {
		sf::Event	event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				quit.play();
				sf::sleep(sf::milliseconds(500));
				window.close();
			}
			if (event.type == sf::Event::KeyPressed && !greetings) {
				if (pause) {
					if (current_menu == MainMenu) {
						switch (event.key.code) {
							case sf::Keyboard::Up:
								menu1.play();
								if (--main_item < 0) main_item = main_menu.size() - 1;
								break;
							case sf::Keyboard::Down:
								menu1.play();
								if (++main_item == main_menu.size()) main_item = 0;
								break;
							case sf::Keyboard::Escape:
								if (game_started && !gameover) {
									unps.play();
									pause = false;
								}
								break;
							case sf::Keyboard::Enter:
								if (main_menu[main_item] == "New Game") {
									tsok.play();
									current_lvl = 0;
									hearts = 3;
									init_game();
									pause = false;
									game_started = true;
								} else if (main_menu[main_item] == "Settings") {
									tsok.play();
									current_menu = SettingsMenu;
									main_item = 0;
								} else if (main_menu[main_item] == "Control") {
									tsok.play();
									current_menu = ControlMenu;
									main_item = 0;
								} else if (main_menu[main_item] == "Quit") {
									quit.play();
									sf::sleep(sf::milliseconds(500));
									window.close();
								}
								break;
						}
					} else if (current_menu == SettingsMenu) {
						switch (event.key.code) {
							case sf::Keyboard::Up:
								menu1.play();
								if (--main_item < 0) main_item = settings_menu.size() - 1;
								break;
							case sf::Keyboard::Down:
								menu1.play();
								if (++main_item == settings_menu.size()) main_item = 0;
								break;
							case sf::Keyboard::Left:
								menu1.play();
								if (settings_menu[main_item] == "Speed" && sleep_time < 500) {
									sleep_time += 50;
									--speed;
									pause = true;
									game_started = false;
								}
								if (settings_menu[main_item] == "Volume" && volume > 0) {
									volume -= 5;
									set_volume();
								}
									break;
							case sf::Keyboard::Right:
								menu1.play();
								if (settings_menu[main_item] == "Speed" && sleep_time > 50) {
									sleep_time -= 50;
									++speed;
									pause = true;
									game_started = false;
								}
								if (settings_menu[main_item] == "Volume" && volume < 100) {
									volume += 5;
									set_volume();
								}
									break;
							case sf::Keyboard::Escape:
								quit.play();
								current_menu = MainMenu;
								main_item = 0;
								break;
							case sf::Keyboard::Enter:
								if (settings_menu[main_item] == "Game Type") {
									tsok.play();
									current_menu = GameTypeMenu;
									main_item = 0;
								}
								break;
						}
					} else if (current_menu == ControlMenu && event.key.code == sf::Keyboard::Escape) {
						quit.play();
						current_menu = MainMenu;
						main_item = 0;
					} else if (current_menu == GameTypeMenu) {
						switch (event.key.code) {
							case sf::Keyboard::Up:
								menu1.play();
								if (--main_item < 0) main_item = game_type_menu.size() - 1;
								break;
							case sf::Keyboard::Down:
								menu1.play();
								if (++main_item == game_type_menu.size()) main_item = 0;
								break;
							case sf::Keyboard::Escape:
								quit.play();
								current_menu = SettingsMenu;
								main_item = 0;
								break;
							case sf::Keyboard::Enter:
								tsok.play();
								pause = true;
								game_started = false;
								if (game_type_menu[main_item] == "Classic") game_type = Classic;
								else if (game_type_menu[main_item] == "New") {
									current_menu = LvlMenu;
									main_item = 0;
								} else if (game_type_menu[main_item] == "Campaign") {
									game_type = Campaign;
									current_lvl = 0;
								}
//								init_game();
								break;
						}
					} else if (current_menu == LvlMenu) {
						switch (event.key.code) {
							case sf::Keyboard::Up:
								menu1.play();
								if (--main_item < 0) main_item = lvl.size() - 1;
								break;
							case sf::Keyboard::Down:
								menu1.play();
								if (++main_item == lvl.size()) main_item = 0;
								break;
							case sf::Keyboard::Escape:
								quit.play();
								current_menu = GameTypeMenu;
								main_item = 0;
								break;
							case sf::Keyboard::Enter:
								tsok.play();
								pause = true;
								game_started = false;
								if (lvl[main_item] == "Box") game_type = Box;
								else if (lvl[main_item] == "Tunnel") game_type = Tunnel;
								else if (lvl[main_item] == "Mill") game_type = Mill;
								else if (lvl[main_item] == "Railway") game_type = Railway;
								else if (lvl[main_item] == "Room") game_type = Room;
								break;
						}
					}
				} else {
					e_dir snake_direction_last = dir_queue.empty() ? dir : dir_queue[0];
					switch (event.key.code) {
						case sf::Keyboard::W:
							if (snake_direction_last != UP && snake_direction_last != DOWN &&
								dir_queue.size() < 3)
								dir_queue.insert(dir_queue.begin(), UP);
							break;
						case sf::Keyboard::A:
							if (snake_direction_last != LEFT && snake_direction_last != RIGHT &&
								dir_queue.size() < 3)
								dir_queue.insert(dir_queue.begin(), LEFT);
							break;
						case sf::Keyboard::S:
							if (snake_direction_last != DOWN && snake_direction_last != UP &&
								dir_queue.size() < 3)
								dir_queue.insert(dir_queue.begin(), DOWN);
							break;
						case sf::Keyboard::D:
							if (snake_direction_last != LEFT && snake_direction_last != RIGHT &&
								dir_queue.size() < 3)
								dir_queue.insert(dir_queue.begin(), RIGHT);
							break;
						case sf::Keyboard::Escape:
							ps.play();
							pause = true;
							break;
					}
				}
			}
		}
		if (!dir_queue.empty()) {
			dir = dir_queue.back();
			dir_queue.pop_back();
		}
		window.clear(sf::Color(175, 212, 126));
		draw_field(window);
		draw_score_bar(window);
		if (!pause && !gameover) {
			logic();
			sf::sleep(sf::milliseconds(sleep_time));
		}
		if (campaign_done) {
			current_lvl = 0;
			hearts = 3;
			total_score_text.setString("TOTAL SCORE: " + std::to_string(total_score));
			total_score_text.setPosition((width - total_score_text.getLocalBounds().width) / 2, (height - total_score_text.getLocalBounds().height) / 2);
			window.draw(total_score_text);
			window.display();
			pobeda.play();
			sf::sleep(sf::milliseconds(1500));
			total_score = 0;
			campaign_done = false;
		}
		if (heartbreak_timer > 0) {
			while (--heartbreak_timer > 0) {
				draw_score_bar(window);
				window.display();
				sf::sleep(sf::milliseconds(150));
			}
			init_game();
			pause = false;
			game_started = true;
		}
		if (gameover && !pause) {
			if (game_type == Campaign) {
				current_lvl = 0;
				hearts = 3;
				total_score = 0;
			}
			window.draw(gameover_text);
			window.display();
			game_started = false;
			pause = true;
			sf::sleep(sf::seconds(1));
			nebo.play();
			sf::sleep(sf::seconds(4));
		}
		if (greetings) {
			window.draw(greetings_text);
			vasyan.setPosition(greetings_text.getLocalBounds().width, (height - vasyan.getLocalBounds().height) / 2);
			window.draw(vasyan);
			window.display();
			boyyy.play();
			sf::sleep(sf::seconds(3));
			greetings = false;
		}
		if (pause) {
			switch (current_menu) {
				case MainMenu:
					draw_menu(window);
					break;
				case SettingsMenu:
					draw_settings_menu(window);
					break;
				case ControlMenu:
					draw_control_menu(window);
					break;
				case GameTypeMenu:
					draw_game_type_menu(window);
					break;
				case LvlMenu:
					draw_lvl_menu(window);
					break;
			}
		}
		window.display();
	}
}
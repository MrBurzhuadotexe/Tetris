#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>



using namespace std;
using  namespace sf;


const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point {
	int x, y;
} a[4], b[4];

int shapes[7][4] =
{
	1, 3, 5, 7, // I
	2, 4, 5, 7, // Z
	3, 5, 4, 6, // S
	3, 5, 4, 7, // T
	2, 3, 5, 7, // L
	3, 5, 7, 6, // J
	2, 3, 4, 5 // O
};

bool check() {
	for (int i = 0; i < 4; i++) {
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) {
			return 0;
		}
		else if (field[a[i].y][a[i].x]) {
			return 0;
		}
	}
	return 1;
};
void move(int dx) {
	for (int i = 0; i < 4; i++) {
		b[i] = a[i];
		a[i].x += dx;
	}
	if (!check()) {
		for (int i = 0; i < 4; i++) {
			a[i] = b[i];
		}
	}
}
void rotateBlock(bool rotate) {
	if (rotate) {
		Point p = a[1]; //centre of rotation
		for (int i = 0; i < 4; i++) {
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}
		if (!check()) {
			for (int i = 0; i < 4; i++) {
				a[i] = b[i];
			}
		}
	}
}


int main() {


	srand(time(0));

	RenderWindow window(VideoMode(560, 760), "Tetris!");
	Texture t1, t2, t3;
	t1.loadFromFile("C:/D/homework/TetrisGame/images/tiles3.png");
	t2.loadFromFile("C:/D/homework/TetrisGame/images/background2.png");
	t3.loadFromFile("C:/D/homework/TetrisGame/images/frame.png");
	Sprite s(t1), background(t2), frame(t3);

	int dx = 0;
	bool rotate = 0;
	int colorNum = 0;
	float timer = 0, delay = 0.3;
	int score = 0;
	bool gameover = false;
	string topscore;

	Clock clock;
	Font font;

	//loading textures
	if (!font.loadFromFile("C:/D/homework/TetrisGame/fonts/font.ttf")) {
		cerr << "Failed to load font!" << endl;
		return -1;
	}


	ifstream file;
	file.open("C:/D/homework/TetrisGame/topscore.txt");
	getline(file, topscore);
	file.close();

	Text scoreText, GameOver, TopScore;
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(470, 180);

	GameOver.setFont(font);
	GameOver.setPosition(130, 20);
	
	GameOver.setStyle(sf::Text::Bold);
	GameOver.setCharacterSize(50);
	GameOver.setOutlineThickness(5);

	TopScore.setFont(font);
	TopScore.setCharacterSize(30);
	TopScore.setString(topscore);
	TopScore.setFillColor(Color::White);
	TopScore.setPosition(470, 300);

	while (window.isOpen()) {
		
	    float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		for (int i = 0; i < 4; i++) {
			if (field[a[i].y][a[i].x] != 0) {
				gameover = true;
				break;
			}
		}

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Up) {
					rotate = true;
				}
				else if (e.key.code == Keyboard::Left) {
					dx = -1;
				}
				else if (e.key.code == Keyboard::Right) {
					dx = 1;
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			delay = 0.05;
		}

		move(dx);
		
		rotateBlock(rotate);

		//tick
		if (timer > delay) {
			
			for (int i = 0; i < 4; i++) {
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check()) {

				for (int i = 0; i < 4; i++) {
					field[b[i].y][b[i].x] = colorNum;
				}

				// Update score based on the number of lines cleared
				int linesCleared = 0;
				for (int i = 0; i < M; i++) {
					int count = 0;
					for (int j = 0; j < N; j++) {
						if (field[i][j]) {
							count++;
						}
					}
					if (count == N) {
						linesCleared++;
						for (int k = i; k > 0; k--) {
							for (int j = 0; j < N; j++) {
								field[k][j] = field[k - 1][j];
							}
						}
						for (int j = 0; j < N; j++) {
							field[0][j] = 0;
						}
					}
				}

				// Update score based on the number of lines cleared
				if (linesCleared > 0) {
					score += linesCleared;
					cout << score;
				}

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				
				for (int i = 0; i < 4; i++) {
					a[i].x = shapes[n][i] % 2 + 4;
					a[i].y = shapes[n][i] / 2;
				}
			}

			timer = 0;
		}
		//check lines
		int k = M - 1;
		for (int i = M - 1; i > 0; i--) {
			int count = 0;
			for (int j = 0; j < N; j++) {
				if (field[i][j]) {
					count++;
				}
				field[k][j] = field[i][j];
			}
			if (count < N) {
				k--;
				
			}
	
		}
		


		dx = 0; rotate = 0;
		delay = 0.3;

		//draw
		
		window.draw(background);
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				if (field[i][j] == 0) {
					continue;
				}
				s.setTextureRect(IntRect(field[i][j] * 36, 0, 36, 36));
				s.setPosition(j * 36, i * 36);
				s.move(30, 31); //offset
				window.draw(s);
			}
		}

		for (int i = 0; i < 4; i++) {
			s.setTextureRect(IntRect(colorNum * 36, 0, 36, 36));
			s.setPosition(a[i].x * 36, a[i].y * 36);
			s.move(30, 31); //offset
			window.draw(s);
		}

		// Render the score
		scoreText.setString(to_string(score));
		window.draw(scoreText);
		window.draw(TopScore);
		if (gameover) {
			
			break;
		}
		window.display();
	}
	if (stoi(topscore) < score) {
		std::ofstream file("C:/D/homework/TetrisGame/topscore.txt", std::ofstream::trunc);
		file << to_string(score);

	}
	GameOver.setString("GAME OVER!\n your score:" + to_string(score));
	RenderWindow gameoverScreen(VideoMode(560, 200), "Game Over!");
	while (gameoverScreen.isOpen()) {
		gameoverScreen.clear(Color::Black);
		gameoverScreen.draw(GameOver);
		gameoverScreen.display();
	}
	
	return 0;
}

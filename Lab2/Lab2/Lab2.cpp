#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>
#include <windows.h>
using namespace std;
// Загальні налаштування
const int AREA_WIDTH = 100;
const int AREA_HEIGHT = 25;
const int NUM_RABBITS = 10;
const int NUM_ALBINOS = 7;
const int CHANGE_DIRECTION_INTERVAL = 3; 
const int VELOCITY = 1;

mutex mtx;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(0, 3);

class Rabbit {
public:
	int x, y;

};
class Albino :public Rabbit
{
public:
	bool movement = 1;
};

void moveRabbit(Rabbit& rab) {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(CHANGE_DIRECTION_INTERVAL * 100));

		int direction = dis(gen);
		mtx.lock();		
		COORD c;
		c.X = rab.x;
		c.Y = rab.y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		cout << " ";

		switch (direction) {
		case 0: 
			rab.x = min(rab.x + VELOCITY, AREA_WIDTH - 1);
			break;
		case 1:
			rab.x = max(rab.x - VELOCITY, 0);
			break;
		case 2: 
			rab.y = max(rab.y - VELOCITY, 0);
			break;
		case 3: 
			rab.y = min(rab.y + VELOCITY, AREA_HEIGHT - 1);
			break;
		}
		c.X = rab.x;
		c.Y = rab.y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		cout << "R";
		mtx.unlock();

	}
}

void moveAlbino(Albino& rab)
{
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(100));

		mtx.lock();
		COORD c;
		c.X = rab.x;
		c.Y = rab.y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		cout << " ";
		if ((rab.x == 0 && !rab.movement) || (rab.x == 99 && rab.movement))
			rab.movement = !rab.movement;
		rab.x = (rab.x + (rab.movement ? VELOCITY : -VELOCITY));
		c.X = rab.x;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		cout << "A";
		mtx.unlock();
	}
}
void setCursorrabition(int x, int y) {
	cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}
void setColor(int color) {
	cout << "\033[" << color << "m";
}
void drawBoundary() {
	setColor(31);
	for (int i = 0; i <= AREA_WIDTH; ++i) {

		setCursorrabition(i, AREA_HEIGHT);
		cout << "#";
	}
	for (int i = 0; i <= AREA_HEIGHT; ++i) {

		setCursorrabition(AREA_WIDTH, i);
		cout << "#";
	}
	setColor(0);
}
int main() {
	srand(time(NULL));
	drawBoundary();
	vector<Rabbit> rabbits(NUM_RABBITS);
	vector<Albino> albinos(NUM_ALBINOS);

	for (int i = 0; i < NUM_RABBITS; ++i)
	{
		rabbits[i].x = rand() % AREA_WIDTH;
		rabbits[i].y = rand() % AREA_HEIGHT;
		COORD c;
		c.X = rabbits[i].x;
		c.Y = rabbits[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		cout << "R";
	}
	for (int i = 0; i < NUM_ALBINOS; ++i)
	{
		albinos[i].x = rand() % AREA_WIDTH;
		albinos[i].y = rand() % AREA_HEIGHT;
		COORD c;
		c.X = albinos[i].x;
		c.Y = albinos[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		cout << "A";
	}
	vector< thread> rabbitThreads;
	for (int i = 0; i < NUM_RABBITS; ++i) {
		rabbitThreads.emplace_back(moveRabbit, ref(rabbits[i]));
	}
	vector< thread> albinoThreads;
	for (int i = 0; i < NUM_ALBINOS; ++i) {
		albinoThreads.emplace_back(moveAlbino, ref(albinos[i]));
	}

	for (auto& thread : rabbitThreads) {
		thread.join();
	}
	for (auto& thread : albinoThreads) {
		thread.join();
	}

	return 0;
}

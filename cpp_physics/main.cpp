#include <iostream>
#include <math.h>
#include <Windows.h>
#include <ctime>
#include <conio.h>
using namespace std;

struct Vector
{
	float x;
	float y;

	Vector(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}
};
COORD p = { 0, 0 };

int game_over;
const int height = 24;
const int width = 70;
const float g = 0.2f;
const float scale_time = 1.2f;
int update = 16;

int static_map[height][width];
int map[height][width];

int const balls_count = 120;



class Ball
{
public:
	Vector next_frame_location;
	Vector speed_vector;
	Vector previous_location;
	Vector frame_location_help;
	bool vector_y_rest;
	float mass;

	Ball(float objectmass = 10, Vector ini_pos = Vector(width / 2, height / 2), Vector ini_speed = Vector(0, 0))
	{
		this->mass = objectmass;
		this->next_frame_location = ini_pos;
		this->previous_location = ini_pos;
		this->speed_vector = ini_speed;
		this->frame_location_help = ini_pos;
		vector_y_rest = false;
	}

	void Collision_y()
	{
		this->speed_vector.x -= this->speed_vector.x*0.06f;
		//cout << this->speed_vector.y << " - " << this->speed_vector.y*0.9f << " = " << this->speed_vector.y - this->speed_vector.y*0.2f;
		this->speed_vector.y -= this->speed_vector.y*0.2f;
		this->speed_vector.y *= -1;

	}

	void Collision_x()
	{
		//this->vector.y -= this->vector.y*0.01f;
		this->speed_vector.x -= this->speed_vector.x*0.01f;
		this->speed_vector.x *= -1;
	}

	void Collisions()
	{
		// collision with floor + ceiling
		if (this->next_frame_location.y + this->speed_vector.y * scale_time >= height - 1 || this->next_frame_location.y + this->speed_vector.y* scale_time <= 1)
			Collision_y();

		// collision with walls
		if (this->next_frame_location.x + this->speed_vector.x > width - 1 || this->next_frame_location.x + this->speed_vector.x  < 0)
			Collision_x();

		for (int i = 0; i < 20; i++)
		{
			if ((int)(next_frame_location.y + speed_vector.y * scale_time *(float)i / 20) > height || (int)(next_frame_location.y + speed_vector.y * scale_time *(float)i / 20) < 0)
				continue;

			if ((int)(next_frame_location.x + speed_vector.x * scale_time *(float)i / 20) > width || (int)(next_frame_location.x + speed_vector.x * scale_time *(float)i / 20) < 0)
				continue;

			if (static_map[(int)(next_frame_location.y + speed_vector.y * scale_time *(float)i / 20)][(int)(next_frame_location.x + speed_vector.x * scale_time *(float)i / 20)] == 2)
			{
				Collision_y();
				break;
			}

			if (static_map[(int)(next_frame_location.y + speed_vector.y * scale_time *(float)i / 20)][(int)(next_frame_location.x + speed_vector.x * scale_time *(float)i / 20)] == 3)
			{
				Collision_x();
				break;
			}


		}

	}

	void check_for_rest()
	{


		if ((int)next_frame_location.y == height - 2 && abs(speed_vector.y) < 0.1f)
		{
			this->vector_y_rest = true;
			this->speed_vector.y = 0;
			this->speed_vector.x = 0;
			this->next_frame_location.y = height - 1;
		}



		//if (static_map[(int)next_frame_location.y-3][(int)next_frame_location.x] == 2 && abs(speed_vector.y) < 0.1f)
		//	{
		//		this->vector_y_rest = true;
		//		this->speed_vector.y = 0;
		//		this->speed_vector.x = 0;
		//	//	this->next_frame_location.y = (int)next_frame_location.y ;
		//	}


	}

	void calculate_frame()
	{
		previous_location.x = frame_location_help.x;
		previous_location.y = frame_location_help.y;

		check_for_rest();


		if (next_frame_location.y + speed_vector.y <= height - 1)
		{
			speed_vector.y += g * scale_time;
		}
		Collisions();

		next_frame_location.y += speed_vector.y * scale_time;
		next_frame_location.x += speed_vector.x * scale_time;



		frame_location_help.x = next_frame_location.x;
		frame_location_help.y = next_frame_location.y;

		//makes sure the ball does not leave the map
		if (next_frame_location.y >= height - 1)
		{
			frame_location_help.y = height - 1;

		}
		else if (next_frame_location.y <= 1)
		{
			frame_location_help.y = 1;
		}
		else if (next_frame_location.x <= 1)
		{
			frame_location_help.x = 1;
		}
		else if (next_frame_location.x >= width - 1)
		{
			frame_location_help.x = width - 1;
		}


		map[(int)previous_location.y][(int)previous_location.x] = 0;
		map[(int)frame_location_help.y][(int)frame_location_help.x] = 2;


	}


};

void draw()
{
	//system("cls");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			switch (static_map[i][j])
			{
			case 0:

				switch (map[i][j])
				{
				case 2:
					cout << "O";
					break;
				default:
					cout << " ";
					break;
				}

				break;
			case 1:
				cout << "#";
				break;
			case 2:
				cout << "_";
				break;
			case 3:
				cout << "|";
				break;
			case 10:
				cout << "o";
				break;
			case 11:
				cout << "|";
				break;
			case 12:
				cout << "\\";
				break;
			case 13:
				cout << "/";
				break;
			case 14:
				cout << "^";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

void setup()
{
	//     0 = empty   1 = mapbed  2 = floor 3 = wall
	game_over = false;
	srand((unsigned int)time(NULL));

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == height - 1 || i == 0 || j == 0 || j == width - 1)
				static_map[i][j] = 1;
			else
				static_map[i][j] = 0;


			map[i][j] = 0;
		}

	}


	//static_map[18][50] = 10; // o
	//static_map[19][50] = 11; // |
	//static_map[20][51] = 12; // /
	//static_map[20][49] = 13; //  \ 
	//static_map[20][50] = 14; //  
	//static_map[19][49] = 14; //  \ 
	//static_map[19][50] = 14; //  

	for (int i = 2; i < 50; i++)
	{
		static_map[height / 2][i] = 2;
	}

	for (int i = 4; i < 20; i++)
	{
		static_map[i][width / 2 + 10] = 3;
	}

	for (int i = 4; i < 20; i++)
	{
		static_map[i][width / 2 - 20] = 3;
	}
}



int current_count = 20;
void input()
{
	if (GetAsyncKeyState(1) == -32767)
	{
		current_count++;

	}

	if (GetAsyncKeyState(2) == -32767)
	{
		current_count--;

	}

}

int main()
{
	Ball balls[balls_count];
	setup();


	for (int i = 0; i < balls_count; i++)
	{
		balls[i].speed_vector.x = (float)((rand() % 600) - 300) / 100.0f;
		balls[i].speed_vector.y = (float)((rand() % 600) - 300) / 100.0f;

		//	balls[i].next_frame_location.x = (float)((rand() % (width-2)) + 2);
		//	balls[i].next_frame_location.y = (float)((rand() % (height-2)) + 2);
	}


	while (!game_over)
	{
		for (int i = 0; i < current_count; i++)
			balls[i].calculate_frame();

		input();

		draw();
		Sleep(update);

	}

}
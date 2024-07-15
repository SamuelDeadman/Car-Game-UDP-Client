#include "receiver.hpp"
#include "tcp_client.h"
#include "game_main.h"
#include "UdpClient.h"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

void sendchat();

using namespace sf;

const int num = 8; //checkpoints
// TODO: use checkpoint to make sure we are on the track.
// Slow speed when not on the track.
int points[num][2] = { 300, 610,
  1270,430,
  1380,2380,
  1900,2460,
  1970,1700,
  2550,1680,
  2560,3150,
  500, 3300 };

UdpClient* client = 0; //global called client
tcp_client* tcpc = 0;
const int Ncars = 6;

struct Car
{
	float x, y, speed, angle; int n; // X Y speed angle for all cars all floats, 4 bytes
	unsigned int CarIpAdd; //car ID given ID of IP when connected
	unsigned short CarIDPort;
	Car() { speed = 2; angle = 0; n = 0; CarIpAdd = 0; }
	void move()
	{
		x += sin(angle) * speed;
		y -= cos(angle) * speed;

		if (CarIpAdd == 0)
		{
			findTarget(); //"ai" CARS
		}

		if (CarIpAdd == -1) //player
		{
			//call client send x y angle -> when i recive find car then update X y
			//UdpClient *client = new UdpClient();
			client->UDP_Send(x, y, angle); //collect player current X Y and angle, speed not needed ?
		}
	}
	void findTarget()
	{
		float tx = points[n][0];
		float ty = points[n][1];
		float beta = angle - atan2(tx - x, -ty + y); // angle between way points 
		if (sin(beta) < 0) angle += 0.005 * speed; else angle -= 0.005 * speed;
		// Check if passed a checkpoint
		if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25) n = (n + 1) % num; // TODO: simplify
	}
};

void printcars(Car* cars) //debug output 
{
	//std::cout << "\x1B[2J\x1B[H" << std::endl;
	for (int i = 0; i < Ncars; i++)
	{
		//printf("%8X,%d\n", cars[i].CarIpAdd, cars[i].CarIDPort);
	}
}

void getchat()
{
	tcpc->tcp_recive();
}

void updates(Car* cars)
{
	int status;
	float x, y, angle;
	unsigned int IP4;
	unsigned short port;
	status = client->UDP_Recive(&x, &y, &angle, &IP4, &port); //will hang on blocking if niothing recived BAD :(
	if (status == 0)//only run if recived info
	{
		bool foudcar = false;
		foudcar = false;
		for (int i = 0; i < Ncars; i++)
		{
			if ((cars[i].CarIpAdd == IP4) && (cars[i].CarIDPort == port))
			{
				cars[i].x = x;
				cars[i].y = y;
				cars[i].angle = angle;
				foudcar = true;
			}
			if (foudcar == false)
			{
				if (cars[i].CarIpAdd == 0) // creates new car with unqiye* port + IP4 recived
				{
					cars[i].CarIpAdd = IP4;
					cars[i].CarIDPort = port;
					std::cout <<"car " << i << " added with port " << port << " car added with IP " << IP4 << std::endl;
					cars[i].x = x;
					cars[i].y = y;
					cars[i].angle = angle;
					foudcar = true;
				}
			}
		}
	}
	
}

int main()
{
	client = new UdpClient(); //creates udp new
	
	tcpc = new tcp_client(client->UDP_serverip);
	tcpc->tcp_message("Hello");
	RenderWindow app(VideoMode(640, 480), "Sam's Multiplayer Car Racing Game!"); //setup sfml "screen" view
	app.setFramerateLimit(60); // maybe this can be reduced if the networking makes the car look laggy 
	Texture t1, t2;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/car.png");
	t1.setSmooth(true);
	t2.setSmooth(true);
	Sprite sBackground(t1), sCar(t2);
	sBackground.scale(2, 2);
	sCar.setOrigin(22, 22);
	float R = 22;
	 // 6 cars in total
	Car car[Ncars];
	car[0].CarIpAdd = -1;
	//create reciver - decoded, updates X Y or CarsIP, special allocates new user
	
	std::cout << "uses 'T' to open prompt to send message\n";

	// Starting positions
	for (int i = 0; i < Ncars; i++)
	{
		car[i].x = 300 + i * 50;
		car[i].y = 1700 + i * 80;
		car[i].speed = 7 + i;
	}
	float speed = 0, angle = 0;
	float maxSpeed = 12.0;
	float acc = 0.2, dec = 0.3;
	float turnSpeed = 0.08;
	int offsetX = 0, offsetY = 0;
	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}
		bool Up = 0, Right = 0, Down = 0, Left = 0;
		if (Keyboard::isKeyPressed(Keyboard::Up)) Up = 1;
		if (Keyboard::isKeyPressed(Keyboard::Right)) Right = 1;
		if (Keyboard::isKeyPressed(Keyboard::Down)) Down = 1;
		if (Keyboard::isKeyPressed(Keyboard::Left)) Left = 1;
		if (Keyboard::isKeyPressed(Keyboard::T)) sendchat();
		//car movement
		if (Up && speed < maxSpeed)
		{
			if (speed < 0)  speed += dec;
			else  speed += acc;
		}
		if (Down && speed > -maxSpeed)
		{
			if (speed > 0) speed -= dec;
			else  speed -= acc;
		}
		if (!Up && !Down)
		{
			if (speed - dec > 0) speed -= dec;
			else if (speed + dec < 0) speed += dec;
			else speed = 0;
		}
		if (Right && speed != 0)  angle += turnSpeed * speed / maxSpeed;
		if (Left && speed != 0)   angle -= turnSpeed * speed / maxSpeed;
		car[0].speed = speed; // CAR 0 IS THE MAINPLAYER
		car[0].angle = angle; // CAR 0 IS THE MAINPLAYER set player info from input
		
		for (int i = 0; i < Ncars; i++) car[i].move(); // car 0 takes player input

		
		updates(car); //mutiplayer car updates
		getchat();
		//printcars(car);

		//for (int i = 1; i < N; i++) 
		//car[1].move(); // car 1 takes player input
		//for (int i = 2; i < N; i++) car[i].findTarget(); replaced in move allowing for non controlled to be AI controlled



		//collision
		for (int i = 0; i < Ncars; i++)
			for (int j = 0; j < Ncars; j++)
			{
				if (i == j) {
					break;
				}
				int dx = 0, dy = 0;
				while (dx * dx + dy * dy < 4 * R * R)
				{
					car[i].x += dx / 10.0;
					car[i].x += dy / 10.0;
					car[j].x -= dx / 10.0;
					car[j].y -= dy / 10.0;
					dx = car[i].x - car[j].x;
					dy = car[i].y - car[j].y;
					if (!dx && !dy) break;
				}
			}
		app.clear(Color::White);
		// TODO: Stay within the limit of the map.
		// TODO: Don't show white at bottom/right.
		if (car[0].x > 320) offsetX = car[0].x - 320;
		if (car[0].y > 240) offsetY = car[0].y - 240;
		sBackground.setPosition(-offsetX, -offsetY);
		app.draw(sBackground); // set background then car is written over this 
		Color colors[10] = { Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::White };
		for (int i = 0; i < Ncars; i++)
		{
			sCar.setPosition(car[i].x - offsetX, car[i].y - offsetY);
			sCar.setRotation(car[i].angle * 180 / 3.141593); 
			sCar.setColor(colors[i]);
			app.draw(sCar);
		}
		app.display();
	}
	return 0;
}

void sendchat()
{
	std::string s; //string to coantain user input of there ip
	std::cout << "type in message: ";
	std::cin >> s;
	tcpc->tcp_message(s);
}

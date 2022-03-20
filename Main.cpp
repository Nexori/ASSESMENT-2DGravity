#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
//#include <cmath>
#include <vector>
#include <sstream>
//#include <memory>
#include <fstream>
#include "Object.h"

#define M_PI 3.14159265358979323846
//#include "Simulation.h"

using namespace std;
using namespace sf;

int main()
{
	//Window settings
	sf::Font arial; arial.loadFromFile("arial.ttf");
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	int WINX = 1600;
	int WINY = 900;
	double zoom = 1.0;

	//Rendering variables
	int resPT = 50;
	sf::Vector2f mousepos;
	std::vector<object> Celestial;
	std::vector<sf::CircleShape> celestialsprite;
	double ObjMass = 100;
	double ObjSize = 5;
	int ID = 0;
	int r, g, b;

	//Simulation quality variables
	double time = 1;
	double timetemp = 0;

	//Misc & debug variables
	bool called = 0;
	bool clear = 0;
	int debugFlag = 0;
	bool dispHelp = 1;

	sf::RenderWindow window(sf::VideoMode(WINX, WINY), "GraviSym", sf::Style::Default, settings);
	sf::View view(sf::FloatRect(0, 0, WINX, WINY));

	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		//Cursor settings
		sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
		sf::CircleShape cursor(ObjSize, 50);
		cursor.setOrigin(ObjSize, ObjSize);
		cursor.setFillColor(sf::Color(255, 0, 0, 150));
		cursor.setPosition(mousepos);

		//Handling zoom
		double scalex, scaley, scale;
		scalex = view.getSize().x / window.getSize().x;
		scaley = view.getSize().y / window.getSize().y;
		scale = sqrt((scalex * scalex) + (scaley * scaley));

		if (Keyboard::isKeyPressed(sf::Keyboard::Q) && scalex < 6) {
			zoom += 0.01;
		}
		if (Keyboard::isKeyPressed(sf::Keyboard::E) && scalex > 0.1) {
			zoom -= 0.01;
		}
		view.zoom(zoom);
		zoom = 1;

		//EventCatcher
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close(); printf("Window Closed\n");
			}
			if (ev.type == sf::Event::MouseWheelScrolled)
			{
				if (ev.mouseWheelScroll.delta > 0) //Scroll up
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
						ObjMass = ObjMass + 1000;
						ObjSize = sqrt(ObjMass / M_PI);
						printf("ObjMass increased drastically:%.f\t ObjSize: %.f\n", ObjMass, ObjSize);
						break;
					}
					ObjMass = ObjMass + 50;
					ObjSize = sqrt(ObjMass / M_PI);
					cursor.setOrigin(ObjSize, ObjSize);
					printf("ObjMass increased:%.f\t ObjSize: %.f\n", ObjMass, ObjSize);
				}
				else if ((ev.mouseWheelScroll.delta < 0) && (ObjMass > 50)) //Scroll down
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (ObjMass > 1000)) {
						ObjMass = ObjMass - 1000;
						ObjSize = sqrt(ObjMass / M_PI);
						printf("ObjMass decreased drastically:%.f\t ObjSize: %.f\n", ObjMass, ObjSize);
						break;
					}
					ObjMass = ObjMass - 50;
					ObjSize = sqrt(ObjMass / M_PI);
					cursor.setOrigin(ObjSize, ObjSize);
					printf("ObjMass decreased:%.f\t ObjSize: %.f\n", ObjMass, ObjSize);
				}
			}
			if (ev.type == sf::Event::KeyPressed) {
				if (ev.key.code == sf::Keyboard::R)
				{
					//view.setViewport(sf::FloatRect(1,1,WINX,WINY));
					view = window.getDefaultView();
					celestialsprite.clear();
					Celestial.clear();
					ObjSize = 5;
					ObjMass = 50;
					ID = 0;
					time = 1;
				}
				//Viewport control

				if (ev.key.code == sf::Keyboard::Z) {
					time = time + 0.05;
				}
				if (ev.key.code == sf::Keyboard::X) {
					time = time - 0.05;
				}
				if (ev.key.code == sf::Keyboard::Space) {
					if (time == 0) {
						time = timetemp;
					}
					else {
						timetemp = time;
						time = 0;
					}
				}
				if (ev.key.code == sf::Keyboard::F2) {
					debugFlag = !debugFlag;
				}
				if (ev.key.code == sf::Keyboard::F1) {
					dispHelp = !dispHelp;
				}
			}
		}

		//Cast object with initial speed
		sf::VertexArray linez(sf::Lines, 2);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (time) {
				for (int i = 0; i < ID; i++)
				{
					window.draw(celestialsprite[i]);
				}
			}
			sf::Vector2f point2;
			sf::Vector2f point1 = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			linez[0].position = sf::Vector2f(point1);
			while (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				point2 = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
				linez[1].position = sf::Vector2f(point2);
				window.clear(sf::Color::Black);
				window.draw(cursor);
				window.draw(linez);
				for (int i = 0; i < ID; i++) {
					window.draw(celestialsprite[i]);
				}
				window.display();
				called = true;
			}
			if (point1 == point2) {
				object object(true, ObjMass, ObjSize, point1);
				ID++;
				if (debugFlag == 1)
				{
					printf("<STATIC>|| ID: %d || P1: %.f %.f || P2: %.f %.f || Direciton: %f %5f||\n", ID, point1.x, point1.y, point2.x, point2.y, object.UnitVector.x, object.UnitVector.y);
				}
				object.isStatic = true;
				Celestial.push_back(object);
				celestialsprite.resize(celestialsprite.size() + 1);
			}
			else {
				window.draw(cursor);
				object object(ObjMass, ObjSize, point1, point2);
				ID++;
				if (debugFlag == 1)
				{
					printf("<DYNAMIC>|| ID: %d || P1: %.f %.f || P2: %.f %.f ||  Direciton: %f %5f||\n", ID, point1.x, point1.y, point2.x, point2.y, object.UnitVector.x, object.UnitVector.y);
				}
				Celestial.push_back(object);
				celestialsprite.resize(celestialsprite.size() + 1);
			}
		}

		//Physics core
		if (clear)
		{
			{
				//Calculate gravitational vectors
				if (time != 0)
				{
					for (int i = 0; i < ID; i++) {
						for (int j = 0; j < ID; j++) {
							if (i != j && !Celestial[i].isStatic) {		// It is preventing simulation of object A - object A which would result in undefined behavior
								double FScalar;
								//Reset values (prevent undefined behaviour)
								//Celestial[i].ForceVector = sf::Vector2f(0, 0);
								Celestial[i].ForceDirection = sf::Vector2f(0, 0);
								Celestial[i].AccelerationVector = sf::Vector2f(0, 0);

								//For notation purposes dx = d.x2 - d.x1
								double dx = Celestial[j].position.x - Celestial[i].position.x;
								double dy = Celestial[j].position.y - Celestial[i].position.y;

								//Normalize
								Celestial[i].ForceDirection.x = (dx) / sqrt(((dx) * (dx)) + ((dy) * (dy)));
								Celestial[i].ForceDirection.y = (dy) / sqrt(((dx) * (dx)) + ((dy) * (dy)));

								//Obtain Gravitational Force F = GmM/r^2
								double m = Celestial[i].ObjMass; double M = Celestial[j].ObjMass;
								double r = sqrt((dx * dx) + (dy * dy));
								FScalar = 6.67408 * (Celestial[i].ObjMass * Celestial[j].ObjMass) / (sqrt((dx * dx) + (dy * dy)) * sqrt((dx * dx) + (dy * dy)));
								//Convert to F(orce)Scalar by multiplying AccelerationVector * ForceDirection vector * Force scalar
								Celestial[i].AccelerationVector.x = Celestial[i].AccelerationVector.x + Celestial[i].ForceDirection.x * FScalar / m;
								Celestial[i].AccelerationVector.y = Celestial[i].AccelerationVector.y + Celestial[i].ForceDirection.y * FScalar / m;
								//Change Velocity Vector v = v0 + at
								Celestial[i].VelocityVector.x = Celestial[i].VelocityVector.x + Celestial[i].AccelerationVector.x * time;
								Celestial[i].VelocityVector.y = Celestial[i].VelocityVector.y + Celestial[i].AccelerationVector.y * time;
							}
						}
					}
				}
				Vector2f Delta = Vector2f(0, 0);

				//Update graphics
				for (int i = 0; i < ID; i++)
				{
					if (!Celestial[i].isStatic && time != 0)
					{ //update position s = vt or v = at -> s =at^2
						Delta.x = (time * Celestial[i].VelocityVector.x + time * Celestial[i].AccelerationVector.x);
						Delta.x = (time * Celestial[i].VelocityVector.x + time * Celestial[i].AccelerationVector.x);
						Delta.y = (Celestial[i].VelocityVector.y + time * Celestial[i].AccelerationVector.y);
						Celestial[i].position.x = Delta.x + Celestial[i].position.x;
						Celestial[i].position.y = Delta.y + Celestial[i].position.y;
					}
					for (int i = 0; i < ID; i++)
					{
						celestialsprite[i].setPointCount(resPT);
						celestialsprite[i].setOrigin(sf::Vector2f(Celestial[i].ObjSize, Celestial[i].ObjSize));
						celestialsprite[i].setPosition(Celestial[i].position);
						celestialsprite[i].setRadius(Celestial[i].ObjSize);
					}
				}
			}
		}

		//Collision detector
	quo:
		for (int i = 0; i < ID; i++)
		{
			for (int j = 0; j < ID; j++)
			{
				if (j != i)
				{
					double dx = Celestial[j].position.x - Celestial[i].position.x;
					double dy = Celestial[j].position.y - Celestial[i].position.y;
					double r = Celestial[i].ObjSize + Celestial[j].ObjSize;
					double d = sqrt((dx * dx) + (dy * dy));
					if (r >= d && Celestial[i].ObjSize >= Celestial[j].ObjSize) {
						object child(Celestial[i], Celestial[j]);
						Celestial.at(i) = child;
						Celestial.erase(Celestial.begin() + j);
						ID--;
						goto quo; //Breaks the loop and forces to check again for colisions (otherwise vector would throw exception)
					}
					if (r >= d && Celestial[i].ObjSize < Celestial[j].ObjSize) {
						object child(Celestial[j], Celestial[i]);
						Celestial.at(j) = child;
						Celestial.erase(Celestial.begin() + i);
						ID--;
						goto quo; //Breaks the loop and forces to check again for colisions (otherwise vector would throw exception)
					}
				}
			}
		}

		//Remove obsolete objects (2k px outside border limit)
	quo2:
		for (int i = 0; i < ID; i++) {
			if (Celestial[i].position.x > 6000 || Celestial[i].position.x < -5000 || Celestial[i].position.y > 4000 || Celestial[i].position.y < -3000) {
				Celestial.erase(Celestial.begin() + i);
				ID--;
				goto quo2; //Breaks the loop and forces to check again for obsolete objects (otherwise vector would throw exception)
			}
		}

		sf::Text help("R - restart simulation\nSPACE - pause\nZ - decrease SimSpeed\nX - increase SimSpeed \nSCROLL UP - increase size and mass\nSCROLL DOWN - decrease size and mass\nCTRL + SCROLL UP increase size x20\nCTRL + SCROLL DOWN decrease size x20\nQ/E - zoom out / zoom in\n F1 - hide this text\n F2 - shof debug mode\n", arial, 12 * scale); help.setPosition(window.mapPixelToCoords(sf::Vector2i(10, 10), view));
		if (dispHelp)window.draw(help);

		if (debugFlag)
		{
			ostringstream ticks;
			ticks << "Body number:" << ID << endl << "SIMSPEED:" << time << endl << "Current mass:" << ObjMass << "\n Mousepos:" << mousepos.x << " " << mousepos.y << "\nScale:" << scalex;
			sf::Text tick(ticks.str(), arial, 15 * scale); tick.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 740), view));
			window.draw(tick);
		}
		window.draw(cursor);
		if (!called) {
			for (int i = 0; i < ID; i++) {
				double vel;
				vel = sqrt((Celestial[i].VelocityVector.x * Celestial[i].VelocityVector.x) + (Celestial[i].VelocityVector.y * Celestial[i].VelocityVector.y));
				ostringstream velvec;
				if (debugFlag == 1) {
					velvec << "Mass:" << Celestial[i].ObjMass << "\nV:" << vel << endl << "VVec:" << endl << "x: " << Celestial[i].VelocityVector.x << "  y:" << Celestial[i].VelocityVector.x << endl << "AVec:\nx:" << Celestial[i].AccelerationVector.x << " y:" << Celestial[i].AccelerationVector.y << endl << "FVec:\nx:" << Celestial[i].ForceDirection.x << " y:" << Celestial[i].ForceDirection.y << endl << "Static:" << Celestial[i].isStatic;
				}
				else velvec << "Mass:" << Celestial[i].ObjMass << endl;
				sf::String velc(velvec.str()); sf::Text velp(velc, arial, 12 * scale); velp.setPosition(sf::Vector2f(Celestial[i].position.x + Celestial[i].ObjSize, Celestial[i].position.y + Celestial[i].ObjSize));

				r = 800 * (Celestial[i].ObjMass / Celestial[i].ObjSize);
				g = 800 * (Celestial[i].ObjSize / Celestial[i].ObjMass);
				b = 800 * (1 - (Celestial[i].ObjSize / Celestial[i].ObjMass));
				if (r < 100) r = 100;
				if (g < 100) g = 100;
				if (b < 100) b = 100;
				celestialsprite[i].setFillColor(sf::Color(r, g, b));
				window.draw(celestialsprite[i]);
				window.draw(velp);
			}
			window.display();
		}

		window.clear(sf::Color::Black);
		window.setView(view);
		called = false;
		clear = true;
	}
	return 0;
}
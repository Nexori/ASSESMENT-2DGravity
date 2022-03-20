#pragma once
#include <cmath>
#include <cstdlib>
class object
{
public:
	bool isStatic;
	sf::Vector2f position;
	sf::Vector2f start;
	sf::Vector2f end;
	double ObjMass;
	double ObjSize;
	double Velocity;
	sf::Vector2f VelocityVector;
	sf::Vector2f UnitVector; //aka unit vector
	sf::Vector2f ForceDirection;
	sf::Vector2f AccelerationVector;
	sf::Vector2f ForceVector;
	object(double, double, sf::Vector2f, sf::Vector2f);
	object(bool, double, double, sf::Vector2f);
	object(object, object);
	object() { isStatic = false; }
	~object();
	double calculateVelocity();
	sf::Vector2f calculateDirection();
private:
};
object::object(bool isStatic, double x, double y, sf::Vector2f point) {
	isStatic = true;
	ObjMass = x;
	ObjSize = y;
	position = point;
	Velocity = 0;
}
object::object(object a, object b) : object() {
	sf::Vector2f VelocityVector;
	sf::Vector2f UnitVector;
	sf::Vector2f ForceDirection;
	sf::Vector2f AccelerationVector;
	sf::Vector2f ForceVector;
	this->position = a.position;

	if (a.isStatic || b.isStatic)
		this->isStatic = true;
	else
		this->isStatic = false;

	this->ObjMass = a.ObjMass + b.ObjMass;
	if (a.ObjSize > b.ObjSize) {
		this->ObjSize = sqrt(a.ObjMass + b.ObjMass / 3.142);
	}
	else {
		this->ObjSize = sqrt(a.ObjMass + b.ObjMass / 3.142);
	}

	this->VelocityVector.x = (b.VelocityVector.x + a.VelocityVector.x) * (1 - a.ObjMass / this->ObjMass);
	this->VelocityVector.y = (b.VelocityVector.y + a.VelocityVector.y) * (1 - a.ObjMass / this->ObjMass);
	//this->AccelerationVector.y = a.VelocityVector.y * a.ObjMass / this->ObjMass;
	//this->ForceDirection.x = a.VelocityVector.x * a.ObjMass / this->ObjMass;
	//this->ForceDirection.y = a.VelocityVector.y * a.ObjMass / this->ObjMass;
}
object::object(double x, double y, sf::Vector2f a, sf::Vector2f b)
{
	isStatic = false;
	ObjMass = x;
	ObjSize = y;
	start = a;
	end = b;
	position = a;
	sf::CircleShape circle(y);
	calculateDirection();
	calculateVelocity();
	VelocityVector.x = (Velocity * UnitVector.x);
	VelocityVector.y = (Velocity * UnitVector.y);
	AccelerationVector = sf::Vector2f(0, 0);
	ForceVector = sf::Vector2f(0, 0);
}

object::~object()
{
}

sf::Vector2f object::calculateDirection()
{
	UnitVector.x = (end.x - start.x) / (sqrt(((end.x - start.x) * (end.x - start.x)) + 0.00001 + ((end.y - start.y) * (end.y - start.y))));
	UnitVector.y = (end.y - start.y) / (sqrt(((end.x - start.x) * (end.x - start.x)) + 0.00001 + ((end.y - start.y) * (end.y - start.y))));
	if (UnitVector.x < -2 || UnitVector.y > 2)
	{
		UnitVector.x = 0; UnitVector.y = 0;
		return UnitVector;
	}
	return UnitVector;
}
double object::calculateVelocity() {
	// basically a vectorlenght / 50
	return Velocity = sqrt(((end.x - start.x) * (end.x - start.x)) + ((end.y - start.y) * (end.y - start.y))) / 50;
}
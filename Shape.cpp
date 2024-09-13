#include "Shape.h"
#include <random>

Shape::Shape() {}
Shape::Shape(std::string type, int id, int w, int h) : sf::RectangleShape(sf::Vector2f(w, h)), type(type), id(id) {}

std::vector<Shape>& Shape::generate()
{
	int num = rand() % 4;
	if (num == 0) { return LShape(); }
	if (num == 1) { return SquareShape(); }
	if (num == 2) { return TShape(); }
	if (num == 3) { return LineShape(); }
}

std::vector<Shape>& Shape::LShape()
{
	container.clear();

	Shape rectangle("L", 11, w, h);
	rectangle.setPosition(201, 51);
	rectangle.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle);

	Shape rectangle2("L", 42, w, h);
	rectangle2.setPosition(141, 81);
	rectangle2.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle2);

	Shape rectangle3("L", 41, w, h);
	rectangle3.setPosition(171, 81);
	rectangle3.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle3);

	Shape rectangle1("L", 0, w, h);
	rectangle1.setPosition(201, 81);
	rectangle1.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle1);
	return container;
}

std::vector<Shape>& Shape::SquareShape()
{
	container.clear();

	Shape rectangle("S", 0, w, h);
	rectangle.setPosition(201, 51);
	rectangle.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle);

	Shape rectangle1("S", 0, w, h);
	rectangle1.setPosition(171, 51);
	rectangle1.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle1);

	Shape rectangle2("S", 0, w, h);
	rectangle2.setPosition(171, 81);
	rectangle2.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle2);

	Shape rectangle3("S", 0, w, h);
	rectangle3.setPosition(201, 81);
	rectangle3.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle3);

	return container;
}

std::vector<Shape>& Shape::TShape()
{
	container.clear();

	Shape rectangle("T", 11, w, h);
	rectangle.setPosition(171, 51);
	rectangle.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle);

	Shape rectangle1("T", 41, w, h);
	rectangle1.setPosition(141 , 81);
	rectangle1.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle1);

	Shape rectangle2("T", 0, w, h);
	rectangle2.setPosition(171, 81);
	rectangle2.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle2);

	Shape rectangle3("T", 21, w, h);
	rectangle3.setPosition(201, 81);
	rectangle3.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle3);

	return container;
}

std::vector<Shape>& Shape::LineShape()
{
	container.clear();

	Shape rectangle("Line", 41, w, h);
	rectangle.setPosition(141, 51);
	rectangle.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle);
	
	Shape rectangle1("Line", 0, w, h);
	rectangle1.setPosition(171, 51);
	rectangle1.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle1);

	Shape rectangle2("Line", 21, w, h);
	rectangle2.setPosition(201, 51);
	rectangle2.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle2);

	Shape rectangle3("Line", 22, w, h);
	rectangle3.setPosition(231, 51);
	rectangle3.setFillColor(sf::Color(100, 100, 100));
	container.push_back(rectangle3);

	return container;
}
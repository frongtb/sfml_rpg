#ifndef ENTITY_H
#define ENTITY_H

#include<vector>
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>
#include<map>

#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include"SFML\Graphics.hpp"
#include"SFML\Audio.hpp"
#include"SFML\Network.hpp"

using namespace sf;
using namespace std;
class Entity
{
private:


protected:
	sf::RectangleShape shape;
	float movementSpeed;

public:
	Entity();
	virtual ~Entity();

	//functions
	virtual void move(const float& dt, const float x, const float y);

	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target);
};

#endif
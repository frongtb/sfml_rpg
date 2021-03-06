#ifndef GAME_H
#define GAME_H


#include"GameState.h"

class Game
{
private:
	//Variables
	sf::RenderWindow *window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	std::map<std::string, int> supportedKeys;

	//Initialization
	void initWindow(); 
	void initStates();
	void initKeys();


public:
	//Constructors/Destructors
	Game();
	virtual ~Game();

	//Functions

	//Regular
	void endApp();

	//Update
	void updateDt();
	void updateSFMLEvent();
	void update();
	
	//Render
	void render();
	
	//Core
	void run();

};

#endif
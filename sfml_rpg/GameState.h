#ifndef GAMESTATE_H 
#define GAMESTATE_H
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

#include "State.h"

class GameState :
    public State
{
private:
    Player* player;

public:
    GameState(sf::RenderWindow* window);
    virtual ~GameState();
    

    //Functions
    void endState();

    void updatInput(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target = NULL);
};

#endif 
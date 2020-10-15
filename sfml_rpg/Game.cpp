#include "Game.h"
//Static functions

//Initializer functions

//Constructors/Destructors

void Game::initWindow()
{
    std::ifstream ifs("Config/window.ini");
    std::string title = "None";
    sf::VideoMode window_bounds(800,600);

    unsigned framelate_limit = 120;
    bool vertical_sync_enabled = false;
   if (ifs.is_open())
   {
       std::getline(ifs, title);
       ifs >> window_bounds.width >> window_bounds.height;
       ifs >> framelate_limit;
       ifs >> vertical_sync_enabled;
   }

    ifs.close();

	this -> window = new sf::RenderWindow(window_bounds, title);
    this->window->setFramerateLimit(framelate_limit);
    this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

Game::Game()
{
    this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

void Game::updateDt()
{
    this->dt = this->dtClock.restart().asSeconds();


}

//Functions
void Game::updateSFMLEvent()
{
    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }
}
void Game::update()
{
    this->updateSFMLEvent();

}
void Game::render()
{
   this -> window->clear();
   
    //Render items

    this -> window->display();
}
void Game::run()
{
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
   
    }
}

#include "Game.h"
//Static functions

//Initializer functions

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

void Game::initStates()
{
   this->states.push(new GameState(this->window));
}

void Game::initKeys()
{
    this->supportedKeys.emplace("A", sf::Keyboard::Key::A);
    this->supportedKeys.emplace("D", sf::Keyboard::Key::D);
    this->supportedKeys.emplace("W", sf::Keyboard::Key::W);
    this->supportedKeys.emplace("S", sf::Keyboard::Key::S);

    std::cout << this->supportedKeys.at("A") << "\n";
}

//Constructors/Destructors
Game::Game()
{
    this->initWindow();
    this->initStates();
    this->initKeys();
}

Game::~Game()
{
	delete this->window;

    while (!this->states.empty())
    {
       delete this->states.top();
       this->states.pop();
    }
}


//Functions

void Game::endApp()
{
    std::cout << "Ending App" << "\n";
}

void Game::updateDt()
{
    this->dt = this->dtClock.restart().asSeconds();


}

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

    if (!this->states.empty())
    {
        this->states.top()->update(this->dt);

        if (this->states.top()->getQuit())
        {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }
    //app end
    else
    {
        this->endApp();
        this->window->close();
    }
}

void Game::render()
{
    this->window->clear();

    //Render items
    if (!this->states.empty())
        this->states.top()->render();

    this->window->display();
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

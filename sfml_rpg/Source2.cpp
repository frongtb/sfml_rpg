#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include<SFML/Main.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<map>
#include<time.h>
#include<iostream>
#include <sstream>
#include <vector>
using namespace sf;

sf::RectangleShape playerOILBar;
sf::RectangleShape playerOILBarBack;
sf::RectangleShape playerHpBar;
sf::RectangleShape playerHpBarBack;
int width = 1024;
int height = 768;
int roadW = 4000;
int segL = 200; //segment length
float camD = 0.84; //camera depth

void drawQuad(RenderWindow& w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1 - w1, y1));
    shape.setPoint(1, Vector2f(x2 - w2, y2));
    shape.setPoint(2, Vector2f(x2 + w2, y2));
    shape.setPoint(3, Vector2f(x1 + w1, y1));
    w.draw(shape);
}



struct Line
{
    float x, y, z; //3d center of line
    float X, Y, W; //screen coord
    float curve, spriteX, clip, scale;
    Sprite sprite;

    Line()
    {
        spriteX = curve = x = y = z = 0;
    }

    void project(int camX, int camY, int camZ)
    {
        scale = camD / (z - camZ);
        X = (1 + scale * (x - camX)) * width / 2;
        Y = (1 - scale * (y - camY)) * height / 2;
        W = scale * roadW * width / 2;
    }

    void drawSprite(RenderWindow& app)
    {
        Sprite s = sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;

        float destX = X + scale * spriteX * width / 2;
        float destY = Y + 4;
        float destW = w * W / 266;
        float destH = h * W / 266;

        destX += destW * spriteX; //offsetX
        destY += destH * (-1);    //offsetY

        float clipH = destY + destH - clip;
        if (clipH < 0) clipH = 0;

        if (clipH >= destH) return;
        s.setTextureRect(IntRect(0, 0, w, h - h * clipH / destH));
        s.setScale(destW / w, destH / h);
        s.setPosition(destX, destY);
        app.draw(s);
    }
};


int main()
{
    RenderWindow app(VideoMode(width, height), "Run Ran Run Racing");
    app.setFramerateLimit(60);

    //mark point
    Font font;
    Text mark;
    if (!font.loadFromFile("Fonts/Cascadia.ttf"))
        std::cout << "game failed to load font" << "\n";

    mark.setFont(font);
    mark.setCharacterSize(18);
    mark.setFillColor(sf::Color::Red);
    mark.setString("X");
    mark.setPosition(app.getSize().x / 2.f - mark.getGlobalBounds().width / 2.f, app.getSize().y / 2.f - mark.getGlobalBounds().height / 2.f);

    Texture t[50];
    Sprite object[50];
    for (int i = 1; i <= 7; i++)
    {
        t[i].loadFromFile("images/" + std::to_string(i) + ".png");
        t[i].setSmooth(true);
        object[i].setTexture(t[i]);
    }

    Texture bg;
    bg.loadFromFile("images/bg.png");
    bg.setRepeated(true);
    Sprite sBackground(bg);
    sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
    sBackground.setPosition(-2000, 0);

    std::vector<Line> lines;

    for (int i = 0; i < 1600; i++)
    {
        Line line;
        line.z = i * segL;

        if (i > 300 && i < 700) line.curve = 0.5;
        if (i > 1100) line.curve = -0.7;

        if (i < 300 && i % 20 == 0) { line.spriteX = -2.5; line.sprite = object[5]; }
        if (i % 17 == 0) { line.spriteX = 2.0; line.sprite = object[6]; }
        if (i > 300 && i % 20 == 0) { line.spriteX = -0.7; line.sprite = object[4]; }
        if (i > 800 && i % 20 == 0) { line.spriteX = -1.2; line.sprite = object[1]; }
        if (i == 400) { line.spriteX = -1.2; line.sprite = object[7]; }

        if (i > 750) line.y = sin(i / 30.0) * 1500;

        lines.push_back(line);
    }

    int N = lines.size();
    float playerX = 0;
    int pos = 0;
    int H = 1500;
    int oil_MAX = 5000;
    int oil;
    oil = oil_MAX;
    int hpMax =  100;
    int hp;
    hp = hpMax;
    //oil bar
    playerOILBar.setSize(sf::Vector2f(300.f, 25.f));
    playerOILBar.setFillColor(sf::Color::Blue);
    playerOILBar.setPosition(sf::Vector2f(700.f, 80.f));

    playerOILBarBack = playerOILBar;
    playerOILBarBack.setFillColor(sf::Color(25, 25, 25, 200));

    //hp bar
    playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
    playerHpBar.setFillColor(sf::Color::Red);
    playerHpBar.setPosition(sf::Vector2f(700.f, 30.f));

    playerHpBarBack = playerHpBar;
    playerHpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
    //game
    while (app.isOpen())
    {
      
        int speed = 200;
        oil_MAX--;
        bool jump = false;
        Event e;

        if (Keyboard::isKeyPressed(Keyboard::Right)) playerX += 0.1;
        if (Keyboard::isKeyPressed(Keyboard::Left)) playerX -= 0.1;
      
        if (Keyboard::isKeyPressed(Keyboard::Down)) speed = -200;
        if (Keyboard::isKeyPressed(Keyboard::Tab))
        {
            speed *= 3;
            oil_MAX = oil_MAX - 3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            H += 100;
            jump = true;
        }
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
            if (e.type == Event::KeyReleased)
            {
                jump = false;
            }
        }
        if (H >= 2000)
        {
            jump = false;
        }
        if (H>1500 && !(H<1500))
        {
            H = H-50;
        }
        if (oil_MAX <= 0)
        {
            oil_MAX = 0;
            speed = 0;
        }
        pos += speed;
        while (pos >= N * segL) pos -= N * segL;
        while (pos < 0) pos += N * segL;

        app.clear(Color(105, 205, 4));
        app.draw(sBackground);
        int startPos = pos / segL;
        int camH = lines[startPos].y + H;
        if (speed > 0) sBackground.move(-lines[startPos].curve * 2, 0);
        if (speed < 0) sBackground.move(lines[startPos].curve * 2, 0);

        int maxy = height;
        float x = 0, dx = 0;

        

        if (hpMax <= 0)
            hpMax = 0;
        if (hpMax >= 100)
            hpMax = 100;
        //bar
        app.draw(playerHpBarBack);
        app.draw(playerHpBar);
        float hpPercent = static_cast<float>(hpMax / 1);
        playerHpBar.setSize(sf::Vector2f(3.f * hpPercent, playerHpBar.getSize().y));
        app.draw(playerOILBarBack);
        app.draw(playerOILBar);
        float oilPercent = static_cast<float>( oil_MAX/100 );
        playerOILBar.setSize(sf::Vector2f(6.f * oilPercent, playerOILBar.getSize().y));
        ///////draw road////////
        for (int n = startPos; n < startPos + 300; n++)
        {
            Line& l = lines[n % N];
            l.project(playerX * roadW - x, camH, startPos * segL - (n >= N ? N * segL : 0));
            x += dx;
            dx += l.curve;

            l.clip = maxy;
            if (l.Y >= maxy) continue;
            maxy = l.Y;

            Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
            Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
            Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

            Line p = lines[(n - 1) % N]; //previous line

            drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
            drawQuad(app, rumble, p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
            drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
        }

        ////////draw objects////////
        for (int n = startPos + 300; n > startPos; n--)
        {
            lines[n % N].drawSprite(app);
        }
        //mark
        app.draw(mark);
        /*if (object[1].getOrigin()==mark.getPosition())
        {
            hpMax = hpMax - 1;

        }
        else if (mark.getGlobalBounds().intersects(object[4].getGlobalBounds()))
        {
            hpMax = hpMax - 100;
        }
        else if (mark.getGlobalBounds().intersects(object[5].getGlobalBounds()))
        {
            hpMax = hpMax - 100;
        }
        else if (mark.getGlobalBounds().intersects(object[7].getGlobalBounds()))
        {
            hpMax = hpMax - 100;
        }*/
        app.display();
    }

    return 0;
}


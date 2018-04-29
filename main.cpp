#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>

class Player
{
public :
    sf::RectangleShape Rect;
    sf::Time CoolDown;
    bool Shoot = false;
    int health;
    float speed;
    Player(const sf::RenderWindow &window, sf::Color Color, int healthVal, float speedVal) :
        health(healthVal),
        speed(speedVal)
    {
        Rect.setSize(sf::Vector2f(18, 76));
        Rect.setOrigin(Rect.getSize().x / 2, Rect.getSize().y / 2);
        Rect.setFillColor(Color);
    }
    virtual void action(const sf::RenderWindow &window) = 0;
    void setShoot(bool condition)
    {
        Shoot = condition;
    }
    bool getShoot()
    {
        return Shoot;
    }
    void decHealth(int amount)
    {
        health -= amount;
    }
    int getHealth()
    {
        return health;
    }
    void updateCoolDown(sf::Time elapsed)
    {
        CoolDown -= elapsed;
    }
};

class Player1 : public Player
{
public :
    Player1(const sf::RenderWindow &window, int healthVal, float speedVal) :
        Player(window, sf::Color::Blue, healthVal, speedVal)
    {
        Rect.setPosition(sf::Vector2f(window.getSize().x - Rect.getSize().x / 2, window.getSize().y / 2));
    }
    void action(const sf::RenderWindow &window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            Rect.move(0, -(speed));
            if ((Rect.getPosition().y - (Rect.getSize().y / 2)) < 0) Rect.move(0, speed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            Rect.move(0, speed);
            if ((Rect.getPosition().y + (Rect.getSize().y / 2)) > window.getSize().y) Rect.move(0, -(speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (CoolDown > sf::milliseconds(0)) return;
            Shoot = true;
            CoolDown = sf::milliseconds(400);
        }
    }
};

class Player2 : public Player
{
public :
    Player2(const sf::RenderWindow &window, int healthVal, float speedVal) :
        Player(window, sf::Color::Red, healthVal, speedVal)
    {
        Rect.setPosition(sf::Vector2f(0 + Rect.getSize().x / 2, window.getSize().y / 2));
    }
    void action(const sf::RenderWindow &window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            Rect.move(0, -(speed));
            if ((Rect.getPosition().y - (Rect.getSize().y / 2) < 0)) Rect.move(0, speed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        {
            Rect.move(0, speed);
            if ((Rect.getPosition().y + (Rect.getSize().y / 2)) > window.getSize().y) Rect.move(0, -(speed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        {
            if (CoolDown > sf::milliseconds(0)) return;
            Shoot = true;
            CoolDown = sf::milliseconds(400);
        }
    }
};

class Beam
{
public :
    const float speed;
    sf::RectangleShape Rect;
    Beam(sf::Vector2f Vector, Player &player, float speedVal) :
        speed(speedVal)
    {
        Rect.setSize(sf::Vector2f(18, 9));
        Rect.setPosition(Vector);
        Rect.setOrigin(sf::Vector2f(Rect.getSize().x / 2, Rect.getSize().y));
        Rect.setFillColor(sf::Color::Red);
        Rect.setOutlineThickness(1);
        Rect.setOutlineColor(sf::Color::White);
        player.setShoot(false);
    }
    virtual bool hit(const Player &player) = 0;
    virtual void update() = 0;
    virtual bool check(const sf::RenderWindow &window) = 0;
};

class Beam1 : public Beam
{
public :
    Beam1(sf::Vector2f Vector, Player &player, float speedVal) :
        Beam(Vector, player, speedVal)
    {}
    bool hit(const Player &player)
    {
        if (Rect.getGlobalBounds().intersects(player.Rect.getGlobalBounds())) return true;
        return false;
    }
    void update()
    {
        Rect.move(-(speed), 0);
    }
    bool check(const sf::RenderWindow &window)
    {
        if (Rect.getPosition().x < 0) return true;
        return false;
    }
};

class Beam2 : public Beam
{
public :
    Beam2(sf::Vector2f Vector, Player &player, float speedVal) :
        Beam(Vector, player, speedVal)
    {}
    bool hit(const Player &player)
    {
        if (Rect.getGlobalBounds().intersects(player.Rect.getGlobalBounds())) return true;
        return false;
    }
    void update()
    {
        Rect.move(speed, 0);
    }
    bool check(const sf::RenderWindow &window)
    {
        if (Rect.getPosition().x < 0) return true;
        return false;
    }
};

int main()
{
    std::ifstream getOption;
    int speed;
    float pspeed;
    int health1;
    int health2;
    getOption.open("option.txt");
    if (!getOption.is_open()) return -1;
    else
    {
        getOption >> pspeed >> speed >> health1 >> health2;
    }
    getOption.close();
    sf::RenderWindow window(sf::VideoMode(1080, 600), "game window");
    sf::Text DispPlayer1;
    sf::Text DispPlayer2;
    sf::Font font;
    sf::Sound Sound;
    sf::SoundBuffer Buffer;
    Player1 player1(window, health1, pspeed);
    Player2 player2(window, health2, pspeed);
    sf::Clock Clock;
    sf::Clock Clock2;
    std::vector<Beam1*> Player1Beam;
    std::vector<Beam2*> Player2Beam;
    int action;
    do
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            action = MessageBoxA(NULL, "error loading font", "error", MB_RETRYCANCEL);
        }
    }while (action == IDRETRY);
    action = 0;
    DispPlayer1.setFont(font);
    DispPlayer1.setCharacterSize(20);
    DispPlayer1.setPosition(window.getSize().x - (DispPlayer1.getCharacterSize() + DispPlayer1.getCharacterSize() + DispPlayer1.getCharacterSize()), 0);
    DispPlayer2.setFont(font);
    DispPlayer2.setCharacterSize(20);
    DispPlayer2.setPosition(0, 0);
    do
    {
        if(!Buffer.loadFromFile("Shoot.wav"))
        {
            action = MessageBoxA(NULL, "error loading Shoot.wav", "error", MB_RETRYCANCEL);
        }
    }while (action == IDRETRY);
    Sound.setBuffer(Buffer);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        for (size_t i = 0; i < Player1Beam.size(); ++i)
        {
            Player1Beam.at(i)->update();
            if (Player1Beam.at(i)->check(window))
            {
                if (Player1Beam.size() == 1);
                else
                {
                    for (size_t j = 0; j < Player1Beam.size(); ++j)
                    {
                        if (j == Player1Beam.size() - 1) break;
                        Player1Beam.at(j) = Player1Beam.at(j + 1);
                    }
                    Player1Beam.pop_back();
                }
            }
            if (Player1Beam.at(i)->hit(player2))
            {
                player2.decHealth(5);
                for (size_t j = 0; j < Player1Beam.size(); ++j)
                {
                    if (j == Player1Beam.size() - 1) break;
                    Player1Beam.at(j) = Player1Beam.at(j + 1);
                }
                Player1Beam.pop_back();
            }
        }
        for (size_t i = 0; i < Player2Beam.size(); ++i)
        {
            Player2Beam.at(i)->update();
            if (Player2Beam.at(i)->check(window))
            {
                if (Player2Beam.size() == 1);
                else
                {
                    for (size_t j = 0; j < Player2Beam.size(); ++j)
                    {
                        if (j == Player2Beam.size() - 1) break;
                        Player2Beam.at(j) = Player2Beam.at(j + 1);
                    }
                    Player2Beam.pop_back();
                }
            }
            if (Player2Beam.at(i)->hit(player1))
            {
                player1.decHealth(5);
                for (size_t j = 0; j < Player2Beam.size(); ++j)
                {
                    if (j == Player2Beam.size() - 1) break;
                    Player2Beam.at(j) = Player2Beam.at(j + 1);
                }
                Player2Beam.pop_back();
            }
        }
        player1.action(window);
        player2.action(window);
        if (player1.getShoot())
        {
            Player1Beam.push_back(new Beam1(player1.Rect.getPosition(), player1, speed));
            Sound.play();
        }
        if (player2.getShoot())
        {
            Player2Beam.push_back(new Beam2(player2.Rect.getPosition(), player2, speed));
            Sound.play();
        }
        if (player1.getHealth() == 0)
        {
            window.close();
            std::cout << "player 2 wins";
        }
        if (player2.getHealth() == 0)
        {
            window.close();
            std::cout << "player 1 wins";
        }
        sf::Time elapsed = Clock.restart();
        player1.updateCoolDown(elapsed);
        sf::Time elapsed2 = Clock2.restart();
        player2.updateCoolDown(elapsed2);
        DispPlayer1.setString(std::to_string(player1.getHealth()));
        DispPlayer2.setString(std::to_string(player2.getHealth()));
        window.clear();
        window.draw(player1.Rect);
        window.draw(player2.Rect);
        window.draw(DispPlayer1);
        window.draw(DispPlayer2);
        for (size_t i = 0; i < Player1Beam.size(); ++i)
        {
            window.draw(Player1Beam.at(i)->Rect);
        }
        for (size_t i = 0; i < Player2Beam.size(); ++i)
        {
            window.draw(Player2Beam.at(i)->Rect);
        }
        window.display();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) system("cls");
    }
    std::cin.ignore();
    return 0;
}

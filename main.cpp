#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

class Walls{
public:
    sf::Texture wallTexture;
    std::vector<sf::Sprite> wallBottom, wallTop, wallLeft, wallRight;
    bool hit=true;
    sf::Clock wallTimer;
    bool wallActive = false;
    Walls ()  {
        if (!wallTexture.loadFromFile("assets/wall.png")) {
            std::cerr << "Failed to load ball texture\n";
        }
    }
    void appearTop() {
        if (!hit) return;
        if(wallActive) return;
        hit = false;
        for (int i = 0; i < 8; ++i) {
            sf::Sprite piece;
            piece.setTexture(wallTexture);
            piece.setScale(0.1f,0.05f);
            piece.setPosition(0 + i * 102.4, -25);
            wallTop.push_back(piece);
        }
        wallActive = true;
        wallTimer.restart();
    }
    void appearBottom(){
        if (!hit) return;
        if(wallActive) return;
        hit = false;
        for (int i = 0; i < 8; ++i) {
            sf::Sprite piece;
            piece.setTexture(wallTexture);
            piece.setScale(0.1f,0.05f);
            piece.setPosition(0 + i * 102.4, 573.8);
            wallBottom.push_back(piece);
        }
        wallActive = true;
        wallTimer.restart();
    }
    void appearLeft(){
        if (!hit) return;
        if(wallActive) return;
        hit = false;
        for (int i = 0; i < 15; ++i) { //Left
            sf::Sprite piece;
            piece.setTexture(wallTexture);
            piece.setScale(0.05f,0.05f);
            piece.setPosition(-25 , 0 +i * 51.2);
            wallLeft.push_back(piece);
        }
        wallActive = true;
        wallTimer.restart();
    }
    void appearRight(){
        if (!hit) return;
        if(wallActive) return;
        hit = false;
        for (int i = 0; i < 15; ++i) {
            sf::Sprite piece;
            piece.setTexture(wallTexture);
            piece.setScale(0.05f,0.05f);
            piece.setPosition(775 , 0 +i * 51.2);
            wallRight.push_back(piece);
        }
        wallActive = true;
        wallTimer.restart();
    }
    void clearWalls() {
        wallTop.clear();
        wallBottom.clear();
        wallLeft.clear();
        wallRight.clear();
    }
    void draw(sf::RenderWindow& window) const {
        for (const auto& wall : wallTop) window.draw(wall);
        for (const auto& wall : wallBottom) window.draw(wall);
        for (const auto& wall : wallLeft) window.draw(wall);
        for (const auto& wall : wallRight) window.draw(wall);
    }
    void update() {
        if ((wallActive && wallTimer.getElapsedTime().asSeconds() > 1.5f) || hit==true) {
            clearWalls();
            wallActive = false;
        }
    }
};


class Ball{
private:
    float speed=100.0f;
    sf::Texture ballTexture;
    sf::Sprite ball;
    float angle;
public:
    Ball()  {
        if (!ballTexture.loadFromFile("assets/ball.png")) {
            std::cerr << "Failed to load wall texture\n";
        }
        ball.setTexture(ballTexture);
        ball.setPosition(400,300);
        ball.setScale(0.05f,0.05f);
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(ball);
    }
    void randomDirection(){
        float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // in range [0, 1]
        angle = t * 2.f * M_PI;
    }
    void increaseSpeed(float deltaSpeed) {
        speed += 5.0f*deltaSpeed;
    }
    void move(float dt) {
        ball.move(std::cos(angle)*speed * dt,std::sin(angle)*speed * dt);
    }
    void bounceFromWalls(Walls& walls) {
        sf::FloatRect ballBounds = ball.getGlobalBounds();
        float offset = ((rand() % 21) - 10) * (M_PI / 180);
        for (const auto& wall : walls.wallTop) {
            if (ballBounds.intersects(wall.getGlobalBounds())) {
                angle = -angle+ offset;
                walls.hit = true;
                return;
            }
        }
        for (const auto& wall : walls.wallBottom) {
            if (ballBounds.intersects(wall.getGlobalBounds())) {
                angle = -angle+ offset;
                walls.hit = true;
                return;
            }
        }
        for (const auto& wall : walls.wallLeft) {
            if (ballBounds.intersects(wall.getGlobalBounds())) {
                angle = M_PI - angle+ offset;
                walls.hit = true;
                return;
            }
        }
        for (const auto& wall : walls.wallRight) {
            if (ballBounds.intersects(wall.getGlobalBounds())) {
                angle = M_PI - angle + offset;
                walls.hit = true;
                return;
            }
        }
    }
    bool isOutOfBounds(const sf::RenderWindow& window) const {
        sf::Vector2f pos = ball.getPosition();
        sf::Vector2u size = window.getSize();
        return pos.x < -52 || pos.x > size.x + 52 || pos.y < -52 || pos.y > size.y + 52;
    }
    void reset() {
        ball.setPosition(400, 300);
        speed = 100.0f;
        randomDirection();
    }
};

int main(){
    srand(static_cast<unsigned>(time(0))); //random seed
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ball game");
    sf::Clock clock, gameClock;;
    Ball ball;
    Walls walls;
    sf::Music music;
    sf::Font font;
    sf::Text scoreText, restartText;
    bool gameOver = false;
    int score = 0;
    ball.randomDirection();

    if (!font.loadFromFile("assets/text.ttf")) {
        std::cerr << "Font load failed\n";
    }

    //music
    if (!music.openFromFile("assets/music.ogg")) {
        std::cerr << "Font load failed\n";
    }
    music.setLoop(true);
    music.play();

    //Score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 10);

    //restart text
    restartText.setFont(font);
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::Black);
    restartText.setString("Press R to restart game");
    restartText.setPosition(200, 280);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up)
                    walls.appearTop();
                else if (event.key.code == sf::Keyboard::Down)
                    walls.appearBottom();
                else if (event.key.code == sf::Keyboard::Left)
                    walls.appearLeft();
                else if (event.key.code == sf::Keyboard::Right)
                    walls.appearRight();
            }
            if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                gameOver = false;
                gameClock.restart();
                clock.restart();
                walls.clearWalls();
                walls.hit = true;
                ball.reset();
            }
        }


        float t = gameClock.getElapsedTime().asSeconds();
        float dt = clock.restart().asSeconds();

        if (!gameOver) {
            walls.update();
            ball.increaseSpeed(dt);
            ball.move(dt);
            ball.bounceFromWalls(walls);
            score = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        }

        if (!gameOver && ball.isOutOfBounds(window)) {
            gameOver = true;
        }

        scoreText.setString("Score: " + std::to_string(score));

        //background
        sf::Uint8 r = static_cast<sf::Uint8>(127 + 127 * std::sin(t));
        sf::Uint8 g = static_cast<sf::Uint8>(127 + 127 * std::sin(t + 2));
        sf::Uint8 b = static_cast<sf::Uint8>(127 + 127 * std::sin(t + 4));
        window.clear(sf::Color(r, g, b));

        ball.draw(window);
        walls.draw(window);
        window.draw(scoreText);



        if (gameOver) window.draw(restartText);
        window.display();
    }
    return 0;
}

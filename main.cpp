#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

// CONSTANTS
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

// CLASSES
class Paddle {
    public:
        sf::RectangleShape rect;
        sf::Vector2f pos;
        float w = 10;
        float h = 100;
        float velocity = 5.0;
        bool isMovingDown = false;
        bool isMovingUp = false;
        Paddle(float x, float y) {
            pos.x = x;
            pos.y = y;
            rect.setPosition(pos);
            rect.setSize(sf::Vector2f(w, h));
        };
        void update() {
            if (isMovingDown && pos.y+h < WINDOW_HEIGHT) {
                pos.y += velocity;
                rect.setPosition(pos);
            }
            if (isMovingUp && pos.y > 0) {
                pos.y -= velocity;
                rect.setPosition(pos);
            }
        }
};

class Ball {
    public:
        sf::RectangleShape rect;
        float xVelocity;
        float yVelocity;
        float acceleration = 0.1;
        const float maxVelocity = 8.0;
        sf::Vector2f pos;
        float initial_x;
        float initial_y;
        float h = 20.0;
        float w = 20.0;
        Ball(float x, float y) {
            pos.x = x;
            pos.y = y;
            initial_x = x;
            initial_y = y;
            rect.setPosition(pos);
            rect.setSize(sf::Vector2f(w, h));
            chooseRandomDirection();
        }
        void update() {
            if (pos.y < 0 || pos.y > WINDOW_HEIGHT-h) {
                bounceY();
            }
            if (pos.x < 0 || pos.x+w > WINDOW_WIDTH) {
                bounceX();
            }
            pos.x += xVelocity;
            pos.y += yVelocity;
            rect.setPosition(pos);
        }
        bool collision(Paddle &paddle) {
            if (pos.y >= paddle.pos.y + paddle.h) {
                return false;
            }
            if (pos.x >= paddle.pos.x + paddle.w) {
                return false;
            }
            if (pos.y + h <= paddle.pos.y) {
                return false;
            }
            if (pos.x + w <= paddle.pos.x) {
                return false;
            }
            return true;
        }
        void bounceX() {
            xVelocity *= -1;
        }
        void bounceY() {
            yVelocity *= -1;
        }
        void increaseVelocity() {
            if (xVelocity < maxVelocity && yVelocity < maxVelocity) {
                xVelocity += xVelocity*acceleration;
                yVelocity += yVelocity*acceleration;
            }
        }
        void reset() {
            chooseRandomDirection();
            pos.x = initial_x;
            pos.y = initial_y;
            rect.setPosition(pos);
        }
        void chooseRandomDirection() {
            int x = rand() % 4;
            switch (x) {
                case 0:
                    xVelocity = -4.0;
                    yVelocity = -4.0;
                    break;
                case 1:
                    xVelocity = 4.0;
                    yVelocity = 4.0;
                    break;
                case 2:
                    xVelocity = -4.0;
                    yVelocity = 4.0;
                    break;
                case 3:
                    xVelocity = 4.0;
                    yVelocity = -4.0;
                    break;
            }
        }
};

class Score {
    public:
        float padding = 400;
        sf::Text text;
        sf::Vector2f pos;
        float width;
        int score;
        Score(sf::Font &font) {
            score = 0;
            text.setFont(font);
            text.setString(std::to_string(score));
            width = text.getLocalBounds().width;
        }
        void setPosition(float x, float y) {
            pos.x = x;
            pos.y = y;
            text.setPosition(pos);
        }
        void increaseScore() {
            score += 1;
            text.setString(std::to_string(score));
        }
};

int main() {
    
    // FONTS
    sf::Font font;
    font.loadFromFile("./Ubuntu-M.ttf");

    // WINDOW OBJECT
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Two Player Pong");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    // BALL OBJECT
    Ball ball(600, 350);

    // SCORE
    Score scorePlayerOne(font);
    scorePlayerOne.setPosition(scorePlayerOne.padding-(scorePlayerOne.width/2), 0);
    
    Score scorePlayerTwo(font);
    scorePlayerTwo.setPosition(WINDOW_WIDTH-scorePlayerTwo.padding-(scorePlayerTwo.width/2), 0);

    // PLAYER PADDLES
    Paddle playerOne(10, (WINDOW_HEIGHT/2));
    Paddle playerTwo(WINDOW_WIDTH-10-10, WINDOW_HEIGHT/2);

    // GAME LOOP
    while (window.isOpen()) {

        // EVENT PROCESSING
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                playerTwo.isMovingDown = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                playerTwo.isMovingUp = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                playerOne.isMovingDown = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                playerOne.isMovingUp = true;
            }
            if (event.type == event.KeyReleased) {
                if (event.key.code == sf::Keyboard::Down) {
                    playerTwo.isMovingDown = false;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    playerTwo.isMovingUp = false;
                }
                if (event.key.code == sf::Keyboard::S) {
                    playerOne.isMovingDown = false;
                }
                if (event.key.code == sf::Keyboard::W) {
                    playerOne.isMovingUp = false;
                }
            }
        }

        if (ball.pos.x < 0) {
            scorePlayerTwo.increaseScore();
            ball.reset();
        }

        if (ball.pos.x+ball.w > WINDOW_WIDTH) {
            scorePlayerOne.increaseScore();
            ball.reset();
        }

        if (ball.collision(playerOne)) {
            ball.bounceX();
            ball.increaseVelocity();
        }

        if (ball.collision(playerTwo)) {
            ball.bounceX();
            ball.increaseVelocity();
        }

        ball.update();
        playerOne.update();
        playerTwo.update();

        // RENDER
        window.clear();
        window.draw(ball.rect);
        window.draw(playerOne.rect);
        window.draw(playerTwo.rect);
        window.draw(scorePlayerOne.text);
        window.draw(scorePlayerTwo.text);
        window.display();
    }
    return 0;
}

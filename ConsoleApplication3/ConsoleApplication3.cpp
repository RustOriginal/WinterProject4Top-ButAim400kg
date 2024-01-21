#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

const int windowWidth = 1000;
const int windowHeight = 800;
const int initialTargetSize = 50;
const int targetSizeIncreaseRate = 2;

class Target {
public:
    Target() {
        respawn();
    }

    void respawn() {
        size = initialTargetSize;
        shape.setSize(sf::Vector2f(size, size));
        shape.setFillColor(sf::Color::Red);

        float x = static_cast<float>(rand() % (windowWidth - size));
        float y = static_cast<float>(rand() % (windowHeight - size));
        shape.setPosition(x, y);

        growing = true;  // Начинаем увеличение при каждом появлении
    }

    void decreaseSize() {
        size = std::max(0, size - targetSizeIncreaseRate);
        shape.setSize(sf::Vector2f(size, size));
        growing = false;  // После уменьшения переходим в состояние увеличения
    }

    void increaseSize() {
        size += targetSizeIncreaseRate;
        shape.setSize(sf::Vector2f(size, size));
        growing = true;  // После увеличения переходим в состояние уменьшения
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

    int getSize() const {
        return size;
    }

    bool isGrowing() const {
        return growing;
    }

    void stopGrowing() {
        growing = false;
    }

private:
    int size;
    sf::RectangleShape shape;
    bool growing;
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Aim Trainer");
    window.setFramerateLimit(60);

    Target target;

    int score = 0;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (target.getBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    target.respawn();
                    target.decreaseSize();
                    score++;
                }
            }
        }

        if (!target.isGrowing()) {
            target.increaseSize();
            target.stopGrowing();
        }

        std::ostringstream scoreStr;
        scoreStr << "Score: " << score;
        scoreText.setString(scoreStr.str());

        window.clear();

        target.draw(window);
        window.draw(scoreText);

        window.display();
    }

    return 0;
}

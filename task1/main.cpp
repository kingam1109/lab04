#include <iostream>
#include <cstdlib>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CustomRectangleShape : public sf::RectangleShape{
private:
    double _verticalspeed = 0;
    double _horizontalspeed = 0;
    double _rotationspeed = 0;
    int _left = 0;
    int _right = 0;
    int _top = 0;
    int _bottom = 0;

public:
    CustomRectangleShape(const sf::Vector2f& size, const sf::Vector2f& position) : sf::RectangleShape(size){
        setPosition(position);
    }
    void setSpeed(double vert, double horiz, double rotation){
        _verticalspeed = vert;
        _horizontalspeed = horiz;
        _rotationspeed = rotation;
    }

    void setBounds(int left, int right, int top, int bottom){
        _left = left;
        _right = right;
        _top = top;
        _bottom = bottom;
    }

    void moveInDirection(const sf::Time &elapsed, const sf::Keyboard::Key &key){
        float dt = elapsed.asSeconds();

        sf::Vector2f movement(0.f, 0.f);

        if(key == sf::Keyboard::Up)
        {
            movement.y = -std::abs(_verticalspeed);
        }

        if(key == sf::Keyboard::Down)
        {
            movement.y = std::abs(_verticalspeed);
        }

        if(key == sf::Keyboard::Left)
        {
            movement.x = -std::abs(_horizontalspeed);
        }

        if(key == sf::Keyboard::Right)
        {
            movement.x = std::abs(_horizontalspeed);
        }

         move(movement * dt);

        sf::Vector2f pos = getPosition();
        sf::Vector2f size = getSize();

        if (pos.x < _left) {
            setPosition(_left, pos.y);
            _verticalspeed = std::abs(_verticalspeed);
        }

        else if (pos.x + size.x > _right) {
            setPosition(_right - size.x, pos.y);
            _verticalspeed = -std::abs(_verticalspeed);
        }

        if (pos.y < _top) {
            setPosition(pos.x, _top);
            _horizontalspeed = std::abs(_horizontalspeed);
        }

        else if (pos.y + size.y > _bottom) {
            setPosition(pos.x, _bottom - size.y);
            _horizontalspeed = -std::abs(_horizontalspeed);
        }
    }

    bool isClicked(sf::Vector2i &mouse_position) const{
        sf::Vector2f mouse_pos(mouse_position);

        return getGlobalBounds().contains(mouse_pos);
    }

};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::Vector2f size(120.0, 60.0);
    sf::Vector2f position(120.0, 60.0);
    CustomRectangleShape rectangle(size, position);
    rectangle.setFillColor(sf::Color(100, 50, 250));
    rectangle.setSpeed(100, 150, 10);
    rectangle.setBounds(0, window.getSize().x, 0, window.getSize().y);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                if (rectangle.isClicked(mouse_pos)) {
                    rectangle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                }
            }
        }

        sf::Time elapsed = clock.restart();

        rectangle.moveInDirection(elapsed, sf::Keyboard::Unknown);

        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}

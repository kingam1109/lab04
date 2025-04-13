#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

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
    bool _selected = false;

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

    void setSelected(bool selected) {
        _selected = selected;
        if (_selected)
            setFillColor(sf::Color::Red);
        else
            setFillColor(sf::Color(0, 255, 0));
    }

    bool isSelected() const {
        return _selected;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    std::srand(std::time(nullptr));

    std::vector<CustomRectangleShape> rectangles;

    for (int i = 0; i < 10; i++) {
        sf::Vector2f size(120.0, 60.0);
        sf::Vector2f position(std::rand() % (window.getSize().x - 120), std::rand() % (window.getSize().y - 60));
        CustomRectangleShape rect(size, position);
        rect.setFillColor(sf::Color(0, 255, 0));
        rect.setBounds(0, window.getSize().x, 0, window.getSize().y);
        rect.setSpeed(200, 200, 0);
        rectangles.emplace_back(rect);
    }
    sf::Clock clock;

    CustomRectangleShape* selectedRect = nullptr;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();  // dodaj to tutaj!

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                for (auto& rect : rectangles) {
                    if (rect.isClicked(mousePos)) {
                        if (selectedRect) {
                            selectedRect->setSelected(false);
                        }
                        rect.setSelected(true);
                        selectedRect = &rect;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && selectedRect != nullptr) {
                selectedRect->moveInDirection(elapsed, event.key.code);
            }
        }
        window.clear(sf::Color::Black);

        for (auto& rect : rectangles) {
            window.draw(rect);
        }

        window.display();

    }

    return 0;
}

#include "SFML/Graphics.hpp"

#include <iostream>

using namespace std;

int main(){

    sf::RenderWindow window(
        sf::VideoMode(512, 512, 32U), 
        "SFML_Test_Window"
    );

    // circle object construction
    // sf::CircleShape test_circ(100); // args-1: rad, arg-2 num rectangles used for rendering target
    // test_circ.setFillColor(sf::Color::Blue);
    // test_circ.setOrigin(sf::Vector2f(200/2, 200/2));
    // test_circ.setPosition(sf::Vector2f(512/2, 512/2));
    
    // rectangle object construction
    float rec_width = 50.0, rec_height = 10.0;
    sf::RectangleShape rec(sf::Vector2f(50.0,10.0));
    rec.setFillColor(sf::Color::Blue);
    rec.setOrigin(sf::Vector2f(rec_width, rec_height));
    rec.setPosition(sf::Vector2f(512/2,512/2));

    while(window.isOpen()){
        sf::Event event;
        // window close handle
        while(window.pollEvent(event)){
            if(event.type == sf::Event::KeyPressed){
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                    window.close();
                }
            }
        }
        window.clear(sf::Color::Black);

        // drawing a dot or a block or coloring a pixel
        window.draw(rec);

        window.display();

    }


    



    return 0;

}


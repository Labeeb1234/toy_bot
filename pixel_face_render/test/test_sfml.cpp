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
    // float aspect_ratio = 0.5; // unused
    float rec_width = 100.0, rec_height = 50.0;
    // sf::RectangleShape rec(sf::Vector2f(rec_width,rec_height));
    // rec.setFillColor(sf::Color::Blue);
    // rec.setOrigin(sf::Vector2f(rec_width, rec_height));
    // rec.setPosition(sf::Vector2f(512/2,512/2));


    // using convex shape func for some polygon drawing
    sf::ConvexShape polygon;
    polygon.setPointCount(4);
    // clockwise cycle ordering (manually set)
    polygon.setPoint(0, sf::Vector2f(0, 0));
    polygon.setPoint(1, sf::Vector2f(rec_width, 0));
    polygon.setPoint(2, sf::Vector2f(rec_width, rec_height));
    polygon.setPoint(3, sf::Vector2f(0, rec_height));
    polygon.setFillColor(sf::Color::Blue);
    polygon.setOrigin(sf::Vector2f(rec_width/2, rec_height/2));
    polygon.setPosition(sf::Vector2f(512/2, 512/2));

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
        window.draw(polygon);

        window.display();

    }


    



    return 0;

}


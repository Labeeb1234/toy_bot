#include "SFML/Graphics.hpp"

#include <iostream>
#include <cmath>


using namespace std;

// a global util to draw rounded rectangles
// 4 corners of rec(4) + resolution{num_points} of the corner arcs 4+some_num

class RoundedRectangle{
private:
size_t arc_res_;
size_t current_point_count_ = 0;
sf::ConvexShape* rounded_rectangle_{nullptr};
float rec_width_;
float rec_height_;

void addArc(
    sf::ConvexShape* rounded_rectangle,
    float fillet_radius,
    float start_angle,
    float end_angle,
    size_t arc_res,
    float cx, float cy
){
    size_t current_point_count = rounded_rectangle->getPointCount(); // if any (add error handle)
    rounded_rectangle->setPointCount(current_point_count+arc_res);
    for(int i=0; i<static_cast<int>(arc_res); ++i){
        float d = static_cast<float>(i)/(arc_res-1);
        float angle_i = start_angle + d*(end_angle-start_angle); // in [rads]
        float x_i = cx+fillet_radius*cos(angle_i);
        float y_i = cy+fillet_radius*sin(angle_i);
        rounded_rectangle->setPoint(i, sf::Vector2f(x_i, y_i));
    }
}





public:

RoundedRectangle(
    size_t arc_resolution,
    float rectangle_width,
    float rectangle_height
): arc_res_(arc_resolution), rec_width_(rectangle_width), rec_height_(rectangle_height){
    current_point_count_ = arc_res_; // starting with resolution for the first arc
    rounded_rectangle_ = new sf::ConvexShape();
    rounded_rectangle_->setPointCount(current_point_count_);
}
~RoundedRectangle(){rounded_rectangle_{nullptr};}

void addRoundedRec(
    sf::ConvexShape* rounded_rectangle,
    size_t arc_res, float rectangle_width, 
    float rectangle_height, float fillet_radius
){
    // clockwise contruction order

    // adding arc-1
    // cx,cy for first arc is centred around (0,0) pixels later then shifted to view
    float cx_tl = 0, float cy_tl = 0;
    addArc(rounded_rectangle, 50, -M_PI/2, -M_PI, 100, cx_tl, cy_tl);
    size_t num_points = rounded_rectangle->getPointCount();
    rounded_rectangle->setPointCount(num_points+2);
    // calculate first rec_section point top-left-2
    float x_tl1 = cx_tl+fillet_radius*cos(-M_PI/2);
    float y_tl1 = cy_tl+fillet_radius*sin(-M_PI/2);
    rounded_rectangle->setPoint(num_points+0, sf::Vector2f(x_tl1, y_tl1));
    // top-right point-1
    float x_tr1 = x_tl1+rectangle_width;
    float y_tr1 = y_tl1;
    rounded_rectangle->setPoint(num_point+1, sf::Vector2f(x_tr1, y_tr1));

    // adding arc-2
    // calc cx, and cy for arc-2
    float cx_tr = x_tr1-fillet_radius*cos(-M_PI/2);
    float cy_tr = y_tr1-fillet_radius*sin(-M_PI/2);
    addArc(rounded_rectangle, 50, 0, -M_PI/2, 100, cx_tr, cy_tr);
    num_points = rounded_rectangle->getPointCount();
    rounded_rectangle->setPointCount(num_points+2);
    // calculate 2nd rec_section point top-right-2
    float x_tr2 = cx_tr+fillet_radius*cos(0);
    float y_tr2 = cy_tr+fillet_radius*sin(0);
    rounded_rectangle->setPoint(num_points+0, sf::Vector2f(x_tl, y_tl));
    // bottom_right-1
    float x_br1 = x_tr2;
    float y_br1 = y_tr2+rectangle_height;
    rounded_rectangle->setPoint(num_point+1, sf::Vector2f(x_tr, y_tr));

    // adding arc-3
    float cx_br = x_br1-fillet_radius*cos(M_PI/2);
    float cy_br = y_br1-fillet_radius*sin(M_PI/2);
    addArc(rounded_rectangle, 50, M_PI/2, 0, 100, cx_br, cy_br);
    num_points = rounded_rectangle->getPointCount();
    rounded_rectangle->setPointCount(num_points+2);
    // calculate 3-rec section points bottom-right-2
    float x_br2 = cx_br+fillet_radius*cos(M_PI/2);
    float y_br2 = cy_br+fillet_radius*sin(M_PI/2);
    rounded_rectangle->setPoint(num_points+0, sf::Vector2f(x_br2, y_br2));
    // calculate bottom-left-1 point
    float x_bl1 = x_br2-rectangle_width;
    float y_bl1 = y_br2;
    rounded_rectangle->setPoint(num_points+1, sf::Vector2f(x_bl1, y_bl1));

    // adding arc-4
    // calc cx, cy for arc-4
    float cx_bl = x_bl1-fillet_radius*cos(0);
    float cy_bl = y_bl1-fillet_radius*sin(0);
    addArc(rounded_rectangle, 50, 0, -M_PI/2, 100, cx_bl, cy_bl);
    num_points = rounded_rectangle->getPointCount();
    // calculate the 4-sec bottom left point-2
    float x_bl2 = cx_bl+fillet_radius*cos(-M_PI/2);
    float y_bl2 = cy_bl+fillet_radius*sin(-M_PI/2);
    rounded_rectangle->setPoint(num_points+0, sf::Vector2f(x_bl2, y_bl2));
    // calculate top-left-1 point
    float x_tl1 = x_bl2;
    float y_tl1 = y_bl2-rectangle_height;
    rounded_rectangle->setPoint(num_points+1, sf::Vector2f(x_tl1, y_tl1));
}


};
















struct Eye{} eye;
struct Face{} face;






int main(int argc, char** argv){

    unsigned int window_size = 512; // let it be square window
    sf::RenderWindow window(
        sf::VideoMode(window_size, window_size, 32U), 
        "Two_Eye_Face_Animation_Testing"
    );


    // float aspect_ratio = 0.5; // unused
    float rec_width = 100.0, rec_height = 50.0;


    // using convex shape func for some rounded_rec drawing
    sf::ConvexShape rounded_rec;
    rounded_rec.setPointCount(4);
    // clockwise cycle ordering (manually set)
    rounded_rec.setPoint(0, sf::Vector2f(0, 0));
    rounded_rec.setPoint(1, sf::Vector2f(rec_width, 0));
    rounded_rec.setPoint(2, sf::Vector2f(rec_width, rec_height));
    rounded_rec.setPoint(3, sf::Vector2f(0, rec_height));
    rounded_rec.setFillColor(sf::Color::Blue);
    rounded_rec.setOrigin(sf::Vector2f(rec_width/2, rec_height/2));
    rounded_rec.setPosition(sf::Vector2f(window_size/2, window_size/2));

    // rounding rec method
    sf::ConvexShape quat;
    float circle_radius = 50;
    quat.setPointCount(102);
    float start_angle = -M_PI/2;
    float end_angle = -M_PI;
    for(int i=0; i<static_cast<int>(quat.getPointCount())-2; ++i){
        float d = static_cast<float>(i)/(quat.getPointCount()-1-2);
        float angle_i = start_angle + d*(end_angle-start_angle); // A.P 
        float x_i = circle_radius*cos(angle_i);
        float y_i = circle_radius*sin(angle_i);
        quat.setPoint(i, sf::Vector2f(x_i, y_i));
    }
    // line starts
    float x_s1 = circle_radius*cos(start_angle);
    float y_s1 = circle_radius*sin(start_angle);
    quat.setPoint(100, sf::Vector2f(x_s1, y_s1));
    quat.setPoint(101, sf::Vector2f(x_s1+rec_width, y_s1));
    quat.setFillColor(sf::Color::Blue);
    quat.setOrigin(sf::Vector2f(rec_width/2, rec_height/2));
    quat.setPosition(sf::Vector2f(window_size/2, window_size/2));






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
        window.draw(quat);

        window.display();

    }


    



    return 0;

}


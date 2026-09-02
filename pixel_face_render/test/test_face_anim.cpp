#include "SFML/Graphics.hpp"

#include <iostream>
#include <cmath>
#include <cstdio>
#include <random>


using namespace std;
// aborting rounded rectangular eyes due to time constraints (primary test for the facial animations for now)
// all units are in pixel scales for now
struct Eye{
    // properties
    // eye dimensions (Basics)
    float eye_length{100.0f}; // rec_width (wrt sfml window conventions)
    float eye_width{50.0f}; // rec_height
    float eye_posx{0.0f}; // topLeft corner of the rec
    float eye_posy{0.0f}; // topLeft corner of the rec

    float openness{1.0f}; // values between [0,1]
    float gaze_x{0.0f}; // for pupil motion
    float gaze_y{0.0f};
    float rotation{0.0f}; // only 3d rotations needs to be in quaternions (we are in 2D btw so just yaw is there)

    // pupil
    float pupil_radius{10.0f};
    // put the pupil TF origin at the REC EYE's centre/ otherwise referred as pupil offset wrt centre of eye
    float pupil_offsetX{0.0f};
    float pupil_offsetY{0.0f};
    // rotation is useless for a circle symmetry
};

class Face{
private:
    // some important attribs
    Eye eye_left_;
    Eye eye_right_;
    sf::RenderTexture face_canvas_;

    // properties
    float eye_separation_{50.0f}; // right top corner of left eye to left top corner of the right eye
    unsigned int face_size_{512};

    sf::ConvexShape make_eye(
        const float eye_posx, const float eye_posy, 
        const float eye_len, const float eye_width
    ){
        sf::ConvexShape rec_eye;
        rec_eye.setPointCount(4);
        rec_eye.setPoint(0, sf::Vector2f(eye_posx, eye_posy));
        rec_eye.setPoint(1, sf::Vector2f(eye_posx+eye_len, eye_posy));
        rec_eye.setPoint(2, sf::Vector2f(eye_posx+eye_len, eye_posy+eye_width));
        rec_eye.setPoint(3, sf::Vector2f(eye_posx, eye_posy+eye_width));
        rec_eye.setFillColor(sf::Color::Blue);
        return rec_eye;
    }
    sf::CircleShape make_pupil(
        const float pupil_radius, 
        const float eye_pos_x, const float eye_pos_y,
        const float eye_len, const float eye_width,
        const float pupil_offset_x, const float pupil_offset_y
    ){
        sf::CircleShape circle_pupil(pupil_radius);
        float pupil_pos_x = eye_pos_x + (eye_len)/2 + pupil_offset_x;
        float pupil_pos_y = eye_pos_y + (eye_width/2) + pupil_offset_y;
        circle_pupil.setFillColor(sf::Color::White);
        circle_pupil.setOrigin(sf::Vector2f(pupil_radius, pupil_radius));
        circle_pupil.setPosition(sf::Vector2f(pupil_pos_x, pupil_pos_y)); // wrt the respective eye
        return circle_pupil;
    }


public:

    Face(unsigned int face_size): face_size_(face_size){
        // create render texture
        if(!face_canvas_.create(face_size, face_size_)){
            cerr << "Failed to create face RenderTexture" << endl;
        }

        face_canvas_.clear(sf::Color::Black);
        // initialize the eye positions (right wrt left)
        // left eye
        eye_left_.eye_posx = 128.0f; // make class args and cmdline args later on
        eye_left_.eye_posy = 128.0f; // ,,
        eye_left_.eye_length = 100.f; // ,,
        eye_left_.eye_width = 50.0f; // ,,
        // right eye
        eye_right_.eye_posx = (eye_left_.eye_posx + eye_separation_ + eye_left_.eye_length);
        eye_right_.eye_posy = eye_left_.eye_posy;
        eye_right_.eye_length = eye_left_.eye_length;
        eye_right_.eye_width = eye_left_.eye_width;

    }
    ~Face()=default; // normal complier generated destruction (note for future)


    void create_face(){
        sf::ConvexShape eye_left = make_eye(
            eye_left_.eye_posx, eye_left_.eye_posy,
            eye_left_.eye_length, eye_left_.eye_width
        );
        sf::CircleShape pupil_left = make_pupil(
            eye_left_.pupil_radius, eye_left_.eye_posx, eye_left_.eye_posy,
            eye_left_.eye_length, eye_left_.eye_width,
            eye_left_.pupil_offsetX, eye_left_.pupil_offsetY
        );
        sf::ConvexShape eye_right = make_eye(
            eye_right_.eye_posx, eye_right_.eye_posy,
            eye_right_.eye_length, eye_right_.eye_width
        );
        sf::CircleShape pupil_right = make_pupil(
            eye_right_.pupil_radius, eye_right_.eye_posx, eye_right_.eye_posy,
            eye_right_.eye_length, eye_right_.eye_width,
            eye_right_.pupil_offsetX, eye_right_.pupil_offsetY
        );
        face_canvas_.draw(eye_left);
        face_canvas_.draw(eye_right);
        face_canvas_.draw(pupil_left);
        face_canvas_.draw(pupil_right);
        // render texture
        face_canvas_.display();
    }

    void draw_face(sf::RenderWindow& window){
        sf::Sprite face_sprite(face_canvas_.getTexture());
        window.draw(face_sprite); // important for animations control later on outside class
    }
    // important exposes
    const sf::Texture& get_face()const{
        return face_canvas_.getTexture();
    }
    Eye& get_left_eye(){
        return eye_left_;
    }
    Eye& get_right_eye(){
        return eye_right_;
    }
};


// gaze anim testing

int main(int argc, char** argv){

    unsigned int window_size = 512; // let it be square window
    sf::RenderWindow window(
        sf::VideoMode(window_size, window_size, 32U), 
        "Two_Eye_Face_Animation_Testing"
    );
    // init clock object for SFML window
    sf::Clock clock;


    Face* face = new Face(512);
    face->create_face();


    double t = 0.0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> amp_dist(1.0f, 10.0f);
    std::uniform_real_distribution<float> omega_dist(5.0f, 6.0f);
    std::uniform_real_distribution<float> phase_dist(0.0f, 2.0f * M_PI);
    
    float amp1 = amp_dist(gen);
    float omega_driving1 = omega_dist(gen); // in rads/s
    float phase1 = phase_dist(gen);
    float amp2 = amp_dist(gen);
    float omega_driving2 = omega_dist(gen);
    float phase2 = phase_dist(gen);


    // change forcing function to LERP animation for naturallity of gazing (TODO)

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

        // get animation update rate
        float dt = clock.restart().asSeconds();
        
        // physics/setup update
        t += dt;
        Eye& left_eye = face->get_left_eye(); // return by reference
        Eye& right_eye = face->get_right_eye();
        float left_pupil_offsetX = amp1*cos(omega_driving1*t+phase1); // TODO: clamp withing eye dimensions
        float left_pupil_offsetY = amp1*cos(omega_driving1*t+phase1);
        left_eye.pupil_offsetX = left_pupil_offsetX;
        left_eye.pupil_offsetY = left_pupil_offsetY;

        float right_pupil_offsetX = amp2*sin(omega_driving2*t+phase2);
        float right_pupil_offsetY = amp2*sin(omega_driving2*t+phase2);
        right_eye.pupil_offsetX = right_pupil_offsetX;
        right_eye.pupil_offsetY = right_pupil_offsetY;

        // render
        window.clear(sf::Color::Black);
        face->create_face();
        // drawing face (static for now)
        face->draw_face(window);
        window.display();
    }

    delete face;
    return 0;
}


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>
#include <vector>


int width = 800;
int height = 600;

class anatomy{
public:
    float velocity;
    float rotation;
    sf::Transform T;
    sf::Transform R;

    void set_matix(sf::Vector2f clawPosition, float vel, float rot){
        rotation = rot;
        velocity = vel;
        T.translate(clawPosition);
        R.rotate(rotation);
    }
    void updatePosition(float rotate){
        rotation = rotate;
        sf::Transform Rt;
        Rt.rotate(rotation);
        R = Rt;
    }
};

class limb : public anatomy {
public:
    sf::VertexArray the_limb;
    sf::Vector2f limb_lines[5];

    limb(int length, int width, sf::Color color, sf::Vector2f limbPosition, float vel) {
        the_limb = sf::VertexArray(sf::LinesStrip, 5);
        limb_lines[0]=sf::Vector2f(0,0);
        limb_lines[1]=sf::Vector2f(length,0);
        limb_lines[2]=sf::Vector2f(length,width);
        limb_lines[3]=sf::Vector2f(0,width);
        limb_lines[4]=limb_lines[0];

        for(int i=0;i<5;i++){
            the_limb[i].color = color;
        }
        set_matix(limbPosition, vel, 0);
    }
};
class claw : public anatomy {
public:
    claw(sf::Vector2f clawPosition, float vel, float rot) {
        set_matix(clawPosition, vel, rot);
    }
};
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(width, height, 32), "Robot Arm");
    window.setFramerateLimit(60);

    int length = 150;
    int claw_length = 100;
    int width = 30;
    const static int size = 7;
    sf::Vector2f left_claw_lines[size];
    sf::Vector2f right_claw_lines[size];

    limb upper_arm = limb(length, width, sf::Color::Blue, sf::Vector2f(width/2+length,height/2), 3);
    limb fore_arm = limb(length, width, sf::Color::Green, sf::Vector2f(length,0), 1);

    sf::VertexArray left_claw = sf::VertexArray(sf::LinesStrip, size);
    left_claw_lines[0]=sf::Vector2f(0,0);
    left_claw_lines[1]=sf::Vector2f(100,0);
    left_claw_lines[2]=sf::Vector2f(100,100);
    left_claw_lines[3]=sf::Vector2f(100-width,100-width);
    left_claw_lines[4]=sf::Vector2f(100-width,width);
    left_claw_lines[5]=sf::Vector2f(0,width);
    left_claw_lines[6]=left_claw_lines[0];
    claw left_claw_limb = claw(sf::Vector2f(length,15), 1, 270);


    sf::VertexArray right_claw = sf::VertexArray(sf::LinesStrip, size);
    right_claw_lines[0]=sf::Vector2f(0,0);
    right_claw_lines[1]=sf::Vector2f(100,0);
    right_claw_lines[2]=sf::Vector2f(100,-100);
    right_claw_lines[3]=sf::Vector2f(100-width,-40-width);
    right_claw_lines[4]=sf::Vector2f(100-width,-width);
    right_claw_lines[5]=sf::Vector2f(0,-width);
    right_claw_lines[6]=right_claw_lines[0];
    claw right_claw_limb = claw(sf::Vector2f(length,15), 1, 90);

    for(int i=0;i<size;i++){
        left_claw[i].color = sf::Color::Red;
        right_claw[i].color = sf::Color::Red;
    }

    // Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();

//            Upper Arm Rotation
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                if(upper_arm.rotation < 75)
                    upper_arm.updatePosition(upper_arm.rotation += upper_arm.velocity);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                if(upper_arm.rotation > -75)
                    upper_arm.updatePosition(upper_arm.rotation -= upper_arm.velocity);

//            Fore Arm Rotation
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                if(fore_arm.rotation < 25) {
                    fore_arm.updatePosition(fore_arm.rotation += fore_arm.velocity);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                if(fore_arm.rotation > -115)
                    fore_arm.updatePosition(fore_arm.rotation -= fore_arm.velocity);

//            Claws Rotation
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                if(left_claw_limb.rotation < 315)
                    left_claw_limb.updatePosition(left_claw_limb.rotation += left_claw_limb.velocity);
                if(right_claw_limb.rotation > 45)
                    right_claw_limb.updatePosition(right_claw_limb.rotation -= right_claw_limb.velocity);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if(left_claw_limb.rotation > 270)
                    left_claw_limb.updatePosition(left_claw_limb.rotation -= left_claw_limb.velocity);
                if(right_claw_limb.rotation < 90)
                    right_claw_limb.updatePosition(right_claw_limb.rotation += right_claw_limb.velocity);
            }
        }

        sf::Transform Mua=upper_arm.T*upper_arm.R;
        sf::Transform Mfa=Mua*fore_arm.T*fore_arm.R;
        sf::Transform Mlc=Mfa*left_claw_limb.T*left_claw_limb.R;
        sf::Transform Mrc=Mfa*right_claw_limb.T*right_claw_limb.R;

        //update sun
        for(int i=0;i<5;i++){
            upper_arm.the_limb[i].position=Mua.transformPoint(upper_arm.limb_lines[i]);
            fore_arm.the_limb[i].position=Mfa.transformPoint(fore_arm.limb_lines[i]);
        }
        for(int i=0;i<7;i++){
            left_claw[i].position=Mlc.transformPoint(left_claw_lines[i]);
            right_claw[i].position=Mrc.transformPoint(right_claw_lines[i]);
        }


        //prepare frame
        window.clear();

        //draw frame items
        window.draw(upper_arm.the_limb);
        window.draw(fore_arm.the_limb);
        window.draw(left_claw);
        window.draw(right_claw);


        // Finally, display rendered frame on screen
        window.display();
    } //loop back for next frame

    return EXIT_SUCCESS;
}

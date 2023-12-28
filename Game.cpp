#include <SFML/Graphics.hpp>

using namespace sf;

int main() {

    //Rendering a window with close button and TitleBar
    RenderWindow window(VideoMode(1400, 700), "Cadillacs and Dinosaurs Game", Style::Close | Style::Titlebar);

    Image image; //Manipulating and storing image

        //Checking whether it is possible to load image from a file or not
    if (image.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/bg1.png")) {

        Texture texture,texture1; //Encapsulating textures

        texture.loadFromImage(image);  //Loading texture from an image

        texture1.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.png"); //Loading texture from an image file


        Sprite sprite(texture),sprite1(texture1); //Drawing texture


        // Set the initial position
        sprite1.setPosition(250.0f, 400.0f);


        while (window.isOpen()) { //loop will continue if window is not closed

            Event event{}; //Defining system event

            while (window.pollEvent(event)) { //Loop will continue if event queue is not empty

                if (event.type == Event::Closed) {  //Checking whether the window is requested to be closed or not

                    window.close();  //Closing the window
                }
            }


            // Move the sprite based on keyboard input
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                sprite1.move(-1.0f, 0.0f);
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                sprite1.move(1.0f, 0.0f);
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                sprite1.move(0.0f, -1.0f);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                sprite1.move(0.0f, 1.0f);
            }



            // Getting the size of the window
            Vector2u windowSize = window.getSize();

            // Setting the scale of the sprite to fill the window
            sprite.setScale(static_cast<float>(windowSize.x) / sprite.getLocalBounds().width,
                            static_cast<float>(windowSize.y) / sprite.getLocalBounds().height);

            window.clear(); //Clearing the previous contents of the target
            window.draw(sprite); //Drawing sprite
            window.draw(sprite1); //Drawing sprite
            window.display(); //Displaying on the screen
        }
    }

    return 0;
}

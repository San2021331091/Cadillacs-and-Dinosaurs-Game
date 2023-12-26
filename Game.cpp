#include <SFML/Graphics.hpp>

using namespace sf;

int main() {

    //Rendering a window with close button and TitleBar
    RenderWindow window(VideoMode(1200, 700), "Cadillacs and Dinosaurs Game", Style::Close | Style::Titlebar);

    Image image; //Manipulating and storing image

        //Checking whether it is possible to load image from a file
    if (image.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/bg1.png")) {

        Texture texture; //Encapsulating textures

        texture.loadFromImage(image);  //Loading texture from an image

        Sprite sprite(texture);  //Drawing texture

        while (window.isOpen()) { //loop will continue if window is not closed

            Event event{}; //Defining system event

            while (window.pollEvent(event)) { //Loop will continue if event queue is not empty
                if (event.type == Event::Closed) {

                    window.close();  //Closing the window
                }
            }

            // Getting the size of the window
            Vector2u windowSize = window.getSize();

            // Setting the scale of the sprite to fill the window
            sprite.setScale(static_cast<float>(windowSize.x) / sprite.getLocalBounds().width,
                            static_cast<float>(windowSize.y) / sprite.getLocalBounds().height);

            window.clear(); //Clearing the previous contents of the target
            window.draw(sprite); //Drawing sprite
            window.display(); //Displaying on the screen
        }
    }

    return 0;
}

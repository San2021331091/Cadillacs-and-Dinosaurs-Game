#include "game.h"

struct game_t* game;

const float PI = 22.0f/7.0;





int main() {
    RenderWindow window(VideoMode(1400, 700), "Cadillacs and Dinosaurs Game", Style::Close | Style::Titlebar);

    Image backgroundImage;
    if (backgroundImage.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/bg1.png")) {
        Texture backgroundTexture, characterTexture;
        backgroundTexture.loadFromImage(backgroundImage);

        // Load different images for player movement
        Texture playerTexture1, playerTexture2;
        playerTexture1.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.png");

        // Use a PNG image with a transparent background
        playerTexture2.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/player-jack.png");

        Sprite backgroundSprite(backgroundTexture), characterSprite(playerTexture1);

        characterSprite.setPosition(250.0f, 400.0f);
        playerTexture2.setSmooth(true);

        Clock clock;
        float movementSpeed = 200.0f;

        while (window.isOpen()) {
            Event event{};
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                // Check for 'C' key press to switch player movement image
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::C) {
                    characterSprite.setTexture(playerTexture2);
                }
            }

            float deltaTime = clock.restart().asSeconds();

            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                characterSprite.move(-movementSpeed * deltaTime, 0.0f);
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                characterSprite.move(movementSpeed * deltaTime, 0.0f);
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                characterSprite.move(0.0f, -movementSpeed * deltaTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                characterSprite.move(0.0f, movementSpeed * deltaTime);
            }

            Vector2u windowSize = window.getSize();
            backgroundSprite.setScale(static_cast<float>(windowSize.x) / backgroundSprite.getLocalBounds().width,
                                      static_cast<float>(windowSize.y) / backgroundSprite.getLocalBounds().height);

            window.clear();
            window.draw(backgroundSprite);
            window.draw(characterSprite);
            window.display();
        }
    }

    return 0;
}

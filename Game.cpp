#include "game.h"
#include "Sprite.h"
#include "Utils.h"

game_t* game;

const float PI = 22.0f/7.0;
void extern handleEvents();
void game_spawn_enemies();
void game_draw_debug(RenderWindow &window);
void game_init_dblBuffer() {

    RenderTexture dblBuffer; //target for off-screen 2d rendering into a texture

    Vector2u clientSize = game->window.getSize();

    if (!dblBuffer.create(clientSize.x, clientSize.y)) {

        return;
    }

    // Display the contents of the render texture
    dblBuffer.display();
}

void game_load_resources();


bool game_init_window() {
    game->height = 668;
    game->width = 1024;

    game->title = "Cadillacs and Dinosaurs Game";
    game->window.create(VideoMode(game->width, game->height), game->title, Style::Close | Style::Titlebar);

    if (!game->window.isOpen()) {
        return false;
    }

    game_init_dblBuffer();

    return true;
}


Image game_load_image(const string &path){


    Image image;

    image.loadFromFile(path);

    return image;


}

void game_load_resources() {
    game->img_bg = game_load_image("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/ep-5.bmp");
    game->img_bg_far = game_load_image("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/ep-5-far.bmp");
    game->img_level_layers = new Image[10];
    game->img_level_layers[0] = game_load_image("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/ep-5-layer.bmp");
    game->img_text = game_load_image("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/smack.bmp");

    game->level_fire.push_back(*Sprite_Handle::sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/ep-5-animation.bmp", 320, 224, 1.0));

    sprite_t *sp;
    sp->frames_per_row = 4;
    Animation_Handle::sprite_add_animation(game->level_fire, "fire", 0, 3, 0);
    Animation_Handle::sprite_set_animation(game->level_fire, "fire");

    game->img_avatar_player = game_load_image("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.bmp/jack.bmp");

    game->font = Utils::create_font("E:/ClionProjects/Cadillacs and Dinosaurs Game/Font/RobotoCondensed-Italic.ttf", 20);

    game->fx_punch.sprite.push_back(*Sprite_Handle::sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/punch.bmp", 120, 120, 1.4));
    sp->frames_per_row = 4;
    game->fx_punch.anim = Animation_Handle::sprite_add_animation(game->fx_punch.sprite, "punch1", 0, 3, 0);
    draw_t *draw;
    draw->draw_top = 1;
    game->fx_punch.draw_punch = 0;
    Utils::vector3d_zero(&game->fx_punch);

    game->fx_blood.sprite.push_back(*Sprite_Handle::sprite_new("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/blood.bmp", 120, 120, 1.2));
    sp->frames_per_row = 4;
    game->fx_blood.anim = Animation_Handle:: sprite_add_animation(game->fx_blood.sprite, "punch1", 0, 6, 0);
    game->fx_blood.anim->speed = 1.5f;

    draw->draw_top = 1;
    game->fx_blood.draw_punch = 0;
    Utils::vector3d_zero(&game->fx_blood);

    game->fx_smack_time = 0;
    game->fx_smack_x = 0;
    game->fx_smack_y = 0;
}




int main() {
    RenderWindow window(VideoMode(1400, 700), "Cadillacs and Dinosaurs Game", Style::Close | Style::Titlebar);

    Image backgroundImage;
    if (backgroundImage.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/ss.png")) {
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

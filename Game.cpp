#include "game.h"
#include "Sprite.h"
#include "Utils.h"
#include "Player.h"
#include "Enemy.h"

game_t* game;

const float PI = 22.0f/7.0;
void handleEvents();
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

    game->img_avatar_player = game_load_image("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.bmp");

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

void game_start_new() {
    game->view_x = 0;
    game->view_x_far = 0;
    game->enemy_count = 0;
    game_spawn_enemies();
    game->state = GAME_STATE_INTRO;
    //game->state = GAME_STATE_PLAYING;
}


bool game_init() {
    game = new game_t;
    fill(game->keyboard_state.begin(), game->keyboard_state.end(), 0);

    if (!game_init_window()) {
        return false;
    }

    game_load_resources();

    game->draw_max = 100;
    game->draw_list = new draw_t[game->draw_max];
    game->draw_count = 0;

    game->time_text_blink = 0;
    game->intro_time = 300;

    game->max_z = 70;
    game->min_z = -70;

    game->view_x = 0.0f;
    game->view_x_far = 0.0f;

    game->max_view_x = 0;

    game->ground_y = 520;
    game->gravity = 0.8f;

    game->state = GAME_STATE_MENU;
    game->player.push_back(*Player::player_new());

    game->enemy_max = 100;
    game->enemies.resize(game->enemy_max);
    game->enemy_count = 0;

    game->spawn_trigger = 0;

    return true;
}





void game_spawn_enemies(){


    Enemy::enemy_spawn(10, 40, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(800, 80, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(900, -50, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1000, 80, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(1150, 0, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1200, 80, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(1350, 0, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1500, 80, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(1600, 20, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(1700, 30, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(1900, 70, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(2200, 80, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(2500, 20, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(2800, 30, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(3100, 70, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(3550, 0, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(3800, 80, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(3900, 20, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(4300, 30, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(4700, 70, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(5200, 20, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(5300, -50, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(6500, -20, ENEMY_TYPE_GNEISS);

    Enemy::enemy_spawn(7000, 30, ENEMY_TYPE_FERRIS);

    Enemy::enemy_spawn(8500, 40, ENEMY_TYPE_BUTCHER);

}

void game_cleanup_enemies() {
    for(int i = 0;i < game->enemy_count;i++) {

        enemy_t* enemy = &game->enemies[i];
        Enemy::enemy_cleanup(enemy);
    }
    game->enemy_count = 0;
}



void game_delete() {
    vector<player_t>*pl = &game->player;
    for (auto &player: *pl) {

        if (game != nullptr) {
            game_cleanup_enemies();
            Player::player_delete(&player);
        }
    }
}

void game_draw_avatar(RenderWindow& window, int x, int y) {
    Texture avatarTexture;
    avatarTexture.loadFromImage(game->img_avatar_player);

    Sprite avatarSprite(avatarTexture);
    avatarSprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    avatarSprite.setScale(2.0f, 2.0f); // Adjust the scale as needed

    window.draw(avatarSprite);
}

void game_draw_player_statistics(RenderWindow& window, int x) {
    player_t *player;
    Text playerName;
    playerName.setFillColor(Color(255,255,255));
    playerName.setString("Player Jack");
    playerName.setPosition((float)x + 60, 20);
    Text playerScore;
    playerScore.setFillColor(Color(255,255,0));
    playerScore.setString("Score: " + to_string(player->score));
    playerScore.setPosition((float)x + 200, 20);
    window.draw(playerName);
    window.draw(playerScore);

}

float level_stretch = 0.4f;
float org_width = 3664;

void game_draw_level(sf::RenderWindow& window, FloatRect rect) {
    float level_scaled = (float)1.0 / level_stretch;

    game->max_view_x = org_width * level_scaled - rect.width;

    float stretched_width = rect.width * level_stretch;

    Texture bgFarTexture;
    if (!bgFarTexture.loadFromImage(game->img_bg_far)) {
        return;
    }

    Sprite bgFarSprite(bgFarTexture);
    bgFarSprite.setScale((float)(level_stretch - 0.3), 0.7);
    bgFarSprite.setTextureRect(IntRect((int)(game->view_x_far * (level_stretch - 0.3)), 0, (int)stretched_width, 144));
    window.draw(bgFarSprite);

    Texture bgTexture;
    if (!bgTexture.loadFromImage(game->img_bg)) {
       return;
    }

    Sprite bgSprite(bgTexture);
    bgSprite.setScale(level_stretch, 1);
    bgSprite.setTextureRect(IntRect((int)(game->view_x * level_stretch), 0, (int)stretched_width, 224));
    bgSprite.setColor(Color(255, 174, 201)); // Set the color mask
    window.draw(bgSprite);
}

void effect_draw(effect_t* fx) {
    if (fx->draw_punch) {
        Sprite_Handle::sprite_draw(fx->sprite, fx, CHARACTER_DIRECTION_RIGHT);
    }
}

void game_draw_level_fire(RenderWindow& window, FloatRect &rect) {
    float level_scaled = 1.0f / level_stretch;

    draw_t draw;
    Sprite_Handle::sprite_draw_2(&game->level_fire, &draw);

    float scaled_width = (float)draw.width * level_scaled;
    float gap = rect.width - scaled_width;

    Texture texture;
    texture.loadFromImage(draw.image);
    Sprite sprite(texture);
    sprite.setTextureRect(IntRect(draw.src_x, draw.src_y, draw.src_width, draw.src_height));
    sprite.setScale(level_stretch, 1.0f);
    sprite.setPosition((-game->view_x) + game->max_view_x + gap, 0);

    window.draw(sprite);
}





int main() {
    RenderWindow window(VideoMode(1400, 700), "Cadillacs and Dinosaurs Game", Style::Close | Style::Titlebar);

    Image backgroundImage;
    if (backgroundImage.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/bg1.bmp")) {
        Texture backgroundTexture, characterTexture;
        backgroundTexture.loadFromImage(backgroundImage);

        // Load different images for player movement
        Texture playerTexture1, playerTexture2;
        playerTexture1.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.bmp");

        // Use a PNG image with a transparent background
        playerTexture2.loadFromFile("E:/ClionProjects/Cadillacs and Dinosaurs Game/images/jack.bmp");

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

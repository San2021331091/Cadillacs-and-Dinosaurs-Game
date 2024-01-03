#include "Utils.h"


void Utils ::vector3d_zero(vector3d_t *vec) {

    vec->x = 0;
    vec->y = 0;
    vec->z = 0;


}

int Utils::get_rand(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, max - min);

    return distribution(gen) + min;
}


float Utils ::get_Random(float a, float b) {

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float random = distribution(gen);
    float diff = b - a;
    float r = random * diff;

    return a + r;



}

Font Utils :: create_font(const string& filename, unsigned int size) {
    Font font;
    font.loadFromFile(filename);
    Text text;
    text.setFont(font);
    text.setCharacterSize(size);
    return font;


}

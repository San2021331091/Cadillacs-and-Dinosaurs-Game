
#include "Utils.h"


void Utils :: vector3d_zero(vector3d_t* vec) {
    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
}

int Utils :: get_rand(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, max - min);

    return distribution(gen) + min;
}

float Utils :: get_RandF(float a, float b) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float random = distribution(gen);
    float diff = b - a;
    float r = random * diff;

    return a + r;
}

HFONT Utils ::  create_font(const wchar_t* name, int size) {
    HDC hDC = GetDC(HWND_DESKTOP);
    int height = -MulDiv(size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    ReleaseDC(HWND_DESKTOP, hDC);
    HFONT font = CreateFontW(height, 0, 0, 0, FW_BOLD, 0, 0, 0,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, name);
    return font;
}

float XorShift32 :: nextFloat(float min, float max) {
        uint32_t x = state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        state = x;
        float normalized = static_cast<float>(x) / static_cast<float>(UINT32_MAX);
        return min + normalized * (max - min);
    }

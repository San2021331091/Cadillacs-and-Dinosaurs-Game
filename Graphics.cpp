#include "Graphics.h"

void Graphics ::  graphics_clear() {
    game->draw_count = 0;
}

void Graphics :: graphics_draw(draw_t* args) {
    if (game->draw_count >= game->draw_max) {
        return;
    }

    draw_t* draw = &game->draw_list[game->draw_count];
    memcpy(draw, args, sizeof(draw_t));
    game->draw_count++;
}

void Graphics ::  graphics_draw_rect(HDC hdc, RECT* rect, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, 2, color);
    auto brush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Rectangle(hdc, (-game->view_x) + rect->left, rect->top, (-game->view_x) + rect->right, rect->bottom);
    //DeleteObject(brush);
    DeleteObject(pen);
}

void Graphics ::  graphics_draw_surface(HDC hdc, HBITMAP bitmap, int x, int y, int w, int h,
                           int src_x, int src_y, int src_w, int src_h, int flip, COLORREF key)
{
    HDC hdc_bmp = CreateCompatibleDC(hdc);
    HDC hdc_flipped = CreateCompatibleDC(hdc);

    HBITMAP bmp_flipped = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(hdc_flipped, bmp_flipped);

    SelectObject(hdc_bmp, bitmap);

    if (flip) {
        StretchBlt(hdc_flipped, w-1, 0, -w, h, hdc_bmp, src_x, src_y, src_w, src_h, SRCCOPY);
    } else {
        StretchBlt(hdc_flipped, 0, 0, w, h, hdc_bmp, src_x, src_y, src_w, src_h, SRCCOPY);
    }

    TransparentBlt(hdc, x, y, w, h, hdc_flipped, 0, 0, w, h, key);

    DeleteObject(bmp_flipped);
    DeleteDC(hdc_flipped);
    DeleteDC(hdc_bmp);
}



// custom comparison function
auto compare_draw = [](const draw_t& a, const draw_t& b) {
    if (a.draw_top)
        return true;
    else if (b.draw_top)
        return false;

    if (a.position.z == b.position.z)
        return false;
    else
        return a.position.z < b.position.z;
};

// Merge function to merge two sorted arrays
void merge(draw_t* arr, int left, int middle, int right, const function<bool(const draw_t&, const draw_t&)>& compare) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create unique_ptr to manage memory for temporary arrays
   unique_ptr<draw_t[]> leftArray(new draw_t[n1]);
   unique_ptr<draw_t[]> rightArray(new draw_t[n2]);

    // Copy data to temporary arrays leftArray[] and rightArray[]
    for (int i = 0; i < n1; ++i)
        leftArray[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        rightArray[j] = arr[middle + 1 + j];

    // Merge the temporary arrays back into arr[left-right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compare(leftArray[i], rightArray[j])) {
            arr[k] = leftArray[i];
            ++i;
        } else {
            arr[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of leftArray[], if there are any
    while (i < n1) {
        arr[k] = leftArray[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of rightArray[], if there are any
    while (j < n2) {
        arr[k] = rightArray[j];
        ++j;
        ++k;
    }
}


// Merge sort function
void mergeSort(draw_t* arr, int left, int right, const function<bool(const draw_t&, const draw_t&)>& compare) {
    if (left < right) {
        // Same as (left + right) / 2, but avoids overflow for large left and right
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, middle, compare);
        mergeSort(arr, middle + 1, right, compare);

        // Merge the sorted halves
        merge(arr, left, middle, right, compare);
    }
}


void Graphics :: graphics_swap_buffer(HDC hdc, RECT* rect) {

    auto* drawList = new draw_t[game->draw_count];  // dynamically allocated array

    // Use mergeSort to sort the array with the custom comparison function
    mergeSort(drawList, 0, game->draw_count - 1, compare_draw);

    for(int i = 0;i < game->draw_count;i++) {
        draw_t* draw = &game->draw_list[i];

        float depth = draw->position.z;

        if (!draw->draw_top) {
            for(int j = 0;j < game->draw_count;j++) {
                draw_t* jDraw = &game->draw_list[j];
                float diff = jDraw->position.z - depth;
                if (diff < 1) {
                    depth += 1;
                }
            }
        }

        float x = (-game->view_x) + draw->position.x;
        float y = draw->position.y + depth;

        Graphics::graphics_draw_surface(hdc, draw->hbitmap,
                              x, y - draw->height, draw->width, draw->height,
                              draw->src_x, draw->src_y, draw->src_width, draw->src_height,
                              draw->flip, draw->color_mask);


    }
}


void Graphics::graphics_draw_lifeBar(HDC hdc, int x, int y, int height, int width, float life) {
    HBRUSH brush;
    HPEN pen;

    pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
    brush = CreateSolidBrush(RGB(255, 255, 0));

    int life_bar_y = y;
    int life_bar_x = x;
    int life_bar_width = width;

    int dx_life = static_cast<int>(life * ((float)life_bar_width / 100.0f));

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    Rectangle(hdc, life_bar_x, life_bar_y, life_bar_x + dx_life, life_bar_y + height);

    DeleteObject(brush);
    DeleteObject(pen);

    pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    brush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    Rectangle(hdc, life_bar_x, life_bar_y, life_bar_x + life_bar_width, life_bar_y + height);

    DeleteObject(brush);
    DeleteObject(pen);
}

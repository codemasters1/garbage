#include <stdio.h>
#include <stdlib.h>
#include <windows.h>




int main()
{
    srand(888);

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC screen = GetDC(NULL);

    HBRUSH brush;
    RECT rect;

    while(1)
    {
        rect.left = rand() % width;
        rect.top = rand() % height;
        rect.right = rect.left + rand() % 20;
        rect.bottom = rect.top + rand() % 20;;

        brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        FillRect( screen, &rect, brush );
    }

    ReleaseDC(NULL,screen);

    return 0;
}

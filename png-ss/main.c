#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <windows.h>
#include "lodepng.h"


std::tuple<int, int, char> getSystemMetrics()
{
    const int width = GetSystemMetrics(SM_CXSCREEN);
    const int height = GetSystemMetrics(SM_CYSCREEN);
    const char depth = GetSystemMetrics( BITSPIXEL );

    return std::make_tuple(width, height, depth);
}


int main(int argc, char *argv[])
{
    auto [width, height, depth] = getSystemMetrics();

    HDC hScreenDC = GetDC(HWND_DESKTOP);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

	SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    BITMAPINFO bi;
    ZeroMemory(&bi, sizeof(BITMAPINFO));
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth  = width;
    bi.bmiHeader.biHeight = -height;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = depth;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;

    std::vector<uint8_t> bits{height * width * (depth / 8)};
    GetDIBits(hScreenDC, hBitmap, 0, height, bits.data(), &bi, DIB_RGB_COLORS);

    DeleteObject(hMemoryDC);
	DeleteDC(hScreenDC);

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            /*const int pos = (depth / 8) * (width * y + x);

            unsigned char tmp = bits[pos + 0];
            bits[pos + 0] = bits[pos + 2];
            bits[pos + 2] = tmp;*/
        }
    }
    
    lodepng_encode32_file("test.png", bits, width, height);

    return 0;
}

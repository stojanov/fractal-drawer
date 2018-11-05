#include <thread>
#include <iostream>
#include <complex>
#include <chrono>

#include "SFML/Graphics.hpp"

#include "Mandelbrot.h"
#include "PixelStore.h"

int Mandelbrot::s_THREADS = 8;
int Mandelbrot::s_MAX_ITERATIONS = 1000;

Mandelbrot::Mandelbrot(
        const int width, 
        const int height
    ):  m_width(width),
        m_height(height)
        {}

int Mandelbrot::iterations(int x, int y, int width, int height)
{
    float xTranslated = (x - width /2) * 2.f / width;
    float yTranslated = (y - height /2) * 2.f / height;

    std::complex<float> z = 0;
    std::complex<float> c(xTranslated, yTranslated);
    
    int iter = 0;

    while(iter++ < s_MAX_ITERATIONS) {
        z = z * z + c;
        if(abs(z) > 2) break;
    }

    return iter;
}

void Mandelbrot::proccesRange(sf::Vector2i point1, sf::Vector2i point2, sf::Vector2i offset) 
{
    for(int i = point1.x; i < point2.x; i++)
    {
        for(int j = point1.y; j < point2.y; j++)
        {
            int iter = iterations(i, j, m_width, m_height);
            float k = 1.f * iter / s_MAX_ITERATIONS;

            int clr =  256 * k;

            int red = clr;
            int green = clr;
            int blue = clr;

            sf::Uint8 clrsf[]{ red, green, blue, 255 };

            m_store->assignToPixel(i, j, clrsf);
        }
    }
}

void Mandelbrot::spawn(int x, int y)
{
    int horizontal = s_THREADS / 2;

    int wK = m_width / horizontal;
    int hK = m_height / 2;

    for(int i = 0; i < horizontal; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            int widthStart = i * wK;
            int heightStart = j * hK;
            int widthEnd = widthStart + wK;
            int heightEnd = heightStart + hK;

            std::thread(
                [=] { proccesRange(
                    sf::Vector2i(widthStart, heightStart),
                    sf::Vector2i(widthEnd, heightEnd),
                    sf::Vector2i(x, y)
                ); }
            ).detach();
        }
    }
}

void Mandelbrot::addPixelBuffer(PixelStore<sf::Uint8>* store) { m_store = store; }
#include <thread>
#include <iostream>
#include <complex>
#include "SFML/Graphics.hpp"

#include "Mandelbrot.h"
#include "pixel_store.hpp"

Mandelbrot::Mandelbrot(
        const int width, 
        const int height, 
        const int maxiter, 
        const int base_threads
    ):  m_width(width),
        m_height(height),
        m_maxiter(maxiter),
        m_threads_per_block(base_threads) {}

float Mandelbrot::iterations(int x, int y)
{
    float xTranslated = (x - m_width /2) * 2.0/m_width;
    float yTranslated = (y - m_height /2) * 2.0/m_height;

    std::complex<float> z = 0;
    std::complex<float> c(xTranslated, yTranslated);
    
    int iterations = 0;

    while(iterations++ < m_maxiter) {
        z = z * z + c;
        if(abs(z) > 2) break;
    }

    return iterations;
}

void Mandelbrot::proccesRange(sf::Vector2i point1, sf::Vector2i point2, sf::Vector2i offset)
{
    for(int i = point1.x; i < point2.x; i++)
    {
        for(int j = point1.y; j < point2.y; j++)
        {
            float iter = iterations(i + offset.x, j + offset.y);
            float k = iter / m_maxiter;

            int clr = (int) 255 * k;

            sf::Uint8 clrsf[]{ clr, clr, clr, 255 };

            m_store->assignToPixel(i, j, clrsf);
        }
    }
}


// Offset added for future updates if they happen
void Mandelbrot::spawn(int x, int y)
{
    int wK = m_width / m_threads_per_block;
    int hK = m_height / m_threads_per_block;

    for(int i = 0; i < m_width; i += wK)
    {
        for(int j = 0; j < m_height; j += hK)
        {
            std::thread(
                [this, i, j, wK, hK, x, y] { proccesRange(
                    sf::Vector2i(i, j),
                    sf::Vector2i(i + wK, j + hK),
                    sf::Vector2i(x, y)
                ); }
            ).detach();
        }
    }
}

void Mandelbrot::addPixelBuffer(PixelStore<sf::Uint8>* store) { m_store = store; }

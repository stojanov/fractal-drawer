#include <thread>
#include <iostream>
#include <complex>
#include <chrono>
#include "SFML/Graphics.hpp"

#include "Mandelbrot.h"
#include "PixelStore.hpp"

Mandelbrot::Mandelbrot(
        const int width, 
        const int height, 
        const int maxiter, 
        const int base_threads
    ):  m_width(width),
        m_height(height),
        m_maxiter(maxiter),
        m_threads_per_block(base_threads)
        {}

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

void Mandelbrot::proccesRange(sf::Vector2i point1, sf::Vector2i point2, sf::Vector2i offset, bool *thread) 
{
    for(int i = point1.x; i < point2.x; i++)
    {
        for(int j = point1.y; j < point2.y; j++)
        {
            float iter = iterations(i, j);
            float k = iter / m_maxiter;

            int clr =  256 * k;

            int red = clr;
            int green = clr;
            int blue = clr;

            sf::Uint8 clrsf[]{ red, green, blue, 255 };

            m_store->assignToPixel(i, j, clrsf);
        }
    }

    (*thread) = true;
}


// Offset added for future updates if they happen
void Mandelbrot::spawn(int x, int y)
{
    int k = m_threads_per_block;

    int wK = m_width / k;
    int hK = m_height / k;

    bool finished_threads[k * k];

    int thread_counter = 0;
    for(int i = 0; i < k; i++)
    {
        for(int j = 0; j < k; j++)
        {
            int widthStart = i * wK;
            int heightStart = j * hK;
            int widthEnd = widthStart + wK;
            int heightEnd = heightStart + hK;

            bool *this_thread = &finished_threads[thread_counter++];
            (*this_thread) = false; 

            std::thread(
                [=] { proccesRange(
                    sf::Vector2i(widthStart, heightStart),
                    sf::Vector2i(widthEnd, heightEnd),
                    sf::Vector2i(x, y),
                    this_thread
                ); }
            ).detach();
        }
    }

    // Very bad
    bool finished = false;
    while(!finished) // Block this thread while the others finish (exceptions not handled)
    {
        int i = 0;
        while(i++ < thread_counter)
        {
            if(finished_threads[i] == false)
                break;
        }

        if(i == thread_counter)
            finished = true;
    }

    std::cout << "Done.\n";
}

void Mandelbrot::addPixelBuffer(PixelStore<sf::Uint8>* store) { m_store = store; }
#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "SFML/Graphics.hpp"
#include "PixelStore.h"

class Mandelbrot
{
private:
    int m_width;
    int m_height; 
    PixelStore<sf::Uint8>* m_store;
    
    static int s_MAX_ITERATIONS;
    static int s_THREADS;
    static int iterations(int, int, int, int);

public:
    Mandelbrot(const int, const int);

    void addPixelBuffer(PixelStore<sf::Uint8>* store);

    void proccesRange(sf::Vector2i, sf::Vector2i, sf::Vector2i);

    void start();

    void spawn(int, int);
};

#endif
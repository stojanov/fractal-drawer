#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "SFML/Graphics.hpp"
#include "PixelStore.hpp"

class Mandelbrot
{
private:
    int m_width;
    int m_height; 
    int m_maxiter;
    int m_threads_per_block;
    PixelStore<sf::Uint8>* m_store;
public:
    Mandelbrot(const int, const int, const int, const int);

    void addPixelBuffer(PixelStore<sf::Uint8>* store);

    void proccesRange(sf::Vector2i, sf::Vector2i, sf::Vector2i, bool*);

    void start();

    void spawn(int, int);

    float iterations(int, int);
};

#endif
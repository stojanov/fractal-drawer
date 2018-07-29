#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "PixelStore.hpp"
#include "Mandelbrot.h"

void mainWindow(int width, int height, PixelStore<sf::Uint8> *PixelData) 
{
    sf::RenderWindow window(
        sf::VideoMode(width, height), 
        "Fractal Drawer"
    );

    const int spriteWidth = PixelData->getWidth();
    const int spriteHeight = PixelData->getHeight();

    sf::Texture Image;
    Image.create(spriteWidth, spriteHeight);

    sf::Sprite sprite(Image);

    auto spriteBounds = sprite.getLocalBounds();

    sprite.setScale(
        width / spriteBounds.width, 
        height / spriteBounds.height
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) 
                window.close();
        }

        Image.update(PixelData->getdata());

        window.clear();

        window.draw(sprite);

        window.display(); 

        std::this_thread::sleep_for(
            std::chrono::milliseconds(1000/30)  // The naive way, but it doesn't need any fancier way
        );
    }
}

int main()
{
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;

    const int FRACTAL_WIDTH = 800; // Numbers divisible by number of threads 
    const int FRACTAL_HEIGHT = 800;
    const int ITERATIONS = 1000; 
    const int THREADS_PER_BLOCK = 8; // Will use n*n threads, even numbers

    PixelStore<sf::Uint8> Store(4, FRACTAL_WIDTH, FRACTAL_HEIGHT);

    Mandelbrot Fractal(
        FRACTAL_WIDTH,
        FRACTAL_HEIGHT,
        ITERATIONS,
        THREADS_PER_BLOCK
    );

    std::thread renderer(mainWindow, WINDOW_WIDTH, WINDOW_HEIGHT, &Store);

    Fractal.addPixelBuffer(&Store);

    Fractal.spawn(0, 0); // Will block

    renderer.join(); // Exit when mainWindow is closed

    exit(0);
    return 0;
}
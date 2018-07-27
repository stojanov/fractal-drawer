#ifndef PIXEL_STORE_H
#define PIXEL_STORE_H

#include <iostream>

template<typename pixelType>
class PixelStore
{
private:
    int m_pixelSize;
    int m_width;
    int m_height;
    pixelType *m_data{nullptr};
public:
    PixelStore(int pixelSize, int width, int height)
        :   m_pixelSize(pixelSize),
            m_width(width),
            m_height(height),   
            m_data(new pixelType[m_pixelSize * width * height]) {}

    void fill(int clr)
    {
        for(int i = 0; i < m_width * m_height * m_pixelSize; i++)
            m_data[i] = pixelType(clr);
    }

    pixelType* getdata() { return m_data; }

    void assignToPixel(int x, int y, pixelType* clrData)
    {
        pixelType *pixel = &m_data[(y * m_pixelSize) * m_width + (x * m_pixelSize)];

        for(int i = 0; i < m_pixelSize; i++)
            pixel[i] = clrData[i];
    }

    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

    ~PixelStore() { delete [] m_data; };
};

#endif
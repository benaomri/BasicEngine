//
// Created by OMRI_PRIVATE on 05/12/2022.
//

#include "Image.h"


//---------------------------------  Image  -------------------------------------------

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    data = (unsigned char *) (malloc(width * height * 4));
}

void Image::setColor(int pixelX, int pixelY, vec4 rgba) {
    data[(this->width * pixelY + pixelX) * 4] = (unsigned char) (rgba.r * 255);
    data[(this->width * pixelY + pixelX) * 4 + 1] = (unsigned char) (rgba.g * 255);
    data[(this->width * pixelY + pixelX) * 4 + 2] = (unsigned char) (rgba.b * 255);
    data[(this->width * pixelY + pixelX) * 4 + 3] = (unsigned char) (rgba.a * 255);
}

unsigned char *Image::getData() {
    return data;
}
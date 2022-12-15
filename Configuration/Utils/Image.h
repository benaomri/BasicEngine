#include "glm/glm.hpp"

using namespace glm;

class Image {
public:
    unsigned char* data;
    int width;
    int height;

    Image(int width, int height);

    void setColor(int pixelX, int pixelY, vec4 rgba);


};




#pragma once
#include <iostream>
#include <string>
#include "../stb_image.h"
#include "../utils/Utils.h"
using namespace std;

class Texture
{
protected:
    unsigned int texture;
    int width;
    int height;
public: 

	Texture() 
    {
        width = 0;
        height = 0;
        texture = 0;
	}

    unsigned int getTexture() {
        return texture;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }
};


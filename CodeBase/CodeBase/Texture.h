#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

enum ImageFormat {
	NONE,
	JPG,
	PNG,
	DDS
};

class Texture
{
public:
	Texture(std::string _name, int id, int _width, int _height, ImageFormat _format);
	~Texture();

public:

	std::string name = "";
	int width = 0;
	int height = 0;

	int imageID = 0;

	ImageFormat format = NONE;
	
};

#endif // !__TEXTURE_H__
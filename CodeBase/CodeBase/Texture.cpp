#include "Application.h"
#include "Texture.h"

Texture::Texture(std::string _name, int id, int _width, int _height, ImageFormat _format)
{
	name = _name;
	imageID = id;
	width = _width;
	height = _height;
	format = _format;
}

Texture::~Texture()
{
}

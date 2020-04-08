#include "Globals.h"
#include "FileSystem.h"
#include "Texture.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "DevIL/lib/DevIL.lib")
#pragma comment (lib, "DevIL/lib/ILU.lib")
#pragma comment (lib, "DevIL/lib/ILUT.lib")


FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
}

void FileSystem::InitDevIL()
{
	// Initialize IL
	ilInit();

	// Initialize ILU
	iluInit();

	// Initialize ILUT with OpenGL support.
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

}

string FileSystem::FileToString(string path)
{
	ifstream t(path.c_str());
	stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

Texture* FileSystem::LoadImagePNG(string path)
{
	//Texture pointer to return
	Texture* ret = nullptr;

	//Creating a DevIL name for the new texture
	ILuint imageName=0;
	ilGenImages(1, &imageName);

	//binding the name
	ilBindImage(imageName);

	//Loading the image as PNG format
	ilLoadImage(path.c_str());

	ILenum Error;
	bool errors = false;
	while ((Error = ilGetError()) != IL_NO_ERROR) 
	{
		LOG("%d: %s/n",Error,iluErrorString(Error));
		errors = true;
	}

	if (errors)
	{
		return ret;
	}

	//Getting image Information
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);

	//Flipping image
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}	

	//Getting an OpenGL bind id
	GLuint textureID = ilutGLBindTexImage();

	//Ckecking OpenGL Errors 
	if (textureID > 0)
	{
		//Generating the new Texture
		ret = new Texture("test", textureID, ImageInfo.Width, ImageInfo.Height, PNG);
	}

	//Unbinding and deleting the DevIL name
	ilBindImage(0);
	ilDeleteImage(imageName);

	return ret;
}

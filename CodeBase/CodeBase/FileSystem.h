#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>
#include <streambuf>
#include <fstream>
#include <sstream>

using namespace std;

class Texture;

class FileSystem
{
public:

	FileSystem();
	~FileSystem();

public:

	void InitDevIL();

	string FileToString(string path);

	Texture* LoadImagePNG(string path);

	bool ExportOBJ(float* vertexBuffer, int sizeBuffer);

};

#endif //!__FILESYSTEM_H__
#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>
#include <streambuf>
#include <fstream>
#include <sstream>

using namespace std;

class Texture;

enum ExportFormat 
{
	DAE = 0,
	OBJ_MAT = 3,
	OBJ = 4,
	FBX = 17
};

class FileSystem
{
public:

	FileSystem();
	~FileSystem();

public:

	void InitDevIL();

	string FileToString(string path);

	Texture* LoadImagePNG(string path);

	string AddExtension(string folder, string name, ExportFormat extension);

	bool Export(float* vertexBuffer, int sizeBuffer, string name, ExportFormat format, string& message);
	bool ExportPNG(string path);

	string GetFolderPath(string folder);
	string GetFileNameFromPath(string path);

	string GetFileNameAt(const char* path);
};

#endif //!__FILESYSTEM_H__
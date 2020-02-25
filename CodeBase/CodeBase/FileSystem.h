#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>
#include <streambuf>
#include <fstream>
#include <sstream>

using namespace std;

class FileSystem
{
public:

	FileSystem();
	~FileSystem();

public:

	//Static to avoid an instance of ther class FileSystem
	static string FileToString(string path);

};

#endif //!__FILESYSTEM_H__
#include "FileSystem.h"

FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
}

string FileSystem::FileToString(string path)
{
	ifstream t(path.c_str());
	stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

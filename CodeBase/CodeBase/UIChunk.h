#ifndef __UICHUNK_H__
#define __UICHUNK_H__

#include "UIWindow.h"

class Chunk;

class UIChunk :	public UIWindow
{
public:

	UIChunk(std::string name, bool active=true);
	~UIChunk();

	bool Draw();

public: 

	Chunk* selectedChunk = nullptr;

};

#endif // !__UICHUNK_H__
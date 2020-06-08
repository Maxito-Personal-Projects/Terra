#include "ModuleUI.h"
#include "UIChunk.h"
#include "Chunk.h"



UIChunk::UIChunk(std::string name, bool active) :UIWindow(name, active)
{
}


UIChunk::~UIChunk()
{
}

bool UIChunk::Draw()
{
	bool ret = true;

	return ret;
}

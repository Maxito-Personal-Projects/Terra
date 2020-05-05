#ifndef __CHUNK_H__
#define __CHUNK_H__

class GameObject;
class Mesh;

class Chunk
{
public:
	
	Chunk(GameObject* _parent, int id, int _x, int _y);
	~Chunk();

public:

	GameObject* parent = nullptr;
	Mesh* mesh = nullptr;

	float maxHeight = 20.0f;
	int octaves = 8;

	int chunkID = 0;
	int x=0;
	int y=0;

};

#endif // !__CHUNK_H__

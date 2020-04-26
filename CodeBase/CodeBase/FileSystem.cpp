#include "Globals.h"
#include "FileSystem.h"
#include "Texture.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#include "Assimp/include/assimp/cexport.h"
#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/Exporter.hpp"

#pragma comment (lib, "DevIL/lib/DevIL.lib")
#pragma comment (lib, "DevIL/lib/ILU.lib")
#pragma comment (lib, "DevIL/lib/ILUT.lib")
#pragma comment (lib, "Assimp/lib/assimp-vc141-mtd.lib")


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

	//Available Assimp export formats 
	int count = aiGetExportFormatCount();
	string exports = "";
	for (int i = 0; i < count; i++)
	{
		if (i == 0)
		{
			exports = exports + aiGetExportFormatDescription(i)->fileExtension + "(" + std::to_string(i) + ")";
		}
		else
		{
			exports = exports + ", " + aiGetExportFormatDescription(i)->fileExtension + "(" + std::to_string(i) + ")";
		}

	}

	LOG("Assimp export formats: %s", exports.c_str());
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
	ilLoad(IL_PNG,path.c_str());

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

bool FileSystem::ExportOBJ(float* vertexBuffer, int sizeBuffer)
{
	bool ret = false;

	Assimp::Exporter* exporter = new Assimp::Exporter();

	aiScene* scene = new aiScene();

	//Other info
	scene->mFlags = 0;
	scene->mRootNode = NULL;
	scene->mNumMeshes = 0;
	scene->mMeshes = NULL;
	scene->mNumMaterials = 0;
	scene->mMaterials = NULL;
	scene->mNumAnimations = 0;
	scene->mAnimations = NULL;
	scene->mNumTextures = 0;
	scene->mTextures = NULL;
	scene->mNumLights = 0;
	scene->mLights = NULL;
	scene->mNumCameras = 0;
	scene->mCameras = NULL;
	scene->mPrivate = NULL;

	//Initializing Root
	scene->mRootNode = new aiNode();

	//Initializing Mesh
	scene->mMeshes = new aiMesh*[1];		//Adding one mesh poiter
	scene->mMeshes[0] = nullptr;			//Initializing to null
	scene->mNumMeshes = 1;					//Setting number of meshes

	scene->mMeshes[0] = new aiMesh();		//setting mesh info

	aiMesh* mesh = scene->mMeshes[0];		//Getting mesh pointer
	
	//Now we are just taking the vertex info so the size is:
	int size = sizeBuffer / 3;

	//Setting mesh info
	mesh->mVertices = new aiVector3D[size];
	mesh->mNumVertices = size;

	mesh->mNormals = new aiVector3D[size];
	
	mesh->mTextureCoords[0] = new aiVector3D[size];
	mesh->mNumUVComponents[0] = size;

	int buffIndx = 0;

	for (int i = 0; i < size; ++i)
	{
		mesh->mVertices[i] = aiVector3D(vertexBuffer[buffIndx], vertexBuffer[buffIndx + 1], vertexBuffer[buffIndx + 1]);
		mesh->mNormals[i] = aiVector3D(0.0f, 1.0f, 0.0f);
		mesh->mTextureCoords[0][i] = aiVector3D(1.0f, 1.0f, 0.0f);
		buffIndx += 3;
	}

	int fSize = size / 3;
	mesh->mFaces = new aiFace[fSize];
	mesh->mNumFaces = (uint)(fSize);
	
	int vertIndx = 0;
	
	for (int i = 0; i < fSize; ++i)
	{
		mesh->mFaces[i].mIndices = new uint[3];
		mesh->mFaces[i].mNumIndices = 3;
	
		mesh->mFaces[i].mIndices[0] = vertIndx;
		mesh->mFaces[i].mIndices[1] = vertIndx + 1;
		mesh->mFaces[i].mIndices[2] = vertIndx + 2;

		vertIndx += 3;	
	}

	const aiExportFormatDesc* formatDescription = aiGetExportFormatDescription(17);
	LOG("%s", formatDescription->description);

	if (exporter->Export(scene,formatDescription->id, "test_3.fbx") == AI_SUCCESS)
	{
		LOG("Success exporting obj");
		ret = true;
	}
	else
	{
		LOG("Eror exporting obj");
	}

	return ret;
}

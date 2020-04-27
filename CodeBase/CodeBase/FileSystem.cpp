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
#include "Assimp/include/assimp/postprocess.h"

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

string FileSystem::AddExtension(string folder, string name, ExportFormat extension)
{
	string file = "";

	switch (extension)
	{
	case DAE:
		file = folder +"/" + name + (".dae");
		break;
	case OBJ_MAT:
		file = folder + "/" + name + (".obj");
		break;
	case OBJ:
		file = folder + "/" + name + (".obj");
		break;
	case FBX:
		file = folder + "/" + name + (".fbx");
		break;
	default:
		break;
	}
	return file;
}

bool FileSystem::Export(float* vertexBuffer, int sizeBuffer, string name, ExportFormat format, string& message)
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
	scene->mRootNode->mName = "Terrain";

	//Initializing Material
	scene->mMaterials = new aiMaterial*[1];
	scene->mMaterials[0] = nullptr;
	scene->mNumMaterials = 1;
		 
	scene->mMaterials[0] = new aiMaterial();

	//Initializing Mesh
	scene->mMeshes = new aiMesh*[1];		//Adding one mesh poiter
	scene->mMeshes[0] = nullptr;			//Initializing to null
	scene->mNumMeshes = 1;					//Setting number of meshes

	scene->mMeshes[0] = new aiMesh();		//setting mesh info
	scene->mMeshes[0]->mMaterialIndex = 0;
	
	//Setting mesh to rootNode
	scene->mRootNode->mMeshes = new uint[1];
	scene->mRootNode->mMeshes[0] = 0;
	scene->mRootNode->mNumMeshes = 1;

	aiMesh* mesh = scene->mMeshes[0];		//Getting mesh pointer
	mesh->mName = "Terrain";
	
	//Now we are just taking the vertex info so the size is:
	int size = sizeBuffer / 6;

	//Setting mesh info
	mesh->mVertices = new aiVector3D[size];
	mesh->mNumVertices = size;

	mesh->mNormals = new aiVector3D[size];
	
	mesh->mTextureCoords[0] = new aiVector3D[size];
	mesh->mNumUVComponents[0] = size;

	int buffIndx = 0;

	for (int i = 0; i < size; ++i)
	{
		mesh->mVertices[i] = aiVector3D(vertexBuffer[buffIndx], vertexBuffer[buffIndx + 1], vertexBuffer[buffIndx + 2]);
		mesh->mNormals[i] = aiVector3D(vertexBuffer[buffIndx + 3], vertexBuffer[buffIndx + 4], vertexBuffer[buffIndx + 5]);
		mesh->mTextureCoords[0][i] = aiVector3D(1.0f, 1.0f, 0.0f);
		buffIndx += 6;
	}

	int fSize = size / 3;
	mesh->mFaces = new aiFace[fSize];
	mesh->mNumFaces = (uint)(fSize);
	
	int vertIndx = 0;
	
	for (int i = 0; i < fSize; ++i)
	{
		aiFace &face = mesh->mFaces[i];
		face.mIndices = new uint[3];
		face.mNumIndices = 3;
	
		face.mIndices[0] = vertIndx;
		face.mIndices[1] = vertIndx + 1;
		face.mIndices[2] = vertIndx + 2;

		vertIndx += 3;	
	}

	const aiExportFormatDesc* formatDescription = aiGetExportFormatDescription(int(format));
	LOG("%s", formatDescription->description);

	if (exporter->Export(scene,formatDescription->id, AddExtension("Exports",name,format).c_str(), aiProcess_MakeLeftHanded) == AI_SUCCESS)
	{
		string exprt = formatDescription->fileExtension;
		message = "Success exporting Terrain in "+exprt+" format";
		LOG("%s", message.c_str());
		ret = true;
	}
	else
	{
		string error = exporter->GetErrorString();
		message = "Error exporting Terrarin: " + error;
		LOG("%s", message.c_str());
	}

	return ret;
}

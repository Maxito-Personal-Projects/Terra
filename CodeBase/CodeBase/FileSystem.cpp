#include "Globals.h"
#include "FileSystem.h"
#include "Texture.h"

#include <direct.h>
#include <Windows.h>

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
	string path = GetFolderPath(folder);

	switch (extension)
	{
	case DAE:
		file = path +"\\"  + name + (".dae");
		break;
	case OBJ_MAT:
		file = path + "\\" + name + (".obj");
		break;
	case OBJ:
		file = path + "\\" + name + (".obj");
		break;
	case FBX:
		file = path + "\\" + name + (".fbx");
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

	//Initializing Mesh
	scene->mMeshes = new aiMesh*[1];		  //Adding one mesh pointer
	scene->mMeshes[0] = new aiMesh();		  //setting mesh
	scene->mNumMeshes = 1;					  //Setting number of meshes

	//Initializing Material
	scene->mMaterials = new aiMaterial*[1];   //Adding one material pointer
	scene->mMaterials[0] = new aiMaterial();  //setting material
	scene->mNumMaterials = 1;				  //Setting number of materials

	//Setting mesh material
	scene->mMeshes[0]->mMaterialIndex = 0;
	
	//Setting mesh to rootNode
	scene->mRootNode->mMeshes = new uint[1];
	scene->mRootNode->mMeshes[0] = 0;
	scene->mRootNode->mNumMeshes = 1;

	aiMesh* mesh = scene->mMeshes[0];		 
	mesh->mName = "Terrain";
	
	//Now we are taking the vertex, normals
	
	int numVertices = sizeBuffer / 9;

	//Setting mesh info
	mesh->mVertices = new aiVector3D[numVertices];
	mesh->mNormals = new aiVector3D[numVertices];
	mesh->mTextureCoords[0] = new aiVector3D[numVertices];

	mesh->mNumVertices = numVertices;
	mesh->mNumUVComponents[0] = 3;

	int buffIndx = 0;

	for (int i = 0; i < numVertices; ++i)
	{
		mesh->mVertices[i] = aiVector3D(-vertexBuffer[buffIndx], vertexBuffer[buffIndx + 1], -vertexBuffer[buffIndx + 2]);
		mesh->mNormals[i] = aiVector3D(vertexBuffer[buffIndx + 3], vertexBuffer[buffIndx + 4], vertexBuffer[buffIndx + 5]);
		mesh->mTextureCoords[0][i] = aiVector3D(vertexBuffer[buffIndx + 6], vertexBuffer[buffIndx + 7], 0.0f);

		if (i < 20)
		{
			if (i % 3 == 0)
			{
				LOG("---------------");
			}

			LOG("Position: %f, %f, %f", vertexBuffer[buffIndx], vertexBuffer[buffIndx + 1], vertexBuffer[buffIndx + 2]);
			LOG("Normals: %f, %f, %f", vertexBuffer[buffIndx + 3], vertexBuffer[buffIndx + 4], vertexBuffer[buffIndx + 5]);
			LOG("UV: %f, %f, %f", vertexBuffer[buffIndx + 6], vertexBuffer[buffIndx + 7], 0.0f);
		}
		
		buffIndx += 9;
	}

	int fSize = numVertices / 3;
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

	int flags = aiProcess_MakeLeftHanded;
	flags |= aiProcess_JoinIdenticalVertices;
	flags |= aiProcess_GenSmoothNormals;

	if (exporter->Export(scene,formatDescription->id, AddExtension("Exports",name,format).c_str(), flags) == AI_SUCCESS)
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

	aiReleaseExportFormatDescription(formatDescription);
	
	for (int i = 0; i < fSize; ++i)
	{
		aiFace &face = mesh->mFaces[i];
		delete[] face.mIndices;
		face.mIndices = nullptr;
	}

	delete[] mesh->mFaces;
	mesh->mFaces = nullptr;

	delete[] mesh->mVertices;
	delete[] mesh->mNormals;
	delete[] mesh->mTextureCoords[0];

	mesh->mVertices = nullptr;
	mesh->mNormals = nullptr;
	mesh->mTextureCoords[0] = nullptr;

	delete scene->mMaterials[0];
	scene->mMaterials = nullptr;

	delete[] scene->mMaterials;
	scene->mMaterials = nullptr;

	delete scene->mMeshes[0];
	scene->mMeshes[0] = nullptr;
	
	delete[] scene->mMeshes;		
	scene->mMeshes = nullptr;

	return ret;
}

bool FileSystem::ExportPNG(string path)
{
	bool ret = true;

	string finalpath = "Exports/" + path + "_Texture.png";

	int w = 1024;
	int h = 1024;
	int textureSize = h * w * 4;

	ilEnable(IL_FILE_OVERWRITE);

	ILuint texture = 0;

	uint* bytes = new uint[textureSize];

	//From framebuffer to Array
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	//From Array to PNG
	ilGenImages(1, &texture);
	ilBindImage(texture);
	ilTexImage(w, h, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, bytes);
	ilSave(IL_PNG, finalpath.c_str());

	ILenum PossibleError = ilGetError();

	if (PossibleError != IL_NO_ERROR)
	{
		LOG("Error Exporting image");
		LOG("%d: %s/n", PossibleError, iluErrorString(PossibleError));
	}

	ilDeleteImages(1, &texture);

	delete[] bytes;

	return ret;
}

string FileSystem::GetFolderPath(string folder)
{
	char buffer[MAX_PATH];
	getcwd(buffer, MAX_PATH);
	string path = buffer;
	path += "\\"+folder;

	return path;
}

string FileSystem::GetFileNameFromPath(string path)
{
	string ret = path;
	int length = ret.find_last_of("\\");
	ret.erase(0, length+1);

	return ret;
}

string FileSystem::GetFileNameAt(const char * path)
{
	char buff[256];
	string ret = "";

	OPENFILENAME arg1;

	ZeroMemory(&arg1, sizeof(arg1));
	arg1.lStructSize = sizeof(arg1);
	arg1.hwndOwner = NULL;
	arg1.lpstrFile = buff;
	arg1.lpstrFile[0] = '\0';
	arg1.nMaxFile = sizeof(buff);
	arg1.lpstrFilter = "*.PNG\0*.png\0";
	arg1.nFilterIndex = 1;
	arg1.lpstrFileTitle = NULL;
	arg1.nMaxFileTitle = 0;
	arg1.lpstrInitialDir = path;
	arg1.lpstrTitle = "Select Heightmap";
	arg1.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&arg1))
	{
		ret = buff;
	}

	return ret;
}

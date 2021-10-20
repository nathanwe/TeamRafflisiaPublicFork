#include "pch.h"
#include "Model.h"




Model::Model(const char* path) : path(path)
{
    loadModel(path);
}

Model::Model(std::string path) : path(path)
{
    loadModel(path.c_str());
}

// iterate all meshes, draw all meshes
void Model::Draw(Shader& shader) const
{
    for (auto mesh : meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::OnLoad()
{
}




void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);


    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    std::cout << "Loading model, model address: " << path << std::endl;
    double before = glfwGetTime();
    processNode(scene->mRootNode, scene);
    std::cout << "Model loading complete, loading time: " << glfwGetTime() - before << "s " << std::endl;
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


void Model::setVec3(glm::vec3& des, aiVector3D& source) const
{
    des.x = source.x;
    des.y = source.y;
    des.z = source.z;
}


std::vector<Vertex> Model::processVertices(aiMesh* mesh) const
{
    std::vector<Vertex> vertices;
    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // positions
        setVec3(vertex.Position, mesh->mVertices[i]);

        // normals
        if (mesh->HasNormals())
            setVec3(vertex.Normal, mesh->mNormals[i]);

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        }
        else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    return vertices;
}


std::vector<unsigned int> Model::processIndices(aiMesh* mesh) const
{
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    return indices;
}



Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices = processVertices(mesh);
    std::vector<unsigned int> indices = processIndices(mesh);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices);
}

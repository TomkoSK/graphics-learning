#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <shader/shader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int ID;
  std::string type;
  std::string path;
 };

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
  private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

class Model {
  public://
    Model(const char *path);
    void Draw(Shader &shader);
  private:
    std::vector<Texture> loadedTextures;

    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int textureFromFile(std::string fileName, std::string directory);

};
#endif //MODEL_H

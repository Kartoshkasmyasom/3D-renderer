#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <memory>
#include <vector>

GLenum glCheckError_(const char *file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR){
    std::string error;
    switch (errorCode) {
      case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
      }
      std::cout << error << " | " << file << " (" << line << ")" << std::endl;
  }
  return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#ifdef DEBUG
#define GL_CHECK(x) do { \
    x; \
    GLenum error = glGetError(); \
    if (error != GL_NO_ERROR) { \
        std::cerr << "OpenGL error " << error << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        __builtin_trap(); \
    } \
} while(0)
#else
#define GL_CHECK(x) x
#endif



using Vector3 = Eigen::Vector3d;

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1

enum BUFFER_TYPE {
  INDEX_BUFFER =  0,
  POS_VB = 1,
  NORMAL_VB = 2,
  WVP_MAT_VB = 3,
  WORLD_MAT_VB = 4,
  NUM_BUFFERS = 5
};

GLuint m_VAO = 0;
GLuint m_Buffers[NUM_BUFFERS];

struct Mesh {
  Mesh() {
    num_indices = 0;
    base_vertex = 0;
    base_index = 0;
  }

  unsigned int num_indices;
  unsigned int base_vertex;
  unsigned int base_index;
};

std::vector<Mesh> meshes;
std::vector<Vector3> positions;
std::vector<Vector3> normals;
std::vector<unsigned int> indices;

void Clear() {
  if(m_VAO != 0) {
    std::cout<<"1\n";
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  unsigned int buffer_size = sizeof(m_Buffers) / sizeof(m_Buffers[0]);
  std::cout<<buffer_size<<"\n";
  std::cout<<"2\n";
  for(unsigned int i = 0; i < buffer_size; ++i) {
    std::cout<<m_Buffers[i] << ' ';
    //m_Buffers[i] = 0;
  }
  try {
    GL_CHECK(glDeleteBuffers(NUM_BUFFERS, m_Buffers));
  } catch(...) {
    glCheckError();
  }


  positions.clear();
  normals.clear();
  indices.clear();
}

void count_vertices_and_indices(const aiScene* pScene, unsigned int& num_vertices, unsigned int& num_indices) {
  for(unsigned int i = 0; i < meshes.size(); ++i) {
    meshes[i].num_indices = pScene->mMeshes[i]->mNumFaces * 3;
    meshes[i].base_vertex = num_vertices;
    meshes[i].base_index = num_indices;

    num_vertices += pScene->mMeshes[i]->mNumVertices;
    num_indices += meshes[i].num_indices;
  }
}

void reserve_space(unsigned int num_vertices, unsigned int num_indices) {
  positions.reserve(num_vertices);
  normals.reserve(num_vertices);
  indices.reserve(num_indices);
}

void import_single_mesh(const aiMesh* mesh) {
  for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    const aiVector3D& pos = mesh->mVertices[i];
    positions.push_back(Vector3(pos.x, pos.y, pos.z));

    if (mesh->HasNormals()) {
      const aiVector3D& normal = mesh->mNormals[i];
      normals.push_back(Vector3(normal.x, normal.y, normal.z));
    }
  }

  for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }
}

void import_mesh(const aiScene* scene) {
  for(unsigned int i = 0; i < meshes.size(); ++i) {
    const aiMesh* mesh = scene->mMeshes[i];
    import_single_mesh(mesh);
  }
}

void PopulateBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(POSITION_LOCATION);
  glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(NORMAL_LOCATION);
  glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

bool InitFromScene(const aiScene* pScene, const std::string& pFile) {
  meshes.resize(pScene->mNumMeshes);
  unsigned int num_vertices = 0;
  unsigned int num_indices = 0;

  count_vertices_and_indices(pScene, num_vertices, num_indices);
  reserve_space(num_vertices, num_indices);
  import_mesh(pScene);

  PopulateBuffers();
  return glCheckError();
}


bool DoTheImportThing( const std::string& pFile) {
  bool result = true;
  Clear();

  std::cout<<"3\n";
  glGenVertexArrays(1, &m_VAO);
  std::cout<<"4\n";
  glBindVertexArray(m_VAO);

  std::cout<<"5\n";
  glGenBuffers(sizeof(m_Buffers) / sizeof(m_Buffers[0]), m_Buffers);

  // Create an instance of the Importer class
  Assimp::Importer importer;

  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile( pFile,
    aiProcess_CalcTangentSpace       |
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_GenSmoothNormals       |
    aiProcess_SortByPType);

  // If the import failed, report it
  if (nullptr == scene) {
   // DoTheErrorLogging( importer.GetErrorString());
    throw std::runtime_error(importer.GetErrorString());
  }

  result = InitFromScene(scene, pFile);
  std::cout<<"6\n";
  glBindVertexArray(0);

  // We're done. Everything will be cleaned up by the importer destructor
  return result;
}

int main() {
  if(!glfwInit()) {
    std::cerr << "glfwInit failed\n";
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "test", nullptr, nullptr);
  if(!window) {
    std::cerr << "glfwCreateWindow failed\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  GLenum err = glewInit();

  try {
    auto scene = DoTheImportThing("/home/nasty/Workspace/VSCodeprojects/3d-renderer/file-import/penis.obj");
    std::cout<<"Successful import!\n";
  } catch (std::runtime_error& e) {
      std::cout<<e.what();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
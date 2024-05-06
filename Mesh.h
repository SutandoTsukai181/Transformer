#ifndef TRANSFORMER_MESH_H
#define TRANSFORMER_MESH_H

#include <Eigen/Dense>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh {
public:
    Mesh();
    void init();
    void draw();
    void setVertices(float* vertices, int size);
    void setFaces(unsigned short* faces, int size);
//private:
    int verticesSize, facesSize;
    float* vertices;
    unsigned short* faces;
    GLuint vertexAttribBuffer, indexBuffer;
};


#endif //TRANSFORMER_MESH_H

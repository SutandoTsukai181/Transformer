#include "Mesh.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Mesh::Mesh() {

}

void Mesh::init() {
    glGenBuffers(1, &vertexAttribBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesSize * sizeof(unsigned short), faces, GL_STATIC_DRAW);
}

void Mesh::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBuffer);
    glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
//    glEnableVertexAttribArray(1);    //We like submitting normals on stream 1 for no special reason
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(12));     //The starting point of normals, 12 bytes away
//    glEnableVertexAttribArray(2);    //We like submitting texcoords on stream 2 for no special reason
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, facesSize, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
}

void Mesh::setVertices(float* vertices, int size) {
    this->vertices = vertices;
    verticesSize = size;
}

void Mesh::setFaces(unsigned short* faces, int size) {
    this->faces = faces;
    facesSize = size;
}

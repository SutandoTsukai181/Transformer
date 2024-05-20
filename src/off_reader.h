#ifndef TRANSFORMER_OFF_READER_H
#define TRANSFORMER_OFF_READER_H

#include <fstream>
#include "mesh.h"

void readOff(const std::string& filename, Mesh& mesh) {
    std::ifstream file(filename);

    // Header
    std::string off;
    int vertexCount, faceCount, edgeCount;

    file >> off;
    file >> vertexCount >> faceCount >> edgeCount;

    auto* vertices = new float[vertexCount * 3];
    auto* faces = new unsigned short[faceCount * 3];

    // Vertices
    for (int i = 0; i < vertexCount; i++) {
        file >> vertices[i*3 + 0] >> vertices[i*3 + 1] >> vertices[i*3 + 2];
    }

    // Faces
    int faceVertexCount;
    for (int i = 0; i < faceCount; i++) {
        file >> faceVertexCount;
        assert(faceVertexCount == 3);
        file >> faces[i*3 + 0] >> faces[i*3 + 1] >> faces[i*3 + 2];
    }

    mesh.setVertices(vertices, vertexCount * 3);
    mesh.setFaces(faces, faceCount * 3);

    file.close();
}

#endif //TRANSFORMER_OFF_READER_H

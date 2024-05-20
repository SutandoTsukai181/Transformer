#include "cpu_transformer.h"

#include <cmath>

void CpuTransformer::translate(Eigen::Vector3f t) {
    applyTransformation(translateMat(t[0], t[1], t[2]));
}

void CpuTransformer::rotateAroundX(float angle, float y, float z) {
    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, 0,
            0, std::cos(angle), -std::sin(angle), 0,
            0, std::sin(angle), std::cos(angle), 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(0, y, z) * mat * translateMat(0, -y, -z));
}

void CpuTransformer::rotateAroundY(float angle, float x, float z) {
    Eigen::Matrix4f mat;
    mat <<  std::cos(angle), 0, std::sin(angle), 0,
            0, 1, 0, 0,
            -std::sin(angle), 0, std::cos(angle), 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(x, 0, z) * mat * translateMat(-x, 0, -z));
}

void CpuTransformer::rotateAroundZ(float angle, float x, float y) {
    Eigen::Matrix4f mat;
    mat <<  std::cos(angle), -std::sin(angle), 0, 0,
            std::sin(angle), std::cos(angle), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(x, y, 0) * mat * translateMat(-x, -y, 0));
}

void CpuTransformer::rotateAroundArbitraryAxis(float angle, Eigen::Vector3f p1, Eigen::Vector3f p2) {

}

void CpuTransformer::scaleFromPoint(Eigen::Vector3f s, Eigen::Vector3f p) {

}

void CpuTransformer::reflectOverPlane(Eigen::Vector3f p, Eigen::Vector3f n) {

}

void CpuTransformer::shearX(float sy, float sz) {

}

void CpuTransformer::shearY(float sx, float sz) {

}

void CpuTransformer::shearZ(float sx, float sy) {

}

void CpuTransformer::applyTransformation(const Eigen::Matrix4f& mat) {
    float* vertices;
    int size;

    mesh.mapVertexBuffer(vertices, size);

    Eigen::Vector4f vert;
    Eigen::Vector4f out_vert;
    vert[3] = 1;
    for (int i = 0; i < size / 3; i++) {
        vert[0] = vertices[i*3];
        vert[1] = vertices[i*3+1];
        vert[2] = vertices[i*3+2];

        out_vert = mat * vert;
        vertices[i*3] = out_vert[0];
        vertices[i*3+1] = out_vert[1];
        vertices[i*3+2] = out_vert[2];
    }

    mesh.unmapVertexBuffer();
}

Eigen::Matrix4f CpuTransformer::translateMat(float tx, float ty, float tz) {
    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, tx,
            0, 1, 0, ty,
            0, 0, 1, tz,
            0, 0, 0, 1;

    return mat;
}

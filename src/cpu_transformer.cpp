#include "cpu_transformer.h"

#include <cmath>

CpuTransformer::CpuTransformer() {

}

CpuTransformer::CpuTransformer(BaseTransformer *other) : BaseTransformer(other) {

}

void CpuTransformer::translate(Eigen::Vector3f t) {
    applyTransformation(translateMat(t[0], t[1], t[2]));
}

void CpuTransformer::rotateAroundX(float angle, float y, float z) {
    angle *= EIGEN_PI / 180;

    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, 0,
            0, std::cos(angle), -std::sin(angle), 0,
            0, std::sin(angle), std::cos(angle), 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(0, y, z) * mat * translateMat(0, -y, -z));
}

void CpuTransformer::rotateAroundY(float angle, float x, float z) {
    angle *= EIGEN_PI / 180;

    Eigen::Matrix4f mat;
    mat <<  std::cos(angle), 0, std::sin(angle), 0,
            0, 1, 0, 0,
            -std::sin(angle), 0, std::cos(angle), 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(x, 0, z) * mat * translateMat(-x, 0, -z));
}

void CpuTransformer::rotateAroundZ(float angle, float x, float y) {
    angle *= EIGEN_PI / 180;

    Eigen::Matrix4f mat;
    mat <<  std::cos(angle), -std::sin(angle), 0, 0,
            std::sin(angle), std::cos(angle), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(x, y, 0) * mat * translateMat(-x, -y, 0));
}

void CpuTransformer::rotateAroundArbitraryAxis(float angle, Eigen::Vector3f p1, Eigen::Vector3f p2) {
    auto mat = alignWithZAxisMat(p1, p2 - p1);
    angle *= EIGEN_PI / 180;

    Eigen::Matrix4f rotAroundZ;
    rotAroundZ <<   std::cos(angle), -std::sin(angle), 0, 0,
                    std::sin(angle), std::cos(angle), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;

    applyTransformation(mat.inverse() * rotAroundZ * mat);
}

void CpuTransformer::scaleFromPoint(Eigen::Vector3f s, Eigen::Vector3f p) {
    Eigen::Matrix4f mat;
    mat <<  s[0], 0, 0, 0,
            0, s[1], 0, 0,
            0, 0, s[2], 0,
            0, 0, 0, 1;

    applyTransformation(translateMat(p[0], p[1], p[2]) * mat * translateMat(-p[0], -p[1], -p[2]));
}

void CpuTransformer::reflectOverPlane(Eigen::Vector3f p, Eigen::Vector3f n) {
    auto mat = alignWithZAxisMat(p, n);

    Eigen::Matrix4f reflectOverXY;
    reflectOverXY <<    1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, -1, 0,
                        0, 0, 0, 1;

    applyTransformation(mat.inverse() * reflectOverXY * mat);
}

void CpuTransformer::shearX(float sy, float sz) {
    Eigen::Matrix4f mat;
    mat <<  1, sy, sz, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    applyTransformation(mat);
}

void CpuTransformer::shearY(float sx, float sz) {
    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, 0,
            sx, 1, sz, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    applyTransformation(mat);
}

void CpuTransformer::shearZ(float sx, float sy) {
    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, 0,
            0, 1, 0, 0,
            sx, sy, 1, 0,
            0, 0, 0, 1;

    applyTransformation(mat);
}

void CpuTransformer::applyTransformation(Eigen::Matrix4f mat) {
    float* vertices;
    int size;

    Eigen::Matrix4f oldCurrentInv = currentMatrix.inverse();
    currentMatrix = currentMatrix * mat;
    if (isLocalTrans()) {
        // Multiply by inverse to apply transformation locally
        mat = currentMatrix * oldCurrentInv;
    }

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

Eigen::Matrix4f CpuTransformer::alignWithZAxisMat(const Eigen::Vector3f& p, const Eigen::Vector3f& v) {
    auto u = v.normalized();
    float a = u.x(), b = u.y(), c = u.z();
    float d = sqrtf(b * b + c * c);

    auto transToOrigin = translateMat(-p[0], -p[1], -p[2]);

    Eigen::Matrix4f rotAroundX;
    rotAroundX <<   1, 0, 0, 0,
            0, c/d, -b/d, 0,
            0, b/d, c/d, 0,
            0, 0, 0, 1;

    Eigen::Matrix4f rotAroundY;
    rotAroundY <<   d/u.norm(), 0, a/u.norm(), 0,
            0, 1, 0, 0,
            -a/u.norm(), 0, d/u.norm(), 0,
            0, 0, 0, 1;

    return rotAroundY * rotAroundX * transToOrigin;
}

void CpuTransformer::reset() {
    mesh.reset();
    currentMatrix.setIdentity();
}

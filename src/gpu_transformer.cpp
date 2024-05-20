#include "gpu_transformer.h"

void GpuTransformer::translate(Eigen::Vector3f t) {
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(t[0], t[1], t[2]);
}

void GpuTransformer::rotateAroundX(float angle, float y, float z) {
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(0, -y, -z);
    glRotatef(angle, 1, 0, 0);
    glTranslatef(0, y, z);
}

void GpuTransformer::rotateAroundY(float angle, float x, float z) {
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(-x, 0, -z);
    glRotatef(angle, 0, 1, 0);
    glTranslatef(x, 0, z);
}

void GpuTransformer::rotateAroundZ(float angle, float x, float y) {
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(-x, -y, 0);
    glRotatef(angle, 0, 0, 1);
    glTranslatef(x, y, 0);
}

void GpuTransformer::rotateAroundArbitraryAxis(float angle, Eigen::Vector3f p1, Eigen::Vector3f p2) {
    glMatrixMode(GL_MODELVIEW);
    
    auto u = (p2 - p1).normalized();
    glTranslatef(-p1[0], -p1[1], -p1[2]);
    glRotatef(angle, u[0], u[1], u[2]);
    glTranslatef(p1[0], p1[1], p1[2]);
}

void GpuTransformer::scaleFromPoint(Eigen::Vector3f s, Eigen::Vector3f p) {
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(-p[0], -p[1], -p[2]);
    glScaled(s[0], s[1], s[2]);
    glTranslatef(p[0], p[1], p[2]);
}

void GpuTransformer::reflectOverPlane(Eigen::Vector3f p, Eigen::Vector3f n) {
    glMatrixMode(GL_MODELVIEW);

    auto u = n.normalized();
    float a = u[0], b = u[1], c = u[2];
    float d = sqrtf(b * b + c * c);

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

    glTranslatef(-p[0], -p[1], -p[2]);
//    glRotatef(asinf(b / d), 1, 0, 0);
//    glRotatef(acosf(d / u.norm()), 0, 1, 0);
    glMultMatrixf((rotAroundY * rotAroundX).transpose().eval().data());
    glScalef(1, 1, -1);
    glMultMatrixf((rotAroundY * rotAroundX).inverse().transpose().eval().data());
//    glRotatef(-acosf(d / u.norm()), 0, 1, 0);
//    glRotatef(-asinf(b / d), 1, 0, 0);
    glTranslatef(p[0], p[1], p[2]);
}

void GpuTransformer::shearX(float sy, float sz) {
    Eigen::Matrix4f mat;
    mat <<  1, sy, sz, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    glMultMatrixf(mat.transpose().eval().data());
}

void GpuTransformer::shearY(float sx, float sz) {
    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, 0,
            sx, 1, sz, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    glMultMatrixf(mat.transpose().eval().data());
}

void GpuTransformer::shearZ(float sx, float sy) {
    Eigen::Matrix4f mat;
    mat <<  1, 0, 0, 0,
            0, 1, 0, 0,
            sx, sy, 1, 0,
            0, 0, 0, 1;

    glMultMatrixf(mat.transpose().eval().data());
}

void GpuTransformer::reset() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool GpuTransformer::isLocalTrans() {
    // No global transformation/world coordinate system for GPU mode
    return true;
}

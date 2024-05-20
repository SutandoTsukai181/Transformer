#ifndef TRANSFORMER_BASE_TRANSFORMER_H
#define TRANSFORMER_BASE_TRANSFORMER_H


#include "mesh.h"

class BaseTransformer {
public:
    BaseTransformer();
    ~BaseTransformer();
    virtual void translate(Eigen::Vector3f t) = 0;
    virtual void rotateAroundX(float angle, float y, float z) = 0;
    virtual void rotateAroundY(float angle, float x, float z) = 0;
    virtual void rotateAroundZ(float angle, float x, float y) = 0;
    virtual void rotateAroundArbitraryAxis(float angle, Eigen::Vector3f p1, Eigen::Vector3f p2) = 0;
    virtual void scaleFromPoint(Eigen::Vector3f s, Eigen::Vector3f p) = 0;
    virtual void reflectOverPlane(Eigen::Vector3f p, Eigen::Vector3f n) = 0;
    virtual void shearX(float sy, float sz) = 0;
    virtual void shearY(float sx, float sz) = 0;
    virtual void shearZ(float sx, float sy) = 0;
    virtual void reset() = 0;
    void read(const std::string& filename);
    void draw();

    virtual bool isLocalTrans();
    void setLocalTrans(bool isLocal);

protected:
    Mesh mesh;
    bool isLocal = false;
};


#endif //TRANSFORMER_BASE_TRANSFORMER_H

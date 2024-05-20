#ifndef TRANSFORMER_CPU_TRANSFORMER_H
#define TRANSFORMER_CPU_TRANSFORMER_H

#include "base_transformer.h"

class CpuTransformer : public BaseTransformer {
public:
    void translate(Eigen::Vector3f t) override;

    void rotateAroundX(float angle, float y, float z) override;

    void rotateAroundY(float angle, float x, float z) override;

    void rotateAroundZ(float angle, float x, float y) override;

    void rotateAroundArbitraryAxis(float angle, Eigen::Vector3f p1, Eigen::Vector3f p2) override;

    void scaleFromPoint(Eigen::Vector3f s, Eigen::Vector3f p) override;

    void reflectOverPlane(Eigen::Vector3f p, Eigen::Vector3f n) override;

    void shearX(float sy, float sz) override;

    void shearY(float sx, float sz) override;

    void shearZ(float sx, float sy) override;

private:
    static Eigen::Matrix4f translateMat(float tx, float ty, float tz);
    void applyTransformation(const Eigen::Matrix4f& mat);
};


#endif //TRANSFORMER_CPU_TRANSFORMER_H

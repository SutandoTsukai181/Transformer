#include "base_transformer.h"
#include "off_reader.h"

BaseTransformer::BaseTransformer() = default;

BaseTransformer::BaseTransformer(BaseTransformer *other) {
    // Copy mesh
    // (Mesh does not have a destructor so the pointers will still be valid)
    // (Can't get dangling pointers if you let everything leak)
    mesh = other->mesh;
    isLocal = other->isLocalTrans();
    isInit = other->isInitialized();
}

bool BaseTransformer::read(const std::string& filename) {
    if (readOff(filename, mesh)) {
        mesh.init();
        isInit = true;

        return true;
    }

    return false;
}

void BaseTransformer::draw() {
    mesh.draw();
}

bool BaseTransformer::isLocalTrans() {
    return isLocal;
}

void BaseTransformer::setLocalTrans(bool isLocal) {
    this->isLocal = isLocal;
}

bool BaseTransformer::isInitialized() {
    return isInit;
}

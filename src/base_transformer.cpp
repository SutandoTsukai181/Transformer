#include "base_transformer.h"
#include "off_reader.h"

BaseTransformer::BaseTransformer() = default;

BaseTransformer::~BaseTransformer() = default;

void BaseTransformer::read(const std::string& filename) {
    readOff(filename, mesh);
    mesh.init();
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

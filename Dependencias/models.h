#include "model.h"

#ifndef MODELS_H
#define MODELS_H

Model appleModel;

void loadModels() {
    appleModel.loadModel("../Dependencias/Apple/Apple.obj");
}

#endif
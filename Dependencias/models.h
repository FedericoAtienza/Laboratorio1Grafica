#include "model.h"

#ifndef MODELS_H
#define MODELS_H

Model appleModel;
Model spikeModel;

void loadModels() {
    appleModel.loadModel("../Dependencias/Apple/Apple.obj");
    spikeModel.loadModel("../Dependencias/Spike/Spike.dae");
}

#endif
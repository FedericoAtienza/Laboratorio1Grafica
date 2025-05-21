#include "model.h"

#ifndef MODELS_H
#define MODELS_H

Model appleModel;
Model spikeModel;
Model wormModel;

void loadModels() {
    appleModel.loadModel("../Dependencias/Apple/Apple.obj");
    spikeModel.loadModel("../Dependencias/Spike/Spike.dae");
    wormModel.loadModel("../Dependencias/Wiggler/Wiggler.dae");
}

#endif
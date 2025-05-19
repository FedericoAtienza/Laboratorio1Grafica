#include "model.h"

#ifndef MODELS_H
#define MODELS_H

Model appleModel;
Model wormHeadModel;
Model wormBodyModel;

void loadModels() {
    appleModel.loadModel("../Dependencias/Apple/Apple.obj");
    wormHeadModel.loadModel("../Dependencias/Worm/car_hana1.dae");
    wormBodyModel.loadModel("../Dependencias/Worm/car_hana1body.dae");
}

#endif
#include "model.h"

#ifndef MODELS_H
#define MODELS_H

Model appleModel;
Model spikeModel;
Model wormBody;
Model wormEye;
Model wormHead;
Model wormLips;
Model wormEyelid;

void loadModels() {
    appleModel.loadModel("../Dependencias/Apple/Apple.obj");
    spikeModel.loadModel("../Dependencias/Spike/Spike.dae");
    wormBody.loadModel("../Dependencias/Wiggler/Wiggler.dae");
    wormEye.loadModel("../Dependencias/Wiggler/EyeballWorm.dae");
    wormHead.loadModel("../Dependencias/Wiggler/Cabeza.dae");
    wormLips.loadModel("../Dependencias/Wiggler/Lips.obj");
    wormEyelid.loadModel("../Dependencias/Wiggler/Eyelid.obj");
}

#endif
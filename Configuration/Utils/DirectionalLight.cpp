//
// Created by OMRI_PRIVATE on 05/12/2022.
//

#include "DirectionalLight.h"
DirectionalLight::DirectionalLight(vec3 direction) {
    this->liType = Directional;
    this->direction = direction;
}
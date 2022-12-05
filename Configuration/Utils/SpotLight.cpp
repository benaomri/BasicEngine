//
// Created by OMRI_PRIVATE on 05/12/2022.
//

#include "SpotLight.h"
SpotLight::SpotLight(vec3 direction) {
    this->liType = Spot;
    this->direction = direction;
}
#ifndef _ROC_GRAVITY_COMPONENT_H_
#define _ROC_GRAVITY_COMPONENT_H_

#include "../Component.hpp"

ROCKET_COMPONENT(Gravity,
    ROCKET_PROPERTY_DEFVAL(public, double, gravity, 9.81)
);

#endif
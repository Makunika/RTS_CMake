//
// Created by pshbl on 10.06.2021.
//

#ifndef RTS_HOME_H
#define RTS_HOME_H

#include "../base/StaticObject.h"
#include "../../../light/PointLight.h"

enum HouseType {
    HOUSE1,
    HOUSE2,
    HOUSE3
};

class Home: public StaticObject {
private:
    bool isLight;

public:
    PointLight* pointLight = nullptr;

    Home(float x, float y, State *state, HouseType houseType, float angle);
    Home(float x, float y, State *state, HouseType houseType, float angle, bool light);
    static Shader* modelShader;
};


#endif //RTS_HOME_H

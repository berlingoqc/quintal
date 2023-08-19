#pragma once

#include "iot_control.hpp"

class Part {

public:
    SwitchGroupPart(std::shared_ptr<IotControl> control) : control_(control) {}

protected:
    std::shared_ptr<IotControl> control_;
};
#pragma once

#include <map>

#include "part.hpp"

class SwitchGroupPart : public Part {

public:
    SwitchGroupPart(
        std::map<std::string, int32_t> pins_,
        std::shared_ptr<IotControl> control
    );

}
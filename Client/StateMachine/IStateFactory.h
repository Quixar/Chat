#pragma once

#include "State.h"
#include <memory>

class IStateFactory
{
public:
    virtual std::unique_ptr<State> create() = 0;
    virtual ~IStateFactory() = default;
};

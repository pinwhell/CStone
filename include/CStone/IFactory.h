#pragma once

#include <memory>
#include <CStone/ICapstone.h>

class ICapstoneFactory {
public:
    virtual ~ICapstoneFactory() {}
    virtual std::unique_ptr<ICapstone> CreateInstance(bool bDetailedInst = true) = 0;
};
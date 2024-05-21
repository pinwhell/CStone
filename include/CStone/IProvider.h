#pragma once

#include <CStone/ICapstone.h>
#include <CStone/IFactory.h>

class ICapstoneProvider {
public:
    virtual ICapstone* GetInstance(bool bDetailedInstuction = true, ICapstoneFactory* factory = nullptr) = 0;
};
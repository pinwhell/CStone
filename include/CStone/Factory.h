#pragma once

#include <stdexcept>

#include <CStone/ICapstone.h>
#include <CStone/IFactory.h>
#include <CStone/ECStone.h>

class CapstoneCreationFailedException : public std::runtime_error {
public:
    CapstoneCreationFailedException(const std::string& what);
};

class CapstoneFactory : public ICapstoneFactory
{
public:
    CapstoneFactory(ECapstoneArchMode archMode);

    std::unique_ptr<ICapstone> CreateInstance(bool bDetailedInst = true) override;

    ECapstoneArchMode mArchMode;
};
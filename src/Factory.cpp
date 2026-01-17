#include <CStone/Factory.h>
#include <CStone/Arch/ARM/32/Capstone.h>
#include <CStone/Arch/ARM/64/Capstone.h>
#include <format>

CapstoneFactory::CapstoneFactory(ECapstoneArchMode archMode)
    : mArchMode(archMode)
{}

std::unique_ptr<ICapstone> CapstoneFactory::CreateInstance(bool bDetailedInst) {
    switch (mArchMode) {
    case ECapstoneArchMode::ARM32_ARM:
    case ECapstoneArchMode::ARM32_THUMB:
        return std::move(std::make_unique<ARM32Capstone>(ECapstoneArchMode::ARM32_THUMB == mArchMode, bDetailedInst));

    case ECapstoneArchMode::AARCH64_ARM:
        return std::move(std::make_unique<ARM64Capstone>(bDetailedInst));

    default:
        throw CapstoneCreationFailedException(std::format("ArchMode not implemented."));
    }

    return 0;
}

CapstoneCreationFailedException::CapstoneCreationFailedException(const std::string& what)
    : std::runtime_error(what)
{}
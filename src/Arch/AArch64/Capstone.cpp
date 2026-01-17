#include <CStone/Arch/AArch64/Capstone.h>

AArch64Capstone::AArch64Capstone(bool bDetailedInsn)
    : mCapstone(CS_ARCH_AARCH64, CS_MODE_ARM, bDetailedInsn)
{}

ICapstoneUtility* AArch64Capstone::getUtility() {
    return &mUtility;
}

ICapstoneHeuristic* AArch64Capstone::getHeuristic()
{
    return &mHeuristic;
}

CapstoneDismHandle AArch64Capstone::Disassemble(const void* start, size_t nBytes, uint64_t pcAddr) {
    return mCapstone.Disassemble(start, nBytes, pcAddr);
}

CsInsn AArch64Capstone::DisassembleOne(const void* start, uint64_t pcAddr)
{
    return mCapstone.DisassembleOne(start, pcAddr);
}

void AArch64Capstone::InsnForEach(const void* _start, std::function<bool(const CsInsn& insn)> callback, uint64_t pcAddr, size_t buffSize)
{
    mCapstone.InsnForEach(_start, callback, pcAddr, buffSize);
}

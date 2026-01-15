#include <CStone/Arch/ARM/64/Capstone.h>

ARM64Capstone::ARM64Capstone(bool bDetailedInsn)
    : mCapstone(CS_ARCH_ARM64, CS_MODE_ARM, bDetailedInsn)
{}

ICapstoneUtility* ARM64Capstone::getUtility() {
    return &mUtility;
}

ICapstoneHeuristic* ARM64Capstone::getHeuristic()
{
    return &mHeuristic;
}

CapstoneDismHandle ARM64Capstone::Disassemble(const void* start, size_t nBytes, uint64_t pcAddr) {
    return mCapstone.Disassemble(start, nBytes, pcAddr);
}

CsInsn ARM64Capstone::DisassembleOne(const void* start, uint64_t pcAddr)
{
    return mCapstone.DisassembleOne(start, pcAddr);
}

void ARM64Capstone::InsnForEach(const void* _start, std::function<bool(const CsInsn& insn)> callback, uint64_t pcAddr, size_t buffSize)
{
    mCapstone.InsnForEach(_start, callback, pcAddr, buffSize);
}

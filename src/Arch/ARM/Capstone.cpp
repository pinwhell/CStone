#include <CStone/Arch/ARM/32/Capstone.h>

ARM32Capstone::ARM32Capstone(bool mbThumb, bool bDetailedInsn)
    : mCapstone(CS_ARCH_ARM, mbThumb ? CS_MODE_THUMB : CS_MODE_ARM, bDetailedInsn)
{}

ICapstoneUtility* ARM32Capstone::getUtility() {
    return &mUtility;
}

ICapstoneHeuristic* ARM32Capstone::getHeuristic()
{
    return &mHeuristic;
}

CapstoneDismHandle ARM32Capstone::Disassemble(const void* start, size_t nBytes, uint64_t pcAddr) {
    return mCapstone.Disassemble(start, nBytes, pcAddr);
}

CsInsn ARM32Capstone::DisassembleOne(const void* start, uint64_t pcAddr)
{
    return mCapstone.DisassembleOne(start, pcAddr);
}

void ARM32Capstone::InsnForEach(const void* _start, std::function<bool(const CsInsn& insn)> callback, uint64_t pcAddr, size_t buffSize)
{
    mCapstone.InsnForEach(_start, callback, pcAddr, buffSize);
}
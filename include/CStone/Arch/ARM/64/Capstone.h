#pragma once

#include <CStone/CStone.h>

#include <CStone/Arch/ARM/64/Utility.h>
#include <CStone/Arch/ARM/64/Heuristic.h>

class ARM64Capstone : public ICapstone {
public:
    ARM64Capstone(bool bDetailedInsn = true);

    ICapstoneUtility* getUtility() override;
    ICapstoneHeuristic* getHeuristic() override;
    CapstoneDismHandle Disassemble(const void* start, size_t nBytes, uint64_t pcAddr = 0) override;
    CsInsn DisassembleOne(const void* start, uint64_t pcAddr = 0) override;
    void InsnForEach(const void* start, std::function<bool(const CsInsn& insn)> callback, uint64_t pcAddr = 0, size_t buffSize = SIZE_MAX) override;

    Capstone mCapstone;
    ARM64CapstoneUtility mUtility;
    ARM64CapstoneHeuristic mHeuristic;
};

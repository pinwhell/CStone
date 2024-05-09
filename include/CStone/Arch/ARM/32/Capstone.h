#pragma once

#include <CStone/CStone.h>

#include <CStone/Arch/ARM/32/Utility.h>
#include <CStone/Arch/ARM/32/Heuristic.h>

class ARM32Capstone : public ICapstone {
public:
    ARM32Capstone(bool mbThumb = false, bool bDetailedInsn = true);

    ICapstoneUtility* getUtility() override;
    ICapstoneHeuristic* getHeuristic() override;
    CapstoneDismHandle Disassemble(const void* start, size_t nBytes, uint64_t pcAddr = 0) override;
    CsInsn DisassembleOne(const void* start, uint64_t pcAddr = 0) override;
    void InsnForEach(const void* start, std::function<bool(const CsInsn& insn)> callback, uint64_t pcAddr = 0, size_t buffSize = SIZE_MAX) override;

    Capstone mCapstone;
    ARM32CapstoneUtility mUtility;
    ARM32CapstoneHeuristic mHeuristic;
};
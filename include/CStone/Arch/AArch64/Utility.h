#pragma once

#include <CStone/ICapstone.h>
#include <CStone/Utility.h>

class AArch64CapstoneUtility : public ICapstoneUtility {
public:
    bool InsnHasRegister(const cs_insn* pIns, uint16_t reg) const override;
    int64_t InsnGetImmByIndex(const cs_insn* pIns, size_t index) const override;
    uint16_t InsnGetPseudoDestReg(const cs_insn* pIns) const override;
    bool InsnIsBranch(const cs_insn* pInsn) const override;
    bool InsnHasCondition(const cs_insn* pInsn) const override;

    CapstoneUtility mBaseUtility;
};

const void* AArch64PCCompute(ICapstone* capstone, const void* at, uint64_t disp = 0);
const void* AArch64LDRPCDispResolve(ICapstone* capstone, const void* at, bool bDerref = false);
const void* AArch64FarPcRelLEATryResolve(ICapstone* capstone, const void* at, bool bDerref = false);
#pragma once

#include <CStone/ICapstone.h>
#include <CStone/Utility.h>

class ARM64CapstoneUtility : public ICapstoneUtility {
public:
    bool InsnHasRegister(const cs_insn* pIns, uint16_t reg) const override;
    int64_t InsnGetImmByIndex(const cs_insn* pIns, size_t index) const override;
    uint16_t InsnGetPseudoDestReg(const cs_insn* pIns) const override;
    bool InsnIsBranch(const cs_insn* pInsn) const override;
    bool InsnHasCondition(const cs_insn* pInsn) const override;

    CapstoneUtility mBaseUtility;
};

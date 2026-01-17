#pragma once

#include <CStone/IHeuristic.h>

class AArch64CapstoneHeuristic : public ICapstoneHeuristic {
public:
    bool InsnIsProcedureEntry(const cs_insn* pInsn) const override;
    bool InsnIsProcedureExit(const cs_insn* pInsn) const override;
};

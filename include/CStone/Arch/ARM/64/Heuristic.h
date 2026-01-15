#pragma once

#include <CStone/IHeuristic.h>

class ARM64CapstoneHeuristic : public ICapstoneHeuristic {
public:
    bool InsnIsProcedureEntry(const cs_insn* pInsn) const override;
    bool InsnIsProcedureExit(const cs_insn* pInsn) const override;
};

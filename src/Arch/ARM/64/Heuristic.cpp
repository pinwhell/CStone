#include <CStone/Arch/ARM/64/Heuristic.h>
#include <CStone/Utility.h>

bool ARM64CapstoneHeuristic::InsnIsProcedureEntry(const cs_insn* pInsn) const {
    // STP x29, x30, [sp, ...]
    if (pInsn->id != ARM64_INS_STP)
        return false;

    // Check operands: first two must be X29 (FP) and X30 (LR)
    auto& ops = pInsn->detail->arm64.operands;
    if (pInsn->detail->arm64.op_count < 2) 
        return false;

    if (ops[0].type == ARM64_OP_REG && ops[0].reg == ARM64_REG_X29 &&
        ops[1].type == ARM64_OP_REG && ops[1].reg == ARM64_REG_X30) {
        return true;
    }
    
    return false;
}

bool ARM64CapstoneHeuristic::InsnIsProcedureExit(const cs_insn* pInsn) const {
    return pInsn->id == ARM64_INS_RET;
}

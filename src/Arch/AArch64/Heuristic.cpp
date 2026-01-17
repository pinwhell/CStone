#include <CStone/Arch/AArch64/Heuristic.h>
#include <CStone/Utility.h>

bool AArch64CapstoneHeuristic::InsnIsProcedureEntry(const cs_insn* pInsn) const {
    // STP x29, x30, [sp, ...]
    if (pInsn->id != AARCH64_INS_STP)
        return false;

    // Check operands: first two must be X29 (FP) and X30 (LR)
    auto& ops = pInsn->detail->aarch64.operands;
    if (pInsn->detail->aarch64.op_count < 2) 
        return false;

    if (ops[0].type == AARCH64_OP_REG && ops[0].reg == AARCH64_REG_X29 &&
        ops[1].type == AARCH64_OP_REG && ops[1].reg == AARCH64_REG_X30) {
        return true;
    }
    
    return false;
}

bool AArch64CapstoneHeuristic::InsnIsProcedureExit(const cs_insn* pInsn) const {
    return pInsn->id == AARCH64_INS_RET;
}

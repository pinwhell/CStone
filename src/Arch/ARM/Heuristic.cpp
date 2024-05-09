#include <CStone/Arch/ARM/32/Heuristic.h>
#include <CStone/Utility.h>

bool ARM32CapstoneHeuristic::InsnIsProcedureEntry(const cs_insn* pInsn) const {
    return
        pInsn->detail->arm.cc == ARMCC_AL &&
        pInsn->id == ARM_INS_PUSH &&
        CapstoneUtility::InsnHasRegister(pInsn->detail->arm, ARM_REG_LR);
}

bool ARM32CapstoneHeuristic::InsnIsProcedureExit(const cs_insn* pInsn) const {
    bool foundUnconditionalPopPc =
        pInsn->detail->arm.cc == ARMCC_AL &&
        pInsn->id == ARM_INS_POP &&
        CapstoneUtility::InsnHasRegister(pInsn->detail->arm, ARM_REG_PC);

    bool foundBranchLinkReg =
        pInsn->id == ARM_INS_BX &&
        CapstoneUtility::InsnHasRegister(pInsn->detail->arm, ARM_REG_LR);

    return foundUnconditionalPopPc || foundBranchLinkReg;
}
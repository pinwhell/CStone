#include <CStone/Arch/ARM/32/Utility.h>
#include <stdexcept>
#include <CStone/Arch/ARM/32/Capstone.h>
#include <format>

static bool InsnIsBranch(const cs_insn* pInsn)
{
    bool bIsBranch = pInsn->id == ARM_INS_B || pInsn->id == ARM_INS_BX;
    bool bIsBranchLink = pInsn->id == ARM_INS_BL || pInsn->id == ARM_INS_BLX;
    bool bIsBranchAny = bIsBranch || bIsBranchLink;

    return bIsBranchAny;
}

static bool InsnHasCondition(const cs_insn* pInsn)
{
    return pInsn->detail->arm.cc != ARMCC_AL;
}


bool ARM32CapstoneUtility::InsnHasRegister(const cs_insn* pIns, uint16_t reg) const
{
    return mBaseUtility.InsnHasRegister(pIns->detail->arm, reg);
}

int64_t ARM32CapstoneUtility::InsnGetImmByIndex(const cs_insn* pIns, size_t index) const
{
    std::vector<int64_t> allImms = mBaseUtility.InsnGetAllImms<ARM_REG_INVALID>(pIns->detail->arm);

    if (index < allImms.size())
        return allImms[index];

    throw std::runtime_error(std::format("'{} {}' no imm at index '{}' ", pIns->mnemonic, pIns->op_str, index));

    return 0;
}

uint16_t ARM32CapstoneUtility::InsnGetPseudoDestReg(const cs_insn* pIns) const
{
    std::vector<uint16_t> allRegs = mBaseUtility.InsnGetAllRegisters(pIns->detail->arm);

    if (!allRegs.empty())
        return allRegs[0];  // Fair Assumption, 
    // usual ARM the syntax 
    // often follows a pattern XXX Rd, Rn, Operand2 ...
    // where Rd (first register operand) is the destination

    throw std::runtime_error(std::format("'{} {}' no register operands found", pIns->mnemonic, pIns->op_str));

    return 0;
}

bool ARM32CapstoneUtility::InsnHasCondition(const cs_insn* pInsn) const
{
    return ::InsnHasCondition(pInsn);
}

bool ARM32CapstoneUtility::InsnIsBranch(const cs_insn* pInsn) const
{
    return ::InsnIsBranch(pInsn);
}

const void* ARM32PCCompute(ICapstone* capstone, const void* at, uint64_t disp)
{
    ARM32Capstone* armCstone = (ARM32Capstone*)capstone; // we need access to is thumb ...
    auto pipelineDisp = armCstone->mbIsThumb ? 4ull : 8ull;
    auto finalMask = armCstone->mbIsThumb ? ~0x3ull : ~0x0ull;
   
    return (char*)(((uint64_t)at + pipelineDisp + disp) & finalMask);
}

const void* ARM32LDRPCDispResolve(ICapstone* capstone, const void* at, bool bDerref)
{
    CsInsn insn = capstone->DisassembleOne(at);

    if (insn->id != ARM_INS_LDR)
        throw std::runtime_error(std::format("LEAPCDisp Follow '{} {}': unexpected instruction", insn->mnemonic, insn->op_str));

    auto& memOp = insn->detail->arm.operands[1];

    if (memOp.type != CS_OP_MEM ||
        memOp.mem.index != ARM_REG_INVALID)
        throw std::runtime_error(std::format("LEAPCDisp Follow '{} {}': unexpected instruction format", insn->mnemonic, insn->op_str));

    const void* dstAddr = ARM32PCCompute(capstone, at, memOp.mem.disp);

    if (bDerref)
        return (const void*)(*(uint32_t*)dstAddr);

    return dstAddr;
}

const void* ARM32FarPcRelLEATryResolve(ICapstone* capstone, const void* at, bool bDerref)
{
    size_t pcRelDisp = size_t(ARM32LDRPCDispResolve(capstone, at, true));
    CsInsn ldrInsn = capstone->DisassembleOne((const void*)at);
    ICapstoneUtility* utility = capstone->getUtility();
    auto Rd = utility->InsnGetPseudoDestReg(&ldrInsn.mInsn);
    bool bFound = false;
    const void* res{};
    const void* nextInsnStart = (const char*)at + ldrInsn->size;

    capstone->InsnForEach(nextInsnStart, [&](const CsInsn& insn) {
        if (!utility->InsnHasRegister(&insn.mInsn, Rd))
            return true;

        if (!utility->InsnHasRegister(&insn.mInsn, ARM_REG_PC))
            return true;

        const void* dstAddr = ARM32PCCompute(capstone, (void*)insn->address, pcRelDisp);

        res = bDerref ? (const void*)(*(uint32_t*)dstAddr) : dstAddr;

        return !(bFound = true);
        }, (uint64_t)nextInsnStart);

    if (!bFound)
        throw std::runtime_error(std::format("'{} {}' not found finalizer", ldrInsn->mnemonic, ldrInsn->op_str));

    return res;
}
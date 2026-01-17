#include <CStone/Arch/AArch64/Utility.h>
#include <stdexcept>
#include <format>

static bool StaticInsnIsBranch(const cs_insn* pInsn)
{
    switch (pInsn->id) {
    case AARCH64_INS_B:
    case AARCH64_INS_BL:
    case AARCH64_INS_BR:
    case AARCH64_INS_BLR:
    case AARCH64_INS_CBZ:
    case AARCH64_INS_CBNZ:
    case AARCH64_INS_TBZ:
    case AARCH64_INS_TBNZ:
        return true;
    default:
        return false;
    }
}

static bool StaticInsnHasCondition(const cs_insn* pInsn)
{
    return pInsn->detail->aarch64.cc != AArch64CC_Invalid;
}

bool AArch64CapstoneUtility::InsnHasRegister(const cs_insn* pIns, uint16_t reg) const
{
    return mBaseUtility.InsnHasRegister(pIns->detail->aarch64, reg);
}

int64_t AArch64CapstoneUtility::InsnGetImmByIndex(const cs_insn* pIns, size_t index) const
{
    std::vector<int64_t> allImms = mBaseUtility.InsnGetAllImms<AARCH64_REG_INVALID>(pIns->detail->aarch64);

    if (index < allImms.size())
        return allImms[index];

    throw std::runtime_error(std::format("'{} {}' no imm at index '{}' ", pIns->mnemonic, pIns->op_str, index));
}

uint16_t AArch64CapstoneUtility::InsnGetPseudoDestReg(const cs_insn* pIns) const
{
    std::vector<uint16_t> allRegs = mBaseUtility.InsnGetAllRegisters(pIns->detail->aarch64);

    if (!allRegs.empty())
        return allRegs[0]; 

    throw std::runtime_error(std::format("'{} {}' no register operands found", pIns->mnemonic, pIns->op_str));
}

bool AArch64CapstoneUtility::InsnIsBranch(const cs_insn* pInsn) const
{
    return StaticInsnIsBranch(pInsn);
}

bool AArch64CapstoneUtility::InsnHasCondition(const cs_insn* pInsn) const
{
    return StaticInsnHasCondition(pInsn);
}

const void* AArch64PCCompute(ICapstone* capstone, const void* at, uint64_t disp)
{
    return (const char*)at + disp;
}

const void* AArch64LDRPCDispResolve(ICapstone* capstone, const void* at, bool bDerref)
{
    CsInsn insn = capstone->DisassembleOne(at);

    if (insn->id != AARCH64_INS_LDR && insn->id != AARCH64_INS_LDRSW && insn->id != AARCH64_INS_PRFM)
        throw std::runtime_error(std::format("AArch64LDRPCDispResolve '{} {}': unexpected instruction", insn->mnemonic, insn->op_str));

    auto& ops = insn->detail->aarch64.operands;

    if (insn->detail->aarch64.op_count < 2)
        throw std::runtime_error(std::format("AArch64LDRPCDispResolve '{} {}': too few operands", insn->mnemonic, insn->op_str));

    int64_t offset = 0;
    bool bHandled = false;

    if (ops[1].type == AARCH64_OP_IMM) {
        offset = ops[1].imm;
        bHandled = true;
    }

    if (!bHandled && ops[1].type == AARCH64_OP_MEM) {
        offset = ops[1].mem.disp;
        bHandled = true;
    }

    if (!bHandled)
        throw std::runtime_error(std::format("AArch64LDRPCDispResolve '{} {}': unexpected operand types", insn->mnemonic, insn->op_str));

    const void* dstAddr = AArch64PCCompute(capstone, at, offset);

    if (!bDerref)
        return dstAddr;

    if (insn->id == AARCH64_INS_LDRSW)
        return (const void*)(intptr_t)(*(int32_t*)dstAddr);

    return (const void*)(*(uintptr_t*)dstAddr);
}

const void* AArch64FarPcRelLEATryResolve(ICapstone* capstone, const void* at, bool bDerref)
{
    CsInsn adrpInsn = capstone->DisassembleOne(at, (uint64_t)at);
    if (adrpInsn->id != AARCH64_INS_ADRP || adrpInsn->detail->aarch64.op_count < 2)
        return nullptr;

    aarch64_reg Rd = adrpInsn->detail->aarch64.operands[0].reg;
    uint64_t PageBase = adrpInsn->detail->aarch64.operands[1].imm;

    ICapstoneUtility* utility = capstone->getUtility();
    bool bFound = false;
    const void* res{};
    const void* nextInsnStart = (const char*)at + adrpInsn->size;

    capstone->InsnForEach(nextInsnStart, [&](const CsInsn& insn) {
        if (!utility->InsnHasRegister(&insn.mInsn, Rd))
            return true;

        // Pattern 1: ADD Rd, Rd, #imm
        if (insn->id == AARCH64_INS_ADD) {
            if (insn->detail->aarch64.op_count >= 3 &&
                insn->detail->aarch64.operands[1].reg == Rd &&
                insn->detail->aarch64.operands[2].type == AARCH64_OP_IMM) {

                res = (const void*)(PageBase + insn->detail->aarch64.operands[2].imm);
                return !(bFound = true);
            }
        }

        // Pattern 2: LDR/STR variants
        bool bIsLoadStore = false;
        switch (insn->id)
        {
        case AARCH64_INS_LDR: case AARCH64_INS_STR:
        case AARCH64_INS_LDRB: case AARCH64_INS_STRB:
        case AARCH64_INS_LDRH: case AARCH64_INS_STRH:
        case AARCH64_INS_LDRSB: case AARCH64_INS_LDRSH: case AARCH64_INS_LDRSW:
            bIsLoadStore = true;
            break;
        }

        if (bIsLoadStore) {
            if (insn->detail->aarch64.op_count >= 2 &&
                insn->detail->aarch64.operands[1].type == AARCH64_OP_MEM &&
                insn->detail->aarch64.operands[1].mem.base == Rd) {

                res = (const void*)(PageBase + insn->detail->aarch64.operands[1].mem.disp);
                return !(bFound = true);
            }
        }

        // Stop if Rd is overwritten
        bool bOverwritten = false;
        // Implicit writes
        if (insn->detail->regs_write_count > 0) {
            for (int j = 0; j < insn->detail->regs_write_count; j++) {
                if (insn->detail->regs_write[j] == Rd) { bOverwritten = true; break; }
            }
        }
        // Explicit writes
        if (!bOverwritten && insn->detail->aarch64.op_count > 0 &&
            insn->detail->aarch64.operands[0].type == AARCH64_OP_REG &&
            insn->detail->aarch64.operands[0].reg == Rd &&
            (insn->detail->aarch64.operands[0].access & CS_AC_WRITE)) {
            bOverwritten = true;
        }

        if (bOverwritten) return false;

        return true;
    }, (uint64_t)nextInsnStart);

    return res;
}

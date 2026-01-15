#include <CStone/Arch/ARM/64/Utility.h>
#include <fmt/core.h>
#include <stdexcept>

static bool StaticInsnIsBranch(const cs_insn* pInsn)
{
    switch (pInsn->id) {
    case ARM64_INS_B:
    case ARM64_INS_BL:
    case ARM64_INS_BR:
    case ARM64_INS_BLR:
    case ARM64_INS_CBZ:
    case ARM64_INS_CBNZ:
    case ARM64_INS_TBZ:
    case ARM64_INS_TBNZ:
        return true;
    default:
        return false;
    }
}

static bool StaticInsnHasCondition(const cs_insn* pInsn)
{
    return pInsn->detail->arm64.cc != ARM64_CC_INVALID;
}

bool ARM64CapstoneUtility::InsnHasRegister(const cs_insn* pIns, uint16_t reg) const
{
    return mBaseUtility.InsnHasRegister(pIns->detail->arm64, reg);
}

int64_t ARM64CapstoneUtility::InsnGetImmByIndex(const cs_insn* pIns, size_t index) const
{
    std::vector<int64_t> allImms = mBaseUtility.InsnGetAllImms<ARM64_REG_INVALID>(pIns->detail->arm64);

    if (index < allImms.size())
        return allImms[index];

    throw std::runtime_error(fmt::format("'{} {}' no imm at index '{}' ", pIns->mnemonic, pIns->op_str, index));
}

uint16_t ARM64CapstoneUtility::InsnGetPseudoDestReg(const cs_insn* pIns) const
{
    std::vector<uint16_t> allRegs = mBaseUtility.InsnGetAllRegisters(pIns->detail->arm64);

    if (!allRegs.empty())
        return allRegs[0]; 

    throw std::runtime_error(fmt::format("'{} {}' no register operands found", pIns->mnemonic, pIns->op_str));
}

bool ARM64CapstoneUtility::InsnIsBranch(const cs_insn* pInsn) const
{
    return StaticInsnIsBranch(pInsn);
}

bool ARM64CapstoneUtility::InsnHasCondition(const cs_insn* pInsn) const
{
    return StaticInsnHasCondition(pInsn);
}

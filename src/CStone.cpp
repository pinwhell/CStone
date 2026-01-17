#include <CStone/CStone.h>
#include <CStone/Factory.h>
#include <CStone/Provider.h>
#include <format>

Capstone::Capstone(cs_arch arch, cs_mode mode, bool bDetailedDisasm)
    : mhCapstone(0)
{
    if (cs_err err = cs_open(arch, mode, &mhCapstone)) // != CS_ERR_OK(0)
        throw std::runtime_error(std::format("Capstone Arch:{},Mode:{} {}", (int)arch, (int)mode, cs_strerror(err)));

    if (bDetailedDisasm)
        cs_option(mhCapstone, CS_OPT_DETAIL, CS_OPT_ON);
}

Capstone::~Capstone()
{
    if (mhCapstone)
        cs_close(&mhCapstone);
}

CapstoneDismHandle Capstone::Disassemble(const void* _start, size_t nBytes, uint64_t pcAddr)
{
    const uint8_t* start = (const uint8_t*)_start;
    cs_insn* startIns = nullptr;

    size_t nInstCount = cs_disasm(mhCapstone, start, nBytes, pcAddr, 0, &startIns);

    if (nInstCount < 1)
        throw DismFailedException(std::format("Addr:{},nBytes:{} disassembly failed", (void*)start, nBytes));

    return CapstoneDismHandle(startIns, nInstCount);
}

CsInsn Capstone::DisassembleOne(const void* start, uint64_t pcAddr)
{
    CsInsn r{};

    InsnForEach(start, [&r](const CsInsn& insn) {
        r = insn;
        return false;
        }, pcAddr);

    if (r->address != pcAddr)
        throw DismFailedException(std::format("Addr:{} disassembly failed", (void*)(start)));

    return r;
}

void Capstone::InsnForEach(const void* _start, std::function<bool(const CsInsn& insn)> callback, uint64_t pcAddr, size_t buffSize)
{
    const uint8_t* start = (const uint8_t*)_start;
    const uint8_t* curr = start;
    size_t currSize = buffSize;
    uint64_t currPcAddr = pcAddr;
    CsInsn currInsn{};

    while (cs_disasm_iter(mhCapstone, &curr, &currSize, &currPcAddr, &currInsn.mInsn) && callback(currInsn))
        ;
}

ICapstoneUtility* Capstone::getUtility() {
    return nullptr;
}

ICapstoneHeuristic* Capstone::getHeuristic()
{
    return nullptr;
}

DismFailedException::DismFailedException(const std::string& what)
    : std::runtime_error(what)
{}
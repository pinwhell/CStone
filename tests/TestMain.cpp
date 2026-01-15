#include <iostream>
#include <vector>
#include <cassert>
#include <exception>
#include <CStone/Factory.h>
#include <CStone/Arch/ARM/32/Capstone.h>
#include <CStone/Arch/ARM/64/Capstone.h>

#define TEST_CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "[-] Test Failed: " << #cond << " at line " << __LINE__ << std::endl; \
            std::exit(1); \
        } \
    } while(0)

#define TEST_LOG(msg) std::cout << "[+] " << msg << std::endl

void TestARM32() {
    TEST_LOG("Running ARM32 Tests...");
    
    CapstoneFactory factory(ECapstoneArchMode::ARM32_THUMB);
    auto capstone = factory.CreateInstance();

    // Thumb2: PUSH {R4, LR}
    uint8_t prologueBytes[] = { 0x10, 0xB5 }; 
    auto prologueInsn = capstone->DisassembleOne(prologueBytes);
    
    TEST_CHECK(prologueInsn->id == ARM_INS_PUSH);
    TEST_CHECK(capstone->getHeuristic()->InsnIsProcedureEntry(&prologueInsn.mInsn));
    TEST_LOG("  Verified: Procedure Entry (PUSH {R4, LR})");

    // Thumb2: POP {R4, PC}
    uint8_t epilogueBytes[] = { 0x10, 0xBD };
    auto epilogueInsn = capstone->DisassembleOne(epilogueBytes);

    TEST_CHECK(epilogueInsn->id == ARM_INS_POP);
    TEST_CHECK(capstone->getHeuristic()->InsnIsProcedureExit(&epilogueInsn.mInsn));
    TEST_LOG("  Verified: Procedure Exit (POP {R4, PC})");

    // Thumb2: B.W label
    uint8_t branchBytes[] = { 0x00, 0xF0, 0x00, 0xB8 }; // B.W +0
    auto branchInsn = capstone->DisassembleOne(branchBytes);
    
    TEST_CHECK(capstone->getUtility()->InsnIsBranch(&branchInsn.mInsn));
    TEST_LOG("  Verified: Branch (B.W)");
}

void TestARM64() {
    TEST_LOG("Running ARM64 Tests...");

    CapstoneFactory factory(ECapstoneArchMode::AARCH64_ARM);
    auto capstone = factory.CreateInstance();

    // ARM64: STP X29, X30, [SP, #-16]!
    uint8_t prologueBytes[] = { 0xFD, 0x7B, 0xBF, 0xA9 };
    auto prologueInsn = capstone->DisassembleOne(prologueBytes);

    TEST_CHECK(prologueInsn->id == ARM64_INS_STP);
    TEST_CHECK(capstone->getHeuristic()->InsnIsProcedureEntry(&prologueInsn.mInsn));
    TEST_LOG("  Verified: Procedure Entry (STP x29, x30, ...)");

    // ARM64: RET (C0 03 5F D6)
    uint8_t epilogueBytes[] = { 0xC0, 0x03, 0x5F, 0xD6 };
    auto epilogueInsn = capstone->DisassembleOne(epilogueBytes);

    TEST_CHECK(epilogueInsn->id == ARM64_INS_RET);
    TEST_CHECK(capstone->getHeuristic()->InsnIsProcedureExit(&epilogueInsn.mInsn));
    TEST_CHECK(capstone->getUtility()->InsnIsBranch(&epilogueInsn.mInsn) == false); // Utility doesn't classify RET as branch currently
    TEST_LOG("  Verified: Procedure Exit (RET)");

    // ARM64: B label (01 00 00 14)
    uint8_t branchBytes[] = { 0x01, 0x00, 0x00, 0x14 };
    auto branchInsn = capstone->DisassembleOne(branchBytes);
    
    TEST_CHECK(branchInsn->id == ARM64_INS_B);
    TEST_CHECK(capstone->getUtility()->InsnIsBranch(&branchInsn.mInsn));
    TEST_LOG("  Verified: Branch (B)");
}

int main() {
    try {
        TestARM32();
        TestARM64();
        TEST_LOG("All tests passed successfully.");
    } catch (const std::exception& e) {
        std::cerr << "[-] Exception caught: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

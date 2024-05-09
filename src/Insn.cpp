#include <CStone/Insn.h>

CsInsn::CsInsn()
{
    memset(&mInsn, 0x0, sizeof(mInsn));
    memset(&mDetail, 0x0, sizeof(mDetail));
    mInsn.detail = &mDetail;
}

const cs_insn* CsInsn::operator->() const
{
    return &mInsn;
}
#include <CStone/DismHandle.h>

CapstoneDismHandle::CapstoneDismHandle(cs_insn* pFirst, size_t count)
    : mpFirst(pFirst)
    , mpEnd(pFirst + count)
    , mCount(count)
{}

CapstoneDismHandle::~CapstoneDismHandle()
{
    if (!*this)
        return;

    cs_free(mpFirst, mCount);
}

CapstoneDismHandle::operator bool()
{
    return mpFirst != nullptr && mCount > 0;
}
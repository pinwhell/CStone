#pragma once

#include <CStone/Provider.h>

CapstoneConcurrentInstanceProvider::CapstoneConcurrentInstanceProvider(ICapstoneFactory* defFactory)
    : mDefaultFactory(defFactory)
{}

ICapstone* CapstoneConcurrentInstanceProvider::GetInstance(bool bDetailedInstuction, ICapstoneFactory* _factory)
{
    std::thread::id this_id = std::this_thread::get_id();

    std::unique_lock<std::mutex> lock(mMutex);
    while (mInstances.find(this_id) == mInstances.end()) {
        ICapstoneFactory* factory = _factory ? _factory : mDefaultFactory;

        if (factory == nullptr)
            return nullptr;

        // This thread doesn't have a Capstone object yet, so create one.
        mInstances[this_id] = factory->CreateCapstoneInstance(bDetailedInstuction);
    }

    return mInstances[this_id].get();
}

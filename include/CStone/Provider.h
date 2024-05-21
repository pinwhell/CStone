#pragma once

#include <mutex>
#include <unordered_map>
#include <thread>

#include <CStone/ICapstone.h>
#include <CStone/IProvider.h>

class CapstoneConcurrentProvider : public ICapstoneProvider {
public:
    CapstoneConcurrentProvider(ICapstoneFactory* defFactory = nullptr);

    ICapstone* GetInstance(bool bDetailedInstuction = true, ICapstoneFactory* _factory = nullptr) override;

    std::mutex mMutex;
    std::unordered_map<std::thread::id, std::unique_ptr<ICapstone>> mInstances;
    ICapstoneFactory* mDefaultFactory;
};
#pragma once

#include "Actor.h"
#include "ActorHandle.h"
#include "Native.h"

class HeroSystem {
    void** _vftable;
    char _0x8[0x8];
    int _0x10;
    ActorHandle _hero_handle;

public:
    inline Actor* GetHero() {
        return GetActor(&_hero_handle);
    }
};

MSM2SCRIPTSDK_API HeroSystem* GetHeroSystem();

DECLARE_NATIVE(HeroSystem,
)

#pragma once
#include "OgreSingleton.h"
#include "SoundObject.h"
#include <memory>
#include <list>

class SoundManager : public Ogre::Singleton<SoundManager>{
    public:
        SoundManager(SoundManager const&) = delete;
        void operator=(SoundManager const&) = delete;
        static bool initialize();
        static void destroy();
        static SoundManager& getSingleton();
        static SoundManager* getSingletonPtr();
        static void addSoundObject(const char* filePath, const char* name, bool isEffect, Effect effect=Effect::NONE);
        static std::shared_ptr<SoundObject>& getSoundObjectPtr(const char* name);
        static void updateListenerPosition(float x, float y, float z);
    private:
        static std::list<std::shared_ptr<SoundObject>> soundObjects;
        SoundManager();
        ~SoundManager();
        bool _initialize();
        void _destroy();
};
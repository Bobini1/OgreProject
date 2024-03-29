#include "SoundManager.h"

template<> SoundManager *Ogre::Singleton<SoundManager>::msSingleton = 0;

std::list<std::shared_ptr<SoundObject>> SoundManager::soundObjects;

SoundManager::SoundManager() = default;
SoundManager::~SoundManager() = default;

bool SoundManager::initialize(){
    if(msSingleton == nullptr){
        msSingleton = OGRE_NEW SoundManager;
    }
    msSingleton->_initialize();
    return true;
}

void SoundManager::destroy() {
    if (msSingleton != nullptr) {
        msSingleton->_destroy();

        OGRE_DELETE msSingleton;
        msSingleton = nullptr;
    }
}

SoundManager& SoundManager::getSingleton() {
    assert(msSingleton);
    return (*msSingleton);
}

SoundManager* SoundManager::getSingletonPtr() {
    return msSingleton;
}

void SoundManager::addSoundObject(const char* filePath, const char* name, bool isEffect, Effect effect){
    soundObjects.push_back(std::make_shared<SoundObject>(filePath, name, isEffect, effect));
}

std::shared_ptr<SoundObject>& SoundManager::getSoundObjectPtr(const char* name){
    for(auto& soundObject: soundObjects){
        if(soundObject->getName() == name){
            return soundObject;
        }
    }
}

void SoundManager::updateListenerPosition(float x, float y, float z){
    alListener3f(AL_POSITION, x, y, z);
}

bool SoundManager::_initialize() {
    OGRE_LOCK_AUTO_MUTEX;
    // Do here initialization stuff if needed

    return true;
}

void SoundManager::_destroy() {
    OGRE_LOCK_AUTO_MUTEX;
    // Do here destruction stuff if needed
}
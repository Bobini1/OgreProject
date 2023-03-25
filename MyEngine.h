//
// Created by bobini on 15.01.23.
//

#ifndef LABSOGRE_MYENGINE_H
#define LABSOGRE_MYENGINE_H

#include "PlayerAvatar.h"
#include "RoamingCamera.h"
#include "MovingObject.h"

class MyEngine : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    MyEngine();
    void setup() override;
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

    bool frameStarted(const Ogre::FrameEvent &evt) override;

private:
    void setupSceneManager();
    void setupCamera();
    void populateScene();
    Ogre::SceneManager* scene_manager_{};
    Ogre::Root* root_{};

    std::unique_ptr<PlayerAvatar> player_{};
    std::unique_ptr<RoamingCamera> roaming_camera_{};
    std::unique_ptr<MovingObject> moving_object_{};
};


#endif //LABSOGRE_MYENGINE_H


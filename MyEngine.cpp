//
// Created by bobini on 15.01.23.
//

#include "MyEngine.h"
#include "PickupManager.h"
#include "SoundManager.h"

MyEngine::MyEngine() : OgreBites::ApplicationContext("T-637-GEDE Lab 3")
{
}

void MyEngine::setup() {
    OgreBites::ApplicationContext::setup();
    addInputListener(this);
    setupSceneManager();
    setupCamera();
    populateScene();
}

void MyEngine::setupSceneManager() {
    // Get pointers to root and create scene manager
    root_ = getRoot();
    scene_manager_ = root_->createSceneManager();

    // Register scene with the RTSS
    auto* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shaderGenerator->addSceneManager(scene_manager_);
}

bool MyEngine::keyPressed(const OgreBites::KeyboardEvent &evt) {
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    if (evt.keysym.sym == OgreBites::SDLK_LSHIFT)
    {
        if(moving_object_->getSoundEffect()->isRunning()){
            moving_object_->getSoundEffect()->pause();
        } else{
            moving_object_->getSoundEffect()->run();
        }
    }
    return true;
}

void MyEngine::setupCamera() {
    roaming_camera_ = std::make_unique<RoamingCamera>(scene_manager_, getRenderWindow(), Vector3(0, 0, 50));
}

void MyEngine::populateScene() {
    // Add Sinbad Model
    player_ = std::make_unique<PlayerAvatar>(scene_manager_, "Sinbad.mesh");
    PickupManager::initialize(scene_manager_, player_->entity_node_);
    PickupManager::addPickupObject("Suzanne.mesh");
    // Add music
    SoundManager::initialize();
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhNONE", true, Effect::NONE);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhEAXREVERB", true, Effect::EAXREVERB);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhREVERB", true, Effect::REVERB);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhCHORUS", true, Effect::CHORUS);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhDISTORTION", true, Effect::DISTORTION);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhECHO", true, Effect::ECHO);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhFLANGER", true, Effect::FLANGER);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhFREQUENCY_SHIFTER", true, Effect::FREQUENCY_SHIFTER);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhVOCAL_MORPHER", true, Effect::VOCAL_MORPHER);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhPITCH_SHIFTER", true, Effect::PITCH_SHIFTER);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhRING_MODULATOR", true, Effect::RING_MODULATOR);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhAUTOWAH", true, Effect::AUTOWAH);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhCOMPRESSOR", true, Effect::COMPRESSOR);
    SoundManager::addSoundObject("/home/kajtekk/Music/bruhMONO.mp3", "bruhEQUALIZER", true, Effect::EQUALIZER);
    SoundManager::addSoundObject("/home/kajtekk/Music/coldplay-yellowMONO.mp3", "coldplay-yellow", false);
    // Add object moving in circle
    moving_object_ = std::make_unique<MovingObject>(scene_manager_, "Suzanne.mesh", player_->entity_node_->getPosition(), Vector3(1, 1, 1), Vector3(0, 3, 3));
    // Add Ground
    auto plane = Ogre::Plane(Ogre::Vector3::UNIT_Y, -5);
    auto groundMesh = Ogre::MeshManager::getSingleton()
            .createPlane("MainGround", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                         plane, 1000, 1000, 100, 100, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);

    // build tangent vectors for our mesh, to show the normal texture
    // This will make the floor tiles look like they have depth, even thought they are flat
    unsigned short src, dest;
    if (!groundMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
    {
        groundMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
    }

    Ogre::Entity* groundEntity = scene_manager_->createEntity("LightPlaneEntity", "MainGround");
    scene_manager_->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    groundEntity->setCastShadows(false);

    // Specify the material that is going to be used to render the floor tiles
    groundEntity->setMaterialName("Custom/BrickTiles");

    // Set Shadow Technique
    scene_manager_->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

    // Add Directional Light
    scene_manager_->setAmbientLight(Ogre::ColourValue(0.2f, 0.5f, 0.8f));
    auto* directionalLight = scene_manager_->createLight("DirectionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(1, 1, 1);
    directionalLight->setSpecularColour(.4, .4, .4);
    auto* directionalLightNode = scene_manager_->getRootSceneNode()->createChildSceneNode();
    directionalLightNode->attachObject(directionalLight);
    directionalLightNode->setDirection(Ogre::Vector3(0, -1, -1));

}

bool MyEngine::frameStarted(const Ogre::FrameEvent &evt)
{
    // Main "game loop" of the application
    // Let parent handle this callback as well
    ApplicationContext::frameStarted(evt);
    // Store the time that has passed since last time we got the callback
    const Ogre::Real delta_time = evt.timeSinceLastFrame;
    // Check what keys of the keyboard are being pressed
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    // Update any subsystems
    if (player_ != nullptr) player_->Update(delta_time, state);
    if (roaming_camera_ != nullptr) roaming_camera_->update(delta_time, state);
    if (moving_object_ != nullptr) moving_object_->update(delta_time);
    // Update all the managed pickup objects
    PickupManager::Update(delta_time, state);

    return true;
}
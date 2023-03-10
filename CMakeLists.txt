cmake_minimum_required(VERSION 3.25)
project(OgreProject)

add_executable(LabsOgre main.cpp MyEngine.cpp MyEngine.h PlayerAvatar.cpp PlayerAvatar.h RoamingCamera.cpp RoamingCamera.h PickupManager.cpp PickupManager.h IPickupObject.h IPickupEffect.h IPickupObject.h SwirlEffect.cpp SwirlEffect.h PickupObject.h PickupObject.cpp InputManager.h InputManager.cpp ParticleEffect.h ParticleEffect.cpp sounds/FFmpegOpenAlSound.h sounds/FFmpegOpenAlSound.cpp sounds/FFmpegOpenAlSound.h sounds/FFmpegOpenAlSound.cpp SoundEffect.h SoundEffect.cpp)

find_package(OGRE REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenAL REQUIRED)
find_package(FFmpeg REQUIRED)

target_link_libraries(LabsOgre PUBLIC OgreMain OgreBites OgreMeshLodGenerator OgreOverlay OgrePaging OgreProperty OgreTerrain OgreVolume OgreRTShaderSystem SDL2::SDL2 SDL2::SDL2main OpenAL::OpenAL ffmpeg::ffmpeg)

target_precompile_headers(LabsOgre PUBLIC <OgreRTShaderSystem.h> <Ogre.h> <OgreApplicationContext.h> <OgreInput.h>
        <SDL.h> <iostream> <OgreMath.h>)

configure_file(plugins.cfg.in ${CMAKE_BINARY_DIR}/plugins.cfg)
configure_file(resources.cfg.in ${CMAKE_BINARY_DIR}/resources.cfg)

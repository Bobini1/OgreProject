//
// Created by bobini on 19.01.23.
//

#ifndef LABSOGRE_PLAYERAVATAR_H
#define LABSOGRE_PLAYERAVATAR_H


class PlayerAvatar {
public:
    PlayerAvatar(Ogre::SceneManager* scene_manager, const Ogre::String& mesh_file_name);
    void Update(Ogre::Real delta_time, const Uint8* state);
    void Update(float delta_time, Ogre::Vector2 camera_direction, Ogre::Vector2 character_movement);

    Ogre::SceneNode* entity_node_{};
private:
    void Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time);
    void SetIdleAnimationLoop();
    void SetRunAnimationLoop();
    void StopAnimationLoop() const;
    void StartAnimationLoop() const;
    static Ogre::Radian GetRotation(const Ogre::Vector3& vec);
    Ogre::Real rotation_ = 0.0;
    Ogre::Real rotation_speed_ = 5.0f;
    Ogre::Real walking_speed_ = 10.0f;

    Ogre::SceneManager* scene_manager_{};
    Ogre::Entity* entity_{};
    Ogre::AnimationState* animation_state_base_{};
    Ogre::AnimationState* animation_state_top_{};
};

#endif //LABSOGRE_PLAYERAVATAR_H

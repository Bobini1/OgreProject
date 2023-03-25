#pragma once
#include "SoundManager.h"
#include "SoundEffect.h"

using namespace Ogre;
using namespace OgreBites;

class MovingObject {
    public:
        MovingObject(SceneManager* scene_manager, const char* mesh_file_name, Vector3
            position, Vector3 scale, Vector3 cylindrical_velocity_vector, float radius=30.0, float angle=0.0, float angular_speed=1.0);
        ~MovingObject();

        SceneNode* getSceneNode() const;
        SoundEffect* getSoundEffect() { return sound_effect_; };
        Entity* getEntity() const;
        void update(float delta_time);
    private:
        SceneManager* scene_manager_;
        SceneNode* entity_node_;
        Entity* entity_;
        SoundEffect* sound_effect_;
        float radius_;
        float angle_;
        float angular_speed_;
        // Vector3 center_pos_;
        // Vector3 cylindrical_velocity_vector_;
	    // Vector3 current_cylindrical_pos_;
        // Vector3 getCartesian() const;
};
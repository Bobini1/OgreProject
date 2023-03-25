#include "MovingObject.h"
#include "OgreMath.h"

MovingObject::MovingObject(SceneManager *scene_manager, const char *mesh_file_name,
                           Vector3 position, Vector3 scale, Vector3 cylindrical_velocity_vector, float radius, float angle, float angular_speed)
    : scene_manager_(scene_manager), entity_(scene_manager_->createEntity(mesh_file_name)),
      entity_node_(scene_manager_->getRootSceneNode()->createChildSceneNode()), radius_(radius), angle_(angle), angular_speed_(angular_speed) {
    entity_node_->attachObject(entity_);
    entity_node_->setPosition(position);
    entity_node_->setScale(scale);
    entity_->setCastShadows(true);
    sound_effect_ = new SoundEffect(sounds::FFmpegOpenALSound(SoundManager::getSoundObjectPtr("coldplay-yellow"), entity_node_->getPosition().x, entity_node_->getPosition().y, entity_node_->getPosition().z));
    sound_effect_->run();
    // center_pos_ = entity_node_->getPosition();
    // cylindrical_velocity_vector_ = cylindrical_velocity_vector;
    // current_cylindrical_pos_ = {0,0,0};
}

// Vector3 MovingObject::getCartesian() const {
//     return {current_cylindrical_pos_.y * cos(current_cylindrical_pos_.z),
//             current_cylindrical_pos_.x,
//             current_cylindrical_pos_.y * sin(current_cylindrical_pos_.z)};
// }

SceneNode *MovingObject::getSceneNode() const {
    return entity_node_;
}

Entity *MovingObject::getEntity() const {
    return entity_;
}

void MovingObject::update(float delta_time){
    //current_cylindrical_pos_ = current_cylindrical_pos_ + delta_time * cylindrical_velocity_vector_;
    //entity_node_->setPosition(getCartesian() + center_pos_);
    float x = radius_ * cos(angle_);
    float z = radius_ * sin(angle_);
    entity_node_->setPosition(Vector3(x, 0.0, z));
    angle_ += delta_time * angular_speed_;
    if(angle_ >= Ogre::Math::PI * 2.0) angle_ = 0.0;
    sound_effect_->updatePosition(x, 0.0, z);
}

MovingObject::~MovingObject(){
    delete sound_effect_;
}
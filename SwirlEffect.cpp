//
// Created by bobini on 26.01.23.
//

#include "SwirlEffect.h"

Ogre::Vector3 SwirlEffect::getCartesian() const {
    return {current_cylindrical_pos_.y * cos(current_cylindrical_pos_.z),
            current_cylindrical_pos_.x,
            current_cylindrical_pos_.y * sin(current_cylindrical_pos_.z)};
}

SwirlEffect::SwirlEffect(SceneNode *scene_node, Ogre::Vector3 cylindrical_velocity_vector) {
    scene_node_ = scene_node;
    center_pos_ = scene_node_->getPosition();
    cylindrical_velocity_vector_ = cylindrical_velocity_vector;
    start_cylindrical_pos_ = {0, 0, 0};
    current_cylindrical_pos_ = start_cylindrical_pos_;
    end_cylindrical_pos_ = {10, 1, 0};
    effect_running_ = false;
    effect_finished_ = false;
}

Ogre::Vector3 SwirlEffect::getCylindrical() const {
    return current_cylindrical_pos_;
}

void SwirlEffect::run() {
    effect_running_ = true;
    effect_finished_ = false;
}

void SwirlEffect::update(float delta_time) {
    if (effect_running_)
    {

        if (current_cylindrical_pos_.x >= end_cylindrical_pos_.x)
        {
            effect_running_ = false;
            effect_finished_ = true;
        }
        else
        {
            current_cylindrical_pos_ = current_cylindrical_pos_ + delta_time * 0.5 * cylindrical_velocity_vector_;
            scene_node_->setPosition(getCartesian() + center_pos_);
        }
    }
}

bool SwirlEffect::isRunning() {
    return effect_running_;
}

bool SwirlEffect::isFinished() {
    return effect_finished_;
}

#include "PickupObject.h"
#include "SoundEffect.h"

PickupObject::PickupObject(SceneManager *scene_manager, const char *mesh_file_name,
                           Vector3 position, Vector3 scale) {
    scene_manager_ = scene_manager;
    entity_ = scene_manager_->createEntity(mesh_file_name);
    entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
    entity_node_->attachObject(entity_);
    entity_node_->setPosition(position);
    entity_node_->setScale(scale);
    entity_->setCastShadows(true);
    pickup_effect_ = nullptr;
    picked_up_ = false;
}

PickupObject::~PickupObject() {
    delete (pickup_effect_);
}

SceneNode *PickupObject::getSceneNode() const {
    return entity_node_;
}

IPickupEffect *PickupObject::getPickupEffect() const {
    return pickup_effect_;
}

Entity *PickupObject::getEntity() const {
    return entity_;
}

bool PickupObject::isPickedUp() {
    return picked_up_;
}

bool PickupObject::collidesWith(SceneNode *other_node, float distance) {
    bool collision = false;
    // TODO: Assign true to "collision" if the pickup object collides with the "other_node"

    Vector3 other_position = other_node->getPosition();

    Vector3 pickup_position = entity_node_->getPosition();

    float distance_between = pickup_position.distance(other_position);

    if (distance_between < distance) {
        collision = true;
    }

    return collision;
}

void PickupObject::runPickupEffect(const char* soundName) {
    // TODO: Instantiate and run the effect here (try velocity (5.0, -5, 50.0)
    float x = entity_node_->getPosition().x;
    float y = entity_node_->getPosition().y;
    float z = entity_node_->getPosition().z;
    std::cout << "x: " << entity_node_->getPosition().x << std::endl;
    std::cout << "y: " << entity_node_->getPosition().y << std::endl;
    std::cout << "z: " << entity_node_->getPosition().z << std::endl;
    pickup_effect_ = new SoundEffect(sounds::FFmpegOpenALSound(SoundManager::getSoundObjectPtr(soundName), x, y, z));
    pickup_effect_->run();

    picked_up_ = true;
}

void PickupObject::update(float delta_time) const {
    if (pickup_effect_ != nullptr) {
        if (pickup_effect_->isRunning()) pickup_effect_->update(delta_time);
    }
}
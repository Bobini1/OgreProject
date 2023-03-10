#include "PickupObject.h"
#include "ParticleEffect.h"
#include "sounds/FFmpegOpenAlSound.h"
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
    picked_up_ = false;
}

SceneNode *PickupObject::getSceneNode() const {
    return entity_node_;
}

std::list<IPickupEffect*> PickupObject::getPickupEffects() const {
    return pickup_effects_;
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

void PickupObject::runPickupEffects()
{
    picked_up_ = true;

    IPickupEffect* particleEffect = new ParticleEffect(scene_manager_, entity_node_);
    pickup_effects_.push_back(particleEffect);
    particleEffect->run();

    IPickupEffect* swirlEffect = new SwirlEffect(entity_node_, Ogre::Vector3(5.0, -5, 50.0));
    pickup_effects_.push_back(swirlEffect);
    swirlEffect->run();

    sounds::FFmpegOpenALSound sound = sounds::FFmpegOpenALSound("/home/bobini/Pulpit/07 - 日溜りの街.flac");
    IPickupEffect* soundsEffect = new SoundEffect(std::move(sound));
    pickup_effects_.push_back(soundsEffect);
    soundsEffect->run();
}

void PickupObject::update(float delta_time) const
{
    for (auto i = pickup_effects_.begin(); i != pickup_effects_.end();)
    {
        IPickupEffect* pickupEffect = *i;
        if(pickupEffect->isRunning())
            pickupEffect->update(delta_time);

        ++i;
    }
}

PickupObject::~PickupObject()
{
    for (auto i = pickup_effects_.begin(); i != pickup_effects_.end();)
    {
        delete(*i);
    }
}

#include "PickupManager.h"

template<> PickupManager *Ogre::Singleton<PickupManager>::msSingleton = 0;

// need to declare the static variables, otherwise we get a link error
SceneManager *PickupManager::scene_manager_;
SceneNode *PickupManager::player_node_;
std::list<IPickupObject *> PickupManager::pickup_objects;

PickupManager::PickupManager() = default;

PickupManager::~PickupManager() = default;

bool PickupManager::initialize(SceneManager *sceneManager, SceneNode *playerNode) {
    if (msSingleton == nullptr) {
        msSingleton = OGRE_NEW PickupManager;
    }
    msSingleton->_initialize(sceneManager, playerNode);
    return true;
}

void PickupManager::destroy() {
    if (msSingleton != nullptr) {
        msSingleton->_destroy();

        OGRE_DELETE msSingleton;
        msSingleton = nullptr;
    }
}

PickupManager &PickupManager::getSingleton() {
    assert(msSingleton);
    return (*msSingleton);
}

PickupManager *PickupManager::getSingletonPtr() {
    return msSingleton;
}

/** This function spawns a new pickup object at a random position near the player */
void PickupManager::addPickupObject(const char *mesh_file_name) {
    random_device rd;                           // obtain a random number from hardware
    mt19937 gen(rd());                          // seed the generator
    uniform_real_distribution<> distr(-50, 50); // define the range

    // Create a new pickup Objhe user is also cautioned that this function only erases the element, and that if the element is itself a pointer, the ect in a random position near the player
    Ogre::Vector3 playerPosition = player_node_->getPosition();
    Real randomXOffset = distr(gen);
    Real randomZOffset = distr(gen);
    Ogre::Vector3 newSpawnPosition = Ogre::Vector3(playerPosition.x + randomXOffset, -3.0f,
                                                   playerPosition.z + randomZOffset);
    auto *pickupObject = new PickupObject(scene_manager_, mesh_file_name, newSpawnPosition);
    pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();


    // Set the color of the object
    String MaterialName = pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();
    MaterialPtr pMaterial = MaterialManager::getSingleton().getByName(MaterialName);

    float r = 255.0f / 255;
    float g = 215.0f / 255;
    float b = 0.0f / 255;

    auto color = ColourValue(r, g, b, 1.0F);

    pMaterial->setDiffuse(color);
    pMaterial->setAmbient(color);
    pMaterial->setSpecular(color);


    // Insert the new Pickup Object in the list of managed objects
    pickup_objects.push_back(pickupObject);
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    // Update all the managed pickup objects, and delete them if they finished the effect
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        bool erased = false;
        IPickupObject* pickupObject = *i;

        pickupObject->update(delta_time);

        // Check for collision with a game object that has not yet been picked up
        if (!pickupObject->isPickedUp() && pickupObject->collidesWith(player_node_, 6.0))
        {
            pickupObject->runPickupEffects();
        }

        if (pickupObject->isPickedUp())
        {
            // check whether all effects are finished. If at least one is running the
            // && operation returns false and we don't dispose of the object
            std::list<IPickupEffect*> effects = pickupObject->getPickupEffects();
            bool allEffectsFinished = true;
            for (auto j = effects.begin(); j != effects.end();)
            {
                IPickupEffect* pickupEffect = *j;
                allEffectsFinished = allEffectsFinished && pickupEffect->isFinished();
                if (!allEffectsFinished) break;
                ++j;
            }

            // If every effect is finished we can dispose of the object
            if (allEffectsFinished)
            {
                pickupObject->getSceneNode()->detachAllObjects();
                scene_manager_->destroyEntity(pickupObject->getEntity());
                scene_manager_->destroySceneNode(pickupObject->getSceneNode());
                i = pickup_objects.erase(i);
                erased = true;
            }
        }

        // Don't increase the counter if we have deleted an item, otherwise it throws an error
        if (!erased) ++i;
    }
}


bool PickupManager::_initialize(SceneManager *sceneManager, SceneNode *playerNode) {
    OGRE_LOCK_AUTO_MUTEX;
    // Do here initialization stuff if needed
    scene_manager_ = sceneManager;
    player_node_ = playerNode;

    return true;
}

void PickupManager::_destroy() {
    OGRE_LOCK_AUTO_MUTEX;
    // Do here destruction stuff if needed
    OGRE_DELETE scene_manager_;
    scene_manager_ = nullptr;
    OGRE_DELETE player_node_;
    player_node_ = nullptr;
}

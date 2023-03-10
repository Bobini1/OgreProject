//
// Created by bobini on 22.02.23.
//

#ifndef LABSOGRE_INPUTMANAGER_H
#define LABSOGRE_INPUTMANAGER_H


class InputManager {
public:
    InputManager();
    ~InputManager();
    Ogre::Vector2 getCameraMovement() const;
    Ogre::Vector2 getCharacterMovement() const;
    void update();
private:
    _SDL_Joystick* joystick_;
    int player_axis_x_;
    int player_axis_y_;
    float camera_x_;
    float camera_y_;
};


#endif //LABSOGRE_INPUTMANAGER_H

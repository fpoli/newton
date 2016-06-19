//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_APP_H
#define OGRE_APP_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "ogre_framework.h"
#include "json/json.h"
#include "basic.h"
#include "simulator.h"

#include "TextDisplay.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreApp : public OIS::KeyListener
{
private:
    void setupAppScene();
    void updateScene();
    void runApp();

    Ogre::Vector3        init_camera_pos;
    double                init_camera_roll;
    int                    camera_anchor; // -1 = origine
    std::vector<int>    path_anchor; // -1 = origine
    double                fps_goal;
    Simulator*            simulator;
    Ogre::SceneNode*                    pathNode;
    std::vector<Ogre::SceneNode*>        planetNode;
    std::vector<Ogre::MaterialPtr>        planetMaterial;
    std::vector<Ogre::Entity*>            planetEntity;
    std::vector<Ogre::ManualObject*>    pathManualObject;
    std::vector<ObjectTextDisplay*>        planetLabel;


    TextDisplay*    textTime;
    TextDisplay*    textCenter;
    TextDisplay*    textMsg;
    TextDisplay*    textSimulLoop;

    bool    showPath;
    bool    showLabel;
    bool    showInfo;
    bool    m_bShutdown;
    bool    pause_simulation;

public:
    OgreApp(Simulator *_simulator);
    ~OgreApp();

    void startMainLoop();

    void load_configuration(const Json::Value conf);
    void set_camera_anchor(int anchor);
    void set_path_anchor(int anchor);

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
    bool mouseMoved(const OIS::MouseEvent &evt);
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "ogre_app.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

Degree getRoll(const Camera* mCamera) {
    Vector3 world_up = Vector3::UNIT_Z;
    Vector3 target = mCamera->getDirection();
    Vector3 cam_up = mCamera->getUp();

    // trova il nuovo cam_up (cioè goal)
    Vector3 right = target.crossProduct(world_up);
    Vector3 goal = right.crossProduct(target);
    Quaternion rotup = cam_up.getRotationTo(goal);

    return Degree(acos(rotup.w)*2*180/PI);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreApp::OgreApp(Simulator *_simulator)
{
    simulator =    _simulator;
    init_camera_pos = Vector3(0, 200, 200);
    init_camera_roll = 0;
    fps_goal = 0;
    pause_simulation = 0;
    showInfo = 0;
    showLabel = 0;
    showPath = 1;

    camera_anchor = -1;
    pathNode = NULL;
    m_bShutdown = false;

    textTime = NULL;
    textCenter = NULL;
    textMsg = NULL;
    textSimulLoop = NULL;

    // Initialize OgreFramework singleton
    new OgreFramework();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreApp::~OgreApp()
{
    delete OgreFramework::getSingletonPtr();
    if (textTime) {
        delete textTime;
    }
    if (textCenter) {
        delete textCenter;
    }
    if (textMsg) {
        delete textMsg;
    }
    if (textSimulLoop) {
        delete textSimulLoop;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::load_configuration(const Json::Value conf)
{
    if (conf.isMember("camera"))
        for(int i=0; i<3; i++)
            init_camera_pos[i] = conf.get( "camera", Json::Value(0) ).get( Json::UInt(i), init_camera_pos[i] ).asDouble();
    init_camera_roll = conf.get( "camera_roll", init_camera_roll ).asDouble();
    camera_anchor = conf.get( "camera_anchor", camera_anchor ).asInt();

    fps_goal = conf.get( "fps_goal", fps_goal ).asDouble();

    showPath = conf.get( "show_path", showPath ).asBool();
    showLabel = conf.get( "show_label", showLabel ).asBool();

    int default_path_anchor = conf.get( "default_path_anchor", -1 ).asInt();
    const Json::Value planets = conf["planets"];
    path_anchor.resize(planets.size());
    for ( unsigned int index = 0; index < planets.size(); ++index ) {
        path_anchor[index] = planets[index].get( "path_anchor", default_path_anchor ).asInt();
        if (path_anchor[index] < -1 || path_anchor[index] > (int)planets.size()) {
            printf("Il pianeta #%u %s ha un valore di path_anchor (%d) errato\n",
                index, planets[index].get( "name", "" ).asString().c_str(),
                path_anchor[index]
            );
            path_anchor[index] = -1;
        }
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::startMainLoop()
{
    if(!OgreFramework::getSingletonPtr()->initOgre("Newton", this, 0))
        return;

    m_bShutdown = false;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Newton initialized");

    setupAppScene();
    runApp();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::setupAppScene()
{
    // Light
    OgreFramework::getSingletonPtr()->m_pSceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));
     //OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(0,0,0);

     // Camera
     OgreFramework::getSingletonPtr()->m_pCamera->setPosition(init_camera_pos);
     OgreFramework::getSingletonPtr()->m_pCamera->lookAt(Vector3(0, 0, 0));
     // make camera_up_vetor pointing up! (to UNIT_Z)
    Vector3 target = OgreFramework::getSingletonPtr()->m_pCamera->getDirection();
    Vector3 right = target.crossProduct(Vector3::UNIT_Z);
    Vector3 goal = right.crossProduct(target);
    Quaternion rotup = OgreFramework::getSingletonPtr()->m_pCamera->getUp().getRotationTo(goal);
    OgreFramework::getSingletonPtr()->m_pCamera->rotate(rotup);
    // rotate camera_up_vector as configured
    OgreFramework::getSingletonPtr()->m_pCamera->roll(Degree(-init_camera_roll));

     OgreFramework::getSingletonPtr()->m_pCamera->setNearClipDistance(0.1);
    OgreFramework::getSingletonPtr()->m_pCamera->setFarClipDistance(5E30);
    if (OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
    {
        OgreFramework::getSingletonPtr()->m_pCamera->setFarClipDistance(0); // enable infinite far clip distance if we can
        OgreFramework::getSingletonPtr()->m_pLog->logMessage("Infinite far clip distance enabled");
    }
    else
    {
        OgreFramework::getSingletonPtr()->m_pLog->logMessage("Unable to use infinite far clip distance");
    }

    // Viewport
    OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(ColourValue(0.05f, 0.05f, 0.05f, 1.0f));

    // Entità e oggetti
    char name[512];
    char material[512];
    char label[512];
    Vector pos, color;

    planetNode.resize(simulator->get_planet_number());
    planetMaterial.resize(simulator->get_planet_number());
    planetEntity.resize(simulator->get_planet_number());
    pathManualObject.resize(simulator->get_planet_number());
    planetLabel.resize(simulator->get_planet_number());

    // Attacca il nodo dei tracciati alla radice
    pathNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("pathNode");

    for (int i=0; i<(int)simulator->get_planet_number(); ++i)
    {
        snprintf(material, sizeof(material), "Material_%d", i+1);
        snprintf(name, sizeof(name), "Planet_%d", i+1);
        snprintf(label, sizeof(label), "Label_%d", i+1);
        pos = simulator->get_planet(i).pos;
        color = simulator->get_planet(i).color;

        // Nodo
        planetNode[i] = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(name);
        planetNode[i]->setPosition(pos[0],pos[1],pos[2]);

        // Materiale
        planetMaterial[i] = MaterialManager::getSingleton().create(material,ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        planetMaterial[i]->setReceiveShadows(false);
        planetMaterial[i]->getTechnique(0)->setLightingEnabled(true);
        planetMaterial[i]->getTechnique(0)->getPass(0)->setDiffuse(color[0],color[1],color[2],0);
        planetMaterial[i]->getTechnique(0)->getPass(0)->setAmbient(color[0],color[1],color[2]);
        planetMaterial[i]->getTechnique(0)->getPass(0)->setSelfIllumination(color[0],color[1],color[2]);
        //planetMaterial[i]->dispose();  // dispose pointer, not the material

        // Pianeta (entità)
        planetEntity[i] = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(name, "sphere.mesh");
        planetEntity[i]->setMaterial(planetMaterial[i]);
        planetNode[i]->attachObject(planetEntity[i]);

        // Scala la mesh
        planetNode[i]->setScale(Vector3(simulator->get_planet(i).radius/100));

        // Pianeta (entità)
        planetLabel[i] = new ObjectTextDisplay(label, planetEntity[i], OgreFramework::getSingletonPtr()->m_pCamera);
        planetLabel[i]->setText(simulator->get_planet(i).name);
        planetLabel[i]->enable(showLabel);

        // Tracciato (oggetto manuale)
        pathManualObject[i] = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject(name);
        pathManualObject[i]->setDynamic(1);
        // ottimizza il suo buffer
        pathManualObject[i]->estimateVertexCount(simulator->get_max_path_length());
        pathManualObject[i]->begin(material, RenderOperation::OT_LINE_STRIP);
        pathManualObject[i]->position(pos[0],pos[1],pos[2]);
        pathManualObject[i]->position(pos[0],pos[1],pos[2]);
        pathManualObject[i]->end();

        // Attacca il tracciato al nodo dei tracciati
        pathNode->attachObject(pathManualObject[i]);
    }

    // Ora che sono stati definiti i nodi puoi attaccarci la telecamera
    // Se non viene fatto, set_camera_anchor() cerca di staccare dal nodo un oggetto che non c'è
    camera_anchor = (camera_anchor+1+simulator->get_planet_number()+1)%(simulator->get_planet_number()+1)-1;
    if (camera_anchor < 0) {
        OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->attachObject(OgreFramework::getSingletonPtr()->m_pCamera);
    } else {
        planetNode[camera_anchor]->attachObject(OgreFramework::getSingletonPtr()->m_pCamera);
    }

    textTime = new TextDisplay("Time");
    textCenter = new TextDisplay("Center");
    textMsg = new TextDisplay("Msg");
    textSimulLoop = new TextDisplay("SimulLoop");
    textTime->enable(showInfo);
    textCenter->enable(showInfo);
    textMsg->enable(showInfo);
    textSimulLoop->enable(showInfo);
    textTime->setPosition(0.01, 0.01);
    textCenter->setPosition(0.01, 0.06);
    textMsg->setPosition(0.01, 0.11);
    textSimulLoop->setPosition(0.51, 0.01);
    textMsg->setText(" ");
    textCenter->setTextPrintf("Telecamera centrata su #%d %s",
        camera_anchor, camera_anchor<0?"Origine":simulator->get_planet(camera_anchor).name);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::set_camera_anchor(int anchor)
{

    // Stacca
    if (camera_anchor < 0) {
        OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->detachObject(OgreFramework::getSingletonPtr()->m_pCamera);
    } else {
        planetNode[camera_anchor]->detachObject(OgreFramework::getSingletonPtr()->m_pCamera);
    }

    // Riduci tra -1 e numero_pianeti-1
    anchor = (anchor+1+simulator->get_planet_number()+1)%(simulator->get_planet_number()+1)-1;

    // Attacca
    if (anchor < 0) {
        OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->attachObject(OgreFramework::getSingletonPtr()->m_pCamera);
    } else {
        planetNode[anchor]->attachObject(OgreFramework::getSingletonPtr()->m_pCamera);
    }

/*
    // LookAt
    OgreFramework::getSingletonPtr()->m_pCamera->lookAt(Vector3(0, 0, 0));

    Quaternion rot;
    Vector3 pos;
    rot = OgreFramework::getSingletonPtr()->m_pCamera->getOrientation();
    pos = OgreFramework::getSingletonPtr()->m_pCamera->getPosition();
    if (anchor < 0) {
        OgreFramework::getSingletonPtr()->m_pCameraMan->setTarget(
            OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()
        );
    } else {
        OgreFramework::getSingletonPtr()->m_pCameraMan->setTarget(planetNode[anchor]);
    }
    OgreFramework::getSingletonPtr()->m_pCamera->setOrientation(rot);
    OgreFramework::getSingletonPtr()->m_pCamera->setPosition(pos);
*/

    // Aggiorna
    camera_anchor = anchor;
    printf("Telecamera centrata su #%d %s\n", anchor, anchor<0?"Origine":simulator->get_planet(anchor).name);

    textCenter->setTextPrintf("Telecamera centrata su #%d %s",
        anchor, anchor<0?"Origine":simulator->get_planet(anchor).name);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::set_path_anchor(int anchor)
{
    if (camera_anchor < 0) return;
    // riduci tra -1 e numero_pianeti-1
    anchor = (anchor+1+simulator->get_planet_number()+1)%(simulator->get_planet_number()+1)-1;
    // Aggiorna
    path_anchor[camera_anchor] = anchor;
    printf("Il tracciato di #%d %s ora è relativo a #%d %s\n",
        camera_anchor, simulator->get_planet(camera_anchor).name,
        anchor, anchor<0?"Origine":simulator->get_planet(anchor).name
    );
    textMsg->setTextPrintf("Il tracciato di #%d %s ora è relativo a #%d %s\n",
        camera_anchor, simulator->get_planet(camera_anchor).name,
        anchor, anchor<0?"Origine":simulator->get_planet(anchor).name
    );
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::updateScene()
{
    Vector pos, path, anchor;

    // Per ogni pianeta
    for (int i=0; i<(int)simulator->get_planet_number(); ++i)
    {
        pos = simulator->get_planet(i).pos;

        // Pianeta
        planetNode[i]->setPosition(pos[0],pos[1],pos[2]);

        // Se non devi visualizzare i tracciati
        if (!showPath) continue;

        // Tracciato
        //pathManualObject[i]->estimateVertexCount(simulator->get_path_length()+1);
        pathManualObject[i]->beginUpdate(0);
        //pathManualObject[i]->position(0, 0, 0);
        pathManualObject[i]->position(pos[0],pos[1],pos[2]);

        // Per ogni punto del tracciato
        for (int k=0; k<(int)simulator->get_path_length(); ++k)
        {
            // Ottieni i punti del tracciato: corrente e vecchio
            path = simulator->get_path_point(k)->position[i];

            // Se il tracciato è relativo a qualcosa
            if (path_anchor[i] >= 0 && path_anchor[i] < (int)simulator->get_planet_number())
            {
                path += simulator->get_planet(path_anchor[i]).pos - simulator->get_path_point(k)->position[path_anchor[i]];
            }
            pathManualObject[i]->position(path[0], path[1], path[2]);
        }
        pathManualObject[i]->end();
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::runApp()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");

    double timeSinceLastFrame = 0;
    double startTime = 0;
    unsigned int calibra = 10;
    unsigned int cal_step = 512;
    int trend = 0;

    OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();

    startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

    while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown())
    {
        if (OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed()) m_bShutdown = true;

        WindowEventUtilities::messagePump();

        if (OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
        {

            OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
            OgreFramework::getSingletonPtr()->m_pMouse->capture();

            if (!pause_simulation)
            {
                for (unsigned int i=0; i<calibra; ++i) simulator->evolve();
                if (fps_goal > 0) {
                    // All'inizio è simile ad una ricerca binaria,
                    // dopo che si è stabilizzato 'calibra' cambia solo di +1 o -1
                    //if (OgreFramework::getSingletonPtr()->m_pRenderWnd->getLastFPS() > fps_goal) { // No: è una media
                    if (1000.0f/timeSinceLastFrame > fps_goal) { // le statistiche di SdkSample sono diverse
                        // Simula di più
                        if (trend >= 0) {
                            if (trend >= 1 || cal_step > 1) {
                                // aumenta calibra
                                calibra += cal_step;
                            }
                            trend += 1;
                        } else {
                            //diminuisci cal_step
                            cal_step /= 2;

                            trend = 0;
                        }
                    } else {
                        // Riduci il tempo per la simulazione
                        if (trend <= 0) {
                            if (trend <= -1 || cal_step > 1) {
                                // diminuisci calibra
                                if (cal_step > calibra) calibra = 1;
                                else calibra -= cal_step;
                            }
                            trend -= 1;
                        } else {
                            //diminuisci cal_step
                            cal_step /= 2;

                            trend = 0;
                        }
                    }
                    //if (calibra <= 0) calibra = 1;
                    if (cal_step == 0) cal_step = 1;
                }
            }
            updateScene();

            if (showInfo) {
                textTime->setTextPrintf("Secondi trascorsi: %d", (int)simulator->get_time());
                textSimulLoop->setTextPrintf("dt per frame: %u +- %u  trend: %d", calibra, cal_step, trend);
                textTime->update();
                textCenter->update();
                textMsg->update();
                textSimulLoop->update();
            }
            if (showLabel) {
                for (int i=0; i<(int)simulator->get_planet_number(); ++i) {
                    planetLabel[i]->update();
                }
            }

            OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
            OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

            // Sono multipli di 10 millisecondi.. poco precisi
            timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
            startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            //printf("timeSinceLastFrame %.0lf   fps %.2f\n", timeSinceLastFrame, 1000.0f/timeSinceLastFrame);
            //printf("Ogre-fps: %.2f\n", OgreFramework::getSingletonPtr()->m_pRenderWnd->getLastFPS());
        }
        else
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
        }
    }

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_1))
    {
        set_camera_anchor(camera_anchor-1);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_2))
    {
        set_camera_anchor(camera_anchor+1);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_3))
    {
        set_path_anchor(camera_anchor<0?0:path_anchor[camera_anchor]-1);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_4))
    {
        set_path_anchor(camera_anchor<0?0:path_anchor[camera_anchor]+1);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_P))
    {
        pause_simulation = !pause_simulation;
        printf("Simulazione %s\n", pause_simulation?"in pausa":"attivata");
        textMsg->setTextPrintf("Simulazione %s\n", pause_simulation?"in pausa":"attivata");
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_R))
    {
        printf("Reset\n");
        textMsg->setTextPrintf("Reset");
        simulator->reset();
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_T))
    {
        showPath = !showPath;
        printf("Tracciati dei pianeti: %s\n", showPath?"visibili":"nascosti");
        textMsg->setTextPrintf("Tracciati dei pianeti: %s\n", showPath?"visibili":"nascosti");
        pathNode->setVisible(showPath);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_N))
    {
        showLabel = !showLabel;
        printf("Nomi dei pianeti: %s\n", showLabel?"visibili":"nascosti");
        textMsg->setTextPrintf("Nomi dei pianeti: %s\n", showLabel?"visibili":"nascosti");
        for (int i=0; i<(int)simulator->get_planet_number(); ++i) {
            planetLabel[i]->enable(showLabel);
        }
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_C))
    {
        Vector3 cam = OgreFramework::getSingletonPtr()->m_pCamera->getPosition();
        double roll = getRoll(OgreFramework::getSingletonPtr()->m_pCamera).valueDegrees();

        printf("Posizione della telecamera: [%e, %e, %e] roll +/-%.2lf rispetto a #%d %s\n",
            cam[0], cam[1], cam[2], roll,
            camera_anchor, simulator->get_planet(camera_anchor).name
        );
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
    {
        showInfo = !showInfo;
        printf("Informazioni %s\n", showInfo?"visibili":"nascoste");
        textTime->enable(showInfo);
        textCenter->enable(showInfo);
        textMsg->enable(showInfo);
        textSimulLoop->enable(showInfo);
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::mouseMoved(const OIS::MouseEvent &evt)
{
    OgreFramework::getSingletonPtr()->mouseMoved(evt);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

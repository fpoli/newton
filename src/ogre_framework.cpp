//|||||||||||||||||||||||||||||||||||||||||||||||

#include "ogre_framework.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

namespace Ogre {
    template<> OgreFramework* Singleton<OgreFramework>::msSingleton = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    m_bShutDownOgre        = false;
    m_iNumScreenShots    = 0;

    orbiting            = 0;
    turn_zoom            = 0;

    m_pRoot                = 0;
    m_pSceneMgr            = 0;
    m_pRenderWnd        = 0;
    m_pCamera            = 0;
    m_pViewport            = 0;
    m_pLog                = 0;
    m_pTimer            = 0;

    m_pInputMgr            = 0;
    m_pKeyboard            = 0;
    m_pMouse            = 0;

    m_pTrayMgr          = 0;
    m_FrameEvent        = FrameEvent();

    m_MoveScale         = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::initOgre(String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    new LogManager(); // Don't remove! Initialize LogManager singleton

    m_pLog = LogManager::getSingleton().createLog("ogre.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Root();

    if(!m_pRoot->showConfigDialog())
        return false;
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
    m_pSceneMgr->setAmbientLight(ColourValue(0.7f, 0.7f, 0.7f));

    // Initialize OverlaySystem singleton
    OverlaySystem *m_pOverlaySystem = new OverlaySystem();
    m_pSceneMgr->addRenderQueueListener(m_pOverlaySystem);

    m_pCamera = m_pSceneMgr->createCamera("Camera");
    m_pCamera->setPosition(Vector3(0, 60, 60));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);

    m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
    m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));

    m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));

    m_pViewport->setCamera(m_pCamera);

    // Orbit camera
    //m_pCameraMan = new OgreBites::SdkCameraMan(m_pCamera);
    //m_pCameraMan->setStyle(OgreBites::CS_ORBIT);

    uint64_t hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width     = m_pRenderWnd->getWidth();

    if(pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

    String secName, typeName, archName;
    ConfigFile cf;
    cf.load("resources.cfg");

    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pTimer = new Timer();
    m_pTimer->reset();

    // Fix for 1.9
    mInputContext.mKeyboard = m_pKeyboard;
    mInputContext.mMouse = m_pMouse;

    m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, mInputContext, this);
    //m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    m_pTrayMgr->hideCursor();

    m_pRenderWnd->setActive(true);

    Quaternion yaw(Degree(90), Vector3::UNIT_Y);
    Quaternion pitch(Degree(0), Vector3::UNIT_X);
    Quaternion roll(Degree(30), Vector3::UNIT_Z);
    m_pSceneMgr->setSkyBox(true, "SkyBox", 5E10, true, yaw*pitch*roll);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    if(m_pInputMgr)  OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pTrayMgr)   delete m_pTrayMgr;
    if(m_pRoot)      delete m_pRoot;
    //if(m_pCameraMan) delete m_pCameraMan;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bShutDownOgre = true;
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("screenshot_", ".jpg");
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_M))
    {
        static int mode = 0;

        if(mode == 2)
        {
            m_pCamera->setPolygonMode(PM_SOLID);
            mode = 0;
        }
        else if(mode == 0)
        {
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
        }
        else if(mode == 1)
        {
            m_pCamera->setPolygonMode(PM_POINTS);
            mode = 2;
        }
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideLogo();
            m_pTrayMgr->hideFrameStats();
        }
        else
        {
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        }
    }

    //m_pCameraMan->injectKeyDown(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    //m_pCameraMan->injectKeyUp(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    //m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
    //m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
    //m_pCamera->setPosition(m_pCamera->getPosition()*pow(1.03,-evt.state.Z.rel/10));

    Real dist = m_pCamera->getPosition().length();
    //Vector3 position = m_pCamera->getPosition();
    Quaternion rot = m_pCamera->getOrientation();

    if (orbiting)   // yaw around the target, and pitch locally
    {
        m_pCamera->setPosition(Vector3(0, 0, 0));

        Quaternion yaw(Degree(-evt.state.X.rel * 0.25f), Vector3::UNIT_Y);
        Quaternion pitch(Degree(-evt.state.Y.rel * 0.25f), Vector3::UNIT_X);

        // L'ordine della moltiplicazione è importante!
        // rot * yaw * pitch  ruota rispetto alla assi della visuale (~free camera)
        // yaw * pitch * rot  ruota rispetto agli assi derivati dal nodo padre (~orbit camera)
        m_pCamera->setOrientation(rot * yaw * pitch);

        m_pCamera->moveRelative(Vector3(0, 0, dist));

        // don't let the camera go over the top or around the bottom of the target
    }
    else if (turn_zoom)
    {
        // turn the camera (before zooming!)
        m_pCamera->setPosition(Vector3(0, 0, 0));

        Quaternion yaw(Degree(-evt.state.X.rel * 0.25f), Vector3::UNIT_Y);
        Quaternion roll(Degree(evt.state.Y.rel * 0.25f), Vector3::UNIT_Z);

        // L'ordine della moltiplicazione è importante!
        // rot * yaw * roll  ruota rispetto alla assi della visuale (~free camera)
        // yaw * roll * rot  ruota rispetto agli assi derivati dal nodo padre (~orbit camera)
        m_pCamera->setOrientation(rot * yaw * roll);

        m_pCamera->moveRelative(Vector3(0, 0, dist));


        // move the camera toward or away from the target
        // the further the camera is, the faster it moves
        //m_pCamera->moveRelative(Vector3(0, 0, evt.state.Y.rel * 0.004f * dist));

        // don't let the camera go over the top or around the bottom of the target
    }
    else if (evt.state.Z.rel != 0)  // move the camera toward or away from the target
    {
        // the further the camera is, the faster it moves
        m_pCamera->moveRelative(Vector3(0, 0, -evt.state.Z.rel * 0.0009f * dist));
    }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left) orbiting = true;
    else if (id == OIS::MB_Right) turn_zoom = true;
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left) orbiting = false;
    else if (id == OIS::MB_Right) turn_zoom = false;
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
    m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;

    m_TranslateVector = Vector3::ZERO;

    getInput();
    moveCamera();

    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::moveCamera()
{
    if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);
    else
        m_pCamera->moveRelative(m_TranslateVector / 10);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::getInput()
{

}

//|||||||||||||||||||||||||||||||||||||||||||||||

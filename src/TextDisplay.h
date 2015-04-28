#ifndef __TextDisplay_H__
#define __TextDisplay_H__

#include <stdarg.h>

class AbstractTextDisplay {
protected:
	std::string ID;
	bool m_enabled;
	float pos_x, pos_y;
	float dim_x, dim_y;
	Ogre::Overlay* m_pOverlay;
	Ogre::OverlayElement* m_pText;
	Ogre::OverlayContainer* m_pContainer;
	Ogre::String m_text;
public:
	AbstractTextDisplay(const std::string& _ID) {
		ID = _ID;
		m_enabled = false;
		m_text = "";

		// create an overlay that we can use for later
		m_pOverlay = Ogre::OverlayManager::getSingleton().create("overlay_" + ID);
		m_pContainer = static_cast<Ogre::OverlayContainer*>(
			Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "container_" + ID)
		);

		m_pOverlay->add2D(m_pContainer);

		m_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", ID);
		pos_x = 0;		pos_y = 0;
		dim_x = 0.01;	dim_y = 0.01;
		m_pText->setDimensions(dim_x, dim_y);
		m_pText->setMetricsMode(Ogre::GMM_PIXELS);
		m_pText->setPosition(pos_x, pos_y);
		
		m_pText->setParameter("font_name", "TextFont");
		m_pText->setParameter("char_height", "24");
		m_pText->setParameter("horz_align", "right");
		m_pText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 0.8));

		m_pContainer->addChild(m_pText);
		m_pOverlay->show();
	}

	virtual ~AbstractTextDisplay() {
		// overlay cleanup -- Ogre would clean this up at app exit but if your app 
		// tends to create and delete these objects often it's a good idea to do it here.
		m_pOverlay->hide();
		Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
		m_pContainer->removeChild("shapeNameText");
		m_pOverlay->remove2D(m_pContainer);
		overlayManager->destroyOverlayElement(m_pText);
		overlayManager->destroyOverlayElement(m_pContainer);
		overlayManager->destroy(m_pOverlay);
	}

	void enable(bool enable) {
		m_enabled = enable;
		if (enable) {
			m_pOverlay->show();
		} else {
			m_pOverlay->hide();
		}
	}

	void setText(const Ogre::String& text) {
		m_text = text;
		m_pText->setCaption(m_text);
	}
	
	void setTextPrintf(const char *str, /*args*/ ...) {
		char text[256];
		va_list ap;
	 
		if (str == NULL) {
			text[0]='\0';
		} else {
			va_start(ap, str);
			vsprintf(text, str, ap);
			va_end(ap);
		}
	 
		setText(text);
	}
	
	void update()  {
		if (!m_enabled) return;
		
		m_pContainer->setPosition(pos_x, pos_y);
		m_pContainer->setDimensions(dim_x, dim_y);
	}
};

/* ===== Text on 2D screen ===== */
class TextDisplay : public AbstractTextDisplay {
public:
	TextDisplay(const std::string& _ID) : AbstractTextDisplay(_ID) {
		
	}

	virtual ~TextDisplay() { }

	void setPosition(float x, float y) {
		pos_x = x;
		pos_y = y;
	}
};

/* ===== Text in 3D space ===== */
class ObjectTextDisplay : public AbstractTextDisplay {
protected:
	const Ogre::MovableObject* m_p;
	const Ogre::Camera* m_c;
public:
	ObjectTextDisplay(const std::string& _ID, const Ogre::MovableObject* p, const Ogre::Camera* c) : AbstractTextDisplay(_ID) {
		setObject(p);
		setCamera(c);
	}

	virtual ~ObjectTextDisplay() { }

	void setObject(const Ogre::MovableObject* p) {
		m_p = p;
	}
	
	void setCamera(const Ogre::Camera* c) {
		m_c = c;
	}
	
	/**
	* This little snippet gets the screenspace coordinates for a MovableObject
	*
	* @param   object   The object to retrieve the coordidnates of.
	* @param   camera   The active camera
	* @param   result   The Vector2 to store the result in
	*
	* @return   Returns true if the object is visible and the coordinates were
	*         retrieved, false otherwise.
	*/
	bool getScreenspaceCoords(const Ogre::MovableObject* object, const Ogre::Camera* camera, Ogre::Vector2& result)
	{
		if(!object->isInScene())
			return false;
		
		Ogre::Matrix4 mat = camera->getProjectionMatrix() * camera->getViewMatrix();
		const Ogre::AxisAlignedBox &AABB = object->getWorldBoundingBox(true);
		
		// Get the center point of the object's bounding box
		Ogre::Vector3 point = AABB.getCenter();
		
		// Is the camera facing that point? If not, return false
		Ogre::Plane cameraPlane = Ogre::Plane(Ogre::Vector3(camera->getDerivedOrientation().zAxis()), camera->getDerivedPosition());
		if(cameraPlane.getSide(point) != Ogre::Plane::NEGATIVE_SIDE)
			return false;
		
		// *** Get 2d coords of the nicest point***
		const Ogre::Vector3* corners = AABB.getAllCorners();
		// coordinate space [-1, 1]
		float min_x = 2.0f, max_x = -2.0f, min_y = 2.0f, max_y = -2.0f;
		
		// expand the screen-space bounding-box so that it completely encloses 
		// the object's AABB
		for (int i=0; i<8; i++) {
			// multiply the AABB corner vertex by the view matrix to 
			// get a screen-space vertex
			Ogre::Vector3 corner = mat * corners[i];

			if (corner.x < min_x) min_x = corner.x;
			if (corner.x > max_x) max_x = corner.x;
			if (corner.y < min_y) min_y = corner.y;
			if (corner.y > max_y) max_y = corner.y;
		}
		
		// Transform from coordinate space [-1, 1] to [0, 1] and update in-value
		result.x = (max_x / 2) + 0.5f;
		result.y = 1 - ((max_y / 2) + 0.5f);
		
		return true;
	}
	
	void update()  {
		if (!m_enabled) return;
		
		Ogre::Vector2 pos(0,0);
		bool is_visible = getScreenspaceCoords(m_p, m_c, pos);
		
		if (!is_visible) {
			m_pContainer->setPosition(2, 2); // don't show
		} else {
			m_pContainer->setPosition(pos.x, pos.y);
		}
		m_pContainer->setDimensions(0, 0.1); // 0.1, just "because"
	}
};

#endif

/*
 *  GLScreen.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/29/10.
 *
 */

#include "SimulatorWindow.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mutex.h"
#include <list>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "SimHardware.h"
#include "SimulatorController.h"
#include "SimulatedRobot/SimulatedRobotModel.h"
#include <string>
#include "SimulatedRobot/SimulatedField.h"
#include "FieldObject/FieldObject.h"
#include "FieldObject/Components/Edge.h"
#include <vector>
#include "Shared/RobotProgram.h"

#include <GLUT/glut.h>

using namespace std;


static list<RobotPositionRecord> s_robotMovements;
bool    s_bRobotChanged = false;

SDL_mutex*  s_robotLock=NULL;

class ImageTexture
{
public:
    explicit ImageTexture(const char* filePath);
    friend class Sprite;

    ~ImageTexture();

    void SetTexture();

private:

    GLuint m_texID;

    float m_texCoords[8];
    uint32_t m_dimensions[2];

};

class Sprite
{
public:
    explicit Sprite(const char* filepath, float width, float height)
    : m_pTexture(NULL)
    , m_width(width)
    , m_height(height)
    {
        m_pTexture = new ImageTexture(filepath);

        static const float stdQuadVerts[12] =
        {   -1.f, -1.f, 0.f,
            -1.f, 1.f, 0.f,
            1.f, -1.f, 0.f,
            1.f, 1.f, 0.f
        };

        memcpy(m_vertCoord, stdQuadVerts, 12*sizeof(float));
        for (int i=0; i<12; i+=3)
        {
            m_vertCoord[i] *= m_width/2;

            m_vertCoord[i] *= m_height/2;
        }

    }

    ~Sprite()
    {
        delete m_pTexture;
        m_pTexture = NULL;
    }

    void Draw()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m_vertCoord);
        m_pTexture->SetTexture();
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

private:

    ImageTexture* m_pTexture;
    float m_vertCoord[12];
    float m_width;
    float m_height;
};

std::string GetFilePathForResource(const char* inResource)
{
#if defined(__APPLE__)
    #import <Cocoa/Cocoa.h>

    NSString* resourceString = [[NSBundle mainBundle] resourcePath];
    const char* cResString = [resourceString cStringUsingEncoding:[NSString defaultCStringEncoding]];
    std::string resString(cResString);
    resString.append("/");
    resString.append(inResource);
    return resString;

#elif defined(_WIN32)

    std::string resString = "Resources/";
    resString.append(inResource);
    return resString;
#else
#error "Not a Valid Operating System"
#endif
}

ImageTexture::ImageTexture(const char* filepath)
{
    static const float vertQuadCoords[8] = {0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f};

    SDL_Surface* originalSurface = IMG_Load(filepath);
    if (originalSurface == NULL)
    {
        fprintf(stderr, "Image at %s could not be opened \n", filepath);
        exit(-1);
    }

    int origW = originalSurface->w;
    int origH = originalSurface->h;

    m_dimensions[0] = origW;
    m_dimensions[1] = origH;

    int w = round(pow(2.f, ceil( log((float)(originalSurface->w))/log(2) ) )); // Round up to the nearest power of two
    int h = round(pow(2.f, ceil( log((float)(originalSurface->h))/log(2) ) )); // Round up to the nearest power of two

    uint32_t rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface* newSurface =
                SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
    SDL_BlitSurface(originalSurface, 0, newSurface, 0); // Blit onto a purely RGB Surface

    SDL_FreeSurface(originalSurface);

    glEnable(GL_TEXTURE_2D);
    glGenTextures( 1, &m_texID );
    glBindTexture( GL_TEXTURE_2D, m_texID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, newSurface->pixels );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    SDL_FreeSurface(newSurface);

    float wEdge = (float)origW / (float)w;
    float hEdge = (float)origH / (float)h;

    memcpy(m_texCoords, vertQuadCoords, 8*sizeof(float));
    for (int i=0; i<8; i+=2)
    {
        m_texCoords[i] *= wEdge;
        m_texCoords[i+1] *= hEdge;
    }
}

ImageTexture::~ImageTexture()
{
    glDeleteTextures(1, &m_texID);
}

void ImageTexture::SetTexture()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords);
}

static bool     s_bNeedScreenResize=true;

struct ScreenRectangle
{
    ScreenRectangle()
    : left(0)
    , top(0)
    , right(0)
    , bottom(0)
    {}

    ScreenRectangle(uint32_t x, uint32_t y)
    : left(x)
    , top(y)
    , right(x)
    , bottom(y)
    {}

    void SetSize(uint32_t width, uint32_t height)
    {
        bottom = top + height;
        right = left + width;
    }

    ScreenRectangle InvertHeight(uint32_t refHeight) const
    {
        ScreenRectangle outRec = *this;
        outRec.top = refHeight - top;
        outRec.bottom = refHeight - bottom;
        return outRec;
    }

    uint32_t Width() const { return right - left; }
    uint32_t Height() const { return bottom - top; }

    uint32_t left;
    uint32_t top;
    uint32_t right;
    uint32_t bottom;
};

static uint32_t s_desiredScreenWidth=640;

static ScreenRectangle s_WindowRect;
static ScreenRectangle s_MainViewRect;
static ScreenRectangle s_3DViewScreenRect;

static Vector3  s_3DViewRotations;
static float    s_3DViewScale=1.f;


void DrawRobotPath()
{
    list<RobotPositionRecord>::const_iterator it = s_robotMovements.begin();
    RobotPositionRecord prevRec =*it;
    ++it;
    for (; it != s_robotMovements.end(); ++it)
    {
        RobotPositionRecord rec = *it;
        assert(rec.PositionChanged(prevRec) || rec.AngleChanged(prevRec));
        // draw line between the two points

        float vertices[6];
        vertices[0] = prevRec.pos.x;
        vertices[1] = prevRec.pos.y;
        vertices[2] = 0.f;
        vertices[3] = rec.pos.x;
        vertices[4] = rec.pos.y;
        vertices[5] = 0.f;

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2);
        glColor4f(1.f, 1.f, 1.f, 1.f);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_LINES, 0, 2);

        //glPointSize(4);
        //glDrawArrays(GL_POINTS, 0, 2);
        prevRec = rec;

    }

}

void SetLineColor(FieldObject::Alliance alliance)
{
	if(alliance==FieldObject::red)
		glColor4f(1.f, 0.f, 0.f, 1.f);
	else if(alliance==FieldObject::blue)
		glColor4f(0.f, 0.f, 1.f, 1.f);
	else
		glColor4f(1.f, 1.f, 1.f, 1.f);

}

void shift(const Vector3& pos, const Vector3& angle)
{
	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(angle.x/M_PI*180.0, 1.0, 0.0, 0.0);
	glRotatef(angle.y/M_PI*180.0, 0.0, 1.0, 0.0);
	glRotatef(angle.z/M_PI*180.0, 0.0, 0.0, 1.0);
}


void DrawFieldObject(FieldObject* fieldObject)
{
    const std::vector<Vector3>& relativePositions=fieldObject->GetRelativePositions();
    const std::vector<Vector3>& relativeAngles=fieldObject->GetRelativeAngles();
    const std::vector<ConvexPolyhedron>& objectPolyhedra=fieldObject->GetPolyhedra();
    
    for(int j=0;j<objectPolyhedra.size();j++) { //for each polyhedron
        const ConvexPolyhedron& polyhedron=objectPolyhedra[j];
        const Vector3& relpos=relativePositions[j];
        const Vector3& relAngle = relativeAngles[j];
        
        glPushMatrix();
        
        shift(relpos,relAngle);
        
        const std::vector<Vector3>& faceVertices = polyhedron.GetBirdsEye().GetVertices();
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2);
        
        glVertexPointer(3, GL_DOUBLE, 0, &(faceVertices[0]));
        glDrawArrays(GL_LINE_LOOP, 0, faceVertices.size());
        
        glPopMatrix();
    }
    
}

void DrawFieldObjects(const SimulatedField* field)
{
	const std::vector<FieldObject*>& FieldObjects = field->GetFieldObjects();
	for(int i=0;i<FieldObjects.size();i++) { //for each object
		FieldObject* fieldObject=FieldObjects[i];
        const Vector3& angle=fieldObject->GetAngle();
        const Vector3& pos = fieldObject->GetPos();
        glPushMatrix();
        shift(pos,angle);
        SetLineColor(fieldObject->GetAlliance());
        
		DrawFieldObject(fieldObject);
        
        glPopMatrix();
	}
    
    
}

void Draw3DFieldView(const SimulatedField* field, const Vector3& base_pos)
{

    const std::vector<FieldObject*>& FieldObjects = field->GetFieldObjects();
	for(int i=0;i<FieldObjects.size();i++) { //for each object
		FieldObject* fieldObject=FieldObjects[i];

		//printf("Object: %s\n",fieldObject->GetDescriptor().c_str());
        const Vector3& pos = fieldObject->GetPos() - base_pos;
		const std::vector<ConvexPolyhedron>& objectPolyhedra=fieldObject->GetPolyhedra();
		const std::vector<Vector3>& relativePositions=fieldObject->GetRelativePositions();
		const std::vector<Vector3>& relativeAngles=fieldObject->GetRelativeAngles();
		const Vector3& angle=fieldObject->GetAngle();
		glPushMatrix();
		shift(pos,angle);
		for(int j=0;j<objectPolyhedra.size();j++) { //for each polyhedron
			const ConvexPolyhedron& polyhedron=objectPolyhedra[j];
			const Vector3& relpos=relativePositions[j];
			const Vector3& relAngle = relativeAngles[j];
			const std::vector<Edge>& edges = polyhedron.GetEdges();

			glPushMatrix();
			shift(relpos,relAngle);


			//printf("Angle of Component: %f\n",relAngle.y/M_PI*180.0);


			std::vector<Vector3> vertices;
			for (int k=0; k<edges.size(); k++)
			{
				const Vector3& v1 = edges[k].GetEnds()[0];
				const Vector3& v2 = edges[k].GetEnds()[1];

				vertices.push_back(v1);
				vertices.push_back(v2);
			}

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(3);
			SetLineColor(fieldObject->GetAlliance());
			glVertexPointer(3, GL_DOUBLE, 0, /*vertices*/ &(vertices[0]));
			glDrawArrays(GL_LINES, 0, edges.size()*2);
			glPopMatrix();

		}
		glPopMatrix();

	}

}

void Draw3DAxis()
{
    Vector3 vertices[6];
    vertices[0] = Vector3(-100.0, 0.0, 0.0);
    vertices[1] = Vector3(100.0, 0.0, 0.0);
    vertices[2] = Vector3(0.0, -100.0, 0.0);
    vertices[3] = Vector3(0.0, 100.0, 0.0);
    vertices[4] = Vector3(0.0, 0.0, -100.0);
    vertices[5] = Vector3(0.0, 0.0, 100.0);

    const Vector3 axisColors[3] = { Vector3(1.0,0.0,0.0), Vector3(0,1,0), Vector3(0,0,1)};

    for (int i=0; i<3; i++)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2);
        glColor4f(axisColors[i].x, axisColors[i].y, axisColors[i].z, 1.f);
        glVertexPointer(3, GL_DOUBLE, 0,  &(vertices[i*2]));
        glDrawArrays(GL_LINES, 0, 2);
    }

}


static void initAttributes ()
{
    // Setup attributes we want for the OpenGL context


    // Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
    //    Mac OS X will always use 8-8-8-8 ARGB for 32-bit screens and
    //    5-5-5 RGB for 16-bit screens

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

}

static void printAttributes ()
{
    // Print out attributes of the context we created
    int nAttr;
    int i;

    SDL_GLattr  attr[] = { SDL_GL_RED_SIZE, SDL_GL_BLUE_SIZE, SDL_GL_GREEN_SIZE,
        SDL_GL_ALPHA_SIZE, SDL_GL_BUFFER_SIZE, SDL_GL_DEPTH_SIZE };

    const char *desc[] = { "Red size: %d bits\n", "Blue size: %d bits\n", "Green size: %d bits\n",
        "Alpha size: %d bits\n", "Color buffer size: %d bits\n",
        "Depth bufer size: %d bits\n" };

    nAttr = sizeof(attr) / sizeof(int);

    for (i = 0; i < nAttr; i++) {

        int value;
        SDL_GL_GetAttribute (attr[i], &value);
        printf (desc[i], value);
    }
}

static void createSurface (int fullscreen, const SimulatedField* pField, SDL_Surface** inScreen)
{
    Uint32 flags = 0;

    flags = SDL_OPENGL | SDL_DOUBLEBUF | SDL_RESIZABLE;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;


    const double main_screen_ratio = pField->GetFieldLength() / pField->GetFieldWidth();

    const uint32_t main_screen_height = s_desiredScreenWidth / (main_screen_ratio);
    // from the formula screen_width = screen_height + screen_ratio*screen_height

    s_WindowRect = ScreenRectangle(0,0);
    s_WindowRect.SetSize(s_desiredScreenWidth, 2 * main_screen_height);

    s_MainViewRect = ScreenRectangle(0,0);
    s_MainViewRect.SetSize(s_WindowRect.Width(), main_screen_height);

    s_3DViewScreenRect = ScreenRectangle(s_MainViewRect.left, s_MainViewRect.bottom);
    s_3DViewScreenRect.SetSize(s_WindowRect.Width(), main_screen_height);

    // Create window
    *inScreen = SDL_SetVideoMode (s_WindowRect.Width(), s_WindowRect.Height(), 0, flags);
    if (*inScreen == NULL) {

        fprintf (stderr, "Couldn't set OpenGL video mode: %s\n",
                 SDL_GetError());
		SDL_Quit();
		exit(2);
	}

    SDL_WM_SetCaption("Team 254 Robot Simulator", "RoboSim");
}


int SimulatorWindow::mainLoop(void*)
{

#if defined(__APPLE__)
#import <Foundation/Foundation.h>
    NSAutoreleasePool *pool;
    pool = [[NSAutoreleasePool alloc] init];
#endif
    
    SDL_Surface* surface_screen=NULL;

    // Set GL context attributes
    initAttributes ();


    // Get GL context attributes
    //printAttributes ();

    int fps = 60;
	int delay = 1000/fps;
    int thenTicks = -1;
    int nowTicks;

    SDL_mutexP(s_robotLock);
    RobotPositionRecord lastPos = *(s_robotMovements.rbegin());
    SDL_mutexV(s_robotLock);

    std::string filePath = GetFilePathForResource("car.png");


    const SimulatorController* simCtlr = s_pController;

    const double fieldLength = s_pSimulatedField->GetFieldLength();
    const double fieldWidth = s_pSimulatedField->GetFieldWidth();

    s_3DViewRotations = Vector3();

    Sprite* carSprite = NULL;

    while ( simCtlr->IsProgramActive() ) {

        if (s_bNeedScreenResize)
        {
            if (carSprite)
                delete carSprite;

            if (surface_screen)
                SDL_FreeSurface(surface_screen);

            // Create GL context
            createSurface (0, s_pSimulatedField, &surface_screen);

            glEnable(GL_SCISSOR_TEST);
            glEnable(GL_MULTISAMPLE);
            glHint(GL_LINE_SMOOTH, GL_NICEST);
            glEnable(GL_BLEND);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            carSprite = new Sprite(filePath.c_str(), s_pRobotModel->GetLength(), s_pRobotModel->GetWidth());

            s_bNeedScreenResize = false;
            s_bRobotChanged = true;

        }

		SDL_mutexP(s_robotLock);

		RobotPositionRecord lastPos = *(s_robotMovements.rbegin());

		// Draw the main overhead screen
		{
			ScreenRectangle viewRect = s_MainViewRect.InvertHeight(s_WindowRect.Height());
            glViewport(viewRect.left, viewRect.bottom, viewRect.Width(), viewRect.top - viewRect.bottom);
			glScissor(viewRect.left, viewRect.bottom, viewRect.Width(), viewRect.top - viewRect.bottom);

			glClearColor(0.4, 0.4, 0.4, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, fieldLength, 0, fieldWidth, 0, 10);
			glTranslatef(0, 0, -5.f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			DrawRobotPath();

			DrawFieldObjects(s_pSimulatedField);

            /*
			glPushMatrix();
			glTranslatef(lastPos.pos.x, lastPos.pos.y, lastPos.pos.z);
			glRotatef(lastPos.angle, 0.f, 0.f, 1.f);
            carSprite->Draw(); //add flicker
            glPopMatrix();
             */
            
            {
                Vector3 pos = s_pRobotProgram->getAssumedFieldPosition();
                Vector3 angle = s_pRobotProgram->getAssumedFieldAngle();
                glPushMatrix();
                shift(pos,angle);
                glColor4f(1.0, 0.67, 0.067, 1.0);
                
                DrawFieldObject(s_pRobotModel->GetRobotFieldObject());
                
                glPopMatrix();    
            }

			
		}

		// draw the side screen
		{
			ScreenRectangle viewRect = s_3DViewScreenRect.InvertHeight(s_WindowRect.Height());
            glViewport(viewRect.left, viewRect.bottom, viewRect.Width(), viewRect.top - viewRect.bottom);
			glScissor(viewRect.left, viewRect.bottom, viewRect.Width(), viewRect.top - viewRect.bottom);

            glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			float fov_degrees = 60.0;
            const double aspectRatio = ((double)s_3DViewScreenRect.Width()) / ((double)s_3DViewScreenRect.Height());
			gluPerspective(fov_degrees, aspectRatio, 0.00001, 20.0);
			float fov_radians = fov_degrees * M_PI / 180.0;
			float eye_z = -3.f / tanf(fov_radians / 2.0);
			//glTranslatef(0.0, 0.0, eye_z);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0.0, 0.0, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

			glScalef(-1.f, 1.f, -1.f);

			glRotatef(s_3DViewRotations.x, 1.0, 0.0, 0.0);
			glRotatef(s_3DViewRotations.y, 0.0, 1.0, 0.0);
			glRotatef(s_3DViewRotations.z, 0.0, 0.0, 1.0);
			glScalef(s_3DViewScale, s_3DViewScale, s_3DViewScale);

			Draw3DAxis();

			glColor4f(1.f, 1.f, 1.f, 1.f);
			

			Draw3DFieldView(s_pSimulatedField, lastPos.pos);

		}

		SDL_mutexV(s_robotLock);

		SDL_GL_SwapBuffers ();

        // Time how long each draw-swap-delay cycle takes
        // and adjust delay to get closer to target framerate
        if (thenTicks > 0) {
            nowTicks = SDL_GetTicks ();
            delay += (1000/fps - (nowTicks-thenTicks));
            thenTicks = nowTicks;
            if (delay < 0)
                delay = 1000/fps;
        }
        else {
            thenTicks = SDL_GetTicks ();
        }

        SDL_Delay (delay);
	}

    delete carSprite;

    SDL_FreeSurface(surface_screen);

#if defined(__APPLE__)
    [pool drain];
#endif
    
    return 0;
}

SDL_Thread* SimulatorWindow::s_DisplayThread = NULL;

const SimulatedRobotModel*   SimulatorWindow::s_pRobotModel = NULL;
const RobotProgram*   SimulatorWindow::s_pRobotProgram = NULL;
const SimulatedField*   SimulatorWindow::s_pSimulatedField = NULL;
const SimulatorController* SimulatorWindow::s_pController=NULL;



void SimulatorWindow::StartGLScreen(bool fullscreen,
                                    const SimulatedRobotModel* pRobotModel,
                                    const RobotProgram* pRobotProgram,
                                    const SimulatedField* pSimulatedField,
                                    SimulatorController* pController)
{
    s_pRobotModel = pRobotModel;
    s_pSimulatedField = pSimulatedField;
    s_pController = pController;
    s_pRobotProgram = pRobotProgram;

    s_robotLock = SDL_CreateMutex();
    ResetView();

    s_DisplayThread = SDL_CreateThread(mainLoop, NULL);
    if ( s_DisplayThread == NULL ) {
        fprintf(stderr, "Unable to create display thread: %s\n", SDL_GetError());
        exit(-1);
    }
}

void SimulatorWindow::ResetView()
{
    SDL_mutexP(s_robotLock);
    s_robotMovements.clear();
    s_bRobotChanged = true;
    SDL_mutexV(s_robotLock);
}

void SimulatorWindow::FinishGLScreen()
{
    SDL_WaitThread(s_DisplayThread, NULL);
    SDL_DestroyMutex(s_robotLock);
    s_robotLock = NULL;
}

#include <iostream>
using namespace std;

void SimulatorWindow::UpdateRobot(const RobotPositionRecord& newRecord)
{
    SDL_mutexP(s_robotLock);

    RobotPositionRecord lastRec = *(s_robotMovements.rbegin());
    if (lastRec.PositionChanged(newRecord) || lastRec.AngleChanged(newRecord))
    {
        s_robotMovements.push_back(newRecord);
        s_bRobotChanged = true;
    }

    SDL_mutexV(s_robotLock);
}

void SimulatorWindow::RotateMouseMove(uint32_t x, uint32_t y, int32_t xrel, int32_t yrel)
{
    // if the mouse position is inside the window area we care about
    if (x >= s_3DViewScreenRect.left && x <= s_3DViewScreenRect.right && y >= s_3DViewScreenRect.top && y<= s_3DViewScreenRect.bottom)
    {
        SDL_mutexP(s_robotLock);
        s_3DViewRotations.z += (xrel * 0.5);
        s_3DViewRotations.x += (yrel * 0.5);
        s_bRobotChanged = true;
        SDL_mutexV(s_robotLock);
    }
}

void SimulatorWindow::ScaleMouseMove(uint32_t x, uint32_t y, int32_t scale)
{
    // if the mouse position is inside the window area we care about
    if (x >= s_3DViewScreenRect.left && x <= s_3DViewScreenRect.right && y >= s_3DViewScreenRect.top && y<= s_3DViewScreenRect.bottom)
    {
        SDL_mutexP(s_robotLock);
        s_3DViewScale *= 1.0 - (float)scale / 80.0;
        s_bRobotChanged = true;
        SDL_mutexV(s_robotLock);
    }
}

void SimulatorWindow::Reset3DView()
{
    SDL_mutexP(s_robotLock);
    s_3DViewScale = 1.f;
    s_3DViewRotations = Vector3();
    s_bRobotChanged = true;
    SDL_mutexV(s_robotLock);
}

void SimulatorWindow::ResizeWindow(uint32_t width, uint32_t height)
{
    s_desiredScreenWidth=width;
    s_bNeedScreenResize=true;
}

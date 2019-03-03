#pragma once
    #define GL_GLEXT_PROTOTYPES 1
  #define GLM_FORCE_RADIANS
  #define EPSILON 0.000001

  #define WINDOW_WIDTH  1024
  #define WINDOW_HEIGHT 768
  #define WINDOW_ASPECT_RATIO  WINDOW_WIDTH /  WINDOW_HEIGHT

  #define OPENGL_WORLD_COORDINATE_X 100.0/20.0
  #define OPENGL_WORLD_COORDINATE_Y 100.0/20.0
  #define OPENGL_WORLD_NEAR -1000.05 //negative working? hmm...
  #define OPENGL_WORLD_FAR 200.0


  #define UNILOC_ROTMATRIX  "rotMatrix"
  #define UNILOC_M  "m" //model matrix
  #define UNILOC_V  "v" //view matrix
  #define UNILOC_P  "p" //perspective matrix

  #define UNILOC_V_INV "v_inv"
  #define UNILOC_MV_INV "mv_inv"

  #define UNILOC_ALPHA "alpha"

  #define ATTRIB_VERTEX "vertex"
  #define ATTRIB_NORMAL "normal"

  //used for setting the axis of rotation
  #define XAXIS 0
  #define YAXIS 1
  #define ZAXIS 1

  //rotate the axis in this speed.
  #define ROT_SPEED 0.05
  #define ZOOM_FACTOR 5 

  //max size of array that OpenGLES2 can hande on raspberry Pi3
  //yes 65535 % 12 is not 0 -> thus 65544 - 12 is 65532, is 65532 % 12 = 0. 
  //Remember we have nine GLfloat per triangle and maxsize for unsigned short is 65535
  //before it overflows
  #define MAX_ARRAY_SIZE_OPENGLES2 65532

#include "Interface/IView.h"
#include "Interface/IComponent.h"


#include <QtWidgets/QOpenGLWidget>
//#include <GL/gl.h>
#include <QtOpenGL/qgl.h>
#include <QtGui/QOpenGLFunctions>
//#include <QtGui/QOpenGLExtraFunctions>

#include <QtGui/QOpenGLContext>

#include <QLabel>

#include "GLView.h"
#include "Geometry.h"
#include "polyset.h"
#include "CGAL_Nef_polyhedron.h"
#include "cgalutils.h"
#include "polyset-utils.h"
#include <algorithm>
#include <sstream>
#include "/usr/include/eigen3/Eigen/src/Core/IO.h"
#define USE_QOPENGLWIDGET


/*******
 *
 */

//remove heders later on...

	
#include <iostream>
#include <sstream>
#include <vector>

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>


#include <string>
#include <fstream>
#include <map>
#include <functional>

#include <stdio.h>
#include <unistd.h>

#include "Interface/openglshader.hpp"


#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include <typeinfo>

/*****
 * */

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

#include <QtWidgets/QApplication>

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>



class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class OpenGLWindow : private QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    virtual void changeSize();

    void setAnimating(bool animating);

    float refreshRate() { return screen()->refreshRate(); }

    void setFormat(QSurfaceFormat format) { QWindow::setFormat(format); }

    void create() { QWindow::create(); }

    void show() { QWindow::show(); }

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;

    int width;
    int height;

    qreal retinaScale;


};









class View: public QOpenGLWidget, public IView, public GLView, virtual protected QOpenGLFunctions
{
  
  Q_OBJECT
  Q_PROPERTY(bool showFaces READ showFaces WRITE setShowFaces);
	Q_PROPERTY(bool showEdges READ showEdges WRITE setShowEdges);
	Q_PROPERTY(bool showAxes READ showAxes WRITE setShowAxes);
	Q_PROPERTY(bool showCrosshairs READ showCrosshairs WRITE setShowCrosshairs);
	Q_PROPERTY(bool orthoMode READ orthoMode WRITE setOrthoMode);
	Q_PROPERTY(double showScaleProportional READ showScaleProportional WRITE setShowScaleProportional);


public:
  View(QWidget *parent = nullptr);
   
  ~View();
  
//From QOpenGLWidget
  void initializeGL();
  
  void resizeGL(int w, int h);
  
  void paintGL();

  
//From IView
  void Init(IComponent cmp) override;
  
  void ChangeWindowParameters(IComponent cmp) override;
  
  void Render(IComponent cmp) override;
  
  void RenderComponentGroup(IComponent cmp) override;
 
  void RenderAllComponents() override;
  
  void SwapBuffer() override;
 
  void DrawToOfflineBuffer(IComponent cmp) override;
 
  void SnapShotCurrentBuffer() override;
  
  void SnapShotOfflineBuffer() override;

  void ViewPreview() override;
  
  void ViewAll() override;
  
  void ViewZoomIn() override;
  
  void ViewZoomOut() override;
  
  void ResetView() override;
 
  void ViewLeft() override;
  
  void ViewRight() override;
  
  void ViewTop() override;
  
  void ViewBottom() override;
  
  void ViewFront() override;
 
  void ViewBack() override;
  
  void ViewDiagonal() override;
  
  void ViewCenter() override;
  
  void ViewPerspective() override;
  
  void ViewOrthogonal() override;
  
  void ViewSurface() override;
  
  void ViewWireframe() override;
 
  void Teardown() override;

  
private:
  void InitOpenGLProgram();

//many of these variables downwards will probably be integrated in a view component later on.
glm::mat4 rotMatrix;
glm::mat4 m;

glm::mat4 v_inv;
glm::mat4 mv_inv;
glm::mat4 scaleM;
glm::mat4 e_matrix;

glm::vec3 myRotationAxis;

float fieldOfView = glm::radians(45.0f);
float aspectRatio = 4.0f / 3.0f;
float nearClippingPlane = 0.1f;
float farClippingPlane = 1000.0f;

float angle = 0.0f;
float scale = 1.0;
float* alpha;
bool mouse_drag_active;

openglprogram program;

QOpenGLContext *m_context;

QWidget *parent;
QOpenGLWidget *wdgt;

//OpenGLWindow oglw; 



//from original view class in rotatingWheel
public:
  void normalizedDeviceCoordinates(int screenX, int screenY, float scale,
				 float* normalizedX, float* normalizedY, glm::vec3* normalizedCursorPosition);
  
  glm::mat4 getProjectionMatrix();
  glm::mat4 getViewMatrix();

private:
  int width;
  int height;

  glm::mat4 p;
  glm::mat4 v;
  
  glm::vec3 cameraPosition;
  glm::vec3 cameraTarget;
  glm::vec3 upVector;

  void init();

  void initMatrices();
  void setupCamera(); 

  //store vertices information for OpenGL rendering
  void storeVertices(std::vector<GLfloat>* locVertices, const Transform3d &m)
  {
    struct ObjectVertices* objVertices = new struct ObjectVertices();
    objVertices->m_ptr = locVertices->data();
    objVertices->m_size  = locVertices->size();
    objVertices->m_m  = glm::make_mat4(m.data());
    
    objectVertices3d.push_back(objVertices); 
  } 

  void storeNormals(std::vector<GLfloat>* locVertices)
  {
    struct ObjectVertices* objVertices = new struct ObjectVertices();
    struct std::vector<GLfloat>* localNormals = new std::vector<GLfloat>();

    std::vector<GLfloat>::iterator it;
    //compute normals for every triangle; every twelve floats represent one triangle (three vertices á four GLfloats each)
    assert(locVertices->size() % 12 == 0); //we want to have twelve floats for every triangle. 
    for(it=locVertices->begin(); it != locVertices->end(); it++)
    {
      glm::vec3 v1(*it, *++it, *++it);
      ++it; //skip every fourth value as the next vertex begins on pos 0, 4, 8, 12, ...
      glm::vec3 v2(*++it, *++it, *++it);
      ++it;
      glm::vec3 v3(*++it, *++it, *++it);
      ++it;
      
      glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
      
      //all normals are the same values due to they belong to the same triangle:
      //normal for first vertex
      localNormals->push_back(normal[0]);
      localNormals->push_back(normal[1]);
      localNormals->push_back(normal[2]);
      
      //normal for second vertex
      localNormals->push_back(normal[0]);
      localNormals->push_back(normal[1]);
      localNormals->push_back(normal[2]);

      //normal for third vertex
      localNormals->push_back(normal[0]);
      localNormals->push_back(normal[1]);
      localNormals->push_back(normal[2]);
    }
    
    
    objVertices->m_ptr = localNormals->data();
    objVertices->m_size = localNormals->size();
    
    objectNormals3d.push_back(objVertices);
  }


//from OpenGLWindow
public:

    void initialize();
    void render();

public:
    static View* m_viewInstance;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QMatrix4x4 matrix;
    
    GLfloat *colors;
    GLfloat *vertices;
    unsigned int vertices_count;

    struct ObjectVertices
    {
      GLfloat* m_ptr;
      unsigned short m_size;
      glm::mat4 m_m; //models matrix for local vertices

      ObjectVertices(): 
      m_ptr(nullptr),
      m_size(0),
      m_m(glm::mat4(1.0))
      { }
    };

    std::vector<struct ObjectVertices*> objectVertices3d;
    std::vector<struct ObjectVertices*> objectNormals3d;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    float refreshRate;

  
  /*
   * Functions downwards may be removed or rewritten for the new class that will replace QGLView.
   * 
   * 
   * 
   */


public:

#ifdef ENABLE_OPENCSG
	bool hasOpenCSGSupport() { return this->opencsg_support=true; }
#endif
	// Properties
	bool showFaces() const { return this->showfaces; }
	void setShowFaces(bool enabled) { this->showfaces = enabled; }
	bool showEdges() const { return this->showedges; }
	void setShowEdges(bool enabled) { this->showedges = enabled; }
	bool showAxes() const { return this->showaxes; }
	void setShowAxes(bool enabled) { this->showaxes = enabled; }
	bool showCrosshairs() const { return this->showcrosshairs; }
	void setShowCrosshairs(bool enabled) { this->showcrosshairs = enabled; }
	bool orthoMode() const { return true; }
	void setOrthoMode(bool enabled);
	bool showScaleProportional() const { return this->showscale; }
	void setShowScaleProportional(bool enabled) { this->showscale = enabled; }
	std::string getRendererInfo() const override;
  
#if QT_VERSION >= 0x050100
	float getDPI() override { return this->QPaintDevice::devicePixelRatio(); }
#endif
	
	const QImage & grabFrame();
	bool save(const char *filename) override;
        void clearVertices();
        void setVertices(shared_ptr<const Geometry> root_geom, const Transform3d &m, bool clVerts);

public slots:
	void ZoomIn(void);
	void ZoomOut(void);
        void setVertices(shared_ptr<const Geometry> root_geom);

#ifdef USE_QOPENGLWIDGET
	void updateGL() { update(); }
#endif

public:
  QLabel *statusLabel;
#ifdef USE_QOPENGLWIDGET
	inline QImage grabFrameBuffer() { return grabFramebuffer(); }
#endif
private:
  bool opencsg_support;
  
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

	void normalizeAngle(GLdouble& angle);

#ifdef ENABLE_OPENCSG
	void display_opencsg_warning() override;
private slots:
	void display_opencsg_warning_dialog();
#endif

signals:
	void doAnimateUpdate();

};

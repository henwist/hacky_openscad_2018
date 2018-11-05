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
  #define YAXIS 0
  #define ZAXIS 1

  //rotate the axis in this speed.
  #define ROT_SPEED 0.05
  #define ZOOM_FACTOR 5 


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





//from OpenGLWindow
public:

    void initialize();
    void render();

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QMatrix4x4 matrix;
    GLfloat *vertices;
    GLfloat *colors;

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

public slots:
	void ZoomIn(void);
	void ZoomOut(void);
#ifdef USE_QOPENGLWIDGET
	inline void updateGL() { update(); }
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


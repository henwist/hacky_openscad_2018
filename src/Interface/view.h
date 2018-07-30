#pragma once

#include "IView.h"
#include "IComponent.h"

#include <QtWidgets/QOpenGLWidget>


class View:  public QOpenGLWidget, public IView 
{
  
  Q_OBJECT
  
public:
  
  View(QWidget *parent = nullptr);
  
  ~View();
  
//From QOpenGLWidget
  void initializeGL() override;
  
  void resizeGL(int w, int h) override;
  
  void paintGL() override;

  
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
  
  void ZoomIn() override;
  
  void ZoomOut() override;
  
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
  
  
  /*
   * Functions downwards may be removed or rewritten for the new class that will replace QGLView.
   * 
   * 
   * 
   */

std::string getRendererInfo() const;

#ifdef ENABLE_OPENCSG

void display_opencsg_warning();
void display_opencsg_warning_dialog();

#endif


void mousePressEvent(QMouseEvent *event);

void mouseDoubleClickEvent (QMouseEvent *event);

void normalizeAngle(GLdouble& angle);

void mouseMoveEvent(QMouseEvent *event);

void mouseReleaseEvent(QMouseEvent*);

const QImage & grabFrame();

bool save(const char *filename);

void wheelEvent(QWheelEvent *event);

void setOrthoMode(bool enabled);

};


/*Interface that can be used for the View part to be able to change implementation
*in a standardized way later on if needed.
*/

#include "IComponent.h"

class IView
{
public:
  
  IView() { }
  
  virtual void Init(IComponent cmp)=0;
  
  virtual void ChangeWindowParameters(IComponent cmp)=0;
  
  virtual void Render(IComponent cmp)=0;
  
  virtual void RenderComponentGroup(IComponent cmp)=0;
 
  virtual void RenderAllComponents()=0;
  
  virtual void SwapBuffer()=0;
 
  virtual void DrawToOfflineBuffer(IComponent cmp)=0;
 
  virtual void SnapShotCurrentBuffer()=0;
  
  virtual void SnapShotOfflineBuffer()=0;

  virtual void ViewPreview()=0;
  
  virtual void ViewAll()=0;
  
  virtual void ViewZoomIn()=0;
  
  virtual void ViewZoomOut()=0;
  
  virtual void ResetView()=0;
 
  virtual void ViewLeft()=0;
  
  virtual void ViewRight()=0;
  
  virtual void ViewTop()=0;
  
  virtual void ViewBottom()=0;
  
  virtual void ViewFront()=0;
 
  virtual void ViewBack()=0;
  
  virtual void ViewDiagonal()=0;
  
  virtual void ViewCenter()=0;
  
  virtual void ViewPerspective()=0;
  
  virtual void ViewOrthogonal()=0;
  
  virtual void ViewSurface()=0;
  
  virtual void ViewWireframe()=0;
 
  virtual void Teardown()=0;
  
  virtual ~IView() { }
  
};

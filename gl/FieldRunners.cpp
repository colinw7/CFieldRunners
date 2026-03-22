#include <FieldRunners.h>

#include <Canvas.h>

namespace CQGLFieldRunners {

FieldRunners::
FieldRunners(Canvas *canvas) :
 canvas_(canvas)
{
}

CFieldRunners::Tank *
FieldRunners::
createTank()
{
  (void) canvas_->getTankObj();

  return CFieldRunners::createTank();
}

void
FieldRunners::
stateChanged()
{
  canvas_->updateState();
}

}

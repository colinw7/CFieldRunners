#include <App.h>
#include <Canvas.h>
#include <Toolbar.h>

#include <svg/camera_svg.h>
#include <svg/grid_svg.h>

#include <QVBoxLayout>

namespace CQGLFieldRunners {

App::
App()
{
  auto *layout = new QVBoxLayout(this);

  toolbar_ = new Toolbar(this);

  canvas_ = new Canvas(this);

  layout->addWidget(toolbar_);
  layout->addWidget(canvas_);
}

void
App::
init()
{
  canvas_->init();
}

}

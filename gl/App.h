#ifndef App_H
#define App_H

#include <QFrame>

namespace CQGLFieldRunners {

class Canvas;
class Toolbar;

class App : public QFrame {
  Q_OBJECT

 public:
  App();

  void init();

  Canvas *canvas() const { return canvas_; }

 private:
  Canvas*  canvas_  { nullptr };
  Toolbar* toolbar_ { nullptr };
};

}

#endif

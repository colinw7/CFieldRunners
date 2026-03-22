#ifndef Toolbar_H
#define Toolbar_H

#include <CFieldRunners.h>

#include <QFrame>

class CQIconButton;

class QComboBox;

namespace CQGLFieldRunners {

class App;

class Toolbar : public QFrame {
  Q_OBJECT

 public:
  Toolbar(App *app);

 private Q_SLOTS:
  void cameraSlot(bool b);
  void gridSlot(bool b);

  void mapSlot(int ind);
  void runnerSlot(int ind);
  void emitSlot();

 private:
  App* app_ { nullptr };

  CQIconButton* cameraButton_ { nullptr };
  CQIconButton* gridButton_   { nullptr };
  QComboBox*    mapCombo_     { nullptr };
  QComboBox*    runnerCombo_  { nullptr };

  CFieldRunners::RunnerType runnerType_ { CFieldRunners::RunnerType::NONE };
};

}

#endif

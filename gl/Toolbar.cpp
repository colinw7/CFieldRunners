#include <Toolbar.h>
#include <App.h>
#include <Canvas.h>

#include <CQIconButton.h>

#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace CQGLFieldRunners {

Toolbar::
Toolbar(App *app) :
 app_(app)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  auto *layout = new QHBoxLayout(this);

  //---

  auto addToolButton = [&](const QString &name, const QString &iconName, const QString &tip) {
    auto *button = new CQIconButton;

    button->setFocusPolicy(Qt::NoFocus);

    button->setObjectName(name);
    button->setIcon(iconName);
    button->setIconSize(QSize(32, 32));
    button->setAutoRaise(true);
    button->setToolTip(tip);

    layout->addWidget(button);

    return button;
  };

  auto addCheckButton = [&](const QString &name, const QString &iconName, const QString &tip) {
    auto *button = addToolButton(name, iconName, tip);

    button->setCheckable(true);

    //checkButtons_.push_back(button);

    return button;
  };

  //---

  cameraButton_ = addCheckButton("camera", "CAMERA", "Camera");
  gridButton_   = addCheckButton("grid"  , "GRID"  , "Grid"  );

  connect(cameraButton_, SIGNAL(toggled(bool)), this, SLOT(cameraSlot(bool)));
  connect(gridButton_  , SIGNAL(toggled(bool)), this, SLOT(gridSlot(bool)));

  //---

  layout->addStretch(1);

  mapCombo_ = new QComboBox(this);
  mapCombo_->addItems(QStringList() <<
    "grasslands.map"    <<
    "crossroads.map"    <<
    "drylands.map"      <<
    "crystal_caves.map" <<
    "skyway.map"        <<
    "frostbite.map"     <<
    "mudslide.map"      <<
    "lavaflow.map"      <<
    "test.map");

  mapCombo_->setFocusPolicy(Qt::NoFocus);

  connect(mapCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(mapSlot(int)));

  layout->addWidget(mapCombo_);

  //---

  runnerCombo_ = new QComboBox(this);
  runnerCombo_->addItems(QStringList() <<
    "none"       <<
    "soldier"    <<
    "mercenary"  <<
    "motorbike"  <<
    "car"        <<
    "heavybike"  <<
    "tank"       <<
    "helicopter" <<
    "plane"      <<
    "train"      <<
    "blimp");

  runnerCombo_->setFocusPolicy(Qt::NoFocus);

  connect(runnerCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(runnerSlot(int)));

  layout->addWidget(runnerCombo_);

  auto *emitButton = new QPushButton("Emit");

  layout->addWidget(emitButton);

  connect(emitButton, SIGNAL(clicked()), this, SLOT(emitSlot()));
}

void
Toolbar::
cameraSlot(bool b)
{
  auto *canvas = app_->canvas();

  canvas->setInputType(b ? Canvas::InputType::CAMERA : Canvas::InputType::PLAYER);
}

void
Toolbar::
gridSlot(bool b)
{
  auto *canvas = app_->canvas();

  canvas->setGridVisible(b);

  canvas->update();
}

void
Toolbar::
mapSlot(int ind)
{
  auto *canvas = app_->canvas();

  switch (ind) {
    case 0: canvas->loadMap("grasslands.map"   ); break;
    case 1: canvas->loadMap("crossroads.map"   ); break;
    case 2: canvas->loadMap("drylands.map"     ); break;
    case 3: canvas->loadMap("crystal_caves.map"); break;
    case 4: canvas->loadMap("skyway.map"       ); break;
    case 5: canvas->loadMap("frostbite.map"    ); break;
    case 6: canvas->loadMap("mudslide.map"     ); break;
    case 7: canvas->loadMap("lavaflow.map"     ); break;
    case 8: canvas->loadMap("test.map"         ); break;
    default: break;
  }
}

void
Toolbar::
runnerSlot(int ind)
{
  switch (ind) {
    default:
    case  0: runnerType_ = CFieldRunners::RunnerType::NONE;       break;
    case  1: runnerType_ = CFieldRunners::RunnerType::SOLDIER;    break;
    case  2: runnerType_ = CFieldRunners::RunnerType::MERCENARY;  break;
    case  3: runnerType_ = CFieldRunners::RunnerType::MOTORBIKE;  break;
    case  4: runnerType_ = CFieldRunners::RunnerType::CAR;        break;
    case  5: runnerType_ = CFieldRunners::RunnerType::HEAVYBIKE;  break;
    case  6: runnerType_ = CFieldRunners::RunnerType::TANK;       break;
    case  7: runnerType_ = CFieldRunners::RunnerType::HELICOPTER; break;
    case  8: runnerType_ = CFieldRunners::RunnerType::PLANE;      break;
    case  9: runnerType_ = CFieldRunners::RunnerType::TRAIN;      break;
    case 10: runnerType_ = CFieldRunners::RunnerType::BLIMP;      break;
  }
}

void
Toolbar::
emitSlot()
{
  if (runnerType_ == CFieldRunners::RunnerType::NONE)
    return;

  auto *canvas = app_->canvas();

  auto *fieldRunners = canvas->fieldRunners();

  fieldRunners->emitRunner(runnerType_);
}

}

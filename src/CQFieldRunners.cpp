#ifdef USE_CQ_APP
#  include <CQApp.h>
#else
#  include <QApplication>
#endif

#include <CQFieldRunners.h>
#include <CFieldRunners.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

enum { NUM_ROWS  = 15 };
enum { NUM_COLS  = 26 };
enum { CELL_SIZE = 96 };

class CQFieldRunnersWindow;

class CQFieldRunners : public CFieldRunners {
 public:
  CQFieldRunners(CQFieldRunnersWindow *window);

  CQFieldRunnersWindow *qWindow() const { return qWindow_; }

  Field *createField() override;

  Soldier *createSoldier() override;

  Mercenary *createMercenary() override;

  Car *createCar() override;

  Tank *createTank() override;

  Plane *createPlane() override;

  Train *createTrain() override;

  EmptyCell    *createEmptyCell   (const CellPos &pos) override;
  BorderCell   *createBorderCell  (const CellPos &pos) override;
  GunCell      *createGunCell     (const CellPos &pos) override;
  GlueCell     *createGlueCell    (const CellPos &pos) override;
  SnowbombCell *createSnowbombCell(const CellPos &pos) override;
  MissileCell  *createMissileCell (const CellPos &pos) override;
  ZapCell      *createZapCell     (const CellPos &pos) override;
  BlockCell    *createBlockCell   (const CellPos &pos) override;

  Entrance *createEntrance(const CellPos &pos) override;
  Exit     *createExit    (const CellPos &pos) override;

  Rocket *createRocket(const Point &point, Runner *runner, Orient orient) override;

  void drawBg(const BBox &bbox);

  void drawHealthBar(int x, int y, double percent) override;

  void drawMoney(int money) override;
  void drawLives(int lives) override;
  void drawScore(int score) override;
  void drawLevel(int level) override;

 public:
  friend class CQFieldRunnersField;
  friend class CQFieldRunnersSoldier;
  friend class CQFieldRunnersMercenary;
  friend class CQFieldRunnersCar;
  friend class CQFieldRunnersTank;
  friend class CQFieldRunnersPlane;
  friend class CQFieldRunnersTrain;

  friend class CQFieldRunnersEmpty;
  friend class CQFieldRunnersBorder;
  friend class CQFieldRunnersGun;
  friend class CQFieldRunnersGlue;
  friend class CQFieldRunnersSnowbomb;
  friend class CQFieldRunnersMissile;
  friend class CQFieldRunnersZap;
  friend class CQFieldRunnersBlock;

  friend class CQFieldRunnersEntrance;
  friend class CQFieldRunnersExit;

  friend class CQFieldRunnersRocket;

 private:
  CQFieldRunnersWindow *qWindow_ { nullptr };
};

class CQFieldRunnersField : public CQFieldRunners::Field {
 public:
  CQFieldRunnersField(CQFieldRunners *qFieldRunners);

  CQFieldRunners *qFieldRunners() const { return qFieldRunners_; }

  void draw() override;

  void drawWeaponPrice(CFieldRunners::CellType cellType, int x1, int y1, int w, int h) override;

  void drawPriceBg(bool current, int x1, int y1, int w, int h);

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersSoldier : public CQFieldRunners::Soldier {
 public:
  CQFieldRunnersSoldier(CQFieldRunners *qFieldRunners);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersMercenary : public CQFieldRunners::Mercenary {
 public:
  CQFieldRunnersMercenary(CQFieldRunners *qFieldRunners);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersCar : public CQFieldRunners::Car {
 public:
  CQFieldRunnersCar(CQFieldRunners *fieldRunners);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersTank : public CQFieldRunners::Tank {
 public:
  CQFieldRunnersTank(CQFieldRunners *fieldRunners);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersPlane : public CQFieldRunners::Plane {
 public:
  CQFieldRunnersPlane(CQFieldRunners *fieldRunners);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersTrain : public CQFieldRunners::Train {
 public:
  CQFieldRunnersTrain(CQFieldRunners *fieldRunners);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersEmpty : public CQFieldRunners::EmptyCell {
 public:
  CQFieldRunnersEmpty(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersBorder : public CQFieldRunners::BorderCell {
 public:
  CQFieldRunnersBorder(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersGun : public CQFieldRunners::GunCell {
 public:
  CQFieldRunnersGun(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersGlue : public CQFieldRunners::GlueCell {
 public:
  CQFieldRunnersGlue(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersSnowbomb : public CQFieldRunners::SnowbombCell {
 public:
  CQFieldRunnersSnowbomb(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersMissile : public CQFieldRunners::MissileCell {
 public:
  CQFieldRunnersMissile(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersZap : public CQFieldRunners::ZapCell {
 public:
  CQFieldRunnersZap(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersBlock : public CQFieldRunners::BlockCell {
 public:
  CQFieldRunnersBlock(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersEntrance : public CQFieldRunners::Entrance {
 public:
  CQFieldRunnersEntrance(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersExit : public CQFieldRunners::Exit {
 public:
  CQFieldRunnersExit(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos);

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersRocket : public CQFieldRunners::Rocket {
 public:
  CQFieldRunnersRocket(CQFieldRunners *qFieldRunners, const Point &point,
                       CQFieldRunners::Runner *runner, CFieldRunners::Orient orient);

  void draw() override;

 private:
  CQFieldRunners *qFieldRunners_;
};

class CQFieldRunnersWindow : public CFieldRunnersWindow {
 public:
  CQFieldRunnersWindow(CQFieldRunnersApp *fieldRunners, CQFieldRunnersCanvas *canvas) :
   fieldRunners_(fieldRunners), canvas_(canvas), imageId_(0) {
  }

 ~CQFieldRunnersWindow() { }

  CQFieldRunnersApp *app() const { return fieldRunners_; }

  CQFieldRunnersCanvas *canvas() const { return canvas_; }

  void getSize(Size &size) override {
    size.width  = canvas_->width ();
    size.height = canvas_->height();
  }

  void fill(const Color &color) override {
    canvas_->fill(canvas_->mapColor(color));
  }

  void setBackground(const Color &color) override {
    canvas_->setBackground(canvas_->mapColor(color));
  }

  void setForeground(const Color &color) override {
    canvas_->setForeground(canvas_->mapColor(color));
  }

  void drawPoint(int x, int y) override {
    canvas_->drawPoint(x, y);
  }

  void drawLine(int x1, int y1, int x2, int y2) override {
    canvas_->drawLine(x1, y1, x2, y2);
  }

  void drawRectangle(int x1, int y1, int x2, int y2) override {
    canvas_->drawRectangle(x1, y1, x2, y2);
  }

  void fillRectangle(int x1, int y1, int x2, int y2) override {
    canvas_->fillRectangle(x1, y1, x2, y2);
  }

  void drawImage(int x, int y, ImageId imageId) override {
    auto image = imageMap_[imageId];

    canvas_->drawImage(x, y, image);
  }

  void drawCellImage(int x, int y, ImageId imageId) override {
    auto image = imageMap_[imageId];

    int dy = CELL_SIZE - image.height();

    canvas_->drawImage(x, y + dy, image);
  }

  void drawText(int x, int y, const char *text) override {
    canvas_->drawText(x, y, text);
  }

  ImageId loadImage(const uchar *data, uint len) override {
    QImage image;

    if (! image.loadFromData(data, len, "png"))
      std::cerr << "load Failed" << std::endl;

    ImageId imageId = ++imageId_;

    imageMap_[imageId] = image;

    return imageId;
  }

 private:
  typedef std::map<ImageId, QImage> ImageMap;

  CQFieldRunnersApp    *fieldRunners_ { nullptr };
  CQFieldRunnersCanvas *canvas_ { nullptr };
  QColor                fg_;
  ImageId               imageId_;
  ImageMap              imageMap_;
};

//----------

int
main(int argc, char **argv)
{
#ifdef USE_CQ_APP
  CQApp app(argc, argv);
#else
  QApplication app(argc, argv);
#endif

  auto *fieldRunners = new CQFieldRunnersApp;

  std::string mapFile;

  for (int i = 1; i < argc; ++i) {
    auto arg = std::string(argv[i]);

    if (arg == "-map") {
      ++i;

      if (i < argc)
        mapFile = std::string(argv[i]);
      else
        std::cerr << "Missing map filename\n";
    }
  }

  if (mapFile != "")
    fieldRunners->fieldRunners()->loadMap(mapFile);

  fieldRunners->show();

  return app.exec();
}

//----------

CQFieldRunnersApp::
CQFieldRunnersApp() :
 QFrame(nullptr)
{
  auto font = this->font();
  font.setPointSizeF(32);
  this->setFont(font);

  resize(CELL_SIZE*NUM_COLS, CELL_SIZE*NUM_ROWS);

  auto *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  canvas_ = new CQFieldRunnersCanvas(this);

  layout->addWidget(canvas_);

  status_ = new QFrame(this);

  status_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  auto *statusLayout = new QHBoxLayout(status_);

  statusLabel_ = new QLabel;

  statusLayout->addWidget(statusLabel_);

  layout->addWidget(status_);

  window_ = new CQFieldRunnersWindow(this, canvas_);

  fieldRunners_ = new CQFieldRunners(window_);

  fieldRunners_->init();

  fieldRunners_->build(NUM_ROWS, NUM_COLS);

  timer_ = new QTimer;

  connect(timer_, SIGNAL(timeout()), this, SLOT(timerSlot()));

  timer_->start(100);
}

CQFieldRunnersApp::
~CQFieldRunnersApp()
{
  delete window_;
  delete fieldRunners_;
}

void
CQFieldRunnersApp::
timerSlot()
{
  fieldRunners_->update();

  fieldRunners_->draw();

  canvas_->setChanged();
}

void
CQFieldRunnersApp::
setCurrentPos(const Pos &pos)
{
  if (pos != currentPos_) {
    currentPos_ = pos;

    auto msg = QString("Row=%1 Col=%2").arg(pos.row).arg(pos.col);

    auto pos = CFieldRunners::CellPos(currentPos_.row, currentPos_.col);

    if (fieldRunners_->isInside(pos)) {
      CFieldRunners::Cell *cell;
      fieldRunners_->getCell(pos, &cell);

      if (cell)
        msg += " " + QString::fromStdString(cell->desc());
    }

    setStatus(msg);
  }
}

void
CQFieldRunnersApp::
setStatus(const QString &text)
{
  statusLabel_->setText(text);
}

void
CQFieldRunnersApp::
mousePress(QMouseEvent *event)
{
  int x = event->x();
  int y = event->y();

  CFieldRunners::CellPos pos;

  Point point(x, y);

  fieldRunners_->pixelToPos(point, pos);

  if (! fieldRunners_->isInside(pos))
    return;

  CFieldRunners::Cell *mouseCell;
  fieldRunners_->getCell(pos, &mouseCell);

  //---

  if      (event->button() == Qt::LeftButton) {
    if (! mouseCell || ! mouseCell->canAddWeapon())
      return;

    fieldRunners_->buyCell(pos);

    canvas_->setChanged();
  }
  else if (event->button() == Qt::RightButton) {
    if (mouseCell && mouseCell->isWeapon()) {
      fieldRunners_->sellCell(pos);

      canvas_->setChanged();
    }
  }
  else if (event->button() == Qt::MiddleButton) {
    if (! mouseCell || ! mouseCell->canAddWeapon())
      return;

    fieldRunners_->addBlockCell(pos);

    canvas_->setChanged();
  }
}

void
CQFieldRunnersApp::
keyPress(QKeyEvent *event)
{
  if      (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
    uint ind = (event->key() - Qt::Key_1);

    if (ind >= fieldRunners_->getWeapons().size())
      return;

    auto weapon = fieldRunners_->getWeapons()[ind];

    fieldRunners_->setBuyCellType(weapon);
  }
  else if (event->key() == Qt::Key_P)
    fieldRunners_->setPaused(! fieldRunners_->isPaused());
}

//----------

CQFieldRunnersCanvas::
CQFieldRunnersCanvas(CQFieldRunnersApp *app) :
 CQCanvas2D(nullptr), app_(app)
{
  setMouseTracking(true);
}

CQFieldRunnersCanvas::
~CQFieldRunnersCanvas()
{
}

void
CQFieldRunnersCanvas::
mousePressEvent(QMouseEvent *event)
{
  app_->mousePress(event);
}

void
CQFieldRunnersCanvas::
mouseMoveEvent(QMouseEvent *event)
{
  int x = event->x();
  int y = event->y();

  CFieldRunners::CellPos pos;

  Point point(x, y);

  app_->fieldRunners()->pixelToPos(point, pos);

  app_->setCurrentPos(CQFieldRunnersApp::Pos(pos.row, pos.col));
}

void
CQFieldRunnersCanvas::
keyPressEvent(QKeyEvent *event)
{
  app_->keyPress(event);
}

void
CQFieldRunnersCanvas::
fill(QColor c)
{
  CQCanvas2D::fill(c);
}

void
CQFieldRunnersCanvas::
setForeground(QColor fg)
{
  auto *p = getPainter();

  if (p)
    p->setPen(fg);
}

void
CQFieldRunnersCanvas::
setBackground(QColor bg)
{
  auto *p = getPainter();

  if (p)
    p->setBrush(bg);
}

void
CQFieldRunnersCanvas::
drawPoint(int x, int y)
{
  auto *p = getPainter();

  if (p)
    p->drawPoint(x, y);
}

void
CQFieldRunnersCanvas::
drawLine(int x1, int y1, int x2, int y2)
{
  auto *p = getPainter();

  if (p)
    p->drawLine(x1, y1, x2, y2);
}

void
CQFieldRunnersCanvas::
drawRectangle(int x1, int y1, int x2, int y2)
{
  auto *p = getPainter();

  if (p)
    p->drawRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

void
CQFieldRunnersCanvas::
fillRectangle(int x1, int y1, int x2, int y2)
{
  auto *p = getPainter();

  if (p) {
    QBrush brush;

    brush.setColor(p->pen().color());
    brush.setStyle(Qt::SolidPattern);

    p->fillRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, brush);
  }
}

void
CQFieldRunnersCanvas::
drawEllipse(int x1, int y1, int x2, int y2)
{
  auto *p = getPainter();

  if (p)
    p->drawEllipse(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

void
CQFieldRunnersCanvas::
drawImage(int x, int y, const QImage &image)
{
  auto *p = getPainter();

  if (p)
    p->drawImage(x, y, image);
}

void
CQFieldRunnersCanvas::
setFont(const QFont &font)
{
  auto *p = getPainter();

  if (p)
    p->setFont(font);
}

void
CQFieldRunnersCanvas::
drawContrastText(int x, int y, const QString &text, const QColor &bg, const QColor &fg)
{
  setForeground(bg);

  int d = 2;

  for (int dy = -d; dy <= d; ++dy)
    for (int dx = -d; dx <= d; ++dx)
      drawText(x + dx, y + dx, text);

  setForeground(fg);

  drawText(x, y, text);
}

void
CQFieldRunnersCanvas::
drawText(int x, int y, const char *text)
{
  drawText(x, y, QString(text));
}

void
CQFieldRunnersCanvas::
drawText(int x, int y, const QString &text)
{
  auto *p = getPainter();

  if (p)
    p->drawText(x, y, text);
}

QColor
CQFieldRunnersCanvas::
mapColor(const Color &color) const
{
  return QColor(color.r*255, color.g*255, color.b*255, color.a*255);
}

//------

CQFieldRunners::
CQFieldRunners(CQFieldRunnersWindow *qWindow) :
 CFieldRunners(qWindow), qWindow_(qWindow)
{
}

CFieldRunners::Field *
CQFieldRunners::
createField()
{
  return new CQFieldRunnersField(this);
}

CFieldRunners::Soldier *
CQFieldRunners::
createSoldier()
{
  return new CQFieldRunnersSoldier(this);
}

CFieldRunners::Mercenary *
CQFieldRunners::
createMercenary()
{
  return new CQFieldRunnersMercenary(this);
}

CFieldRunners::Car *
CQFieldRunners::
createCar()
{
  return new CQFieldRunnersCar(this);
}

CFieldRunners::Tank *
CQFieldRunners::
createTank()
{
  return new CQFieldRunnersTank(this);
}

CFieldRunners::Plane *
CQFieldRunners::
createPlane()
{
  return new CQFieldRunnersPlane(this);
}

CFieldRunners::Train *
CQFieldRunners::
createTrain()
{
  return new CQFieldRunnersTrain(this);
}

CFieldRunners::EmptyCell *
CQFieldRunners::
createEmptyCell(const CellPos &pos)
{
  return new CQFieldRunnersEmpty(this, pos);
}

CFieldRunners::BorderCell *
CQFieldRunners::
createBorderCell(const CellPos &pos)
{
  return new CQFieldRunnersBorder(this, pos);
}

CFieldRunners::GunCell *
CQFieldRunners::
createGunCell(const CellPos &pos)
{
  return new CQFieldRunnersGun(this, pos);
}

CFieldRunners::GlueCell *
CQFieldRunners::
createGlueCell(const CellPos &pos)
{
  return new CQFieldRunnersGlue(this, pos);
}

CFieldRunners::SnowbombCell *
CQFieldRunners::
createSnowbombCell(const CellPos &pos)
{
  return new CQFieldRunnersSnowbomb(this, pos);
}

CFieldRunners::MissileCell *
CQFieldRunners::
createMissileCell(const CellPos &pos)
{
  return new CQFieldRunnersMissile(this, pos);
}

CFieldRunners::ZapCell *
CQFieldRunners::
createZapCell(const CellPos &pos)
{
  return new CQFieldRunnersZap(this, pos);
}

CFieldRunners::BlockCell *
CQFieldRunners::
createBlockCell(const CellPos &pos)
{
  return new CQFieldRunnersBlock(this, pos);
}

CFieldRunners::Entrance *
CQFieldRunners::
createEntrance(const CellPos &pos)
{
  return new CQFieldRunnersEntrance(this, pos);
}

CFieldRunners::Exit *
CQFieldRunners::
createExit(const CellPos &pos)
{
  return new CQFieldRunnersExit(this, pos);
}

CFieldRunners::Rocket *
CQFieldRunners::
createRocket(const Point &point, Runner *runner, Orient orient)
{
  return new CQFieldRunnersRocket(this, point, runner, orient);
}

#include <svg/empty_svg.h>
#include <svg/stone_svg.h>
#include <svg/grasslands_bg_svg.h>
#include <svg/crystal_bg_svg.h>
#include <svg/crossroads_bg_svg.h>
#include <svg/frostbite_bg_svg.h>
#include <svg/drylands_bg_svg.h>
#include <svg/lavaflow_bg_svg.h>
#include <svg/skyway_bg_svg.h>

void
CQFieldRunners::
drawBg(const BBox &bbox)
{
  QImage image;

  auto w = bbox.width ();
  auto h = bbox.height();

  if      (bgType() == BgType::GRASSLANDS)
    image = s_GRASSLANDS_BG_SVG.image(w, h);
  else if (bgType() == BgType::STONE)
    image = s_STONE_SVG.image(w, h);
  else if (bgType() == BgType::CRYSTAL)
    image = s_CRYSTAL_BG_SVG.image(w, h);
  else if (bgType() == BgType::CROSSROADS)
    image = s_CROSSROADS_BG_SVG.image(w, h);
  else if (bgType() == BgType::FROSTBITE)
    image = s_FROSTBITE_BG_SVG.image(w, h);
  else if (bgType() == BgType::DRYLANDS)
    image = s_DRYLANDS_BG_SVG.image(w, h);
  else if (bgType() == BgType::LAVAFLOW)
    image = s_LAVAFLOW_BG_SVG.image(w, h);
  else if (bgType() == BgType::SKYWAY)
    image = s_SKYWAY_BG_SVG.image(w, h);
  else
    image = s_EMPTY_SVG.image(w, h);

  auto *canvas = qWindow()->canvas();

  canvas->drawImage(bbox.xmin, bbox.ymin, image);
}

void
CQFieldRunners::
drawHealthBar(int x, int y, double percent)
{
  auto *canvas = qWindow()->canvas();

  Size cellSize;
  getCellSize(cellSize);

  int healthWidth  = cellSize.width;
  int healthHeight = std::max(cellSize.height/10, 3);

  canvas->setForeground(QColor(255, 255, 255));
  canvas->drawRectangle(x + 2, y, x + healthWidth, y + healthHeight);

  canvas->setForeground(QColor(255, 0, 0));
  canvas->fillRectangle(x + 2, y, x + int(healthWidth*percent), y + healthHeight);
}

void
CQFieldRunners::
drawMoney(int money)
{
  auto *canvas = qWindow()->canvas();

  QFontMetrics fm(qWindow_->app()->font());

  auto text = QString("$%1").arg(money);

  int x = 4;
  int y = fm.ascent();

  canvas->setFont(qWindow_->app()->font());

  canvas->drawContrastText(x, y, text, canvas->mapColor(bgColor()), canvas->mapColor(fgColor()));
}

void
CQFieldRunners::
drawLives(int lives)
{
  auto *canvas = qWindow()->canvas();

  QFontMetrics fm(qWindow_->app()->font());

  auto text = QString("Lives %1").arg(lives);

  int x = canvas->width()/2 - fm.horizontalAdvance(text)/2;
  int y = fm.ascent();

  canvas->setFont(qWindow_->app()->font());

  canvas->drawContrastText(x, y, text, canvas->mapColor(bgColor()), canvas->mapColor(fgColor()));
}

void
CQFieldRunners::
drawScore(int score)
{
  auto *canvas = qWindow()->canvas();

  QFontMetrics fm(qWindow_->app()->font());

  auto text = QString("Score %1").arg(score);

  int x = canvas->width() - fm.horizontalAdvance(text) - 4;
  int y = fm.ascent();

  canvas->setFont(qWindow_->app()->font());

  canvas->drawContrastText(x, y, text, canvas->mapColor(bgColor()), canvas->mapColor(fgColor()));
}

void
CQFieldRunners::
drawLevel(int level)
{
  auto *canvas = qWindow()->canvas();

  QFontMetrics fm(qWindow_->app()->font());

  auto text = QString("Level %1").arg(level);

  int x = canvas->width()/2 - fm.horizontalAdvance(text)/2;
  int y = fm.height() + fm.ascent();

  canvas->setFont(qWindow_->app()->font());

  canvas->drawContrastText(x, y, text, canvas->mapColor(bgColor()), canvas->mapColor(fgColor()));
}

//------

#include <svg/gun_price_svg.h>
#include <svg/glue_price_svg.h>
#include <svg/snowbomb_price_svg.h>
#include <svg/missile_price_svg.h>
#include <svg/zap_price_svg.h>

CQFieldRunnersField::
CQFieldRunnersField(CQFieldRunners *fieldRunners) :
 CFieldRunners::Field(fieldRunners), qFieldRunners_(fieldRunners)
{
}

void
CQFieldRunnersField::
draw()
{
  int rows = getNumRows();
  int cols = getNumCols();

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  // draw background
  auto bg = fieldRunners()->bgColor();
  getWindow()->fill(bg);

  // draw grid
  Size windowSize;

  fieldRunners()->getSize(windowSize);

  auto lineColor = Color(0.2, 0.2, 0.2);

  getWindow()->setForeground(lineColor);

  for (int row = 0; row <= rows; ++row)
    getWindow()->drawLine(0, row*cellSize.height, windowSize.width - 1, row*cellSize.height);

  for (int col = 0; col <= cols; ++col)
    getWindow()->drawLine(col*cellSize.width , 0, col*cellSize.width , windowSize.height - 1);

  //---

  drawCells();

  //---

  auto currentPos = qFieldRunners()->qWindow()->app()->currentPos();

  auto currentCellPos = CFieldRunners::CellPos(currentPos.row, currentPos.col);

  CFieldRunners::Cell *currentCell = nullptr;

  if (qFieldRunners()->isInside(currentCellPos))
    qFieldRunners()->getCell(currentCellPos, &currentCell);

  if (currentCell) {
    if (currentCell->canAddWeapon())
      getWindow()->setBackground(Color(0, 1, 0, 0.5));
    else
      getWindow()->setBackground(Color(1, 0, 0, 0.5));

    getWindow()->setForeground(Color(1, 0, 0));

    getWindow()->drawRectangle( currentPos.col     *cellSize.width ,
                                currentPos.row     *cellSize.height,
                               (currentPos.col + 1)*cellSize.width ,
                               (currentPos.row + 1)*cellSize.height);
  }
}

void
CQFieldRunnersField::
drawWeaponPrice(CFieldRunners::CellType cellType, int x1, int y1, int w, int h)
{
  drawPriceBg(qFieldRunners_->getBuyCellType() == cellType, x1, y1, w, h);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  QImage image;

  switch (cellType) {
    case CFieldRunners::CellType::GUN     : image = s_GUN_PRICE_SVG    .image(w, h); break;
    case CFieldRunners::CellType::GLUE    : image = s_GLUE_PRICE_SVG   .image(w, h); break;
    case CFieldRunners::CellType::SNOWBOMB: image = s_SNOWBOMB_PRICE_SVG.image(w, h); break;
    case CFieldRunners::CellType::MISSILE : image = s_MISSILE_PRICE_SVG.image(w, h); break;
    case CFieldRunners::CellType::ZAP     : image = s_ZAP_PRICE_SVG    .image(w, h); break;
    case CFieldRunners::CellType::PULSE   : image = s_ZAP_PRICE_SVG    .image(w, h); break;
    case CFieldRunners::CellType::LASER   : image = s_ZAP_PRICE_SVG    .image(w, h); break;
    case CFieldRunners::CellType::FIREBOMB: image = s_ZAP_PRICE_SVG    .image(w, h); break;
    default: assert(false); break;
  }

  canvas->drawImage(x1, y1, image);
}

void
CQFieldRunnersField::
drawPriceBg(bool current, int x1, int y1, int w, int h)
{
  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if (current)
    canvas->setForeground(QColor(200, 200, 150));
  else
    canvas->setForeground(canvas->mapColor(qFieldRunners_->bgColor()));

  canvas->fillRectangle(x1, y1, x1 + w, y1 + h);
}

//------

#include <svg/soldier_svg.h>
#include <svg/dead_soldier_svg.h>

CQFieldRunnersSoldier::
CQFieldRunnersSoldier(CQFieldRunners *qFieldRunners) :
 CFieldRunners::Soldier(qFieldRunners), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersSoldier::
draw()
{
  int x, y;
  getPos(x, y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if      (! isDying()) {
    auto image = s_SOLDIER_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.2, 0.8));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);

    if (isDamaged())
      drawHealthBar(x, y);
  }
  else if (! isDead()) {
    auto image = s_DEAD_SOLDIER_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.2, 0.8, 0.5));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
  }
}

//------

#include <svg/mercenary_svg.h>
#include <svg/dead_mercenary_svg.h>

CQFieldRunnersMercenary::
CQFieldRunnersMercenary(CQFieldRunners *qFieldRunners) :
 CFieldRunners::Mercenary(qFieldRunners), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersMercenary::
draw()
{
  int x, y;
  getPos(x, y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if      (! isDying()) {
    auto image = s_MERCENARY_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.8, 0.2, 0.2));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);

    if (isDamaged())
      drawHealthBar(x, y);
  }
  else if (! isDead()) {
    auto image = s_DEAD_MERCENARY_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.8, 0.2, 0.2, 0.5));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
  }
}

//------

#include <svg/car_svg.h>
#include <svg/dead_car_svg.h>

CQFieldRunnersCar::
CQFieldRunnersCar(CQFieldRunners *fieldRunners) :
 CFieldRunners::Car(fieldRunners), qFieldRunners_(fieldRunners)
{
}

void
CQFieldRunnersCar::
draw()
{
  int x, y;
  getPos(x, y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if      (! isDying()) {
    auto image = s_CAR_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);

    if (isDamaged())
      drawHealthBar(x, y);
  }
  else if (! isDead()) {
    auto image = s_DEAD_CAR_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2, 0.5));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
  }
}

//------

#include <svg/tank_svg.h>
#include <svg/dead_tank_svg.h>

CQFieldRunnersTank::
CQFieldRunnersTank(CQFieldRunners *fieldRunners) :
 CFieldRunners::Tank(fieldRunners), qFieldRunners_(fieldRunners)
{
}

void
CQFieldRunnersTank::
draw()
{
  int x, y;
  getPos(x, y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if      (! isDying()) {
    auto image = s_TANK_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);

    if (isDamaged())
      drawHealthBar(x, y);
  }
  else if (! isDead()) {
    auto image = s_DEAD_TANK_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2, 0.5));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
  }
}

//------

#include <svg/plane_svg.h>
#include <svg/dead_plane_svg.h>

CQFieldRunnersPlane::
CQFieldRunnersPlane(CQFieldRunners *fieldRunners) :
 CFieldRunners::Plane(fieldRunners), qFieldRunners_(fieldRunners)
{
}

void
CQFieldRunnersPlane::
draw()
{
  int x, y;
  getPos(x, y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if      (! isDying()) {
    auto image = s_PLANE_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);

    if (isDamaged())
      drawHealthBar(x, y);
  }
  else if (! isDead()) {
    auto image = s_DEAD_PLANE_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2, 0.5));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
  }
}

//------

#include <svg/train_svg.h>
#include <svg/dead_train_svg.h>

CQFieldRunnersTrain::
CQFieldRunnersTrain(CQFieldRunners *fieldRunners) :
 CFieldRunners::Train(fieldRunners), qFieldRunners_(fieldRunners)
{
}

void
CQFieldRunnersTrain::
draw()
{
  int x, y;
  getPos(x, y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if      (! isDying()) {
    auto image = s_TRAIN_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);

    if (isDamaged())
      drawHealthBar(x, y);
  }
  else if (! isDead()) {
    auto image = s_DEAD_TRAIN_SVG.image(cellSize.width, cellSize.height);
    canvas->drawImage(x, y, image);

    //getWindow()->setForeground(Color(0.2, 0.8, 0.2, 0.5));
    //getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
  }
}

//------

#include <svg/border_svg.h>
#include <svg/grasslands_border_svg.h>
#include <svg/crystal_border_svg.h>
#include <svg/crossroads_border_svg.h>
#include <svg/frostbite_border_svg.h>
#include <svg/drylands_border_svg.h>
#include <svg/lavaflow_border_svg.h>
#include <svg/skyway_border_svg.h>

#include <svg/track_h_svg.h>
#include <svg/track_v_svg.h>
#include <svg/track_h_cross_svg.h>
#include <svg/track_v_cross_svg.h>
#include <svg/track_ne_svg.h>
#include <svg/track_se_svg.h>
#include <svg/track_sw_svg.h>
#include <svg/track_nw_svg.h>

CQFieldRunnersEmpty::
CQFieldRunnersEmpty(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::EmptyCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersEmpty::
draw()
{
  if (isTransparent())
    return;

  BBox bbox;
  getBBox(bbox);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  qFieldRunners_->drawBg(bbox);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  if (subType() != CFieldRunners::CellSubType::NONE) {
    QImage image;

    if      (subType() == CFieldRunners::CellSubType::TRACK) {
      if      (direction() == CFieldRunners::CellDirection::E ||
               direction() == CFieldRunners::CellDirection::W)
        image = s_TRACK_H_CROSS_SVG.image(cellSize.width, cellSize.height);
      else if (direction() == CFieldRunners::CellDirection::N ||
               direction() == CFieldRunners::CellDirection::S)
        image = s_TRACK_V_CROSS_SVG.image(cellSize.width, cellSize.height);
    }

    if (! image.isNull())
      canvas->drawImage(bbox.xmin, bbox.ymin, image);
  }
}

//------

CQFieldRunnersBorder::
CQFieldRunnersBorder(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::BorderCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersBorder::
draw()
{
  if (isTransparent())
    return;

  BBox bbox;
  getBBox(bbox);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  qFieldRunners_->drawBg(bbox);

  QImage image;

  if      (fieldRunners()->borderType() == CFieldRunners::BorderType::GRASSLANDS)
    image = s_GRASSLANDS_BORDER_SVG.image(cellSize.width, cellSize.height);
  else if (fieldRunners()->borderType() == CFieldRunners::BorderType::CRYSTAL)
    image = s_CRYSTAL_BORDER_SVG.image(cellSize.width, cellSize.height);
  else if (fieldRunners()->borderType() == CFieldRunners::BorderType::CROSSROADS)
    image = s_CROSSROADS_BORDER_SVG.image(cellSize.width, cellSize.height);
  else if (fieldRunners()->borderType() == CFieldRunners::BorderType::FROSTBITE)
    image = s_FROSTBITE_BORDER_SVG.image(cellSize.width, cellSize.height);
  else if (fieldRunners()->borderType() == CFieldRunners::BorderType::DRYLANDS)
    image = s_DRYLANDS_BORDER_SVG.image(cellSize.width, cellSize.height);
  else if (fieldRunners()->borderType() == CFieldRunners::BorderType::LAVAFLOW)
    image = s_LAVAFLOW_BORDER_SVG.image(cellSize.width, cellSize.height);
  else if (fieldRunners()->borderType() == CFieldRunners::BorderType::SKYWAY)
    image = s_SKYWAY_BORDER_SVG.image(cellSize.width, cellSize.height);
  else
    image = s_BORDER_SVG.image(cellSize.width, cellSize.height);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  canvas->drawImage(bbox.xmin, bbox.ymin, image);
}

//------

#include <svg/gun_n_svg.h>
#include <svg/gun_ne_svg.h>
#include <svg/gun_e_svg.h>
#include <svg/gun_se_svg.h>
#include <svg/gun_s_svg.h>
#include <svg/gun_sw_svg.h>
#include <svg/gun_w_svg.h>
#include <svg/gun_nw_svg.h>

CQFieldRunnersGun::
CQFieldRunnersGun(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::GunCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersGun::
draw()
{
  BBox bbox;
  getBBox(bbox);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  qFieldRunners_->drawBg(bbox);

  //---

#if 0
  int xc, yc;

  bbox.getCenter(&xc, &yc);

  double r1 = std::min(bbox.width(), bbox.height())/2.0;
  double r2 = std::min(bbox.width(), bbox.height())/3.0;

  canvas->setForeground(QColor(200, 200, 200));
  canvas->setBackground(QColor(200, 200, 200));

  canvas->drawEllipse(xc - r2, yc - r2, xc + r2, yc + r2);

  auto orient = this->orient();

  auto a = M_PI*orient*45/180.0;

  auto c = std::cos(a);
  auto s = std::sin(a);

  double x1 = xc + c*r1;
  double y1 = yc - s*r1;

  canvas->setForeground(QColor(0, 0, 0));

  canvas->drawLine(xc, yc, x1, y1);
#else
  QImage image;

  switch (orient()) {
    case 0: image = s_GUN_E_SVG .image(bbox.width(), bbox.height()); break;
    case 1: image = s_GUN_NE_SVG.image(bbox.width(), bbox.height()); break;
    case 2: image = s_GUN_N_SVG .image(bbox.width(), bbox.height()); break;
    case 3: image = s_GUN_NW_SVG.image(bbox.width(), bbox.height()); break;
    case 4: image = s_GUN_W_SVG .image(bbox.width(), bbox.height()); break;
    case 5: image = s_GUN_SW_SVG.image(bbox.width(), bbox.height()); break;
    case 6: image = s_GUN_S_SVG .image(bbox.width(), bbox.height()); break;
    case 7: image = s_GUN_SE_SVG.image(bbox.width(), bbox.height()); break;
    default: break;
  }

  canvas->drawImage(bbox.xmin, bbox.ymin, image);
#endif
}

//------

#include <svg/glue_gun_svg.h>

CQFieldRunnersGlue::
CQFieldRunnersGlue(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::GlueCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersGlue::
draw()
{
  BBox bbox;
  getBBox(bbox);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  auto image = s_GLUE_GUN_SVG.image(bbox.width(), bbox.height());
  canvas->drawImage(bbox.xmin, bbox.ymin, image);
}

//------

#include <svg/snowbomb_gun_svg.h>

CQFieldRunnersSnowbomb::
CQFieldRunnersSnowbomb(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::SnowbombCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersSnowbomb::
draw()
{
  BBox bbox;
  getBBox(bbox);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  auto image = s_SNOWBOMB_GUN_SVG.image(bbox.width(), bbox.height());
  canvas->drawImage(bbox.xmin, bbox.ymin, image);
}

//------

#include <svg/missile_gun_svg.h>

CQFieldRunnersMissile::
CQFieldRunnersMissile(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::MissileCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersMissile::
draw()
{
  BBox bbox;
  getBBox(bbox);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  auto image = s_MISSILE_GUN_SVG.image(bbox.width(), bbox.height());
  canvas->drawImage(bbox.xmin, bbox.ymin, image);
}

//------

#include <svg/zap_gun_svg.h>

CQFieldRunnersZap::
CQFieldRunnersZap(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::ZapCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersZap::
draw()
{
  BBox bbox;
  getBBox(bbox);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  auto image = s_ZAP_GUN_SVG.image(bbox.width(), bbox.height());
  canvas->drawImage(bbox.xmin, bbox.ymin, image);
}

//------

#include <svg/block_svg.h>
#include <svg/rock_svg.h>
#include <svg/igloo_svg.h>

CQFieldRunnersBlock::
CQFieldRunnersBlock(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::BlockCell(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersBlock::
draw()
{
  if (isTransparent())
    return;

  BBox bbox;
  getBBox(bbox);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  QImage image;

  int x = bbox.xmin;
  int y = bbox.ymin;
  int w = cellSize.width;
  int h = cellSize.height;

  if      (subType() == CFieldRunners::CellSubType::ROCK)
    image = s_ROCK_SVG.image(w, h);
  else if (subType() == CFieldRunners::CellSubType::TRACK) {
    if      (direction() == CFieldRunners::CellDirection::E ||
             direction() == CFieldRunners::CellDirection::W)
      image = s_TRACK_H_SVG.image(w, h);
    else if (direction() == CFieldRunners::CellDirection::N ||
             direction() == CFieldRunners::CellDirection::S)
      image = s_TRACK_V_SVG.image(w, h);
    else if (direction() == CFieldRunners::CellDirection::NE)
      image = s_TRACK_NE_SVG.image(w, h);
    else if (direction() == CFieldRunners::CellDirection::SE)
      image = s_TRACK_SE_SVG.image(w, h);
    else if (direction() == CFieldRunners::CellDirection::SW)
      image = s_TRACK_SW_SVG.image(w, h);
    else if (direction() == CFieldRunners::CellDirection::NW)
      image = s_TRACK_NW_SVG.image(w, h);
  }
  else if (subType() == CFieldRunners::CellSubType::IGLOO) {
    if (id() == "center") {
      x -= w;
      y -= h;

      image = s_IGLOO_SVG.image(3*w, 3*h);

      bbox.xmin -= w; bbox.ymin -= h;
      bbox.xmax += w; bbox.ymax += h;
    }
  }
  else
    image = s_BLOCK_SVG.image(w, h);

  //---

  qFieldRunners_->drawBg(bbox);

  if (! image.isNull())
    canvas->drawImage(x, y, image);
}

//------

CQFieldRunnersEntrance::
CQFieldRunnersEntrance(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::Entrance(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

//------

CQFieldRunnersExit::
CQFieldRunnersExit(CQFieldRunners *qFieldRunners, const CFieldRunners::CellPos &pos) :
 CFieldRunners::Exit(qFieldRunners, pos), qFieldRunners_(qFieldRunners)
{
}

//------

#include <svg/missile_bullet_e_svg.h>
#include <svg/missile_bullet_ne_svg.h>
#include <svg/missile_bullet_n_svg.h>
#include <svg/missile_bullet_nw_svg.h>
#include <svg/missile_bullet_se_svg.h>
#include <svg/missile_bullet_s_svg.h>
#include <svg/missile_bullet_svg.h>
#include <svg/missile_bullet_sw_svg.h>
#include <svg/missile_bullet_w_svg.h>

CQFieldRunnersRocket::
CQFieldRunnersRocket(CQFieldRunners *qFieldRunners, const Point &point,
                     CFieldRunners::Runner *runner, CFieldRunners::Orient orient) :
 CFieldRunners::Rocket(qFieldRunners, point, runner, orient), qFieldRunners_(qFieldRunners)
{
}

void
CQFieldRunnersRocket::
draw()
{
  auto p = getPoint();

  Size cellSize;
  qFieldRunners_->getCellSize(cellSize);

  int size = cellSize.width/2;

  auto *canvas = qFieldRunners_->qWindow()->canvas();

  QImage image;
  switch (orient()) {
    case 0: image = s_MISSILE_BULLET_E_SVG .image(size, size); break;
    case 1: image = s_MISSILE_BULLET_NE_SVG.image(size, size); break;
    case 2: image = s_MISSILE_BULLET_N_SVG .image(size, size); break;
    case 3: image = s_MISSILE_BULLET_NW_SVG.image(size, size); break;
    case 4: image = s_MISSILE_BULLET_W_SVG .image(size, size); break;
    case 5: image = s_MISSILE_BULLET_SW_SVG.image(size, size); break;
    case 6: image = s_MISSILE_BULLET_S_SVG .image(size, size); break;
    case 7: image = s_MISSILE_BULLET_SE_SVG.image(size, size); break;
    default: break;
  }

  canvas->drawImage(p.x - size/2, p.y - size/2, image);
}

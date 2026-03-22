#ifndef CQCamera3DOverlay2D_H
#define CQCamera3DOverlay2D_H

#include <FaceData.h>

#include <CFieldRunners.h>

#include <QObject>
#include <QRectF>

class CQGLBuffer;
class CQGLTexture;

class QMouseEvent;

namespace CQGLFieldRunners {

class Canvas;
class ShaderProgram;
class Text;

class HUD : public QObject {
  Q_OBJECT

 public:
  HUD(Canvas *canvas);

  //---

  bool isWireframe() const { return wireframe_; }
  void setWireframe(bool b) { wireframe_ = b; }

  //---

  void updateGeometry();

  void drawGeometry();

  //---

  ShaderProgram *shaderProgram();

  CQGLBuffer *initBuffer();

  //---

  bool mousePress(QMouseEvent *e);

 public Q_SLOTS:
  void invalidate();

 private:
  using Texts = std::vector<Text *>;

  enum WidgetType {
    PLAY,
    PAUSE
  };

  struct Widget {
    QRectF                  rect;
    bool                    visible { false };
    bool                    enabled { true };
    CQGLTexture*            texture { nullptr };
    CFieldRunners::CellType type    { CFieldRunners::CellType::NONE };
  };

  using FaceDatas = std::vector<FaceData>;

  Canvas* canvas_ { nullptr };

  ShaderProgram* shaderProgram_ { nullptr };

  CQGLBuffer* buffer_ { nullptr };

  FaceDataList faceDataList_;

  bool valid_     { false };
  bool wireframe_ { false };

  Widget playWidget_;
  Widget pauseWidget_;

  Widget gunWidget_;
  Widget snowbombWidget_;
  Widget missileWidget_;
  Widget shotgunWidget_;
  Widget glueWidget_;
  Widget zapWidget_;
  Widget pulseWidget_;
  Widget laserWidget_;
  Widget firebombWidget_;
  Widget flameWidget_;

  std::vector<Widget *> weaponWidgets_;

  Widget livesWidget_;
  Widget moneyWidget_;

  Texts texts_;

  Text* moneyText_ { nullptr };
  Text* scoreText_ { nullptr };
  Text* roundText_ { nullptr };
  Text* livesText_ { nullptr };

  bool textValid_ { false };
};

}

#endif

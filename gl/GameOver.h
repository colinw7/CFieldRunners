#ifndef GameOver_H
#define GameOver_H

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

class GameOver : public QObject {
  Q_OBJECT

 public:
  GameOver(Canvas *canvas);

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
    START
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

  Widget startWidget_;

  Texts texts_;

  Text* fieldRunnersText_ { nullptr };

  bool textValid_ { false };
};

}

#endif

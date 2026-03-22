#ifndef Axes_H
#define Axes_H

#include <FaceData.h>

#include <QObject>

#include <CRGBA.h>

class CQGLBuffer;

namespace CQGLFieldRunners {

class Canvas;
class ShaderProgram;

class Axes : public QObject {
  Q_OBJECT

 public:
  Axes(Canvas *canvas);

  //---

  bool isVisible() const { return visible_; }
  void setVisible(bool b) { visible_ = b; }

  //---

  void invalidate();

  void updateGeometry();

  void drawGeometry();

  //---

  ShaderProgram *shaderProgram();

  CQGLBuffer *initBuffer();

 private:
  Canvas* canvas_ { nullptr };

  ShaderProgram* shaderProgram_ { nullptr };

  CQGLBuffer* buffer_ { nullptr };

  FaceDataList faceDataList_;

  bool visible_ { false };
  bool valid_   { false };

  double y_         { 0.1 };
  double lineWidth_ { -1.0 };
  CRGBA  color_     { 1.0, 1.0, 1.0 };
};

}

#endif

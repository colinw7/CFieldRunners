#ifndef CQCamera3DShaderProgram_H
#define CQCamera3DShaderProgram_H

#include <QOpenGLShaderProgram>
#include <QString>

class CQGLBuffer;

namespace CQGLFieldRunners {

class Canvas;

class ShaderProgram : public QOpenGLShaderProgram {
 public:
  ShaderProgram(Canvas *canvas);

  void addShaders(const QString &vertex, const QString &fragment);

  void addVertexShader  (const QString &name);
  void addFragmentShader(const QString &name);

  CQGLBuffer *createBuffer();

 private:
  Canvas* canvas_ { nullptr };
};

}

#endif

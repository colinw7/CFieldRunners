#include <ShaderProgram.h>
#include <Canvas.h>

#include <CQGLBuffer.h>

namespace CQGLFieldRunners {

ShaderProgram::
ShaderProgram(Canvas *canvas) :
 canvas_(canvas)
{
}

void
ShaderProgram::
addShaders(const QString &vertex, const QString &fragment)
{
  addVertexShader  (vertex  );
  addFragmentShader(fragment);

  link();
}

void
ShaderProgram::
addVertexShader(const QString &name)
{
  auto buildDir = canvas_->buildDir();

  if (! addShaderFromSourceFile(QOpenGLShader::Vertex, buildDir + "/shaders/" + name))
    std::cerr << log().toStdString() << "\n";
}

void
ShaderProgram::
addFragmentShader(const QString &name)
{
  auto buildDir = canvas_->buildDir();

  if (! addShaderFromSourceFile(QOpenGLShader::Fragment, buildDir + "/shaders/" + name))
    std::cerr << log().toStdString() << "\n";
}

CQGLBuffer *
ShaderProgram::
createBuffer()
{
  return new CQGLBuffer(this);
}

}

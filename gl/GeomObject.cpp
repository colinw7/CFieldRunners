#include <GeomObject.h>
#include <Canvas.h>
#include <ShaderProgram.h>

namespace CQGLFieldRunners {

GeomObject::
GeomObject(CGeomScene3D *pscene, const std::string &name) :
 CGeomObject3D(pscene, name)
{
}

GeomObject::
GeomObject(const GeomObject &object) :
 CGeomObject3D(object)
{
}

GeomObject *
GeomObject::
dup() const
{
  return new GeomObject(*this);
}

CQGLBuffer *
GeomObject::
initBuffer(Canvas *canvas)
{
  program_ = canvas->sceneShaderProgram();
  assert(program_);

  if (! buffer_)
    buffer_ = program_->createBuffer();

  buffer_->clearBuffers();

  faceDatas_.clear();

  return buffer_;
}

void
GeomObject::
addFaceData(const FaceData &faceData)
{
  faceDatas_.push_back(faceData);
}

}

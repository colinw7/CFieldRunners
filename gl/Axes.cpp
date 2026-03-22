#include <Axes.h>
#include <Canvas.h>
#include <ShaderProgram.h>
#include <Camera.h>

#include <CQGLBuffer.h>
#include <CQGLUtil.h>

namespace CQGLFieldRunners {

Axes::
Axes(Canvas *canvas) :
 canvas_(canvas)
{
}

void
Axes::
invalidate()
{
  valid_ = false;
}

ShaderProgram *
Axes::
shaderProgram()
{
  return canvas_->getShader("axis.vs", "axis.fs");
}

void
Axes::
updateGeometry()
{
  auto *buffer = initBuffer();

  //---

  auto nc = canvas_->numCols();
  auto nr = canvas_->numRows();

  //---

  auto lineWidth = lineWidth_;

  if (lineWidth < 0)
    lineWidth = 1.0;

  auto color = color_;

  //---

  auto addPoint = [&](const CPoint3D &p) {
    buffer->addPoint(p.x, p.y, p.z);
    buffer->addNormal(0.0, 1.0, 0.0);
    buffer->addColor(color);
  };

  auto addLine = [&](const CPoint3D &c1, const CPoint3D &c2) {
    FaceData faceData;

    faceData.pos = faceDataList_.pos;
    faceData.len = 2;

    addPoint(c1);
    addPoint(c2);

    faceDataList_.faceDatas.push_back(faceData);

    faceDataList_.pos += faceData.len;
  };

  //---

  for (int ic = 0; ic <= nc; ++ic) {
    addLine(CPoint3D(ic - nc/2.0, y_, -nr/2.0),
            CPoint3D(ic - nc/2.0, y_,  nr/2.0));
  }

  for (int ir = 0; ir <= nr; ++ir) {
    addLine(CPoint3D(-nc/2.0, y_, ir - nr/2.0),
            CPoint3D( nc/2.0, y_, ir - nr/2.0));
  }

  //---

  buffer->load();
}

void
Axes::
drawGeometry()
{
  if (! isVisible())
    return;

  //---

  if (! valid_) {
    updateGeometry();

    valid_ = true;
  }

  //---

  auto *program = shaderProgram();

  program->bind();

  //---

  auto *camera = canvas_->camera();

  // camera projection
  auto projectionMatrix = camera->worldMatrix();
  program->setUniformValue("projection", CQGLUtil::toQMatrix(projectionMatrix));

  // camera/view transformation
  auto viewMatrix = camera->viewMatrix();
  program->setUniformValue("view", CQGLUtil::toQMatrix(viewMatrix));

  // model matrix
  auto modelMatrix = CMatrix3DH::identity();
  program->setUniformValue("model", CQGLUtil::toQMatrix(modelMatrix));

  program->setUniformValue("viewPos", CQGLUtil::toVector(camera->position()));

  //---

  // render axis
  buffer_->bind();

  for (const auto &faceData : faceDataList_.faceDatas) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawArrays(GL_LINES, faceData.pos, faceData.len);
  }

  buffer_->unbind();

  //---

  program->release();
}

CQGLBuffer *
Axes::
initBuffer()
{
  assert(shaderProgram());

  if (! buffer_)
    buffer_ = shaderProgram()->createBuffer();

  buffer_->clearBuffers();

  faceDataList_.clear();

  return buffer_;
}

}

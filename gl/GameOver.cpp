#include <GameOver.h>
#include <Canvas.h>
#include <ShaderProgram.h>
#include <Camera.h>
#include <Font.h>

#include <CGeomObject3D.h>
#include <CQGLTexture.h>
#include <CQGLBuffer.h>
#include <CQGLUtil.h>

#include <QMouseEvent>

namespace CQGLFieldRunners {

GameOver::
GameOver(Canvas *canvas) :
 canvas_(canvas)
{
  connect(canvas_, SIGNAL(resized()), this, SLOT(invalidate()));
}

void
GameOver::
invalidate()
{
  textValid_ = false;
  valid_     = false;
}

ShaderProgram *
GameOver::
shaderProgram()
{
  return canvas_->getShader("overlay2d.vs", "overlay2d.fs");
}

void
GameOver::
updateGeometry()
{
  auto *buffer = initBuffer();

  //---

  struct PointData {
    CPoint2D p;
    CPoint2D tp;

    PointData() { }

    PointData(const CPoint2D &p1, const CPoint2D &p2) :
     p(p1), tp(p2) {
    }
  };

  auto addPoint = [&](const PointData &p, const CRGBA &c) {
    buffer->addPoint(p.p.x, p.p.y, 0.0);
    buffer->addColor(c);
    buffer->addTexturePoint(p.tp.x, p.tp.y);
  };

  auto addPolygon = [&](const std::vector<PointData> &points, const CRGBA &c,
                        CQGLTexture *texture) {
    FaceData faceData;

    faceData.pos = faceDataList_.pos;
    faceData.len = points.size();

    faceData.diffuseTexture = texture;

    for (const auto &p : points) {
      addPoint(p, c);
    }

    faceDataList_.faceDatas.push_back(faceData);

    faceDataList_.pos += faceData.len;
  };

  auto addRect = [&](const QRectF &rect, const CRGBA &c, CQGLTexture *texture) {
    std::vector<PointData> points;

    points.push_back(PointData(CPoint2D(rect.left (), rect.top   ()), CPoint2D(0, 0)));
    points.push_back(PointData(CPoint2D(rect.right(), rect.top   ()), CPoint2D(1, 0)));
    points.push_back(PointData(CPoint2D(rect.right(), rect.bottom()), CPoint2D(1, 1)));
    points.push_back(PointData(CPoint2D(rect.left (), rect.bottom()), CPoint2D(0, 1)));

    addPolygon(points, c, texture);
  };

  auto addWidget = [&](Widget &widget, const QString &imageFile, const QRectF &rect) {
    if (! widget.texture)
      widget.texture = canvas_->loadTexture(imageFile, /*flip*/true);

    widget.rect = rect;

    addRect(widget.rect, CRGBA(0.5, 0.5, 0.5), widget.texture);
  };

  //---

  auto a = canvas_->aspect();

  auto x1 = -0.95;
  auto y1 = -0.90;
  auto iw =  0.10;
  auto ih =  iw*a;

  addWidget(startWidget_, "images/play.png", QRectF(x1, y1, iw, ih));

  //---

  buffer->load();

  //---

  auto *font = canvas_->getFont();

  font->setAspect(a);

  for (auto *text : texts_)
    delete text;

  texts_.clear();

  auto createText = [&](const QString &str, const CPoint2D &pos, double size,
                        const Qt::Alignment &align=Qt::AlignHCenter) {
    auto *text = new Text(str);

    text->setFont(font);
    text->setColor(Font::Color(1, 1, 1));
    text->setPosition(CGLVector3D(pos.x, pos.y, 0));
    text->setAlign(Qt::AlignVCenter | align);
    text->setSize(size);
    text->setOverlay(true);

    texts_.push_back(text);

    return text;
  };

  fieldRunnersText_ = createText("FieldRunners", CPoint2D(0.0, 0.8), 0.1, Qt::AlignCenter);

  for (auto *text : texts_)
    text->updateText();
}

void
GameOver::
drawGeometry()
{
  if (! valid_) {
    updateGeometry();

    valid_ = true;
  }

  //---

  glDisable(GL_DEPTH_TEST);

  //---

  auto *program = shaderProgram();

  program->bind();

  //---

  auto *camera = canvas_->camera();

  // camera projection
  auto projectionMatrix = camera->worldMatrix();
  program->setUniformValue("projection", CQGLUtil::toQMatrix(projectionMatrix));

  // model matrix
  auto modelMatrix = CMatrix3DH::identity();
  program->setUniformValue("model", CQGLUtil::toQMatrix(modelMatrix));

  //---

  // render shapes
  buffer_->bind();

  for (const auto &faceData : faceDataList_.faceDatas) {
    bool enabled = true;

    program->setUniformValue("enabled", enabled);

    bool useDiffuseTexture = !!faceData.diffuseTexture;

    program->setUniformValue("diffuseTexture.enabled", useDiffuseTexture);

    if (useDiffuseTexture) {
      glActiveTexture(GL_TEXTURE0);
      faceData.diffuseTexture->bind();

      program->setUniformValue("diffuseTexture.texture", 0);
    }

    //---

    if (isWireframe()) {
      program->setUniformValue("isWireframe", true);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      glDrawArrays(GL_TRIANGLE_FAN, faceData.pos, faceData.len);
    }

    program->setUniformValue("isWireframe", false);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawArrays(GL_TRIANGLE_FAN, faceData.pos, faceData.len);
  }

  buffer_->unbind();

  //---

  program->release();

  //---

  glDisable(GL_CULL_FACE);

  for (auto *text : texts_)
    text->render(canvas_);

  glEnable(GL_CULL_FACE);

  textValid_ = true;
}

CQGLBuffer *
GameOver::
initBuffer()
{
  assert(shaderProgram());

  if (! buffer_)
    buffer_ = shaderProgram()->createBuffer();

  buffer_->clearBuffers();

  faceDataList_.clear();

  return buffer_;
}

bool
GameOver::
mousePress(QMouseEvent *e)
{
  auto *fieldRunners = canvas_->fieldRunners();

  auto p = e->pos();

  auto x = 2.0*double(p.x())/double(canvas_->width ()) - 1.0;
  auto y = 1.0 - 2.0*double(p.y())/double(canvas_->height());

  auto p1 = QPointF(x, y);

  if      (startWidget_.rect.contains(p1)) {
    valid_ = false;

    fieldRunners->setState(CFieldRunners::State::ACTIVE);
  }

  if (valid_)
    return false;

  canvas_->update();

  return true;
}

}

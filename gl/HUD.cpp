#include <HUD.h>
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

HUD::
HUD(Canvas *canvas) :
 canvas_(canvas)
{
  connect(canvas_, SIGNAL(resized()), this, SLOT(invalidate()));
}

void
HUD::
invalidate()
{
  textValid_ = false;
  valid_     = false;
}

ShaderProgram *
HUD::
shaderProgram()
{
  return canvas_->getShader("overlay2d.vs", "overlay2d.fs");
}

void
HUD::
updateGeometry()
{
  auto *buffer = initBuffer();

  //---

  weaponWidgets_.clear();

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

  auto addWeapon = [&](Widget &widget, const QString &imageFile,
                       const CFieldRunners::CellType &type) {
    if (! widget.texture)
      widget.texture = canvas_->loadTexture(imageFile, /*flip*/true);

    widget.visible = false;
    widget.type    = type;

    weaponWidgets_.push_back(&widget);
  };

  //---

  auto a = canvas_->aspect();

  auto x1 = -0.95;
  auto y1 = -0.90;
  auto iw =  0.10;
  auto ih =  iw*a;

  addWidget(playWidget_ , "images/play.png" , QRectF(x1     , y1, iw, ih));
  addWidget(pauseWidget_, "images/pause.png", QRectF(x1 + iw, y1, iw, ih));

  addWeapon(gunWidget_     , "images/gun_icon.png"     , CFieldRunners::CellType::GUN);
  addWeapon(snowbombWidget_, "images/snowbomb_icon.png", CFieldRunners::CellType::SNOWBOMB);
  addWeapon(missileWidget_ , "images/missile_icon.png" , CFieldRunners::CellType::MISSILE);
  addWeapon(shotgunWidget_ , "images/shotgun_icon.png" , CFieldRunners::CellType::SHOTGUN);
  addWeapon(glueWidget_    , "images/glue_icon.png"    , CFieldRunners::CellType::GLUE);
  addWeapon(zapWidget_     , "images/zap_icon.png"     , CFieldRunners::CellType::ZAP);
  addWeapon(pulseWidget_   , "images/pulse_icon.png"   , CFieldRunners::CellType::PULSE);
  addWeapon(laserWidget_   , "images/laser_icon.png"   , CFieldRunners::CellType::LASER);
  addWeapon(firebombWidget_, "images/firebomb_icon.png", CFieldRunners::CellType::FIREBOMB);
  addWeapon(flameWidget_   , "images/flame_icon.png"   , CFieldRunners::CellType::FLAME);

  addWidget(moneyWidget_, "images/money.png", QRectF(-1.0, 0.9, 0.1, 0.1));
  addWidget(livesWidget_, "images/lives.png", QRectF( 0.9, 0.9, 0.1, 0.1));

  //---

  auto *fieldRunners = canvas_->fieldRunners();

  auto placeWeapons = [&]() {
    const auto &weapons = fieldRunners->getWeapons();

    auto x1 = 0.95 - weapons.size()*iw;
    auto y1 = -0.90;

    for (auto &weapon : weapons) {
      Widget *widget = nullptr ;

      if      (weapon == CFieldRunners::CellType::GUN     ) widget = &gunWidget_;
      else if (weapon == CFieldRunners::CellType::GLUE    ) widget = &glueWidget_;
      else if (weapon == CFieldRunners::CellType::SNOWBOMB) widget = &snowbombWidget_;
      else if (weapon == CFieldRunners::CellType::MISSILE ) widget = &missileWidget_;
      else if (weapon == CFieldRunners::CellType::SHOTGUN ) widget = &shotgunWidget_;
      else if (weapon == CFieldRunners::CellType::ZAP     ) widget = &zapWidget_;
      else if (weapon == CFieldRunners::CellType::PULSE   ) widget = &pulseWidget_;
      else if (weapon == CFieldRunners::CellType::LASER   ) widget = &laserWidget_;
      else if (weapon == CFieldRunners::CellType::FIREBOMB) widget = &firebombWidget_;
      else if (weapon == CFieldRunners::CellType::FLAME   ) widget = &flameWidget_;

      if (widget) {
        widget->visible = true;

        widget->rect = QRectF(x1, y1, iw, ih);

        addRect(widget->rect, CRGBA(0.5, 0.5, 0.5), widget->texture);

        x1 += iw;
      }
    }
  };

  placeWeapons();

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

  moneyText_ = createText("0", CPoint2D(-0.9, 0.9), 0.1, Qt::AlignLeft);
  scoreText_ = createText("0", CPoint2D( 0.0, 0.9), 0.1, Qt::AlignHCenter);
  roundText_ = createText("0", CPoint2D( 0.0, 0.8), 0.1, Qt::AlignHCenter);
  livesText_ = createText("0", CPoint2D( 0.9, 0.9), 0.1, Qt::AlignRight);

  for (auto *text : texts_)
    text->updateText();
}

void
HUD::
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

    if      (faceData.diffuseTexture == playWidget_.texture)
      enabled = ! canvas_->isRunning();
    else if (faceData.diffuseTexture == pauseWidget_.texture)
      enabled = canvas_->isRunning();

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

  auto *fieldRunners = canvas_->fieldRunners();
  auto *player       = fieldRunners->getPlayer();

  auto updateText = [&](Text *text, const QString &str) {
    if (! textValid_ || str != text->text()) {
      text->setText(str);
      text->updateText();
    }
  };

  updateText(moneyText_, QString("%1").arg(player->getMoney()));
  updateText(scoreText_, QString("%1").arg(player->getScore()));
  updateText(roundText_, QString("Round %1").arg(fieldRunners->getLevel()));
  updateText(livesText_, QString("%1").arg(player->getLives()));

  glDisable(GL_CULL_FACE);

  for (auto *text : texts_)
    text->render(canvas_);

  glEnable(GL_CULL_FACE);

  textValid_ = true;
}

CQGLBuffer *
HUD::
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
HUD::
mousePress(QMouseEvent *e)
{
  auto *fieldRunners = canvas_->fieldRunners();

  auto p = e->pos();

  auto x = 2.0*double(p.x())/double(canvas_->width ()) - 1.0;
  auto y = 1.0 - 2.0*double(p.y())/double(canvas_->height());

  auto p1 = QPointF(x, y);

  if      (playWidget_ .rect.contains(p1)) {
    canvas_->startTimer();

    valid_ = false;
  }
  else if (pauseWidget_.rect.contains(p1)) {
    canvas_->stopTimer();

    valid_ = false;
  }
  else {
    for (auto *widget : weaponWidgets_) {
      if (! widget->visible)
        continue;

      if (widget->rect.contains(p1)) {
        fieldRunners->setBuyCellType(widget->type);

        valid_ = false;

        break;
      }
    }
  }

  if (valid_)
    return false;

  canvas_->update();

  return true;
}

}

#include <Canvas.h>
#include <Camera.h>
#include <HUD.h>
#include <Axes.h>
#include <StartUp.h>
#include <GameOver.h>
#include <GeomObject.h>
#include <Texture.h>
#include <ShaderProgram.h>
#include <ParticleBuffer.h>
#include <FaceData.h>
#include <Font.h>

#include <CQGLTexture.h>
#include <CQGLUtil.h>

#include <CGeometry3D.h>
#include <CGeomScene3D.h>
#include <CGeomNodeData.h>
#include <CImportBase.h>
#include <CFile.h>
#include <CLine3D.h>
#include <CPlane3D.h>
#include <CMatrix2D.h>
#include <CMathRand.h>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>

#include <GL/glut.h>

#define Q(x) #x
#define QUOTE(x) Q(x)

namespace CQGLFieldRunners {

class GeomFactory : public CGeometryFactory {
 public:
  GeomFactory() { }
 ~GeomFactory() override { }

  CGeomObject3D *createObject3D(CGeomScene3D *pscene, const std::string &name) const override {
    return new GeomObject(pscene, name);
  }

#if 0
  CGeomFace3D *createFace3D() const override {
    return new GeomFace;
  }

  CGeomLine3D *createLine3D() const override {
    return new GeomLine;
  }

  CGeomLight3D *createLight3D(CGeomScene3D *pscene, const std::string &name) const override {
    return new Light(pscene, name);
  }
#endif

  CGeomTexture *createTexture() const override {
    return new Texture;
  }
};

}

//---

namespace {

bool checkError(const char *msg) {
  // check texture generated
  GLenum err = glGetError();

  if (err != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << gluErrorString(err) << "(" << msg << ")\n";
    return false;
  }

  return true;
}

}

//---

namespace CQGLFieldRunners {

Canvas::
Canvas(App *app) :
 app_(app)
{
  setObjectName("canvas");

  setFocusPolicy(Qt::StrongFocus);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  buildDir_ = QUOTE(BUILD_DIR);
}

void
Canvas::
init()
{
  camera_ = new Camera(this);

  camera_->setOrigin(CVector3D(0, 0, 0));
  camera_->setDistance(20);
  camera_->setPitch(-M_PI/2.0 + 0.1);
  camera_->setYaw(-0.1);

  //---

  startUp_  = new StartUp(this);
  gameOver_ = new GameOver(this);

  hud_ = new HUD(this);

  axes_ = new Axes(this);

  //---

  fieldRunners_ = new FieldRunners(this);

  fieldRunners_->init();

  loadMap(QString::fromStdString(initMap()));

  //---

  CGeometry3DInst->setFactory(new GeomFactory);

  scene_ = CGeometry3DInst->createScene3D();

  //---

  connect(camera_, SIGNAL(stateChangedSignal()), this, SLOT(update()));

  //---

  timer_ = new QTimer;

  connect(timer_, SIGNAL(timeout()), this, SLOT(tickSlot()));
}

void
Canvas::
updateState()
{
  auto state = fieldRunners_->getState();

  if      (state == CFieldRunners::State::ACTIVE)
    startTimer();
  else
    stopTimer();
}

bool
Canvas::
loadMap(const QString &filename)
{
  bool running = isRunning();

  if (running)
    stopTimer();

  auto mapFile = buildDir() + "/maps/" + filename;

  bool rc = fieldRunners_->loadMap(mapFile.toStdString());

  valid_ = false;

  if (running)
    startTimer();

  hud_->invalidate();

  axes_->invalidate();

  return rc;
}

void
Canvas::
toggleTimer()
{
  if (! isRunning())
    startTimer();
  else
    stopTimer();
}

void
Canvas::
startTimer()
{
  if (! running_) {
    timer_->start(100);

    running_ = true;
  }
}

void
Canvas::
stopTimer()
{
  if (running_) {
    timer_->stop();

    running_ = false;
  }
}

//---

bool
Canvas::
isGridVisible() const
{
  return axes_->isVisible();
}

void
Canvas::
setGridVisible(bool b)
{
  axes_->setVisible(b);
}

//---

void
Canvas::
initializeGL()
{
  initializeOpenGLFunctions();

  //---

  font_ = new Font(this);

  font_->init();

  font_->setSize(48);
  font_->setFontName("OpenSans-Regular.ttf");
}

void
Canvas::
resizeGL(int width, int height)
{
  setPixelWidth (width);
  setPixelHeight(height);

  glViewport(0, 0, width, height);

  setAspect(double(width)/double(height));

  camera_->setAspect(aspect());

  Q_EMIT resized();
}

void
Canvas::
paintGL()
{
  auto state = fieldRunners_->getState();

  if      (state == CFieldRunners::State::SETUP)
    drawSetup();
  else if (state == CFieldRunners::State::ACTIVE)
    drawActive();
  else if (state == CFieldRunners::State::GAME_OVER)
    drawGameOver();
}

void
Canvas::
drawSetup()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  startUp_->drawGeometry();
}

void
Canvas::
drawActive()
{
  if (! valid_) {
    addScene();

    addShadows();

    addBullets();

    addHealthBars();

    valid_ = true;
  }

  //---

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //---

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  //glFrontFace(GL_CW);
  glFrontFace(GL_CCW);

  //---

#if 0
  glPointSize(pointSize());
  glLineWidth(lineWidth());
#endif

  //---

  auto *field = fieldRunners_->getField();
  if (! field) return;

  nr_ = field->getNumRows();
  nc_ = field->getNumCols();

  //---

  drawBg();

  drawShadows();

  drawScene();

  drawBullets();

  drawHealthBars();

  drawHUD();

  drawAxes();
}

void
Canvas::
drawGameOver()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  gameOver_->drawGeometry();
}

void
Canvas::
addScene()
{
  auto *field = fieldRunners_->getField();
  if (! field) return;

  //---

  nr_ = field->getNumRows();
  nc_ = field->getNumCols();

  fieldRunners_->getCellSize(cellSize_);

  //---

  loadObjects();

  auto bgType = fieldRunners_->bgType();

  if      (bgType == CFieldRunners::BgType::GRASSLANDS) {
    loadGrasslandsObjects();
  }
  else if (bgType == CFieldRunners::BgType::CROSSROADS) {
    loadCrossroadsObjects();
  }
  else if (bgType == CFieldRunners::BgType::DRYLANDS) {
    loadDrylandsObjects();
  }
  else if (bgType == CFieldRunners::BgType::CRYSTAL) {
    loadCrystalCavesObjects();
  }
  else if (bgType == CFieldRunners::BgType::SKYWAY) {
    loadSkywayObjects();
  }
  else if (bgType == CFieldRunners::BgType::FROSTBITE) {
    loadFrostbiteObjects();
  }
  else if (bgType == CFieldRunners::BgType::MUDSLIDE) {
    loadMudslideObjects();
  }
  else if (bgType == CFieldRunners::BgType::LAVAFLOW) {
    loadCrystalCavesObjects(); // tracks

    loadLavaflowObjects();
  }
  else
    std::cerr << "Invalid bg type " << int(bgType) << "\n";

  //---

  auto ng = uint(nr_*nc_);

  if (sizeGrid_.size() != ng) {
    sizeGrid_.resize(ng);

    for (uint i = 0; i < ng; ++i)
      sizeGrid_[i] = CMathRand::randInRange(0.8, 1.3);
  }

  //---

  updateObjDatas();

  addShadows();

  updateBullets();

  updateHealthBars();

  //---

  bbox_ = CBBox3D();

  objectMeshData_.clear();

  //---

  int    boneNodeIds[4];
  double boneWeights[4];

  //---

  for (auto *object : scene_->getObjects()) {
    auto *object1 = dynamic_cast<GeomObject *>(object);
    assert(object1);

    //---

    auto modelMatrix = CMatrix3DH(object->getHierTransform());
    auto meshMatrix  = CMatrix3DH(object->getMeshGlobalTransform());

    //---

    auto *animObject = object->getAnimObject();

    auto animName = (animObject ? animObject->animName() : "");

    bool isAnim = false;

    if (isAnimEnabled())
      isAnim = (animObject && animName != "");

    //double animTime { 0.0 };

    if (isAnim) {
      //animTime = animObject->animTime();

      auto meshNodeId = object->getMeshNode();

      CGeomNodeData *node = nullptr;

      if (meshNodeId >= 0)
        node = const_cast<CGeomNodeData *>(&animObject->getNode(meshNodeId));

      auto isJointed = (node && object->isJointed());

      if (node && ! isJointed) {
        auto &objectMeshData = objectMeshData_[object];

        objectMeshData.nt = object->animTimeFrames();

        (void) animObject->getAnimationTranslationRange(animName,
                 objectMeshData.tmin, objectMeshData.tmax);

        if (objectMeshData.nt > 1)
          objectMeshData.dt = (objectMeshData.tmax - objectMeshData.tmin)/(objectMeshData.nt - 1);
        else
          objectMeshData.dt = (objectMeshData.tmax - objectMeshData.tmin);

        for (int i = 0; i < objectMeshData.nt; ++i) {
          auto animTime1 = objectMeshData.tmin + i*objectMeshData.dt;

          auto meshMatrix1 =
            CMatrix3DH(object->getNodeAnimHierTransform(*node, animName, animTime1));

          objectMeshData.frameMatrix[i] = meshMatrix1;
        }
      }
    }

    //---

    auto *buffer = object1->initBuffer(this);

    //---

    auto *objectMaterial = object->getMaterialP();

    auto *diffuseTexture  = object->getDiffuseTexture();
    auto *normalTexture   = object->getNormalTexture();
    auto *specularTexture = object->getSpecularTexture();
    auto *emissiveTexture = object->getEmissiveTexture();

    //---

    CBBox3D bbox1;

    int pos = 0;

    auto addFaceData = [&](CGeomFace3D *face, bool reverse=false) {
      FaceData faceData;

      faceData.face = const_cast<CGeomFace3D *>(face);

      //---

      auto *faceMaterial = faceData.face->getMaterialP();

      if (! faceMaterial && objectMaterial)
        faceMaterial = objectMaterial;

      //---

      auto color = face->color().value_or(CRGBA(1, 1, 1));

      if (faceMaterial && faceMaterial->diffuse())
        color = faceMaterial->diffuse().value();

      //---

      // set face textures
      auto *diffuseTexture1  = face->getDiffuseTexture();
      auto *normalTexture1   = face->getNormalTexture();
      auto *specularTexture1 = face->getSpecularTexture();
      auto *emissiveTexture1 = face->getEmissiveTexture();

      if (! diffuseTexture1 ) diffuseTexture1  = diffuseTexture;
      if (! normalTexture1  ) normalTexture1   = normalTexture;
      if (! specularTexture1) specularTexture1 = specularTexture;
      if (! emissiveTexture1) emissiveTexture1 = emissiveTexture;

      if (faceMaterial) {
        if (faceMaterial->diffuseTexture ()) diffuseTexture1  = faceMaterial->diffuseTexture ();
        if (faceMaterial->normalTexture  ()) normalTexture1   = faceMaterial->normalTexture  ();
        if (faceMaterial->specularTexture()) specularTexture1 = faceMaterial->specularTexture();
        if (faceMaterial->emissiveTexture()) emissiveTexture1 = faceMaterial->emissiveTexture();
      }

      if (diffuseTexture1)
        faceData.diffuseTexture = getGLTexture(diffuseTexture1, /*add*/true);

      if (normalTexture1)
        faceData.normalTexture = getGLTexture(normalTexture1, /*add*/true);

      if (specularTexture1)
        faceData.specularTexture = getGLTexture(specularTexture1, /*add*/true);

      if (emissiveTexture1)
        faceData.emissiveTexture = getGLTexture(emissiveTexture1, /*add*/true);

      //---

      auto vertices = face->getVertices();

      if (reverse)
        std::reverse(vertices.begin(), vertices.end());

      //--

      // get face normal
      CVector3D normal;

      if (face->getNormalSet())
        normal = face->getNormal();
      else {
        for (const auto &v : vertices) {
          auto &vertex = object->getVertex(v);

          vertex.setViewed(vertex.getModel());
        }

        face->calcModelNormal(normal);
      }

      //---

      faceData.pos = pos;
      faceData.len = int(vertices.size());

      int iv = 0;

      for (const auto &v : vertices) {
        faceData.vertices.push_back(v);

        const auto &vertex = object->getVertex(v);
        const auto &model  = vertex.getModel();

        auto model1 = meshMatrix *model;
        auto model2 = modelMatrix*model1;

        //---

        // update color, normal for custom vertex value

        auto normal1 = normal;
        auto color1  = color;

        if      (face->hasVertexNormals())
          normal1 = face->getVertexNormal(iv);
        else if (vertex.hasNormal())
          normal1 = vertex.getNormal();

        if (vertex.hasColor())
          color1 = vertex.getColor();

        //---

        if (faceData.normalTexture) {
          CPoint2D tpoint;

          if (vertex.hasTextureMap())
            tpoint = vertex.getTextureMap();
          else
            tpoint = face->getTexturePoint(vertex, iv);

          int tw = faceData.normalTexture->getWidth ();
          int th = faceData.normalTexture->getHeight();

          auto tx = CMathUtil::clamp(tpoint.x, 0.0, 1.0);
          auto ty = CMathUtil::clamp(tpoint.y, 0.0, 1.0);

          // get normal value from texture
          auto rgba = faceData.normalTexture->getImage().pixel(tx*(tw - 1), ty*(th - 1));
          auto tnormal = CVector3D(qRed(rgba)/255.0, qGreen(rgba)/255.0, qBlue(rgba)/255.0);

          // this normal is in tangent space
          normal1 = (tnormal*2.0 - CVector3D(1.0, 1.0, 1.0)).normalized();
        }

        //---

        buffer->addInd(vertex.getInd());

        buffer->addPoint(float(model.x), float(model.y), float(model.z));

        buffer->addNormal(float(normal1.getX()), float(normal1.getY()), float(normal1.getZ()));

        buffer->addColor(color1);

        //---

        if (isAnim) {
          if (vertex.hasJointData()) {
            const auto &jointData = vertex.getJointData();

            for (int i = 0; i < 4; ++i) {
              boneNodeIds[i] = jointData.nodeDatas[i].node;
              boneWeights[i] = jointData.nodeDatas[i].weight;
            }

            buffer->addBoneIds    (boneNodeIds[0], boneNodeIds[1], boneNodeIds[2], boneNodeIds[3]);
            buffer->addBoneWeights(boneWeights[0], boneWeights[1], boneWeights[2], boneWeights[3]);
          }
        }

        //---

        if (faceData.diffuseTexture) {
          const auto &tpoint = face->getTexturePoint(vertex, iv);

          buffer->addTexturePoint(float(tpoint.x), float(tpoint.y));
        }
        else
          buffer->addTexturePoint(0.0f, 0.0f);

        //---

        ++iv;

        bbox1 += model2;
      }

      pos += faceData.len;

      object1->addFaceData(faceData);
    };

    //---

    const auto &faces = object->getFaces();

    for (auto *face : faces) {
      addFaceData(face);

      auto *faceMaterial = const_cast<CGeomFace3D *>(face)->getMaterialP();

      if (! faceMaterial && objectMaterial)
        faceMaterial = objectMaterial;

      if (face->getTwoSided() || (faceMaterial && faceMaterial->isTwoSided()))
        addFaceData(face, /*reverse*/true);
    }

    //---

#if 0
    const auto &lines = object->getLines();

    for (const auto *line : lines) {
      FaceData faceData;

      faceData.line = const_cast<CGeomLine3D *>(line);

      //---

      auto color = line->getColor();

      //---

      auto v1 = line->getStartInd();
      auto v2 = line->getEndInd  ();

      std::vector<uint> vertices;

      vertices.push_back(v1);
      vertices.push_back(v2);

      //--

      faceData.pos = pos;
      faceData.len = int(vertices.size());

      int iv = 0;

      for (const auto &v : vertices) {
        faceData.vertices.push_back(v);

        const auto &vertex = object->getVertex(v);
        const auto &model  = vertex.getModel();

        auto model1 = meshMatrix *model;
        auto model2 = modelMatrix*model1;

        //---

        // update color, normal for custom vertex value

        auto color1 = color;

        if (vertex.hasColor())
          color1 = vertex.getColor();

        //---

        buffer->addInd(vertex.getInd());

        buffer->addPoint(float(model.x), float(model.y), float(model.z));

        buffer->addNormal(0, 0, 1);

        buffer->addColor(color1);

        buffer->addTexturePoint(0.0f, 0.0f);

        //---

        ++iv;

        bbox1 += model2;
      }

      pos += faceData.len;

      object1->addFaceData(faceData);
    }
#endif

    //---

    object1->setBBox(bbox1);

    bbox_ += bbox1;

    //---

    buffer->load();
  }

  //---

  if (! bbox_.isSet()) {
    bbox_.add(CPoint3D(-1, -1, -1));
    bbox_.add(CPoint3D( 1,  1,  1));
  }

  //---

#if 0
  auto c = bbox_.getCenter();
  auto d = bbox_.getMaxSize();

  camera_->setOrigin(CVector3D(c));
  camera_->setDistance(std::sqrt(2.0)*d);
#endif
}

//---

ParticleBuffer *
Canvas::
getBulletBuffer()
{
  if (! bulletBuffer_) {
    bulletBuffer_ = new ParticleBuffer(bulletShaderProgram());

    bulletBuffer_->setFunctions(this);

    bulletBuffer_->init();

    bulletTexture_ = loadTexture("textures/bullet.png");

    if (bulletTexture_)
      bulletBuffer_->setTexture(bulletTexture_);

    bulletBuffer_->setParticleSize(0.3);
  }

  return bulletBuffer_;
}

ShaderProgram *
Canvas::
bulletShaderProgram()
{
  return getShader("bullet.vs", "bullet.fs");
}

//---

ParticleBuffer *
Canvas::
getGlueBuffer()
{
  if (! glueBuffer_) {
    glueBuffer_ = new ParticleBuffer(bulletShaderProgram());

    glueBuffer_->setFunctions(this);

    glueBuffer_->init();

    glueTexture_ = loadTexture("textures/glue.png");

    if (glueTexture_)
      glueBuffer_->setTexture(glueTexture_);

    glueBuffer_->setParticleSize(0.6);
  }

  return glueBuffer_;
}

//---

ParticleBuffer *
Canvas::
getPulseBuffer()
{
  if (! pulseBuffer_) {
    pulseBuffer_ = new ParticleBuffer(bulletShaderProgram());

    pulseBuffer_->setFunctions(this);

    pulseBuffer_->init();

    pulseTexture_ = loadTexture("textures/pulse.png");

    if (pulseTexture_)
      pulseBuffer_->setTexture(pulseTexture_);

    pulseBuffer_->setParticleSize(0.6);
  }

  return pulseBuffer_;
}

//---

CQGLBuffer *
Canvas::
getZapBuffer()
{
  if (! zapBuffer_)
    zapBuffer_ = new CQGLBuffer(zapShaderProgram());

  return zapBuffer_;
}

ShaderProgram *
Canvas::
zapShaderProgram()
{
  return getShader("zap.vs", "zap.fs");
}

//---

CQGLBuffer *
Canvas::
getLaserBuffer()
{
  if (! laserBuffer_)
    laserBuffer_ = new CQGLBuffer(laserShaderProgram());

  return laserBuffer_;
}

ShaderProgram *
Canvas::
laserShaderProgram()
{
  return getShader("laser.vs", "laser.fs");
}

//---

ParticleBuffer *
Canvas::
getFirebombBuffer()
{
  if (! firebombBuffer_) {
    firebombBuffer_ = new ParticleBuffer(firebombShaderProgram());

    firebombBuffer_->setFunctions(this);

    firebombBuffer_->init();

    firebombTexture_ = loadTexture("textures/firebomb.png");

    if (firebombTexture_)
      firebombBuffer_->setTexture(firebombTexture_);

    firebombBuffer_->setParticleSize(0.6);
  }

  return firebombBuffer_;
}

ShaderProgram *
Canvas::
firebombShaderProgram()
{
  return getShader("firebomb.vs", "firebomb.fs");
}

//---

void
Canvas::
addBullets()
{
  auto *bulletBuffer = getBulletBuffer();

  bulletBuffer->clear();

  //---

  auto *glueBuffer = getGlueBuffer();

  glueBuffer ->clear();

  //---

  auto *pulseBuffer = getPulseBuffer();

  pulseBuffer ->clear();

  //---

  auto *zapBuffer = getZapBuffer();

  zapBuffer->clearAll();

  zapFaceDataList_.clear();

  //---

  auto *laserBuffer = getLaserBuffer();

  laserBuffer->clearAll();

  laserFaceDataList_.clear();

  //---

  auto *firebombBuffer = getFirebombBuffer();

  firebombBuffer ->clear();

  //---

  hasMissiles_ = false;
  hasZap_      = false;
  hasLaser_    = false;

  auto c = CRGBA::white();

  auto addZapLine = [&](const CPoint2D &p1, const CPoint2D &p2) {
    static std::vector<CPoint2D> points = {
      CPoint2D(50.000000,  0.000000),
      CPoint2D(16.042501, 19.895148),
      CPoint2D(74.745325, 30.558091),
      CPoint2D(32.375902, 48.783651),
      CPoint2D(69.460110, 59.388759),
      CPoint2D(43.654246, 74.201968),
      CPoint2D(66.118474, 81.933434),
      CPoint2D(50.000000,100.000000)
    };

    hasZap_ = true;

    FaceData faceData;

    faceData.pos = zapFaceDataList_.pos;
    faceData.len = points.size();

    zapFaceDataList_.faceDatas.push_back(faceData);

    auto a = std::atan2(p2.y - p1.y, p2.x - p1.x);
    auto d = p1.distanceTo(p2);

    auto m1 = CMatrix2D::rotation(a);
    auto m2 = CMatrix2D::scale(d, sqrt(d));

    for (const auto &p : points) {
      auto p11 = m1*m2*CPoint2D(p.y/100.0, p.x/100.0 - 0.5);
      auto p12 = p1 + p11;

      zapBuffer->addPoint(CPoint3D(p12.x, 0.25, p12.y));
      zapBuffer->addColor(c);
    }
  };

  auto addLaserLine = [&](const CPoint2D &p, CFieldRunners::Orient orient) {
    hasLaser_ = true;

    FaceData faceData;

    faceData.pos = laserFaceDataList_.pos;
    faceData.len = 2;

    laserFaceDataList_.faceDatas.push_back(faceData);

    laserBuffer->addPoint(CPoint3D(p.x, 0.25, p.y));
    laserBuffer->addColor(c);

    auto p1 = p;

    if      (orient == CFieldRunners::ORIENT_N)
      p1.y += nr_;
    else if (orient == CFieldRunners::ORIENT_S)
      p1.y -= nr_;
    else if (orient == CFieldRunners::ORIENT_E)
      p1.x += nc_;
    else if (orient == CFieldRunners::ORIENT_W)
      p1.x -= nc_;

    laserBuffer->addPoint(CPoint3D(p1.x, 0.25, p1.y));
    laserBuffer->addColor(c);
  };

  for (const auto &bulletData : bulletDatas_) {
    if (! bulletData.active)
      continue;

    auto p = CPoint3D(bulletData.x, 0.5, bulletData.y);

    if      (bulletData.bullet->bulletType() == CFieldRunners::BulletType::BULLET) {
      bulletBuffer->addPoint(p);
      bulletBuffer->addColor(c);
    }
    else if (bulletData.bullet->bulletType() == CFieldRunners::BulletType::GLUE) {
      glueBuffer->addPoint(p);
      glueBuffer->addColor(c);
    }
    else if (bulletData.bullet->bulletType() == CFieldRunners::BulletType::MISSILE) {
      hasMissiles_ = true;
    }
    else if (bulletData.bullet->bulletType() == CFieldRunners::BulletType::PULSE) {
      pulseBuffer->addPoint(p);
      pulseBuffer->addColor(c);
    }
    else if (bulletData.bullet->bulletType() == CFieldRunners::BulletType::LASER) {
      auto *laserBullet = dynamic_cast<CFieldRunners::LaserBullet *>(bulletData.bullet);

      auto orient = laserBullet->orient();

      addLaserLine(CPoint2D(bulletData.x, bulletData.y), orient);
    }
    else if (bulletData.bullet->bulletType() == CFieldRunners::BulletType::FIREBOMB) {
      auto *firebombBullet = dynamic_cast<CFieldRunners::FirebombBullet *>(bulletData.bullet);

      p.y = firebombBullet->radius();

      firebombBuffer->addPoint(p);
      firebombBuffer->addColor(c);
    }
    else if (bulletData.bullet->bulletType() == CFieldRunners::BulletType::ZAP) {
      auto *zapBullet = dynamic_cast<CFieldRunners::ZapBullet *>(bulletData.bullet);

      auto p1 = zapBullet->target();

      auto row = double(p1.y)/double(cellSize_.height);
      auto col = double(p1.x)/double(cellSize_.width );

      double wx, wy;
      posToWorld(row, col, wx, wy);

      addZapLine(CPoint2D(bulletData.x, bulletData.y), CPoint2D(wx, wy));
    }
    else {
      std::cerr << "Invalid bullet type: " << int(bulletData.bullet->bulletType()) << "\n";
    }
  }

  //addZapLine(CPoint2D(-2, -2), CPoint2D(2, 2));
  //addZapLine(CPoint2D(-2, 0), CPoint2D(2, 0));

  if (hasZap_)
    zapBuffer->load();

  if (hasLaser_)
    laserBuffer->load();
}

ParticleBuffer *
Canvas::
getHealthBarBuffer()
{
  if (! healthBarBuffer_) {
    healthBarBuffer_ = new ParticleBuffer(healthBarShaderProgram());

    healthBarBuffer_->setFunctions(this);

    healthBarBuffer_->init();

    healthBarBuffer_->setParticleSize(1.0);
  }

  return healthBarBuffer_;
}

ShaderProgram *
Canvas::
healthBarShaderProgram()
{
  return getShader("healthbar.vs", "healthbar.fs");
}

void
Canvas::
addHealthBars()
{
  auto *healthBarBuffer = getHealthBarBuffer();

  //---

  auto c1 = CRGBA::green();
  auto c2 = CRGBA::red();

  healthBarBuffer->clear();

  for (const auto &healthBarData : healthBarDatas_) {
    if (! healthBarData.active)
      continue;

    auto p1 = CPoint3D(healthBarData.x,         healthBarData.health , healthBarData.y);
    auto p2 = CPoint3D(healthBarData.x, -(1.0 - healthBarData.health), healthBarData.y);

    healthBarBuffer->addPoint(p1);
    healthBarBuffer->addColor(c1);

    healthBarBuffer->addPoint(p2);
    healthBarBuffer->addColor(c2);
  }
}

void
Canvas::
loadGrasslandsObjects()
{
  (void) getGrasslandsObj();
  (void) getTreeObj();
}

CGeomObject3D *
Canvas::
getGrasslandsObj()
{
  if (! grasslandsObj_) {
    grasslandsObj_ = loadModel(buildDir() + "/models/grasslands.scene", CGEOM_3D_TYPE_SCENE);

    placeField(grasslandsObj_, nr_, nc_);
  }

  return grasslandsObj_;
}

void
Canvas::
loadCrossroadsObjects()
{
  if (! crossroadsObj_) {
    crossroadsObj_ = loadModel(buildDir() + "/models/crossroads.scene", CGEOM_3D_TYPE_SCENE);

    placeField(crossroadsObj_, 16, 23);
  }

  getMetalBoxObj();
}

void
Canvas::
loadDrylandsObjects()
{
  if (! drylandsObj_) {
    drylandsObj_ = loadModel(buildDir() + "/models/drylands.scene", CGEOM_3D_TYPE_SCENE);

    placeField(drylandsObj_, nr_, nc_);
  }

  getRockObj();
  getPalmTreeObj();
}

void
Canvas::
loadCrystalCavesObjects()
{
  if (! crystalCavesObj_) {
    crystalCavesObj_ = loadModel(buildDir() + "/models/crystal_caves.scene", CGEOM_3D_TYPE_SCENE);

    placeField(crystalCavesObj_, nr_, nc_);
  }

  (void) getPurpleRockObj();

  (void) getTrackObj();
  (void) getTrackCornerObj();

  (void) getEmptyTrackObj();
}

void
Canvas::
loadSkywayObjects()
{
  if (! skywayObj_) {
    //skywayObj_ = loadModel(buildDir() + "/models/skyway.scene", CGEOM_3D_TYPE_SCENE);
    skywayObj_ = loadModel(buildDir() + "/models/skyway.obj", CGEOM_3D_TYPE_OBJ);

    placeField(skywayObj_, nr_, nc_);
  }

  (void) getTreeObj();
}

void
Canvas::
loadFrostbiteObjects()
{
  if (! frostbiteObj_) {
    frostbiteObj_ = loadModel(buildDir() + "/models/frostbite.scene", CGEOM_3D_TYPE_SCENE);

    //placeField(frostbiteObj_, nr_, nc_);
  }

  (void) getPineTreeObj();
}

void
Canvas::
loadMudslideObjects()
{
  if (! mudslideObj_) {
    mudslideObj_ = loadModel(buildDir() + "/models/mudslide.scene", CGEOM_3D_TYPE_SCENE);

    placeField(mudslideObj_, nr_, nc_);
  }

  (void) getTreeObj();
  (void) getRockObj();

  (void) getTrackObj();
  (void) getTrackCornerObj();

  (void) getEmptyTrackObj();
}

void
Canvas::
loadLavaflowObjects()
{
  if (! lavaflowObj_) {
    //lavaflowObj_ = loadModel(buildDir() + "/models/lavaflow.scene", CGEOM_3D_TYPE_SCENE);

    //placeField(lavaflowObj_, nr_, nc_);
    lavaflowObj_ = loadModel(buildDir() + "/models/lavaflow.obj", CGEOM_3D_TYPE_OBJ);

    lavaflowLavaObj_ = lavaflowObj_->getChildOfName("Lava");
  }

  (void) getLavaRockObj();
}

void
Canvas::
placeField(CGeomObject3D *object, int nr, int nc) const
{
#if 0
  auto printSize = [&]() {
    CBBox3D bbox;
    object->getModelBBox(bbox);

    auto c = bbox.getCenter(); // 0,0,0
    auto s = bbox.getSize();   // 2,2,2
    std::cerr << c << " " << s << "\n";
  };
#endif

  //printSize();

  CBBox3D bbox;
  object->getModelBBox(bbox);
  auto c = bbox.getCenter(); // 0,0,0
  auto s = bbox.getSize();   // 2,2,2

  object->translate(-c.x, -c.y, -c.z, /*hier*/true);
  //printSize();
  object->scale(nc/s.x(), 0.01/s.y(), nr/s.z(), /*hier*/true);
  //printSize();

  bbox = CBBox3D();
  object->getModelBBox(bbox);
  s = bbox.getSize();

  object->translate(0.0, -s.y(), 0.0, /*hier*/true);
  //printSize();
}

//---

CGeomObject3D *
Canvas::
getTreeObj()
{
  if (! treeObj_) {
    treeObj_ = loadModel(buildDir() + "/models/tree.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    treeObj_->getModelBBox(bbox);
    //std::cerr << "Tree: " << bbox << "\n";

    auto c = bbox.getCenter();
    auto s = bbox.getSize();
    //std::cerr << c << " " << s << "\n";

    auto scale = std::min(1/s.x(), 1/s.z());

    treeObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    treeObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return treeObj_;
}

CGeomObject3D *
Canvas::
getMetalBoxObj()
{
  if (! metalBoxObj_) {
    metalBoxObj_ = loadModel(buildDir() + "/models/metal_box.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    metalBoxObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    metalBoxObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    metalBoxObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return metalBoxObj_;
}

CGeomObject3D *
Canvas::
getPalmTreeObj()
{
  if (! palmTreeObj_) {
    palmTreeObj_ = loadModel(buildDir() + "/models/palm_tree.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    palmTreeObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    palmTreeObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    palmTreeObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return palmTreeObj_;
}

CGeomObject3D *
Canvas::
getTrackObj()
{
  if (! trackObj_) {
    trackObj_ = loadModel(buildDir() + "/models/track.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    trackObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    trackObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    trackObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return trackObj_;
}

CGeomObject3D *
Canvas::
getTrackCornerObj()
{
  if (! trackCornerObj_) {
    trackCornerObj_ = loadModel(buildDir() + "/models/track_corner.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    trackCornerObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    trackCornerObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    trackCornerObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return trackCornerObj_;
}

CGeomObject3D *
Canvas::
getEmptyTrackObj()
{
  if (! emptyTrackObj_) {
    emptyTrackObj_ = loadModel(buildDir() + "/models/empty_track.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    emptyTrackObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    emptyTrackObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    emptyTrackObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return emptyTrackObj_;
}

CGeomObject3D *
Canvas::
getRockObj()
{
  if (! rockObj_) {
    rockObj_ = loadModel(buildDir() + "/models/rock.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    rockObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    rockObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    rockObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return rockObj_;
}

CGeomObject3D *
Canvas::
getPurpleRockObj()
{
  if (! purpleRockObj_) {
    purpleRockObj_ = loadModel(buildDir() + "/models/purple_rock.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    purpleRockObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    purpleRockObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    purpleRockObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return purpleRockObj_;
}

CGeomObject3D *
Canvas::
getLavaRockObj()
{
  if (! lavaRockObj_) {
    lavaRockObj_ = loadModel(buildDir() + "/models/lava_rock.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    lavaRockObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    lavaRockObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    lavaRockObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return lavaRockObj_;
}

CGeomObject3D *
Canvas::
getPineTreeObj()
{
  if (! pineTreeObj_) {
    pineTreeObj_ = loadModel(buildDir() + "/models/pine.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    pineTreeObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    pineTreeObj_->translate(-c.x, s.y()/2.0 - c.y, -c.z, /*hier*/true);
    pineTreeObj_->scale(scale, scale, scale, /*hier*/true);
  }

  return pineTreeObj_;
}

void
Canvas::
loadObjects()
{
  auto fitObj = [&](CGeomObject3D *obj, double scale, double dy) {
    CBBox3D bbox;
    obj->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::scale(scale, scale, scale);
    auto m3 = CMatrix3D::translation(0, dy, 0);

    obj->transform(m3*m2*m1, /*hier*/true);
  };

  auto fitObjZ = [&](CGeomObject3D *obj, double sz) {
    CBBox3D bbox;
    obj->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = sz/s.z();

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::scale(scale, scale, scale);
    auto m3 = CMatrix3D::translation(0, sz/2.0, 0);

    obj->transform(m3*m2*m1, /*hier*/true);
  };

  auto fitObjBase = [&](CGeomObject3D *obj) {
    CBBox3D bbox;
    obj->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::scale(scale, scale, scale);

    obj->transform(m2*m1, /*hier*/true);

    CBBox3D bbox1;
    obj->getTransformedModelBBox(bbox1);
    c = bbox1.getCenter();
    s = bbox1.getSize();

    auto m3 = CMatrix3D::translation(0, s.y()/2.0 - c.y, 0);

    obj->transform(m3, /*hier*/true);
  };

  //---

  // Guns

  if (! bulletGunObj_) {
    bulletGunObj_ = loadModel(buildDir() + "/models/gun.obj", CGEOM_3D_TYPE_OBJ);

    fitObj(bulletGunObj_, /*scale*/0.5, /*dy*/0.5);
  }

  if (! glueGunObj_) {
    glueGunObj_ = loadModel(buildDir() + "/models/glue.obj", CGEOM_3D_TYPE_OBJ);

    fitObjBase(glueGunObj_);
    //fitObjZ(glueGunObj_, /*sz*/1.1);
  }

  if (! snowBombObj_) {
    snowBombObj_ = loadModel(buildDir() + "/models/snow_bomb.obj", CGEOM_3D_TYPE_OBJ);

    fitObjBase(snowBombObj_);
    //fitObjZ(snowBombObj_, /*sz*/1.1);
  }

  if (! missileGunObj_) {
    missileGunObj_ = loadModel(buildDir() + "/models/missile_gun.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(missileGunObj_, /*sz*/1.1);
  }

  if (! shotgunObj_) {
    shotgunObj_ = loadModel(buildDir() + "/models/shotgun.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(shotgunObj_, /*sz*/1.1);
  }

  if (! zapTowerObj_) {
    zapTowerObj_ = loadModel(buildDir() + "/models/zap_tower.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(zapTowerObj_, /*sz*/1.1);
  }

  if (! pulseObj_) {
    pulseObj_ = loadModel(buildDir() + "/models/pulse.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(pulseObj_, /*sz*/1.1);
  }

  if (! laserGunObj_) {
    laserGunObj_ = loadModel(buildDir() + "/models/laser_gun.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(laserGunObj_, /*sz*/1.1);
  }

  if (! fireBombObj_) {
    fireBombObj_ = loadModel(buildDir() + "/models/fire_bomb.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(fireBombObj_, /*sz*/1.1);
  }

  if (! flameGunObj_) {
    flameGunObj_ = loadModel(buildDir() + "/models/flame_gun.obj", CGEOM_3D_TYPE_OBJ);

    fitObjZ(flameGunObj_, /*sz*/1.1);
  }

  //---

  if (! missileObj_) {
    missileObj_ = loadModel(buildDir() + "/models/missile.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    missileObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = 0.5/s.z();

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::scale(scale, scale, scale);
    auto m3 = CMatrix3D::translation(0, 0.25, 0);

    missileObj_->transform(m3*m2*m1, /*hier*/true);
  }

  //---

  auto fitFloorObj = [&](CGeomObject3D *obj) {
    CBBox3D bbox;
    obj->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::scale(scale, 0.1, scale);
    auto m3 = CMatrix3D::translation(0, 0.05, 0);

    obj->transform(m3*m2*m1, /*hier*/true);
  };

#if 0
  if (! entranceObj_) {
    entranceObj_ = loadModel(buildDir() + "/models/entrance.scene", CGEOM_3D_TYPE_SCENE);

    fitFloorObj(entranceObj_);
  }
#endif

  if (! currentCellObj_) {
    currentCellObj_ = loadModel(buildDir() + "/models/entrance.scene", CGEOM_3D_TYPE_SCENE);

    fitFloorObj(currentCellObj_);
  }

  //---

  (void) getSoldierObj();
  (void) getMercenaryObj();
}

CGeomObject3D *
Canvas::
getSoldierObj()
{
  if (! solidierObj_) {
    solidierObj_ = loadModel(buildDir() + "/models/Barbarian.glb", CGEOM_3D_TYPE_GLTF);

#if 0
    auto printSize = [&]() {
      CBBox3D bbox;
      solidierObj_->getTransformedModelBBox(bbox);

      auto c = bbox.getCenter(); // 0,0,0
      auto s = bbox.getSize();   // 2,2,2
      std::cerr << c << " " << s << "\n";
    };
#endif

    CBBox3D bbox1;
    solidierObj_->getTransformedModelBBox(bbox1);
    auto c = bbox1.getCenter();
    auto s = bbox1.getSize();

    //printSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    scale *= 0.75;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);

    solidierObj_->setTransform(m3*m2*m1);

    //printSize();

#if 0
    CBBox3D bbox2;
    solidierObj_->getTransformedModelBBox(bbox2);
    s = bbox2.getSize();

    auto m4 = CMatrix3D::translation(0, s.y()/2.0, 0.0);
#else
    auto m4 = CMatrix3D::translation(0, 0.25, 0.0);
#endif
    solidierObj_->setTransform(m4*m3*m2*m1);

    //printSize();

    //---

    solidierObj_->setAnimName("Walking_A");

    solidierObj_->getChildOfName("Barbarian_Hat" )->setVisible(false);
    solidierObj_->getChildOfName("Mug"           )->setVisible(false);
    solidierObj_->getChildOfName("1H_Axe"        )->setVisible(false);
    solidierObj_->getChildOfName("1H_Axe_Offhand")->setVisible(false);

    valid_ = false;
  }

  return solidierObj_;
}

CGeomObject3D *
Canvas::
getMercenaryObj()
{
  if (! mercenaryObj_) {
    mercenaryObj_ = loadModel(buildDir() + "/models/Knight.glb", CGEOM_3D_TYPE_GLTF);

    CBBox3D bbox;
    mercenaryObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, ys/2.0, 0);

    mercenaryObj_->setTransform(m4*m3*m2*m1);

    //---

    mercenaryObj_->setAnimName("Walking_A");

    mercenaryObj_->getChildOfName("Badge_Shield"    )->setVisible(false);
    mercenaryObj_->getChildOfName("Round_Shield"    )->setVisible(false);
    mercenaryObj_->getChildOfName("Spike_Shield"    )->setVisible(false);
    mercenaryObj_->getChildOfName("1H_Sword"        )->setVisible(false);
    mercenaryObj_->getChildOfName("1H_Sword_Offhand")->setVisible(false);

    valid_ = false;
  }

  return mercenaryObj_;
}

CGeomObject3D *
Canvas::
getMotorbikeObj()
{
  if (! motorbikeObj_) {
    motorbikeObj_ = loadModel(buildDir() + "/models/motorbike.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    motorbikeObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, ys/2.0, 0);

    motorbikeObj_->setTransform(m4*m3*m2*m1);

    valid_ = false;
  }

  return motorbikeObj_;
}

CGeomObject3D *
Canvas::
getTankObj()
{
  if (! tankObj_) {
    tankObj_ = loadModel(buildDir() + "/models/tank.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    tankObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, ys/2.0, 0);

    tankObj_->setTransform(m4*m3*m2*m1);

    valid_ = false;
  }

  return tankObj_;
}

CGeomObject3D *
Canvas::
getHelicopterObj()
{
  if (! helicopterObj_) {
    helicopterObj_ = loadModel(buildDir() + "/models/helicopter.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    helicopterObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = 1.75*std::min(1/s.x(), 1/s.z());
  //auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::rotation(M_PI, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, 0.8, 0);

    helicopterObj_->setTransform(m4*m3*m2*m1);

    helicopterPropObj_ = helicopterObj_->getChildOfName("Propellor");

    valid_ = false;
  }

  return helicopterObj_;
}

CGeomObject3D *
Canvas::
getCartObj()
{
  if (! cartObj_) {
    cartObj_ = loadModel(buildDir() + "/models/cart.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    cartObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
//  auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m2 = CMatrix3D::rotation(M_PI, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, ys/2.0, 0);

    cartObj_->setTransform(m4*m3*m2*m1);

    valid_ = false;
  }

  return cartObj_;
}

CGeomObject3D *
Canvas::
getTrainObj()
{
  if (! trainObj_) {
    trainObj_ = loadModel(buildDir() + "/models/train.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    trainObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
//  auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m2 = CMatrix3D::rotation(M_PI, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, ys/2.0, 0);

    trainObj_->setTransform(m4*m3*m2*m1);

    valid_ = false;
  }

  return trainObj_;
}

CGeomObject3D *
Canvas::
getBlimpObj()
{
  if (! blimpObj_) {
    blimpObj_ = loadModel(buildDir() + "/models/blimp.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    blimpObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = std::min(1/s.x(), 1/s.z());
    auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
//  auto m2 = CMatrix3D::rotation(M_PI/2.0, CVector3D(0, 1, 0));
    auto m2 = CMatrix3D::rotation(M_PI, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, ys/2.0, 0);

    blimpObj_->setTransform(m4*m3*m2*m1);

    valid_ = false;
  }

  return blimpObj_;
}

CGeomObject3D *
Canvas::
getPlaneObj()
{
  if (! planeObj_) {
    planeObj_ = loadModel(buildDir() + "/models/plane.obj", CGEOM_3D_TYPE_OBJ);

    CBBox3D bbox;
    planeObj_->getModelBBox(bbox);

    auto c = bbox.getCenter();
    auto s = bbox.getSize();

    auto scale = 1.75*std::min(1/s.x(), 1/s.z());
//  auto ys    = s.y()*scale;

    auto m1 = CMatrix3D::translation(-c.x, -c.y, -c.z);
    auto m2 = CMatrix3D::rotation(M_PI, CVector3D(0, 1, 0));
    auto m3 = CMatrix3D::scale(scale, scale, scale);
    auto m4 = CMatrix3D::translation(0, 1.0, 0);

    planeObj_->setTransform(m4*m3*m2*m1);

    valid_ = false;
  }

  return planeObj_;
}

void
Canvas::
updateObjDatas()
{
  auto rotateWeapon = [&](ObjData &objData, CFieldRunners::WeaponCell *cell) {
    auto orient = cell->orient();

    switch (orient) {
      default: objData.angle = 0.0; break;
      case CFieldRunners::Orient::ORIENT_N : objData.angle =      M_PI/2.0; break;
      case CFieldRunners::Orient::ORIENT_S : objData.angle =     -M_PI/2.0; break;
      case CFieldRunners::Orient::ORIENT_E : objData.angle =           0.0; break;
      case CFieldRunners::Orient::ORIENT_W : objData.angle =      M_PI    ; break;
      case CFieldRunners::Orient::ORIENT_NE: objData.angle =      M_PI/4.0; break;
      case CFieldRunners::Orient::ORIENT_NW: objData.angle =  3.0*M_PI/4.0; break;
      case CFieldRunners::Orient::ORIENT_SE: objData.angle =     -M_PI/4.0; break;
      case CFieldRunners::Orient::ORIENT_SW: objData.angle = -3.0*M_PI/4.0; break;
    }
  };

  //---

  auto *field = fieldRunners_->getField();
  if (! field) return;

  nr_ = field->getNumRows();
  nc_ = field->getNumCols();

  if (int(objDatas_.size()) != nr_*nc_)
    objDatas_.resize(nr_*nc_);

  for (int ir = 0; ir < nr_; ++ir) {
    for (int ic = 0; ic < nc_; ++ic) {
      auto ind = calcInd(ir, ic);

      auto &objData = objDatas_[ind];

      objData.reset();

      objData.hideParts.clear();

      objData.row = ir;
      objData.col = ic;

      auto cellPos = CFieldRunners::CellPos(ir, ic);

      CFieldRunners::FieldCell *cell;
      fieldRunners_->getCell(cellPos, &cell);

      auto type = cell->type();

      if      (type == CFieldRunners::CellType::EMPTY) {
#if 0
        auto function = cell->function();

        if (function == CFieldRunners::CellFunction::ENTRANCE ||
            function == CFieldRunners::CellFunction::EXIT) {
          objData.active = true;
          objData.object = entranceObj_;
        }
#endif

        auto subType = cell->subType();

        if (subType == CFieldRunners::CellSubType::TRACK) {
          bool angled = false;

          auto direction = cell->direction();

          switch (direction) {
            default: objData.angle = 0.0; break;
            case CFieldRunners::CellDirection::N :
              objData.angle =      M_PI/2.0; angled = false; break;
            case CFieldRunners::CellDirection::S :
              objData.angle =     -M_PI/2.0; angled = false; break;
            case CFieldRunners::CellDirection::E :
              objData.angle =           0.0; angled = false; break;
            case CFieldRunners::CellDirection::W :
              objData.angle =      M_PI    ; angled = false; break;
            case CFieldRunners::CellDirection::NE:
              objData.angle =      M_PI/4.0; angled = true; break;
            case CFieldRunners::CellDirection::SE:
              objData.angle =     -M_PI/4.0; angled = true; break;
            case CFieldRunners::CellDirection::NW:
              objData.angle =  3.0*M_PI/4.0; angled = true; break;
            case CFieldRunners::CellDirection::SW:
              objData.angle = -3.0*M_PI/4.0; angled = true; break;
          }

          objData.active = true;
          objData.object = (angled ? getTrackCornerObj() : getEmptyTrackObj());

          if (objData.object == trackCornerObj_) objData.angle -= 3.0*M_PI/4.0;
        }
      }
      else if (type == CFieldRunners::CellType::BORDER) {
        objData.active = true;

        auto borderType = fieldRunners_->borderType();

        if      (borderType == CFieldRunners::BorderType::GRASSLANDS) {
          objData.size   = sizeGrid_[ind];
          objData.object = getTreeObj();
        }
        else if (borderType == CFieldRunners::BorderType::CROSSROADS) {
          auto subType = cell->subType();

          if (subType == CFieldRunners::CellSubType::ROCK) {
            objData.object = getMetalBoxObj();
          }
          else
            objData.active = false;
        }
        else if (borderType == CFieldRunners::BorderType::DRYLANDS) {
          auto subType = cell->subType();

          if (subType == CFieldRunners::CellSubType::ROCK) {
            objData.size   = sizeGrid_[ind];
            objData.object = getRockObj();
          }
          else {
            objData.size   = 1.2*sizeGrid_[ind];
            objData.object = getPalmTreeObj();
          }
        }
        else if (borderType == CFieldRunners::BorderType::CRYSTAL) {
          objData.size   = sizeGrid_[ind];
          objData.object = getPurpleRockObj();
        }
        else if (borderType == CFieldRunners::BorderType::SKYWAY) {
          objData.size = sizeGrid_[ind];

          if (cell->isTransparent()) {
            objData.object = getTreeObj();
          }
          else {
            objData.object = getTreeObj();
          }
        }
        else if (borderType == CFieldRunners::BorderType::FROSTBITE) {
          objData.size   = sizeGrid_[ind];
          objData.object = getPineTreeObj();
        }
        else if (borderType == CFieldRunners::BorderType::MUDSLIDE) {
          objData.size   = sizeGrid_[ind];
          objData.object = getTreeObj();
        }
        else if (borderType == CFieldRunners::BorderType::LAVAFLOW) {
          auto subType = cell->subType();

          if (subType == CFieldRunners::CellSubType::STONE) {
            objData.size   = sizeGrid_[ind];
            objData.object = getLavaRockObj();
          }
          else
            objData.active = false;
        }
        else {
          std::cerr << "Invalid border type " << int(borderType) << "\n";
          objData.active = false;
        }
      }
      else if (type == CFieldRunners::CellType::BLOCK) {
        objData.active = true;

        auto subType = cell->subType();

        if      (subType == CFieldRunners::CellSubType::NONE) {
          objData.active = false;
        }
        else if (subType == CFieldRunners::CellSubType::ROCK) {
          objData.object = getRockObj();
        }
        else if (subType == CFieldRunners::CellSubType::TRACK) {
          bool angled = false;

          auto direction = cell->direction();

          switch (direction) {
            default: objData.angle = 0.0; break;
            case CFieldRunners::CellDirection::N :
              objData.angle =      M_PI/2.0; angled = false; break;
            case CFieldRunners::CellDirection::S :
              objData.angle =     -M_PI/2.0; angled = false; break;
            case CFieldRunners::CellDirection::E :
              objData.angle =           0.0; angled = false; break;
            case CFieldRunners::CellDirection::W :
              objData.angle =      M_PI    ; angled = false; break;
            case CFieldRunners::CellDirection::NE:
              objData.angle =      M_PI/4.0; angled = true; break;
            case CFieldRunners::CellDirection::SE:
              objData.angle =     -M_PI/4.0; angled = true; break;
            case CFieldRunners::CellDirection::NW:
              objData.angle =  3.0*M_PI/4.0; angled = true; break;
            case CFieldRunners::CellDirection::SW:
              objData.angle = -3.0*M_PI/4.0; angled = true; break;
          }

          objData.object = (angled ? getTrackCornerObj() : getTrackObj());

          if (objData.object == trackCornerObj_) objData.angle -= 3.0*M_PI/4.0;
        }
        else {
          std::cerr << "Invalid block sub type " << int(subType) << "\n";
          objData.active = false;
        }
      }
      else if (type == CFieldRunners::CellType::GUN) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = bulletGunObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);

        rotateWeapon(objData, weaponCell);
      }
      else if (type == CFieldRunners::CellType::GLUE) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = glueGunObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);

        rotateWeapon(objData, weaponCell);
      }
      else if (type == CFieldRunners::CellType::SNOWBOMB) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = snowBombObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);
      }
      else if (type == CFieldRunners::CellType::MISSILE) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = missileGunObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);

        rotateWeapon(objData, weaponCell);
      }
      else if (type == CFieldRunners::CellType::SHOTGUN) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = shotgunObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);

        rotateWeapon(objData, weaponCell);
      }
      else if (type == CFieldRunners::CellType::ZAP) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = zapTowerObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);
      }
      else if (type == CFieldRunners::CellType::PULSE) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = pulseObj_;
        objData.shadow = true;

        hideWeaponParts(weaponCell, objData);
      }
      else if (type == CFieldRunners::CellType::LASER) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = laserGunObj_;
        objData.shadow = true;

        rotateWeapon(objData, weaponCell);
      }
      else if (type == CFieldRunners::CellType::FIREBOMB) {
        objData.active = true;
        objData.object = fireBombObj_;
        objData.shadow = true;
      }
      else if (type == CFieldRunners::CellType::FLAME) {
        auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

        objData.active = true;
        objData.object = flameGunObj_;
        objData.shadow = true;

        rotateWeapon(objData, weaponCell);
      }
      else {
        std::cerr << "Invalid cell type " << int(type) << "\n";
        objData.active = false;
      }

      //---

      auto currentCell = fieldRunners_->currentCell();

      if (ir == currentCell.row && ic == currentCell.col) {
        if (! objData.active) {
          objData.active = true;
          objData.object = currentCellObj_;
        }
      }
    }
  }

  for (uint ir = 0; ir < fieldRunners_->getNumRunners(); ++ir) {
    auto *runner = fieldRunners_->getRunner(ir);
    if (runner->isDead()) continue;

    auto cellPos = runner->getPos();

    auto ind = calcInd(cellPos.row, cellPos.col);

    auto &objData = objDatas_[ind];

    objData.x = cellPos.col + double(runner->getDx())/double(cellSize_.width );
    objData.y = cellPos.row + double(runner->getDy())/double(cellSize_.height);

    auto type = runner->type();

    if      (type == CFieldRunners::RunnerType::SOLDIER) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getSoldierObj();
    }
    else if (type == CFieldRunners::RunnerType::MERCENARY) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getMercenaryObj();
    }
    else if (type == CFieldRunners::RunnerType::MOTORBIKE) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getMotorbikeObj();
    }
    else if (type == CFieldRunners::RunnerType::CAR) {
      std::cerr << "Unhandled car runner\n";
    }
    else if (type == CFieldRunners::RunnerType::HEAVYBIKE) {
      std::cerr << "Unhandled heavybike runner\n";
    }
    else if (type == CFieldRunners::RunnerType::TANK) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getTankObj();
    }
    else if (type == CFieldRunners::RunnerType::HELICOPTER) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getHelicopterObj();
    }
    else if (type == CFieldRunners::RunnerType::PLANE) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getPlaneObj();
    }
    else if (type == CFieldRunners::RunnerType::CART) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getCartObj();
    }
    else if (type == CFieldRunners::RunnerType::TRAIN) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getTrainObj();
    }
    else if (type == CFieldRunners::RunnerType::BLIMP) {
      objData.active = true;
      objData.shadow = true;
      objData.runner = getBlimpObj();
    }
    else {
      std::cerr << "Unhandled runner " << int(type) << "\n";
    }

    auto orient = runner->orient();

    switch (orient) {
      default: objData.angle = 0.0; break;
      case CFieldRunners::Orient::ORIENT_N : objData.runnerAngle =      M_PI/2.0; break;
      case CFieldRunners::Orient::ORIENT_S : objData.runnerAngle =     -M_PI/2.0; break;
      case CFieldRunners::Orient::ORIENT_E : objData.runnerAngle =           0.0; break;
      case CFieldRunners::Orient::ORIENT_W : objData.runnerAngle =      M_PI    ; break;
      case CFieldRunners::Orient::ORIENT_NE: objData.runnerAngle =      M_PI/4.0; break;
      case CFieldRunners::Orient::ORIENT_SE: objData.runnerAngle =     -M_PI/4.0; break;
      case CFieldRunners::Orient::ORIENT_NW: objData.runnerAngle =  3.0*M_PI/4.0; break;
      case CFieldRunners::Orient::ORIENT_SW: objData.runnerAngle = -3.0*M_PI/4.0; break;
    }
  }
}

void
Canvas::
updateBullets()
{
  const auto &bullets = fieldRunners_->bullets();

  auto numBullets = bullets.size();

  if (numBullets > bulletDatas_.size())
    bulletDatas_.resize(numBullets);

  uint i = 0;

  for (auto &bulletData : bulletDatas_) {
    if (i < numBullets) {
      auto *bullet = bullets[i];

      bulletData.active = ! bullet->isDone();

      if (bulletData.active) {
        bulletData.bullet = bullet;

        const auto &p = bullet->getPoint();

        auto row = double(p.y)/double(cellSize_.height);
        auto col = double(p.x)/double(cellSize_.width );

        double wx, wy;
        posToWorld(row, col, wx, wy);

        bulletData.x = wx;
        bulletData.y = wy;
      }
    }
    else
      bulletData.active = false;

    ++i;
  }
}

void
Canvas::
updateHealthBars()
{
  struct RunnerData {
    CFieldRunners::CellPos cellPos;
    double                 health { 0.0 };
  };

  std::vector<RunnerData> runnerDataArray;

  for (uint ir = 0; ir < fieldRunners_->getNumRunners(); ++ir) {
    auto *runner = fieldRunners_->getRunner(ir);
    if (runner->isDead()) continue;

    auto type = runner->type();

    RunnerData runnerData;

    runnerData.cellPos = runner->getPos();
    runnerData.health  = double(runner->getHealth())/double(runner->getMaxHealth());

    if      (type == CFieldRunners::RunnerType::SOLDIER) {
      runnerDataArray.push_back(runnerData);
    }
    else if (type == CFieldRunners::RunnerType::MERCENARY) {
      runnerDataArray.push_back(runnerData);
    }
    else if (type == CFieldRunners::RunnerType::TANK) {
      runnerDataArray.push_back(runnerData);
    }
    else if (type == CFieldRunners::RunnerType::HELICOPTER) {
      runnerDataArray.push_back(runnerData);
    }
    else {
      //std::cerr << "Unhandled runner " << int(type) << "\n";
    }
  }

  auto numHealthBars = runnerDataArray.size();

  if (numHealthBars > healthBarDatas_.size())
    healthBarDatas_.resize(numHealthBars);

  uint i = 0;

  for (auto &healthBarData : healthBarDatas_) {
    if (i < numHealthBars) {
      healthBarData.active = true;

      const auto &runnerData = runnerDataArray[i];

      double wx, wy;
      posToWorldI(runnerData.cellPos.row, runnerData.cellPos.col, wx, wy);

      healthBarData.x      = wx;
      healthBarData.y      = wy;
      healthBarData.health = runnerData.health;
    }
    else
      healthBarData.active = false;

    ++i;
  }
}

//---

void
Canvas::
drawBg()
{
  auto *program = this->sceneShaderProgram();

  program->bind();

  //---

  // camera projection
  program->setUniformValue("projection", CQGLUtil::toQMatrix(camera_->perspectiveMatrix()));

  // camera/view transformation
  program->setUniformValue("view", CQGLUtil::toQMatrix(camera_->viewMatrix()));

  // view pos
  program->setUniformValue("viewPos", CQGLUtil::toVector(camera_->position()));

  //---

  // light data
  auto ambientColor = CRGBA::white();
  auto ambientStrength = 0.2;
  auto diffuseStrength = 1.0;
  auto emissiveColor = CRGBA::white();
  auto emissiveStrength = 0.0;
  auto specularColor = CRGBA::white();
  auto specularStrength = 0.2;

  program->setUniformValue("ambientColor", CQGLUtil::toVector(ambientColor));
  program->setUniformValue("ambientStrength", float(ambientStrength));

  program->setUniformValue("diffuseStrength", float(diffuseStrength));

  program->setUniformValue("emissionColor"   , CQGLUtil::toVector(emissiveColor));
  program->setUniformValue("emissiveStrength", float(emissiveStrength));

  program->setUniformValue("specularColor"   , CQGLUtil::toVector(specularColor));
  program->setUniformValue("specularStrength", float(specularStrength));

  program->setUniformValue("fixedDiffuse", false);

  //---

  auto modelMatrix = CMatrix3DH::identity();

  auto bgType = fieldRunners_->bgType();

  if      (bgType == CFieldRunners::BgType::GRASSLANDS) {
    drawObject(modelMatrix, getGrasslandsObj(), nullptr);
  }
  else if (bgType == CFieldRunners::BgType::CROSSROADS) {
    drawObject(modelMatrix, crossroadsObj_, nullptr);
  }
  else if (bgType == CFieldRunners::BgType::DRYLANDS) {
    drawObject(modelMatrix, drylandsObj_, nullptr);
  }
  else if (bgType == CFieldRunners::BgType::CRYSTAL) {
    drawObject(modelMatrix, crystalCavesObj_, nullptr);
  }
  else if (bgType == CFieldRunners::BgType::SKYWAY) {
    drawObject(modelMatrix, skywayObj_, nullptr);
  }
  else if (bgType == CFieldRunners::BgType::FROSTBITE) {
    drawObject(modelMatrix, frostbiteObj_, nullptr);
  }
  else if (bgType == CFieldRunners::BgType::MUDSLIDE) {
    drawObject(modelMatrix, mudslideObj_, nullptr);
  }
  else if (bgType == CFieldRunners::BgType::LAVAFLOW) {
    drawObject(modelMatrix, lavaflowObj_, nullptr);
  }

  //---

  program->release();
}

void
Canvas::
drawScene()
{
  auto *program = this->sceneShaderProgram();

  program->bind();

  //---

  // camera projection
  program->setUniformValue("projection", CQGLUtil::toQMatrix(camera_->perspectiveMatrix()));

  // camera/view transformation
  program->setUniformValue("view", CQGLUtil::toQMatrix(camera_->viewMatrix()));

  // view pos
  program->setUniformValue("viewPos", CQGLUtil::toVector(camera_->position()));

  //---

  // light data
  auto ambientColor = CRGBA::white();
  auto ambientStrength = 0.2;
  auto diffuseStrength = 1.0;
  auto emissiveColor = CRGBA::white();
  auto emissiveStrength = 0.0;
  auto specularColor = CRGBA::white();
  auto specularStrength = 0.2;

  program->setUniformValue("ambientColor", CQGLUtil::toVector(ambientColor));
  program->setUniformValue("ambientStrength", float(ambientStrength));

  program->setUniformValue("diffuseStrength", float(diffuseStrength));

  program->setUniformValue("emissionColor"   , CQGLUtil::toVector(emissiveColor));
  program->setUniformValue("emissiveStrength", float(emissiveStrength));

  program->setUniformValue("specularColor"   , CQGLUtil::toVector(specularColor));
  program->setUniformValue("specularStrength", float(specularStrength));

  program->setUniformValue("fixedDiffuse", false);

  //---

  paintData_.reset();

  //---

  for (const auto &objData : objDatas_) {
    if (! objData.active)
      continue;

    if (objData.object) {
      double wx, wy;
      posToWorldI(objData.row, objData.col, wx, wy);

      auto m1 = CMatrix3DH::translation(wx, 0.0, wy);
      auto m2 = CMatrix3DH::rotation(objData.angle, CVector3D(0, 1, 0));
      auto m3 = CMatrix3DH::scale(objData.size);

      auto modelMatrix = m1*m2*m3;

      drawObject(modelMatrix, objData.object, &objData);
    }

    if (objData.runner) {
      double wx, wy;
      posToWorld(objData.y + 0.5, objData.x + 0.5, wx, wy);

      auto m1 = CMatrix3DH::translation(wx, 0.0, wy);
      auto m2 = CMatrix3DH::rotation(objData.runnerAngle, CVector3D(0, 1, 0));
      auto m3 = CMatrix3DH::scale(objData.runnerSize);

      auto modelMatrix = m1*m2*m3;

      drawObject(modelMatrix, objData.runner, &objData);
    }

  }

  //---

  program->release();
}

ShaderProgram *
Canvas::
sceneShaderProgram()
{
  return getShader("scene.vs", "scene.fs");
}

//---

void
Canvas::
addShadows()
{
  //auto c = CRGBA::black();
  auto c = CRGBA::red();

  auto *shadowBuffer = getShadowBuffer();

  shadowBuffer->clear();

  for (int ir = 0; ir < nr_; ++ir) {
    for (int ic = 0; ic < nc_; ++ic) {
      auto ind = calcInd(ir, ic);

      auto &objData = objDatas_[ind];

      if (objData.shadow) {
        double wx, wy;
        //posToWorldI(objData.row, objData.col, wx, wy);
        posToWorld(objData.y + 0.5, objData.x + 0.5, wx, wy);

        shadowBuffer->addPoint(CPoint3D(wx, 0.01, wy));
        shadowBuffer->addColor(c);
      }
    }
  }

  //shadowBuffer->load();
}

void
Canvas::
drawShadows()
{
  auto *shadowBuffer = getShadowBuffer();

  shadowBuffer->draw(camera_);
}

ParticleBuffer *
Canvas::
getShadowBuffer()
{
  if (! shadowBuffer_) {
    shadowBuffer_ = new ParticleBuffer(shadowShaderProgram());

    shadowBuffer_->setFunctions(this);

    shadowBuffer_->init();

    shadowTexture_ = loadTexture("textures/shadow.png");

    if (shadowTexture_)
      shadowBuffer_->setTexture(shadowTexture_);

    shadowBuffer_->setParticleSize(1.0);
  }

  return shadowBuffer_;
}

ShaderProgram *
Canvas::
shadowShaderProgram()
{
  return getShader("shadow.vs", "shadow.fs");
}

//---

void
Canvas::
drawBullets()
{
  updateBullets();

  addBullets();

  //---

  auto *bulletBuffer = getBulletBuffer();

  bulletBuffer->draw(camera_);

  //---

  auto *glueBuffer = getGlueBuffer();

  glueBuffer->draw(camera_);

  //---

  auto *pulseBuffer = getPulseBuffer();

  pulseBuffer->draw(camera_);

  //---

  if (hasMissiles_)
    drawMissiles();

  if (hasZap_)
    drawZap();

  if (hasLaser_)
    drawLaser();

  //---

  auto *firebombBuffer = getFirebombBuffer();

  firebombBuffer->draw(camera_);
}

void
Canvas::
drawMissiles()
{
  auto *program = this->sceneShaderProgram();

  program->bind();

  //---

  // camera projection
  program->setUniformValue("projection", CQGLUtil::toQMatrix(camera_->perspectiveMatrix()));

  // camera/view transformation
  program->setUniformValue("view", CQGLUtil::toQMatrix(camera_->viewMatrix()));

  // view pos
  program->setUniformValue("viewPos", CQGLUtil::toVector(camera_->position()));

  //---

  // light data
  auto ambientColor = CRGBA::white();
  auto ambientStrength = 0.2;
  auto diffuseStrength = 1.0;
  auto emissiveColor = CRGBA::white();
  auto emissiveStrength = 0.0;
  auto specularColor = CRGBA::white();
  auto specularStrength = 0.2;

  program->setUniformValue("ambientColor", CQGLUtil::toVector(ambientColor));
  program->setUniformValue("ambientStrength", float(ambientStrength));

  program->setUniformValue("diffuseStrength", float(diffuseStrength));

  program->setUniformValue("emissionColor"   , CQGLUtil::toVector(emissiveColor));
  program->setUniformValue("emissiveStrength", float(emissiveStrength));

  program->setUniformValue("specularColor"   , CQGLUtil::toVector(specularColor));
  program->setUniformValue("specularStrength", float(specularStrength));

  program->setUniformValue("fixedDiffuse", false);

  //---

  for (const auto &bulletData : bulletDatas_) {
    if (! bulletData.active)
      continue;

    auto modelMatrix = CMatrix3DH::identity();

    auto *missile = dynamic_cast<CFieldRunners::MissileBullet *>(bulletData.bullet);

    if (missile) {
      auto orient = missile->orient();

      double angle = 0.0;

      switch (orient) {
        default: angle = 0.0; break;
        case CFieldRunners::Orient::ORIENT_N : angle =      M_PI/2.0; break;
        case CFieldRunners::Orient::ORIENT_S : angle =     -M_PI/2.0; break;
        case CFieldRunners::Orient::ORIENT_E : angle =           0.0; break;
        case CFieldRunners::Orient::ORIENT_W : angle =      M_PI    ; break;
        case CFieldRunners::Orient::ORIENT_NE: angle =      M_PI/4.0; break;
        case CFieldRunners::Orient::ORIENT_NW: angle =  3.0*M_PI/4.0; break;
        case CFieldRunners::Orient::ORIENT_SE: angle =     -M_PI/4.0; break;
        case CFieldRunners::Orient::ORIENT_SW: angle = -3.0*M_PI/4.0; break;
      }

      auto p = CPoint3D(bulletData.x, 0.5, bulletData.y);

      auto m1 = CMatrix3DH::translation(p.x, p.y, p.z);
      auto m2 = CMatrix3DH::rotation(angle, CVector3D(0, 1, 0));

      modelMatrix = m1*m2;
    }

    drawObject(modelMatrix, missileObj_, nullptr);
  }

  //---

  program->release();
}

void
Canvas::
drawZap()
{
  auto *zapBuffer = getZapBuffer();
  auto *program   = zapBuffer->program();

  //---

  glLineWidth(8);

  program->bind();

  zapBuffer->bind();

  //---

  // camera projection
  program->setUniformValue("projection", CQGLUtil::toQMatrix(camera_->perspectiveMatrix()));

  // camera/view transformation
  program->setUniformValue("view", CQGLUtil::toQMatrix(camera_->viewMatrix()));

  // view pos
  program->setUniformValue("viewPos", CQGLUtil::toVector(camera_->position()));

  //---

  for (const auto &faceData : zapFaceDataList_.faceDatas) {
    glDrawArrays(GL_LINE_STRIP, faceData.pos, faceData.len);
  }

  //---

  zapBuffer->unbind();

  program->release();
}

void
Canvas::
drawLaser()
{
  auto *laserBuffer = getLaserBuffer();
  auto *program     = laserBuffer->program();

  //---

  glLineWidth(8);

  program->bind();

  laserBuffer->bind();

  //---

  // camera projection
  program->setUniformValue("projection", CQGLUtil::toQMatrix(camera_->perspectiveMatrix()));

  // camera/view transformation
  program->setUniformValue("view", CQGLUtil::toQMatrix(camera_->viewMatrix()));

  // view pos
  program->setUniformValue("viewPos", CQGLUtil::toVector(camera_->position()));

  //---

  for (const auto &faceData : laserFaceDataList_.faceDatas) {
    glDrawArrays(GL_LINE_STRIP, faceData.pos, faceData.len);
  }

  //---

  laserBuffer->unbind();

  program->release();
}

void
Canvas::
drawHealthBars()
{
  updateHealthBars();

  addHealthBars();

  //---

  auto *healthBarBuffer = getHealthBarBuffer();

  healthBarBuffer->draw(camera_);
}

void
Canvas::
drawHUD()
{
  hud_->drawGeometry();
}

void
Canvas::
drawAxes()
{
  axes_->drawGeometry();
}

void
Canvas::
drawObject(const CMatrix3DH &modelMatrix, CGeomObject3D *object, const ObjData *objData)
{
  if (! object) {
    std::cerr << "Null object draw\n";
    return;
  }

  if (! object->getVisible())
    return;

  auto *object1 = dynamic_cast<GeomObject *>(object);
  assert(object1);

  auto *program = object1->program();

  if (object1->faceDatas().empty()) {
    for (auto *child : object->children()) {
      if (objData && objData->hideParts.find(child->getName()) != objData->hideParts.end())
        continue;

      drawObject(modelMatrix, child, objData);
    }

    return;
  }

  //---

  auto *animObject = object->getAnimObject();

  bool isAnim = false;

  if (isAnimEnabled())
    isAnim = (animObject && animObject->animName() != "");

  //---

  // mesh matrix
  CMatrix3DH meshMatrix;
  bool       hasMeshMatrix { false };

  if (isAnim) {
    auto pm = objectMeshData_.find(object);

    if (pm != objectMeshData_.end()) {
      auto animTime = animObject->animTime();

      const auto &objectMeshData = (*pm).second;

      auto frame = int((animTime - objectMeshData.tmin)/objectMeshData.dt + 0.5);

      auto pf = objectMeshData.frameMatrix.find(frame);

      if (pf != objectMeshData.frameMatrix.end()) {
        meshMatrix = (*pf).second;

        hasMeshMatrix = true;
      }
      else {
        std::cerr << "Bad meshMatrix anim time\n";
      }
    }
  }

  if (! hasMeshMatrix)
    meshMatrix = CMatrix3DH(object->getMeshGlobalTransform());

  program->setUniformValue("meshMatrix", CQGLUtil::toQMatrix(meshMatrix));

  //---

  // model matrix
  auto modelMatrix1 = modelMatrix*CMatrix3DH(object->getHierTransform());

  if (object == helicopterPropObj_) {
    auto ticks = fieldRunners_->ticks();

    auto a = ticks/3.0;

    auto mr = CMatrix3DH::rotation(a, CVector3D(0, 1, 0));

    modelMatrix1 = modelMatrix1*mr;
  }

  program->setUniformValue("model", CQGLUtil::toQMatrix(modelMatrix1));

  //---

  QPointF textureOffset(0, 0);

  if (object == lavaflowLavaObj_) {
    auto ticks = fieldRunners_->ticks();

    auto tx = ticks/773.0;
    auto ty = ticks/563.0;

    textureOffset = QPointF(tx - int(tx), ty - int(ty));
  }

  program->setUniformValue("textureOffset", textureOffset);

  //---

  // anim
  program->setUniformValue("useBonePoints", isAnim);

  if (isAnim) {
    updateNodeMatrices(object);

    program->setUniformValueArray("globalBoneTransform",
      &paintData_.nodeQMatrices[0], PaintData::NUM_NODE_MATRICES);
  }

  //---

  object1->buffer()->bind();

  //---

  class DrawState {
   public:
    DrawState(Canvas *canvas, GeomObject *object) :
     canvas_(canvas) {
      program_ = object->program();

      init();
    }

   ~DrawState() {
      flush(state_);
    }

    void drawFace(const FaceData &faceData, double transparency) {
      auto saveState = state_;

      bool changed = false;

      if (setDiffuseTexture (canvas_->isTextured() ? faceData.diffuseTexture  : nullptr) ||
          setNormalTexture  (canvas_->isTextured() ? faceData.normalTexture   : nullptr) ||
          setSpecularTexture(canvas_->isTextured() ? faceData.specularTexture : nullptr) ||
          setEmissiveTexture(canvas_->isTextured() ? faceData.emissiveTexture : nullptr))
       changed = true;

      //---

      if (setEmissiveColor(faceData.emission) ||
          setShininess    (faceData.shininess) ||
          setTransparency (transparency))
        changed = true;

      //---

      if (changed)
        flush(saveState);

      draw(faceData.pos, faceData.len);

      //flush(state_);
    }

    //---

    void init() {
      canvas_->glDisable(GL_BLEND);
      canvas_->glDepthMask(GL_TRUE);

      update(state_);
    }

    //---

    bool setDiffuseTexture(CQGLTexture *texture) {
      return updateTexture(state_.diffuseTexture, texture);
    }

    bool setNormalTexture(CQGLTexture *texture) {
      return updateTexture(state_.normalTexture, texture);
    }

    bool setSpecularTexture(CQGLTexture *texture) {
      return updateTexture(state_.specularTexture, texture);
    }

    bool setEmissiveTexture(CQGLTexture *texture) {
      return updateTexture(state_.emissiveTexture, texture);
    }

    bool setEmissiveColor(const CRGBA &c) {
      if (c == state_.emission) return false;
      state_.emission = c;
      return true;
    }

    bool setShininess(double f) {
      if (f == state_.shininess) return false;
      state_.shininess = f;
      return true;
    }

    bool setTransparency(double f) {
      if (f == state_.transparency) return false;
      state_.transparency = f;
      return true;
    }

    void draw(int pos, int len) {
      if (len_ > 0 && pos != nextPos_)
        flush(state_);

      if (pos_ < 0)
        pos_ = pos;

      len_ += len;

      nextPos_ = pos + len;
    }

   private:
    struct TextureData {
      CQGLTexture* texture { nullptr };

      bool isChanged(CQGLTexture *texture1) {
        return (texture1 != texture);
      }
    };

   private:
    struct State {
      TextureData diffuseTexture;
      TextureData normalTexture;
      TextureData specularTexture;
      TextureData emissiveTexture;

      CRGBA  emission     { 0, 0, 0, 0 };
      double shininess    { 0.0 };
      double transparency { 0.0 };
    };

    bool updateTexture(TextureData &textureData, CQGLTexture *texture) {
      if (! textureData.isChanged(texture))
        return false;

      textureData.texture = texture;

      return true;
    }

    void setTexture(const TextureData &textureData, const char *enableName,
                    const char *textureName, int num) {
      program_->setUniformValue(enableName, !!textureData.texture);

      if (textureData.texture) {
        canvas_->glActiveTexture(GL_TEXTURE0 + num);
        textureData.texture->bind();

        program_->setUniformValue(textureName, num);
      }
    }

    void flush(const State &state) {
      if (len_ > 0) {
        update(state);

        //canvas_->glDrawArrays(GL_TRIANGLE_FAN, pos_, len_);
        canvas_->glDrawArrays(GL_TRIANGLES, pos_, len_);

        pos_     = -1;
        len_     = 0;
        nextPos_ = -1;
      }
    }

    void update(const State &state) {
      setTexture(state.diffuseTexture , "diffuseTexture.enabled" , "diffuseTexture.texture" , 0);
      setTexture(state.normalTexture  , "normalTexture.enabled"  , "normalTexture.texture"  , 1);
      setTexture(state.specularTexture, "specularTexture.enabled", "specularTexture.texture", 2);
      setTexture(state.emissiveTexture, "emissiveTexture.enabled", "emissiveTexture.texture", 3);

      program_->setUniformValue("emissionColor", CQGLUtil::toVector(state.emission));
      program_->setUniformValue("shininess", float(state.shininess));
      program_->setUniformValue("transparency", float(1.0 - state.transparency));
    }

   private:
    Canvas*        canvas_  { nullptr };
    ShaderProgram* program_ { nullptr };

    State state_;

    int pos_     { -1 };
    int len_     { 0 };
    int nextPos_ { -1 };
  };

  //---

  {
  DrawState drawState(this, object1);

  drawState.init();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (const auto &faceData : object1->faceDatas()) {
    if      (faceData.face) {
      auto *face = faceData.face;

      if (! face->getVisible())
        continue;

      drawState.drawFace(faceData, 0.0);
    }
    else
      assert(false);
  }
  }

  //---

  object1->buffer()->unbind();

  //---

  for (auto *child : object->children())
    drawObject(modelMatrix, child, objData);
}

void
Canvas::
updateNodeMatrices(CGeomObject3D *object)
{
  // anim data on anim object
  auto *animObject = object->getAnimObject();

  if (animObject == paintData_.animObject)
    return;

  paintData_.animObject = animObject;

  //---

  // get node matrices for anim name and anim time
  const auto &nodeMatrices = getObjectNodeMatrices(animObject);

  //---

  std::vector<CMatrix3D> nodeMatrixArray;

  for (int i = 0; i < PaintData::NUM_NODE_MATRICES; i++)
    nodeMatrixArray.push_back(CMatrix3D::identity());

  for (const auto &pn : nodeMatrices) {
    auto nodeId = pn.first;

    if (nodeId < 0) {
      std::cerr << "Invalid node id " << nodeId << "\n";
      continue;
    }

    if (nodeId >= PaintData::NUM_NODE_MATRICES) {
      std::cerr << "Too few node matrices for node " << nodeId << "\n";
      continue;
    }

    nodeMatrixArray[nodeId] = pn.second;
  }

  paintData_.nodeMatrices .resize(PaintData::NUM_NODE_MATRICES);
  paintData_.nodeQMatrices.resize(PaintData::NUM_NODE_MATRICES);

  int im = 0;
  for (const auto &m : nodeMatrixArray) {
    paintData_.nodeMatrices [im] = m;
    paintData_.nodeQMatrices[im] = CQGLUtil::toQMatrix(m);

    ++im;
  }
}

const Canvas::NodeMatrices &
Canvas::
getObjectNodeMatrices(CGeomObject3D *object) const
{
  const auto &objectNodeMatrices = getNodeMatrices();

  auto po = objectNodeMatrices.find(object->getInd());
  assert(po != objectNodeMatrices.end());

  return (*po).second;
}

const Canvas::ObjectNodeMatrices &
Canvas::
getNodeMatrices() const
{
  auto *th = const_cast<Canvas *>(this);

  if (! th->objectNodeMatricesValid_) {
    th->objectNodeMatrices_ = calcNodeMatrices();

    th->objectNodeMatricesValid_ = true;
  }

  return objectNodeMatrices_;
}

Canvas::ObjectNodeMatrices
Canvas::
calcNodeMatrices() const
{
  ObjectNodeMatrices objectNodeMatrices;

  auto animObjects = getAnimObjects();

  for (auto *animObject : animObjects) {
    if (! animObject->getVisible())
      continue;

    auto animName = animObject->animName();
    if (animName == "") continue;

    auto animTime = animObject->animTime();

    auto &nodeMatrices = objectNodeMatrices[animObject->getInd()];

    animObject->updateNodesAnimationData(animName, animTime);

    auto meshMatrix        = animObject->getMeshGlobalTransform();
    auto inverseMeshMatrix = meshMatrix.inverse();

    //---

    for (const auto &pn : animObject->getNodes()) {
      auto &node = const_cast<CGeomNodeData &>(pn.second);
      //if (! node.isJoint()) continue;

      if (node.index() < 0)
        continue;

      nodeMatrices[node.index()] = node.calcNodeAnimMatrix(inverseMeshMatrix);
    }
  }

  return objectNodeMatrices;
}

std::vector<CGeomObject3D *>
Canvas::
getAnimObjects() const
{
  std::set<CGeomObject3D *>    animObjectSet;
  std::vector<CGeomObject3D *> animObjects;

  const auto &objects = scene_->getObjects();

  for (auto *object : objects) {
    auto *animObject = object->getAnimObject();
    if (! animObject) continue;

    if (animObjectSet.find(animObject) == animObjectSet.end()) {
      animObjectSet.insert(animObject);

      animObjects.push_back(animObject);
    }
  }

  return animObjects;
}

//---

void
Canvas::
mousePressEvent(QMouseEvent *e)
{
  auto state = fieldRunners_->getState();

  if      (state == CFieldRunners::State::SETUP)
    setupMousePress(e);
  else if (state == CFieldRunners::State::ACTIVE)
    activeMousePress(e);
  else if (state == CFieldRunners::State::GAME_OVER)
    gameOverMousePress(e);
}

void
Canvas::
setupMousePress(QMouseEvent *e)
{
  (void) startUp_->mousePress(e);
}

void
Canvas::
activeMousePress(QMouseEvent *e)
{
  if (hud_->mousePress(e))
    return;

  //---

  mouseData_.pressed = true;
  mouseData_.button  = e->button();
  mouseData_.press   = CPoint2D(e->x(), e->y());

  mouseData_.isShift   = (e->modifiers() & Qt::ShiftModifier);
  mouseData_.isControl = (e->modifiers() & Qt::ControlModifier);

  if (mouseData_.button == Qt::LeftButton) {
    auto x = 2.0*e->x()/(pixelWidth () - 1) - 1.0;
    auto y = 2.0*e->y()/(pixelHeight() - 1) - 1.0;

    CPoint3D rp1, rp2;
    camera_->getPixelRay(x, y, rp1, rp2);

    CLine3D line(rp1, rp2);

    CPlane3D plane(CPoint3D(0, 0, 0), CVector3D(0, 1, 0));

    double t;
    plane.intersectLine(line, &t);
    auto p = line.point(t);
    //std::cerr << t << " " << p << "\n";

    int row, col;
    worldToPos(p.x, p.z, row, col);
    std::cerr << row << " " << col << "\n";

    auto cellPos = CFieldRunners::CellPos(row, col);

#if 0
    if (fieldRunners_->isAddPosValid(cellPos)) {
      CFieldRunners::FieldCell *cell;
      fieldRunners_->getCell(cellPos, &cell);

      auto type = cell->type();

      if (type == CFieldRunners::CellType::EMPTY) {
        fieldRunners_->addWeaponCell(CFieldRunners::CellType::GUN, cellPos);

        valid_ = false;

        update();
      }
    }
#else
    fieldRunners_->setCurrentCell(cellPos);

    valid_ = false;

    update();
#endif
  }
}

void
Canvas::
gameOverMousePress(QMouseEvent *e)
{
  (void) gameOver_->mousePress(e);
}

void
Canvas::
mouseMoveEvent(QMouseEvent *e)
{
  mouseData_.move.x = e->x();
  mouseData_.move.y = e->y();

  mouseData_.isShift   = (e->modifiers() & Qt::ShiftModifier);
  mouseData_.isControl = (e->modifiers() & Qt::ControlModifier);

  //---

  auto dx = CMathUtil::sign(mouseData_.move.x - mouseData_.press.x);
  auto dy = CMathUtil::sign(mouseData_.move.y - mouseData_.press.y);

  if      (mouseData_.button == Qt::LeftButton) {
  }
  else if (mouseData_.button == Qt::MiddleButton) {
    auto da = M_PI/180.0;

    camera_->rotateY(-dx*da);
    camera_->rotateX(-dy*da);
  }
  else if (mouseData_.button == Qt::RightButton) {
    camera_->moveRight(-dx/10.0);
    camera_->moveUp   ( dy/10.0);
  }

  //---

  mouseData_.press = mouseData_.move;
}

void
Canvas::
mouseReleaseEvent(QMouseEvent *)
{
  mouseData_.pressed = false;
  mouseData_.button  = Qt::NoButton;
}

void
Canvas::
wheelEvent(QWheelEvent *e)
{
  auto d  = bbox_.getMaxSize()/100.0;
  auto dw = e->angleDelta().y()/250.0;

  camera_->setDistance(camera_->distance() - dw*d);
}

void
Canvas::
keyPressEvent(QKeyEvent *e)
{
  mouseData_.isShift   = (e->modifiers() & Qt::ShiftModifier);
  mouseData_.isControl = (e->modifiers() & Qt::ControlModifier);

  auto k = e->key();

  auto d  = bbox_.getMaxSize()/100.0;
  auto da = M_PI/60.0;

  if (k == Qt::Key_Space) {
    toggleTimer();

    update();

    return;
  }

  if      (inputType() == InputType::CAMERA) {
    if      (k == Qt::Key_Left) {
      camera_->moveRight(-d);
    }
    else if (k == Qt::Key_Right) {
      camera_->moveRight(d);
    }
    else if (k == Qt::Key_Up) {
      camera_->moveUp(d);
    }
    else if (k == Qt::Key_Down) {
      camera_->moveUp(-d);
    }
    else if (k == Qt::Key_Plus) {
      camera_->moveFront(d);
    }
    else if (k == Qt::Key_Minus) {
      camera_->moveFront(-d);
    }
    else if (k == Qt::Key_W) {
      camera_->rotateX(da);
    }
    else if (k == Qt::Key_S) {
      camera_->rotateX(-da);
    }
    else if (k == Qt::Key_A) {
      camera_->rotateY(da);
    }
    else if (k == Qt::Key_D) {
      camera_->rotateY(-da);
    }
    else if (k == Qt::Key_Q) {
      camera_->rotateZ(-da);
    }
    else if (k == Qt::Key_E) {
      camera_->rotateZ(da);
    }
  }
  else if (inputType() == InputType::PLAYER) {
    auto pos = fieldRunners_->currentCell();

    if      (k == Qt::Key_Left) {
      --pos.col;

      valid_ = false;
    }
    else if (k == Qt::Key_Right) {
      ++pos.col;

      valid_ = false;
    }
    else if (k == Qt::Key_Up) {
      --pos.row;

      valid_ = false;
    }
    else if (k == Qt::Key_Down) {
      ++pos.row;

      valid_ = false;
    }
    else if (k == Qt::Key_1) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::GUN, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_2) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::GLUE, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_3) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::SNOWBOMB, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_4) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::MISSILE, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_5) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::SHOTGUN, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_6) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::ZAP, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_7) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::PULSE, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_8) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::LASER, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_9) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::FIREBOMB, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_0) {
      fieldRunners_->addWeaponCell(CFieldRunners::CellType::FLAME, pos);
      valid_ = false;
    }
    else if (k == Qt::Key_Enter || k == Qt::Key_Return) {
      fieldRunners_->buyCell(pos);
    }
    else if (k == Qt::Key_P) {
      fieldRunners_->addWeaponCell(fieldRunners_->getBuyCellType(), pos);
      valid_ = false;
    }
    else if (k == Qt::Key_L) {
      CFieldRunners::FieldCell *cell;
      fieldRunners_->getCell(pos, &cell);

      auto *weaponCell = dynamic_cast<CFieldRunners::WeaponCell *>(cell);

      if (weaponCell) {
        weaponCell->nextLevel();

        auto ind = calcInd(pos.row, pos.col);

        auto &objData = objDatas_[ind];

        objData.hideParts.clear();

        hideWeaponParts(weaponCell, objData);
      }
    }

    std::cerr << pos << "\n";

    fieldRunners_->setCurrentCell(pos);
  }

  update();
}

void
Canvas::
hideWeaponParts(CFieldRunners::WeaponCell *weaponCell, ObjData &objData)
{
  auto level = weaponCell->level();

  if      (objData.object == bulletGunObj_) {
    if      (level == 1)
      objData.hideParts.insert("Barrel2");
    else if (level == 2)
      objData.hideParts.insert("Barrel1");
  }
  else if (objData.object == glueGunObj_) {
    if      (level == 1) {
      objData.hideParts.insert("Gun2");
      objData.hideParts.insert("Gun3");
    }
    else if (level == 2)
      objData.hideParts.insert("Gun1");
  }
  else if (objData.object == missileGunObj_) {
    if      (level == 1) {
      objData.hideParts.insert("Missile1");
      objData.hideParts.insert("Missile3");
    }
    else if (level == 2)
      objData.hideParts.insert("Missile2");
  }
  else if (objData.object == shotgunObj_) {
    if      (level == 1) {
      objData.hideParts.insert("Gun1");
      objData.hideParts.insert("Gun3");
      objData.hideParts.insert("Magazine1");
      objData.hideParts.insert("Magazine3");
    }
    else if (level == 2) {
      objData.hideParts.insert("Gun2");
      objData.hideParts.insert("Magazine2");
    }
  }
  else if (objData.object == zapTowerObj_) {
    if      (level == 1) {
      objData.hideParts.insert("Orb2");
      objData.hideParts.insert("Orb3");
    }
    else if (level == 2)
      objData.hideParts.insert("Orb3");
  }
  else if (objData.object == pulseObj_) {
    if      (level == 1) {
      objData.hideParts.insert("Ring2");
      objData.hideParts.insert("Ring3");
      objData.hideParts.insert("Support2");
      objData.hideParts.insert("Support3");
    }
    else if (level == 2) {
      objData.hideParts.insert("Ring3");
      objData.hideParts.insert("Support3");
    }
  }
  else if (objData.object == snowBombObj_) {
    if      (level == 1) {
      objData.hideParts.insert("Crystal2");
      objData.hideParts.insert("Crystal3");
      objData.hideParts.insert("Block2");
      objData.hideParts.insert("Block3");
    }
    else if (level == 2) {
      objData.hideParts.insert("Crystal3");
      objData.hideParts.insert("Block3");
    }
  }
}

//---

void
Canvas::
tickSlot()
{
  fieldRunners_->tick();

  for (auto *animObject : getAnimObjects())
    animObject->stepAnimTime();

  updateObjDatas();

  addShadows();

  update();
}

//---

CGeomObject3D *
Canvas::
loadModel(const QString &fileName, CGeom3DType format)
{
  static uint modeInd;

  auto modelName = QString("Model.%1").arg(++modeInd);

  auto *im = CImportBase::createModel(format, modelName.toStdString());

  if (! im) {
    std::cerr << "File format not recognised for '" << fileName.toStdString() << "'\n";
    return nullptr;
  }

  auto textureDir = buildDir() + "/models";

  im->setTextureDir(textureDir.toStdString());
  im->setTriangulate(true);

  CFile file(fileName.toStdString());

  if (! im->read(file)) {
    delete im;
    std::cerr << "Failed to read model for '" << fileName.toStdString() << "'\n";
    return nullptr;
  }

  auto *scene = im->releaseScene();

  delete im;

  //---

  uint numTop = 0;

  for (auto *object : scene->getObjects()) {
    if (! object->parent())
      ++numTop;
  }

  CGeomObject3D *parentObj = nullptr;

  if (numTop > 1) {
    parentObj = CGeometry3DInst->createObject3D(scene_, modelName.toStdString());

    scene_->addObject(parentObj);

    for (auto *object : scene->getObjects()) {
      scene_->addObject(object);

      if (! object->parent())
        parentObj->addChild(object);
    }
  }
  else {
    for (auto *object : scene->getObjects()) {
      if (! object->parent())
        parentObj = object;

      scene_->addObject(object);
    }
  }

  for (auto *material : scene->getMaterials()) {
    scene_->addMaterial(material);
  }

  for (auto *texture : scene->textures()) {
    scene_->addTexture(texture);
  }

  return parentObj;
}

//---

ShaderProgram *
Canvas::
getShader(const QString &vertex, const QString &fragment)
{
  auto id = QString("V:%1,F:%2").arg(vertex).arg(fragment);

  auto ps = shaders_.find(id);

  if (ps == shaders_.end()) {
    auto *shaderProgram = new ShaderProgram(this);

    shaderProgram->addShaders(vertex, fragment);

    ps = shaders_.insert(ps, Shaders::value_type(id, shaderProgram));
  }

  return (*ps).second;
}

//---

CQGLTexture *
Canvas::
loadTexture(const QString &filename, bool flip) const
{
  auto *texture = new CQGLTexture;

  auto filename1 = buildDir() + "/" + filename;

  if (! texture->load(filename1, flip)) {
    std::cerr << "Failed to load texture '" << filename1.toStdString() << "'\n";
    return nullptr;
  }

  return texture;
}

CQGLTexture *
Canvas::
getGLTexture(CGeomTexture *texture, bool /*add*/)
{
  auto *texture1 = dynamic_cast<Texture *>(texture);
  assert(texture1);

  if (! texture1->glTexture(this)) {
    //if (! add) return nullptr;

    initGLTexture(texture1);
  }

  return texture1->glTexture(this);
}

void
Canvas::
initGLTexture(Texture *texture)
{
  const auto &image = texture->image()->image();

  auto flippedImage = image->dup();

  flippedImage->flipH();

  auto *t1 = makeTexture(image);
  auto *t2 = makeTexture(flippedImage);

  t1->setName(texture->name());
  t2->setName(texture->name() + ".flip");

  texture->setGlTextures(this, t1, t2);
}

CQGLTexture *
Canvas::
makeTexture(const CImagePtr &image) const
{
  auto *texture = new CQGLTexture(image);

  texture->setFunctions(const_cast<Canvas *>(this));

  return texture;
}

//---

bool
Canvas::
createFontTexture(uint *texture, int w, int h, uchar *data)
{
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // allocate texture id
  glGenTextures(1, texture);
  if (! checkError("glGenTextures")) return false;

  // set texture type
  glBindTexture(GL_TEXTURE_2D, *texture);
  if (! checkError("glBindTexture")) return false;

#if 0
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  if (! checkError("glTexParameteri")) return false;

#if 0
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
#else
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif
  if (! checkError("glTexParameteri")) return false;

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);

  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  if (! checkError("glTexParameteri")) return false;
#else
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  //if (! checkError("glPixelStorei")) return false;
#endif

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
  //if (! checkError("glTexImage2D")) return false;

  glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
  //if (! checkError("glHint")) return false;

  glGenerateMipmap(GL_TEXTURE_2D);
  //if (! checkError("glGenerateMipmap")) return false;

  //---

  #if 0
  // dump font to png
  auto src = CImageNoSrc();

  auto image = CImageMgrInst->createImage(src);

  image->setDataSize(w, h);

  int ii = 0;

  for (int iy = 0; iy < h; ++iy) {
    for (int ix = 0; ix < w; ++ix, ++ii) {
      image->setRGBAPixel(ix, iy, 1.0, 1.0, 1.0, data[ii]/255.0);
    }
  }

  CFile file("texture.png");

  image->writePNG(&file);
#endif

  return true;
}

//---

int
Canvas::
calcInd(int row, int col) const
{
  return row*nc_ + col;
}

void
Canvas::
posToWorld(double row, double col, double &wx, double &wy) const
{
  wx = col - nc_/2.0;
  wy = row - nr_/2.0;
}

void
Canvas::
posToWorldI(double row, double col, double &wx, double &wy) const
{
  wx = col - nc_/2.0 + 0.5;
  wy = row - nr_/2.0 + 0.5;
}

void
Canvas::
worldToPos(double wx, double wy, int &row, int &col) const
{
  col = int(wx + nc_/2.0);
  row = int(wy + nr_/2.0);

  row = nr_ - 1 - row;
}

}

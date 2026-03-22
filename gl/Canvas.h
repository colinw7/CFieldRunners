#ifndef Canvas_H
#define Canvas_H

#include <FaceData.h>
#include <FieldRunners.h>

#include <CImagePtr.h>
#include <CGeom3DType.h>
#include <CMatrix3DH.h>
#include <CBBox3D.h>

#include <QGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>

class CQGLBuffer;
class CGeomScene3D;
class CGeomObject3D;
class CGeomTexture;
class CMatrix3DH;

class QTimer;

namespace CQGLFieldRunners {

class App;
class HUD;
class Axes;
class StartUp;
class GameOver;
class Camera;
class ShaderProgram;
class Texture;
class ParticleBuffer;
class Font;

//---

class Canvas : public QGLWidget, public QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  enum class InputType {
    PLAYER,
    CAMERA
  };

 public:
  Canvas(App *app);

  void init();

  //---

  void updateState();

  bool loadMap(const QString &filename);

  //---

  FieldRunners *fieldRunners() const { return fieldRunners_; }

  Camera *camera() const { return camera_; }

  ShaderProgram *sceneShaderProgram();

  //---

  int numRows() const { return nr_; }
  int numCols() const { return nc_; }

  //---

  int ind() const { return 0; }

  //---

  bool isGridVisible() const;
  void setGridVisible(bool b);

  //---

  const InputType &inputType() const { return inputType_; }
  void setInputType(const InputType &v) { inputType_ = v; }

  //---

  int pixelWidth() const { return pixelWidth_; }
  void setPixelWidth(int i) { pixelWidth_ = i; }

  int pixelHeight() const { return pixelHeight_; }
  void setPixelHeight(int i) { pixelHeight_ = i; }

  double aspect() const { return aspect_; }
  void setAspect(double r) { aspect_ = r; }

  //---

  const QString &buildDir() const { return buildDir_; }
  void setBuildDir(const QString &s) { buildDir_ = s; }

  bool isAnimEnabled() const { return animEnabled_; }
  void setAnimEnabled(bool b) { animEnabled_ = b; }

  //---

  const std::string &initMap() const { return initMap_; }
  void setInitMap(const std::string &s) { initMap_ = s; }

  //---

  bool isTextured() const { return true; }

  //---

  void initializeGL() override;

  void resizeGL(int, int) override;

  void paintGL() override;

  //---

  void drawSetup();
  void drawActive();
  void drawGameOver();

  //---

  void mousePressEvent  (QMouseEvent *e) override;
  void mouseMoveEvent   (QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

  void wheelEvent(QWheelEvent *e) override;

  void keyPressEvent(QKeyEvent *event) override;

  //---

  void setupMousePress(QMouseEvent *e);
  void activeMousePress(QMouseEvent *e);
  void gameOverMousePress(QMouseEvent *e);

  //---

  bool isRunning() const { return running_; }

  void startTimer();
  void stopTimer();

  //---

  ShaderProgram *getShader(const QString &vertex, const QString &fragment);

  bool createFontTexture(uint *texture, int w, int h, uchar *data);

  CQGLTexture *loadTexture(const QString &filename, bool flip=false) const;

  //---

  Font* getFont() const { return font_; }

  //---

  CGeomObject3D *getSoldierObj();
  CGeomObject3D *getMercenaryObj();
  CGeomObject3D *getMotorbikeObj();
  CGeomObject3D *getTankObj();
  CGeomObject3D *getHelicopterObj();
  CGeomObject3D *getCartObj();
  CGeomObject3D *getTrainObj();
  CGeomObject3D *getPlaneObj();
  CGeomObject3D *getBlimpObj();

  //---

  QSize sizeHint() const override { return QSize(2048, 1536); }

 Q_SIGNALS:
  void resized();

 private Q_SLOTS:
  void tickSlot();

 private:
  struct ObjData {
    bool           active      { false };
    bool           shadow      { false };
    int            row         { 0 };
    int            col         { 0 };
    CGeomObject3D* object      { nullptr };
    CGeomObject3D* runner      { nullptr };
    double         angle       { 0.0 };
    double         size        { 1.0 };
    double         runnerAngle { 0.0 };
    double         runnerSize  { 1.0 };
    double         x           { 0.0 };
    double         y           { 0.0 };

    std::set<std::string> hideParts;

    void reset() {
      active      = false;
      shadow      = false;
      object      = nullptr;
      runner      = nullptr;
      angle       = 0.0;
      size        = 1.0;
      runnerAngle = 0.0;
      runnerSize  = 1.0;
      x           = 0.0;
      y           = 0.0;
    }
  };

  struct BulletData {
    bool                   active { false };
    CFieldRunners::Bullet* bullet { nullptr };
    double                 x      { 0.0 };
    double                 y      { 0.0 };
  };

  struct HealthBarData {
    bool   active { false };
    double x      { 0.0 };
    double y      { 0.0 };
    double health { 0.0 };
  };

  using NodeMatrices       = std::map<int, CMatrix3D>;
  using ObjectNodeMatrices = std::map<uint, NodeMatrices>;

 private:
  void toggleTimer();

  //---

  void drawBg();

  //---

  void addScene();
  void drawScene();

  //---

  void addShadows();
  void drawShadows();

  ParticleBuffer *getShadowBuffer();

  ShaderProgram *shadowShaderProgram();

  //---

  void addBullets();
  void updateBullets();
  void drawBullets();

  ParticleBuffer *getBulletBuffer();
  ParticleBuffer *getGlueBuffer();
  ParticleBuffer *getPulseBuffer();
  ParticleBuffer *getFirebombBuffer();

  ShaderProgram *bulletShaderProgram();
  ShaderProgram *firebombShaderProgram();

  //---

  void drawMissiles();

  //---

  void drawZap();

  CQGLBuffer *getZapBuffer();

  ShaderProgram *zapShaderProgram();

  //---

  void drawLaser();

  CQGLBuffer *getLaserBuffer();

  ShaderProgram *laserShaderProgram();

  //---

  void addHealthBars();
  void updateHealthBars();
  void drawHealthBars();

  ParticleBuffer *getHealthBarBuffer();

  ShaderProgram *healthBarShaderProgram();

  //---

  void drawHUD();

  void drawAxes();

  void loadGrasslandsObjects();
  void loadCrossroadsObjects();
  void loadDrylandsObjects();
  void loadCrystalCavesObjects();
  void loadSkywayObjects();
  void loadFrostbiteObjects();
  void loadMudslideObjects();
  void loadLavaflowObjects();

  void placeField(CGeomObject3D *object, int nr, int nc) const;

  CGeomObject3D *getGrasslandsObj ();
  CGeomObject3D *getTreeObj       ();
  CGeomObject3D *getPineTreeObj   ();
  CGeomObject3D *getPalmTreeObj   ();
  CGeomObject3D *getRockObj       ();
  CGeomObject3D *getPurpleRockObj ();
  CGeomObject3D *getLavaRockObj   ();
  CGeomObject3D *getMetalBoxObj   ();
  CGeomObject3D *getTrackObj      ();
  CGeomObject3D *getTrackCornerObj();
  CGeomObject3D *getEmptyTrackObj ();

  void loadObjects();

  void updateObjDatas();

  CGeomObject3D *loadModel(const QString &fileName, CGeom3DType format);

  void drawObject(const CMatrix3DH &modelMatrix, CGeomObject3D *object, const ObjData *objData);

  void hideWeaponParts(CFieldRunners::WeaponCell *weaponCell, ObjData &objData);

  //---

  void updateNodeMatrices(CGeomObject3D *object);

  const NodeMatrices &getObjectNodeMatrices(CGeomObject3D *object) const;

  const ObjectNodeMatrices &getNodeMatrices() const;

  ObjectNodeMatrices calcNodeMatrices() const;

  void invalidateNodeMatrices() { objectNodeMatricesValid_ = false; }

  //---

  std::vector<CGeomObject3D *> getAnimObjects() const;

  //---

  CQGLBuffer *initParticleBuffer(Canvas *canvas);

  //---

  CQGLTexture *getGLTexture(CGeomTexture *texture, bool /*add*/);

  void initGLTexture(Texture *texture);

  CQGLTexture *makeTexture(const CImagePtr &image) const;

  //---

  int calcInd(int row, int col) const;

  void posToWorld (double row, double col, double &wx, double &wy) const;
  void posToWorldI(double row, double col, double &wx, double &wy) const;

  void worldToPos(double wx, double wy, int &row, int &col) const;

 private:
  struct PaintData {
    enum { NUM_NODE_MATRICES = 128 };

    CGeomObject3D*          animObject { nullptr };
    std::vector<CMatrix3D>  nodeMatrices;
    std::vector<QMatrix4x4> nodeQMatrices;

    void reset() {
      animObject = nullptr;

      nodeMatrices .clear();
      nodeQMatrices.clear();
    }
  };

  //---

  using ObjDatas       = std::vector<ObjData>;
  using BulletDatas    = std::vector<BulletData>;
  using HealthBarDatas = std::vector<HealthBarData>;

  struct MouseData {
    bool            pressed   { false };
    bool            isShift   { false };
    bool            isControl { false };
    Qt::MouseButton button    { Qt::NoButton };
    CPoint2D        press     { 0.0, 0.0 };
    CPoint2D        move      { 0.0, 0.0 };
  };

  using Shaders = std::map<QString, ShaderProgram *>;

  using FrameMatrix = std::map<int, CMatrix3DH>;

  struct ObjectMeshData {
    double tmin { 0.0 };
    double tmax { 1.0 };
    int    nt   { 10 };
    double dt   { 0.1 };

    FrameMatrix frameMatrix;
  };

  using ObjectMeshDataMap = std::map<CGeomObject3D *, ObjectMeshData>;

  //---

  App* app_ { nullptr };

  FieldRunners* fieldRunners_ { nullptr };

  Camera* camera_ { nullptr };

  HUD* hud_ { nullptr };

  Axes* axes_ { nullptr };

  StartUp*  startUp_  { nullptr };
  GameOver* gameOver_ { nullptr };

  int nr_ { 0 };
  int nc_ { 0 };

  Size cellSize_;

  InputType inputType_ { InputType::PLAYER };

  // state
  int pixelWidth_  { 2000 };
  int pixelHeight_ { 1500 };

  double aspect_ { 1.0 };

  QString buildDir_;

  bool animEnabled_ { true };

  CGeomScene3D *scene_ { nullptr };

  Font *font_ { nullptr };

  QTimer* timer_ { nullptr };

  std::string initMap_ { "grasslands.map" };

  //---

  // Maps
  CGeomObject3D *grasslandsObj_ { nullptr };
  CGeomObject3D *treeObj_       { nullptr };

  CGeomObject3D *crossroadsObj_ { nullptr };
  CGeomObject3D *metalBoxObj_   { nullptr };

  CGeomObject3D *drylandsObj_ { nullptr };
  CGeomObject3D *palmTreeObj_ { nullptr };

  CGeomObject3D *crystalCavesObj_ { nullptr };
  CGeomObject3D *rockObj_         { nullptr };
  CGeomObject3D *purpleRockObj_   { nullptr };
  CGeomObject3D *lavaRockObj_     { nullptr };
  CGeomObject3D *trackObj_        { nullptr };
  CGeomObject3D *trackCornerObj_  { nullptr };
  CGeomObject3D *emptyTrackObj_   { nullptr };

  CGeomObject3D *skywayObj_ { nullptr };

  CGeomObject3D *frostbiteObj_ { nullptr };
  CGeomObject3D *pineTreeObj_  { nullptr };

  CGeomObject3D *mudslideObj_ { nullptr };

  CGeomObject3D *lavaflowObj_     { nullptr };
  CGeomObject3D *lavaflowLavaObj_ { nullptr };

  //---

  // Runners
  CGeomObject3D *solidierObj_       { nullptr };
  CGeomObject3D *mercenaryObj_      { nullptr };
  CGeomObject3D *motorbikeObj_      { nullptr };
  CGeomObject3D *tankObj_           { nullptr };
  CGeomObject3D *helicopterObj_     { nullptr };
  CGeomObject3D *helicopterPropObj_ { nullptr };
  CGeomObject3D *cartObj_           { nullptr };
  CGeomObject3D *trainObj_          { nullptr };
  CGeomObject3D *planeObj_          { nullptr };
  CGeomObject3D *blimpObj_          { nullptr };

  //---

  // Guns
  CGeomObject3D *bulletGunObj_  { nullptr };
  CGeomObject3D *glueGunObj_    { nullptr };
  CGeomObject3D *snowBombObj_   { nullptr };
  CGeomObject3D *missileGunObj_ { nullptr };
  CGeomObject3D *shotgunObj_    { nullptr };
  CGeomObject3D *zapTowerObj_   { nullptr };
  CGeomObject3D *pulseObj_      { nullptr };
  CGeomObject3D *laserGunObj_   { nullptr };
  CGeomObject3D *fireBombObj_   { nullptr };
  CGeomObject3D *flameGunObj_   { nullptr };

  // Bullets
  CGeomObject3D *missileObj_ { nullptr };

  //---

  // Annotations
  CGeomObject3D *entranceObj_ { nullptr };

  //---

  CGeomObject3D *currentCellObj_ { nullptr };

  ObjDatas       objDatas_;
  BulletDatas    bulletDatas_;
  HealthBarDatas healthBarDatas_;

  std::vector<double> sizeGrid_;

  MouseData mouseData_;

  CBBox3D bbox_;

  ParticleBuffer* bulletBuffer_  { nullptr };
  CQGLTexture*    bulletTexture_ { nullptr };

  ParticleBuffer* glueBuffer_  { nullptr };
  CQGLTexture*    glueTexture_ { nullptr };

  ParticleBuffer* pulseBuffer_  { nullptr };
  CQGLTexture*    pulseTexture_ { nullptr };

  ParticleBuffer* shadowBuffer_  { nullptr };
  CQGLTexture*    shadowTexture_ { nullptr };

  CQGLBuffer*  zapBuffer_ { nullptr };
  FaceDataList zapFaceDataList_;

  CQGLBuffer*  laserBuffer_ { nullptr };
  FaceDataList laserFaceDataList_;

  ParticleBuffer* firebombBuffer_  { nullptr };
  CQGLTexture*    firebombTexture_ { nullptr };

  ParticleBuffer* healthBarBuffer_ { nullptr };

  Shaders shaders_;

  bool valid_ { false };

  bool running_ { false };

  bool hasMissiles_ { false };
  bool hasZap_      { false };
  bool hasLaser_    { false };

  //---

  PaintData paintData_;

  ObjectMeshDataMap objectMeshData_;

  ObjectNodeMatrices objectNodeMatrices_;
  bool               objectNodeMatricesValid_ { false };
};

}

#endif

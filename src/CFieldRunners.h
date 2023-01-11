#ifndef CFIELD_RUNNERS_H
#define CFIELD_RUNNERS_H

#include <CAStar.h>

using uint  = unsigned int;
using uchar = unsigned char;

#include <list>
#include <vector>
#include <map>
#include <set>
#include <memory>

#include <ostream>

//! simple bounding box structure
struct BBox {
  int xmin { 0 }, ymin { 0 };
  int xmax { 0 }, ymax { 0 };

  BBox() { }

  BBox(int x1, int y1, int x2, int y2) :
   xmin(x1), ymin(y1), xmax(x2), ymax(y2) {
  }

  void getCenter(int *x, int *y) {
    *x = (xmin + xmax)/2;
    *y = (ymin + ymax)/2;
  }

  void getLL(int *x, int *y) {
    *x = xmin;
    *y = ymin;
  }

  int width () const { return xmax - xmin + 1; }
  int height() const { return ymax - ymin + 1; }
};

//! simple size (width/height) structure
struct Size {
  int width  { 0 };
  int height { 0 };

  Size() { }

  Size(int w, int h) :
   width(w), height(h) {
  }
};

//! simple point (x/y) structure
struct Point {
  int x { 0 };
  int y { 0 };

  Point() { }

  Point(int x1, int y1) :
   x(x1), y(y1) {
  }
};

//! color structure
struct Color {
  double r { 0.0 }, g { 0.0 }, b { 0.0 }, a { 1.0 };

  Color() { }

  Color(double r1, double g1, double b1, double a1=1.0) :
   r(r1), g(g1), b(b1), a(a1) {
  }
};

using ImageId = unsigned int;

//! virtual class for window/graphic display
class CFieldRunnersWindow {
 public:
  CFieldRunnersWindow() { }

  virtual ~CFieldRunnersWindow() { }

  // window size
  virtual void getSize(Size &size) = 0;

  // required graphics operations
  virtual void fill(const Color &color) = 0;

  virtual void setBackground(const Color &rgba) = 0;
  virtual void setForeground(const Color &rgba) = 0;

  virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

  virtual void drawPoint(int x, int y) = 0;

  virtual void drawRectangle(int x1, int y1, int x2, int y2) = 0;
  virtual void fillRectangle(int x1, int y1, int x2, int y2) = 0;

  virtual void drawImage(int x, int y, ImageId image) = 0;

  virtual void drawCellImage(int x, int y, ImageId image) = 0;

  virtual void drawText(int x, int y, const char *text) = 0;

  // image buffer
  virtual ImageId loadImage(const uchar *data, uint len) = 0;
};

//! Field Runners Class
class CFieldRunners {
 public:
  //! Position (row,col)
  struct CellPos {
    int row;
    int col;

    CellPos(int row1=0, int col1=0) :
     row(row1), col(col1) {
    }

    // equality
    bool operator==(const CellPos &pos) const {
      return (row == pos.row && col == pos.col);
    }

    // equality
    bool operator!=(const CellPos &pos) const {
      return ! operator==(pos);
    }

    // less than (for sort)
    bool operator<(const CellPos &pos) const {
      return (row < pos.row || (row == pos.row && col < pos.col));
    }

    // print (for debug)
    friend std::ostream &operator<<(std::ostream &os, const CellPos &pos) {
      os << "(" << pos.row << "," << pos.col << ")";

      return os;
    }
  };

 public:
  // state
  enum class State {
    ACTIVE,
    GAME_OVER
  };

  // bg type
  enum class BgType {
    NONE,
    GRASSLANDS,
    STONE,
    CRYSTAL,
    CROSSROADS,
    FROSTBITE,
    DRYLANDS,
    LAVAFLOW,
    SKYWAY
  };

  // border type
  enum class BorderType {
    NONE,
    TREE,
    GRASSLANDS,
    CRYSTAL,
    CROSSROADS,
    FROSTBITE,
    DRYLANDS,
    LAVAFLOW,
    SKYWAY
  };

  // cell type
  enum { WEAPON_CELL = 1<<8 };

  enum class CellType {
    NONE,

    EMPTY,
    BORDER,
    BLOCK,

    GUN      = WEAPON_CELL + 1,
    GLUE     = WEAPON_CELL + 2,
    SNOWBOMB = WEAPON_CELL + 3,
    MISSILE  = WEAPON_CELL + 4,
    ZAP      = WEAPON_CELL + 5,
    PULSE    = WEAPON_CELL + 6,
    LASER    = WEAPON_CELL + 7,
    FIREBOMB = WEAPON_CELL + 8
  };

  // cell sub type
  enum class CellSubType {
    NONE,
    ROCK,
    GRASS,
    STONE,
    TRACK,
    IGLOO
  };

  // cell direction
  enum class CellDirection {
    NONE,
    N,
    S,
    E,
    W,
    NE,
    SE,
    NW,
    SW
  };

  // cell function
  enum class CellFunction {
    NONE,
    ENTRANCE,
    EXIT,
    EPATH
  };

  struct CellData {
    char          c           { '\0' };
    CellType      type        { CellType::NONE };
    CellSubType   subType     { CellSubType::NONE };
    CellDirection direction   { CellDirection::NONE };
    CellFunction  function    { CellFunction::NONE };
    std::string   id;
    std::string   desc;
    bool          transparent { false };
  };

  enum class RunnerType {
    NONE,
    SOLDIER,
    MERCENARY,
    CAR,
    TANK,
    HELICOPTER,
    PLANE,
    TRAIN,
    BLIMP
  };

 public:
  //! Base Class for Individual Cell on the Field Grid
  //! (Virtual - derive from this for different types of cells)
  class Cell : public CAStar<CellPos>::Node {
   public:
    Cell(CFieldRunners *fieldRunners, const CellPos &pos);

    virtual ~Cell() { }

    CFieldRunners *fieldRunners() const { return fieldRunners_; }

    void setPos(const CellPos &pos) { pos_ = pos; }
    const CellPos &getPos() const { return pos_; }

    //! boundary box (get/set)
    void setBBox(const BBox &bbox) { bbox_ = bbox; }
    void getBBox(BBox &bbox) const { bbox = bbox_; }

    // get/set sub type
    const CellSubType &subType() const { return cellData_.subType; }
    void setSubType(const CellSubType &t) { cellData_.subType = t; }

    // get/set direction
    const CellDirection &direction() const { return cellData_.direction; }
    void setDirection(const CellDirection &d) { cellData_.direction = d; }

    // get/set function
    const CellFunction &function() const { return cellData_.function; }
    void setFunction(const CellFunction &f) { cellData_.function = f; }

    // get/set id
    const std::string &id() const { return cellData_.id; }
    void setId(const std::string &s) { cellData_.id = s; }

    // get/set desc
    const std::string &desc() const { return cellData_.desc; }
    void setDesc(const std::string &s) { cellData_.desc = s; }

    // get/set transparent
    bool isTransparent() const { return cellData_.transparent; }
    void setTransparent(bool b) { cellData_.transparent = b; }

    CFieldRunnersWindow *getWindow() const;

    //! is block solid (runners can't walk through)
    virtual bool isSolid() const = 0;

    virtual bool isWeapon() const { return false; }

    //! can add
    virtual bool canAddWeapon() {
      if (isSolid())
        return false;

      if (function() == CellFunction::ENTRANCE ||
          function() == CellFunction::EXIT ||
          function() == CellFunction::EPATH)
        return false;

      return true;
    }

    //! buy price
    virtual int getBuyPrice() { return 0; }

    //! sell price
    virtual int getSellPrice() { return 0; }

    // get damage
    virtual int getDamage() const { return damage_; }

    //! update state
    virtual void update() { }

    //! draw
    virtual void draw() { }

   protected:
    CFieldRunners *fieldRunners_;

    CellPos  pos_;
    BBox     bbox_;
    CellData cellData_;
    int      damage_ { 0 };
  };

  //---

  //! Basic Empty Cell
  class EmptyCell : public Cell {
   public:
    EmptyCell(CFieldRunners *fieldRunners, const CellPos &pos) :
     Cell(fieldRunners, pos) {
    }

    // not solid
    bool isSolid() const override { return false; }
  };

  //---

  //! Border Cell
  class BorderCell : public Cell {
   public:
    BorderCell(CFieldRunners *fieldRunners, const CellPos &pos) :
     Cell(fieldRunners, pos) {
    }

    // solid (can't walk through)
    bool isSolid() const override { return true; }
  };

  //---

  //! Block Cell
  class BlockCell : public Cell {
   public:
    BlockCell(CFieldRunners *fieldRunners, const CellPos &pos) :
     Cell(fieldRunners, pos) {
    }

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    // draw block
    void draw() override;
  };

  //! orientation
  enum Orient {
    ORIENT_NONE = -1,
    ORIENT_E    = 0,
    ORIENT_NE   = 1,
    ORIENT_N    = 2,
    ORIENT_NW   = 3,
    ORIENT_W    = 4,
    ORIENT_SW   = 5,
    ORIENT_S    = 6,
    ORIENT_SE   = 7
  };

  //---

  //! Basic Gun Cell
  class GunCell : public Cell {
   public:
    static int buyPrice() { return 5; }

    GunCell(CFieldRunners *fieldRunners, const CellPos &pos);

    Orient orient() const { return orient_; }

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 3; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;

   private:
    Orient orient_ { ORIENT_NONE };

    static ImageId images_[8];    //! images for gun (rotate 0-315 by 45)
    static bool    imagesLoaded_; //! are images loaded ?
  };

  //---

  //! Glue Gun Cell
  class GlueCell : public Cell {
   public:
    static int buyPrice() { return 10; }

    GlueCell(CFieldRunners *fieldRunners, const CellPos &pos);

    Orient orient() const { return orient_; }

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 7; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;

   private:
    Orient orient_ { ORIENT_NONE };

    static ImageId images_[8];    //! images for gun (rotate 0-315 by 45)
    static bool    imagesLoaded_; //! are images loaded ?
  };

  //---

  //! Snowbomb Gun Cell
  class SnowbombCell : public Cell {
   public:
    static int buyPrice() { return 10; }

    SnowbombCell(CFieldRunners *fieldRunners, const CellPos &pos);

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 7; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;
  };

  //---

  //! Missile Gun Cell
  class MissileCell : public Cell {
   public:
    static int buyPrice() { return 20; }

    MissileCell(CFieldRunners *fieldRunners, const CellPos &pos);

    Orient orient() const { return orient_; }

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 15; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;

   private:
    Orient orient_ { ORIENT_NONE };
    int    reload_ { 0 };

    static ImageId images_[8];    //! images for gun (rotate 0-315 by 45)
    static bool    imagesLoaded_; //! are images loaded ?
  };

  //---

  //! Zap Gun Cell
  class ZapCell : public Cell {
   public:
    static int buyPrice() { return 70; }

    ZapCell(CFieldRunners *fieldRunners, const CellPos &pos);

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 50; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;

   private:
    static ImageId image_;       //! images for zapper
    static bool    imageLoaded_; //! are images loaded ?
  };

  //---

  //! Pulse Gun Cell
  class PulseCell : public Cell {
   public:
    static int buyPrice() { return 70; }

    PulseCell(CFieldRunners *fieldRunners, const CellPos &pos);

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 50; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;
  };

  //---

  //! Laser Gun Cell
  class LaserCell : public Cell {
   public:
    static int buyPrice() { return 70; }

    LaserCell(CFieldRunners *fieldRunners, const CellPos &pos);

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 50; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;
  };

  //---

  //! Firebomb Gun Cell
  class FirebombCell : public Cell {
   public:
    static int buyPrice() { return 70; }

    FirebombCell(CFieldRunners *fieldRunners, const CellPos &pos);

    // solid (can't walk through)
    bool isSolid() const override { return true; }

    bool isWeapon() const override { return true; }

    //! buy price
    int getBuyPrice() override { return buyPrice(); }

    //! sell price
    int getSellPrice() override { return 50; }

    //! range (how close to detect/shoot)
    int getRange() const { return 2; }

    // update gun
    void update() override;

    // draw block
    void draw() override;
  };

  //---

  //! Regular Grid of Cells (rows x cols)
  // TODO always regular grid ?
  class CellArray {
   public:
    CellArray() { }

    void setSize(uint rows, uint cols) {
      clear();

      rows_ = rows;
      cols_ = cols;

      cells_.resize(rows*cols);

      for (uint i = 0; i < cells_.size(); ++i)
        cells_[i] = nullptr;
    }

    void getSize(uint &rows, uint &cols) const {
      rows = rows_;
      cols = cols_;
    }

    bool isInside(int r, int c) const {
      return (r >= 0 && uint(r) < rows_ && c >= 0 && uint(c) < cols_);
    }

    Cell *getCell(uint row, uint col) const {
      return cells_[row*cols_ + col];
    }

    void setCell(uint row, uint col, Cell *cell) {
      cells_[row*cols_ + col] = cell;
    }

    void clear() {
      for (auto *cell : cells_)
        delete cell;

      cells_.clear();
    }

   private:
    uint rows_ { 0 }; // number of rows
    uint cols_ { 0 }; // number of columns

    // vector of cells
    std::vector<Cell *> cells_;
  };

  //------

  //! Player
  class Player {
   public:
    Player(CFieldRunners *fieldRunners);

    virtual ~Player() { }

    CFieldRunnersWindow *getWindow() const;

    int getScore() const { return score_; }
    int getLives() const { return lives_; }

    void loseLife(){ lives_ = lives_ - 1; }

    int getMoney() const { return money_; }

    void addMoney(int money) { money_ += money; }
    void subMoney(int money) { money_ -= money; }

    //! draw player details
    virtual void draw();

   private:
    CFieldRunners *fieldRunners_;

    int score_;
    int lives_;
    int money_;
  };

  //---

  //! Field (to run across)
  class Field {
   public:
    Field(CFieldRunners *fieldRunners);

    virtual ~Field() { }

    CFieldRunners *fieldRunners() const { return fieldRunners_; }

    CFieldRunnersWindow *getWindow() const;

    int getNumRows() const;
    int getNumCols() const;

    bool isInside(const CellPos &pos) const;

    void getCell(const CellPos &pos, Cell **cell) const;

    void setCell(const CellPos &pos, Cell *cell);

    //! setup field
    void build(uint rows, uint cols);

    void resizeCells(uint rows, uint cols);

    //! update field elements
    void update();

    //! draw field elements
    virtual void draw();

    //! draw field cells
    virtual void drawCells();

    //! draw weapons and prices
    virtual void drawWeaponPrices();

    virtual void drawWeaponPrice(CellType cellType, int x1, int y1, int w, int h);

    int cellPrice(CellType cellType) const;

   private:
    static bool imageLoaded_; //! is image loaded ?

    static ImageId gun_price_image_;             //! gun price image
    static ImageId glue_price_image_;            //! glue price image
    static ImageId missile_price_image_;         //! missile price image
    static ImageId zap_price_image_;             //! zap price image

    static ImageId gun_price_disable_image_;     //! disabled gun price image
    static ImageId glue_price_disable_image_;    //! disabled glue price image
    static ImageId missile_price_disable_image_; //! disabled missile price image
    static ImageId zap_price_disable_image_;     //! disabled zap price image

    CFieldRunners *fieldRunners_;

    CellArray cells_; //! field cells
  };

  //---

  //! Entrance to emit runners from
  class Entrance : public Cell {
   public:
    Entrance(CFieldRunners *fieldRunners, const CellPos &pos) :
     Cell(fieldRunners, pos) {
    }

    virtual ~Entrance() { }

    // not solid
    bool isSolid() const override { return false; }
  };

  //---

  //! Exit runners want to get to
  class Exit : public Cell {
   public:
    Exit(CFieldRunners *fieldRunners, const CellPos &pos) :
     Cell(fieldRunners, pos) {
    }

    virtual ~Exit() { }

    // not solid
    bool isSolid() const override { return false; }
  };

  //---

  //! Base class for Runner
  //! (Virtual - derive from this for different types of runners)
  // TODO: need to model speed, health
  // TODO: handle dying/dead/alive
  class Runner {
   public:
    Runner(CFieldRunners *fieldRunners);

    virtual ~Runner() { }

    CFieldRunners *fieldRunners() const { return fieldRunners_; }

    CFieldRunnersWindow *getWindow() const;

    // get/set current position
    const CellPos &getPos() const { return pos_; }
    void setPos(const CellPos &pos) { pos_ = pos; }

    // set/get goal position
    const CellPos &getGoal() const { return goal_; }
    void setGoal(const CellPos &goal) { goal_ = goal; }

    bool isAtGoal() const { return atGoal_; }

    bool isDamaged() const { return damaged_; }

    bool isDying() const { return dying_; }

    bool isDead() const { return dead_; }

    bool isDone() const { return isAtGoal() || isDead(); }

    // max health
    virtual int getMaxHealth() const = 0;

    // get/set health
    int getHealth() const { return health_; }

    void setHealth(int health) { health_ = health; }

    // damage runner
    void damage(int damage);

    // slow down runner
    void slowDown(int count);

    // get speed
    virtual double getSpeed() const = 0;

    // money at death
    virtual int getDeathMoney() const = 0;

    virtual int getNumFrames() const = 0;

    virtual bool updateDying();

    virtual ImageId getFrameImage(int frame) = 0;

    virtual ImageId getDeadImage();

    //! update runner (move to next cell)
    virtual void update();

    //! search next node
    virtual Cell *searchNext() const;

    //! draw runner
    virtual void draw();

   protected:
    void drawHealthBar(int x, int y);

    int getFrame() const;

    void getPos(int &x, int &y) const;

   protected:
    CFieldRunners *fieldRunners_;

    CellPos pos_;               //! current position
    CellPos goal_;              //! goal position
    bool    atGoal_  { false }; //! made it to the goal position
    int     health_  { 0 };     //! current health
    bool    damaged_ { false }; //! was damaged
    bool    dying_   { false }; //! is dying
    bool    dead_    { false }; //! is dead
    int     slow_    { 0 };     //! slow down count
    int     dx_      { 0 };     //! x distance travelled across current cell (pixels)
    int     dy_      { 0 };     //! y distance travelled across current cell (pixels)
    double  dist_    { 0.0 };   //! total distance travelled
    double  speed_   { 0.0 };   //! speed
  };

  //---

  //! Soldier Runner
  class Soldier : public Runner {
   public:
    Soldier(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 100; }

    int getDeathMoney() const override { return 1; }

    double getSpeed() const override { return 0.1; }

    int getNumFrames() const override { return 6; }

    bool updateDying() override;

    ImageId getFrameImage(int frame) override;

    ImageId getDeadImage() override;

   private:
    static ImageId images_[6];     //! images
    static ImageId deadImages_[6]; //! dead images
    static bool    imagesLoaded_;  //! are images loaded ?

    int deadFrame_;
  };

  //---

  //! Mercenary Runner
  class Mercenary : public Runner {
   public:
    Mercenary(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 150; }

    int getDeathMoney() const override { return 1; }

    double getSpeed() const override { return 0.1; }

    int getNumFrames() const override { return 6; }

    ImageId getFrameImage(int frame) override;

   private:
    static ImageId images_[6];    //! runner images
    static bool    imagesLoaded_; //! are images loaded ?
  };

  //---

  //! Car Runner
  class Car : public Runner {
   public:
    Car(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 200; }

    int getDeathMoney() const override { return 2; }

    double getSpeed() const override { return 0.2; }

    int getNumFrames() const override { return 1; }

    ImageId getFrameImage(int frame) override;

   private:
    static ImageId image_;       //! image
    static bool    imageLoaded_; //! is image loaded ?
  };

  //---

  //! Tank Runner
  class Tank : public Runner {
   public:
    Tank(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 500; }

    int getDeathMoney() const override { return 5; }

    double getSpeed() const override { return 0.2; }

    int getNumFrames() const override { return 1; }

    ImageId getFrameImage(int frame) override;

   private:
    static ImageId image_;       //! image
    static bool    imageLoaded_; //! is image loaded ?
  };

  //---

  //! Helicopter Runner
  class Helicopter : public Runner {
   public:
    Helicopter(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 300; }

    int getDeathMoney() const override { return 3; }

    double getSpeed() const override { return 0.5; }

    int getNumFrames() const override { return 1; }

    ImageId getFrameImage(int frame) override;

    Cell *searchNext() const override;

   private:
    static ImageId image_;       //! image
    static bool    imageLoaded_; //! is image loaded ?
  };

  //---

  //! Plane Runner
  class Plane : public Runner {
   public:
    Plane(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 300; }

    int getDeathMoney() const override { return 3; }

    double getSpeed() const override { return 0.5; }

    int getNumFrames() const override { return 1; }

    ImageId getFrameImage(int frame) override;

    Cell *searchNext() const override;

   private:
    static ImageId image_;       //! image
    static bool    imageLoaded_; //! is image loaded ?
  };

  //---

  //! Train Runner
  class Train : public Runner {
   public:
    Train(CFieldRunners *fieldRunners);

    int getMaxHealth() const override { return 300; }

    int getDeathMoney() const override { return 3; }

    double getSpeed() const override { return 0.5; }

    int getNumFrames() const override { return 1; }

    ImageId getFrameImage(int frame) override;

    Cell *searchNext() const override;

   private:
    static ImageId image_;       //! image
    static bool    imageLoaded_; //! is image loaded ?

    using CellSet = std::set<Cell *>;

    mutable CellSet visited_;
  };

  //-----

  //! Base class for Bullet
  //! (Virtual - derive from this for different types of bullets)
  class Bullet {
   public:
    Bullet(CFieldRunners *fieldRunners, const Point &point);

    virtual ~Bullet() { }

    CFieldRunnersWindow *getWindow() const;

    // get/set current point
    void setPoint(const Point &point) { point_ = point; }

    const Point &getPoint() const { return point_; }

    bool isAtGoal() const { return atGoal_; }

    virtual bool isDone() const { return isAtGoal(); }

    //! update bullet (move to next cell)
    virtual void update() = 0;

    //! draw bullet
    virtual void draw() = 0;

   protected:
    CFieldRunners *fieldRunners_;

    Point point_;  //! current point
    bool  atGoal_; //! made it to the goal point
  };

  //---

  class Rocket : public Bullet {
   public:
    Rocket(CFieldRunners *fieldRunners, const Point &point, Runner *runner, Orient orient);

    virtual ~Rocket() { }

    Orient orient() const { return orient_; }

    bool isDone() const override;

    int getDamage() const { return 20; }

    //! update rocket (move to next cell)
    void update() override;

    //! draw rocket
    void draw() override;

   private:
    Runner *runner_ { nullptr };
    Orient  orient_ { ORIENT_NONE };
//  int     frame_  { -1 };

    static ImageId images_[8];    //! rocket images
    static bool    imagesLoaded_; //! are images loaded ?
  };

  //-----

  // AStar search class
  class SearchField : public CAStar<CellPos> {
   protected:
    using AStar = CAStar<CellPos>;

   public:
    SearchField(CFieldRunners *fieldRunners);

   ~SearchField() { }

    // smallest/optimal cost to goal
    double pathCostEstimate(const CellPos &startLoc, const CellPos &goalLoc) override;

    double traverseCost(const CellPos &pos, const CellPos &newPos) override;

    NodeList getNextNodes(Node *node) const override;

    Node *lookupNode(const CellPos &loc) const override;

   private:
    CFieldRunners *fieldRunners_ { nullptr };
  };

  //-----

  using EntranceList = std::vector<Entrance *>;
  using ExitList     = std::vector<Exit *>;
  using RunnerList   = std::vector<Runner *>;
  using BulletList   = std::vector<Bullet *>;

  //-----

  using Weapons = std::vector<CellType>;

 public:
  CFieldRunners(CFieldRunnersWindow *window);

  virtual ~CFieldRunners();

  void init();

  CFieldRunnersWindow *getWindow() const { return window_; }

  //---

  virtual Player *createPlayer();

  virtual Field *createField();

  virtual Cell *createCell(CellType cellType, const CellPos &pos);

  virtual EmptyCell  *createEmptyCell (const CellPos &pos);
  virtual BorderCell *createBorderCell(const CellPos &pos);
  virtual BlockCell  *createBlockCell (const CellPos &pos);

  virtual GunCell      *createGunCell     (const CellPos &pos);
  virtual GlueCell     *createGlueCell    (const CellPos &pos);
  virtual SnowbombCell *createSnowbombCell(const CellPos &pos);
  virtual MissileCell  *createMissileCell (const CellPos &pos);
  virtual ZapCell      *createZapCell     (const CellPos &pos);
  virtual PulseCell    *createPulseCell   (const CellPos &pos);
  virtual LaserCell    *createLaserCell   (const CellPos &pos);
  virtual FirebombCell *createFirebombCell(const CellPos &pos);

  virtual Entrance *createEntrance(const CellPos &pos);
  virtual Exit     *createExit    (const CellPos &pos);

  virtual Soldier *createSoldier();

  virtual Mercenary *createMercenary();

  virtual Car *createCar();

  virtual Tank *createTank();

  virtual Helicopter *createHelicopter();

  virtual Plane *createPlane();

  virtual Train *createTrain();

  virtual Rocket *createRocket(const Point &point, Runner *runner, Orient orient);

  //---

  Player *getPlayer() const { return player_.get(); }

  uint getLevel() const { return level_; }

  const Weapons &getWeapons() const { return weapons_; }

  //---

  void getSize(Size &size) const;

  void getCellSize(Size &size) const { size = cellSize_; }

  void setCellSize(const Size &size) { cellSize_ = size; }

  int getNumRows() const;
  int getNumCols() const;

  bool isInside(const CellPos &pos) const;

  const Color &bgColor() const { return bgColor_; }
  void setBgColor(const Color &c) { bgColor_ = c; }

  const Color &fgColor() const { return fgColor_; }
  void setFgColor(const Color &c) { fgColor_ = c; }

  const BgType &bgType() const { return bgType_; }
  void setBgType(const BgType &t) { bgType_ = t; }

  const BorderType &borderType() const { return borderType_; }
  void setBorderType(const BorderType &t) { borderType_ = t; }

  void pixelToPos(const Point &point, CellPos &pos) const;
  void posToPixel(const CellPos &pos, Point &point) const;

  void getCell(const CellPos &pos, Cell **cell) const;

  uint getNumRunners() const;

  Runner *getRunner(uint i) const;

  // get/set buy cell type
  CellType getBuyCellType() const { return buyCellType_; }
  void setBuyCellType(CellType cellType);

  // buy cell
  void buyCell(const CellPos &pos);

  // sell cell
  void sellCell(const CellPos &pos);

  // add block cell
  void addBlockCell(const CellPos &pos);

  // is entrance at pos
  bool isEntrance(const CellPos &pos) const;

  // is exit at pos
  bool isExit(const CellPos &pos) const;

  //---

  // send out a soldier
  void emitSoldier(const std::string &entranceId="");

  // send out a mercenary
  void emitMercenary(const std::string &entranceId="");

  // send out a car
  void emitCar(const std::string &entranceId="");

  // send out a tank
  void emitTank(const std::string &entranceId="");

  // send out a helicopter
  void emitHelicopter(const std::string &entranceId="");

  // send out a plane
  void emitPlane(const std::string &entranceId="");

  // send out a train
  void emitTrain(const std::string &entranceId="");

  //---

  // send out a rocket
  void emitRocket(const Point &point, Runner *runner, Orient orient);

  //---

  CellPos getStartPos(std::string &id) const;
  CellPos getEndPos  (const std::string &id) const;

  void playerAddMoney(int money);

  void playerLoseLife();

  Runner *nearestRunner(const CellPos &loc, int &dist) const;
  Runner *nearestRunner(const CellPos &loc, int &dist, int &dx, int &dy) const;

  void initBuild();

  //! build game using (rows x cols) grid
  void build(uint rows, uint cols);

  Entrance *addEntrance(int r, int c);
  Exit     *addExit    (int r, int c);

  bool loadMap(const std::string &filename);

  //! get/set paused
  bool isPaused() const { return paused_; }
  void setPaused(bool b) { paused_ = b; }

  //! update game
  void update();

  //! draw game
  void draw();

  //! draw health bar
  virtual void drawHealthBar(int x, int y, double percent);

  virtual void drawMoney(int money);
  virtual void drawScore(int score);
  virtual void drawLives(int lives);
  virtual void drawLevel(int level);

  //! bool of specified percent (0->1) chance
  static bool randBool(double weight);

  static int randInt(int n);

  //! integer to string
  static std::string toString(int i);

 private:
  using Window  = CFieldRunnersWindow;
  using PlayerP = std::unique_ptr<Player>;
  using FieldP  = std::unique_ptr<Field>;

  struct RunnerData {
    RunnerType  type       { RunnerType::NONE };
    std::string entranceId;
    uint        ticks      { 10 };
  };

  using RunnerDatas = std::vector<RunnerData>;

  using LevelRunners = std::map<int, RunnerDatas>;

  Window*      window_;                           //!< Window to draw to
  PlayerP      player_;                           //!< Current player
  FieldP       field_;                            //!< Field to run across
  EntranceList entrances_;                        //!< List of entrance for runners
  ExitList     exits_;                            //!< List of exits for runners
  RunnerList   runners_;                          //!< List of runners
  BulletList   bullets_;                          //!< List of bullets
  Size         cellSize_;                         //!< Cell size
  Color        bgColor_     { 100, 200, 100 };    //!< Bg color
  Color        fgColor_     { 0, 0, 0 };          //!< Fg color
  BgType       bgType_      { BgType::NONE };     //!< Bg type
  BorderType   borderType_  { BorderType::NONE }; //!< Border type
  bool         paused_      { false };            //!< is paused
  uint         tick_        { 0 };                //!< Update Count
  uint         level_       { 1 };                //!< Current Level
  uint         levelTicks_  { 100 };              //!< Current Level ticks
  Weapons      weapons_;                          //!< Weapons
  CellType     buyCellType_ { CellType::GUN };    //!< Buy Cell Type
  LevelRunners levelRunners_;                     //!< Runners per level
};

#endif

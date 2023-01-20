#include <CFieldRunners.h>
#include <CFile.h>
#include <CStrUtil.h>

#include <algorithm>
#include <climits>
#include <cmath>

CFieldRunners::
CFieldRunners(CFieldRunnersWindow *window) :
 window_(window)
{
  srand(uint(time(nullptr)));
}

CFieldRunners::
~CFieldRunners()
{
}

void
CFieldRunners::
init()
{
  player_ = PlayerP(createPlayer());
  field_  = FieldP (createField ());
}

//---

CFieldRunners::Player *
CFieldRunners::
createPlayer()
{
  std::cerr << "createPlayer\n";

  return new Player(this);
}

CFieldRunners::Field *
CFieldRunners::
createField()
{
  std::cerr << "createField\n";

  return new Field(this);
}

CFieldRunners::FieldCell *
CFieldRunners::
createCell(CellType cellType, const CellPos &pos)
{
  //std::cerr << "createCell\n";

  FieldCell *cell = nullptr;

  if      (cellType == CellType::EMPTY   ) cell = createEmptyCell   (pos);
  else if (cellType == CellType::BORDER  ) cell = createBorderCell  (pos);
  else if (cellType == CellType::BLOCK   ) cell = createBlockCell   (pos);

  else if (cellType == CellType::GUN     ) cell = createGunCell     (pos);
  else if (cellType == CellType::GLUE    ) cell = createGlueCell    (pos);
  else if (cellType == CellType::SNOWBOMB) cell = createSnowbombCell(pos);
  else if (cellType == CellType::MISSILE ) cell = createMissileCell (pos);
  else if (cellType == CellType::SHOTGUN ) cell = createShotgunCell (pos);
  else if (cellType == CellType::ZAP     ) cell = createZapCell     (pos);
  else if (cellType == CellType::PULSE   ) cell = createPulseCell   (pos);
  else if (cellType == CellType::LASER   ) cell = createLaserCell   (pos);
  else if (cellType == CellType::FIREBOMB) cell = createFirebombCell(pos);

  assert(cell);

  return cell;
}

CFieldRunners::WeaponCell *
CFieldRunners::
createWeaponCell(CellType cellType, const CellPos &pos)
{
  auto *cell = createCell(cellType, pos);

  auto *weapon = dynamic_cast<WeaponCell *>(cell);
  assert(weapon);

  return weapon;
}

//---

CFieldRunners::EmptyCell *
CFieldRunners::
createEmptyCell(const CellPos &pos)
{
  return new EmptyCell(this, pos);
}

CFieldRunners::BorderCell *
CFieldRunners::
createBorderCell(const CellPos &pos)
{
  return new BorderCell(this, pos);
}

CFieldRunners::BlockCell *
CFieldRunners::
createBlockCell(const CellPos &pos)
{
  return new BlockCell(this, pos);
}

//---

CFieldRunners::GunCell *
CFieldRunners::
createGunCell(const CellPos &pos)
{
  return new GunCell(this, pos);
}

CFieldRunners::GlueCell *
CFieldRunners::
createGlueCell(const CellPos &pos)
{
  return new GlueCell(this, pos);
}

CFieldRunners::SnowbombCell *
CFieldRunners::
createSnowbombCell(const CellPos &pos)
{
  return new SnowbombCell(this, pos);
}

CFieldRunners::MissileCell *
CFieldRunners::
createMissileCell(const CellPos &pos)
{
  return new MissileCell(this, pos);
}

CFieldRunners::ShotgunCell *
CFieldRunners::
createShotgunCell(const CellPos &pos)
{
  return new ShotgunCell(this, pos);
}

CFieldRunners::ZapCell *
CFieldRunners::
createZapCell(const CellPos &pos)
{
  return new ZapCell(this, pos);
}

CFieldRunners::PulseCell *
CFieldRunners::
createPulseCell(const CellPos &pos)
{
  return new PulseCell(this, pos);
}

CFieldRunners::LaserCell *
CFieldRunners::
createLaserCell(const CellPos &pos)
{
  return new LaserCell(this, pos);
}

CFieldRunners::FirebombCell *
CFieldRunners::
createFirebombCell(const CellPos &pos)
{
  return new FirebombCell(this, pos);
}

//---

CFieldRunners::EntranceCell *
CFieldRunners::
createEntrance(const CellPos &pos)
{
  std::cerr << "createEntrance\n";

  return new EntranceCell(this, pos);
}

CFieldRunners::ExitCell *
CFieldRunners::
createExit(const CellPos &pos)
{
  std::cerr << "createExit\n";

  return new ExitCell(this, pos);
}

//---

CFieldRunners::Soldier *
CFieldRunners::
createSoldier()
{
  std::cerr << "createSoldier\n";

  return new Soldier(this);
}

CFieldRunners::Mercenary *
CFieldRunners::
createMercenary()
{
  std::cerr << "createMercenary\n";

  return new Mercenary(this);
}

CFieldRunners::Motorbike *
CFieldRunners::
createMotorbike()
{
  std::cerr << "createMotorbike\n";

  return new Motorbike(this);
}

CFieldRunners::Car *
CFieldRunners::
createCar()
{
  std::cerr << "createCar\n";

  return new Car(this);
}

CFieldRunners::Heavybike *
CFieldRunners::
createHeavybike()
{
  std::cerr << "createHeavybike\n";

  return new Heavybike(this);
}

CFieldRunners::Tank *
CFieldRunners::
createTank()
{
  std::cerr << "createTank\n";

  return new Tank(this);
}

CFieldRunners::Helicopter *
CFieldRunners::
createHelicopter()
{
  std::cerr << "createHelicopter\n";

  return new Helicopter(this);
}

CFieldRunners::Plane *
CFieldRunners::
createPlane()
{
  std::cerr << "createPlane\n";

  return new Plane(this);
}

CFieldRunners::Train *
CFieldRunners::
createTrain()
{
  std::cerr << "createTrain\n";

  return new Train(this);
}

CFieldRunners::Blimp *
CFieldRunners::
createBlimp()
{
  std::cerr << "createBlimp\n";

  return new Blimp(this);
}

//---

CFieldRunners::MissileBullet *
CFieldRunners::
createMissileBullet(const Point &point, RunnerCell *runner, Orient orient)
{
  std::cerr << "createMissileBullet\n";

  return new MissileBullet(this, point, runner, orient);
}

CFieldRunners::PulseBullet *
CFieldRunners::
createPulseBullet(const Point &point, Orient orient)
{
  std::cerr << "createPulseBullet\n";

  return new PulseBullet(this, point, orient);
}

CFieldRunners::LaserBullet *
CFieldRunners::
createLaserBullet(const Point &point, Orient orient)
{
  std::cerr << "createLaserBullet\n";

  return new LaserBullet(this, point, orient);
}

//---

void
CFieldRunners::
getSize(Size &size) const
{
  window_->getSize(size);
}

int
CFieldRunners::
getNumRows() const
{
  return field_->getNumRows();
}

int
CFieldRunners::
getNumCols() const
{
  return field_->getNumCols();
}

bool
CFieldRunners::
isInside(const CellPos &pos) const
{
  return field_->isInside(pos);
}

void
CFieldRunners::
pixelToPos(const Point &point, CellPos &pos) const
{
  Size cellSize;
  getCellSize(cellSize);

  pos.col = point.x/cellSize.width;
  pos.row = point.y/cellSize.height;
}

void
CFieldRunners::
posToPixel(const CellPos &pos, Point &point) const
{
  return posToPixel(pos, 0.0, 0.0, point);
}

void
CFieldRunners::
posToPixel(const CellPos &pos, double dx, double dy, Point &point) const
{
  Size cellSize;
  getCellSize(cellSize);

  point.x = int((pos.col + dx)*cellSize.width );
  point.y = int((pos.row + dy)*cellSize.height);
}

void
CFieldRunners::
getCell(const CellPos &pos, FieldCell **cell) const
{
  return field_->getCell(pos, cell);
}

uint
CFieldRunners::
getNumRunners() const
{
  return uint(runners_.size());
}

CFieldRunners::RunnerCell *
CFieldRunners::
getRunner(uint i) const
{
  return runners_[i];
}

void
CFieldRunners::
getCellRunners(const CellPos &pos, RunnerArray &runners) const
{
  for (auto *runner : runners_)
    if (runner->getPos() == pos)
      runners.push_back(runner);
}

void
CFieldRunners::
setBuyCellType(CellType cellType)
{
  buyCellType_ = cellType;
}

CFieldRunners::Cell *
CFieldRunners::
buyCell(const CellPos &pos)
{
  auto price = cellPrice(buyCellType_);

  if (player_->getMoney() < price)
    return nullptr;

  int num_rows = getNumRows();
  int num_cols = getNumCols();

  if (pos.row <= 0 || pos.row >= num_rows - 1 ||
      pos.col <= 0 || pos.col >= num_cols - 1)
    return nullptr;

  FieldCell *cell;
  field_->getCell(pos, &cell);

  if (! cell || ! cell->canAddWeapon())
    return nullptr;

  auto *cell1 = createWeaponCell(buyCellType_, pos);

  field_->setCell(pos, cell1);

  player_->subMoney(price);

  return cell1;
}

void
CFieldRunners::
sellCell(const CellPos &pos)
{
  int num_rows = getNumRows();
  int num_cols = getNumCols();

  if (pos.row <= 0 || pos.row >= num_rows - 1 ||
      pos.col <= 0 || pos.col >= num_cols - 1)
    return;

  FieldCell *cell;
  field_->getCell(pos, &cell);
  if (! cell) return;

  if (! cell->isWeapon())
    return;

  sellCell(cell);
}

void
CFieldRunners::
sellCell(FieldCell *cell)
{
  assert(cell && cell->isWeapon());

  auto *weapon = dynamic_cast<WeaponCell *>(cell);
  assert(weapon);

  int money = weapon->getSellPrice();

  auto pos = cell->getPos();

  auto *cell1 = createCell(CellType::EMPTY, pos);

  field_->setCell(pos, cell1);

  player_->addMoney(money);
}

bool
CFieldRunners::
upgradeCell(FieldCell *cell)
{
  assert(cell && cell->isWeapon());

  auto *weapon = dynamic_cast<WeaponCell *>(cell);
  assert(weapon);

  auto price = weapon->getUpgradePrice();

  if (player_->getMoney() < price)
    return false;

  if (weapon->level() >= 3)
    return false;

  weapon->setLevel(weapon->level() + 1);

  player_->subMoney(price);

  return true;
}

void
CFieldRunners::
selectCell(FieldCell *cell)
{
  deselectAll();

  cell->setSelected(true);
}

void
CFieldRunners::
deselectAll()
{
  for (auto *cell1 : field_->cells().cells()) {
    cell1->setSelected(false);
  }
}

void
CFieldRunners::
addBlockCell(const CellPos &pos)
{
  int num_rows = getNumRows();
  int num_cols = getNumCols();

  if (pos.row <= 0 || pos.row >= num_rows - 1 ||
      pos.col <= 0 || pos.col >= num_cols - 1)
    return;

  FieldCell *cell;
  field_->getCell(pos, &cell);
  if (! cell) return;

  if (! cell->isSolid()) {
    auto *cell1 = createCell(CellType::BLOCK, pos);

    field_->setCell(pos, cell1);
  }
}

void
CFieldRunners::
build(uint rows, uint cols)
{
  initBuild();

  (void) addEntrance(int(rows/2), 0);
  (void) addExit    (int(rows/2), int(cols - 1));

  //---

  field_->build(rows, cols);

  if (weapons_.empty()) {
    weapons_.push_back(CellType::GUN);
    weapons_.push_back(CellType::GLUE);
    weapons_.push_back(CellType::MISSILE);
    weapons_.push_back(CellType::ZAP);
  }
}

void
CFieldRunners::
initBuild()
{
  entrances_.clear();
  exits_    .clear();

  tick_  = 0;
  level_ = 1;

  levelTicks_ = 100;

  weapons_.clear();

  levelRunners_.clear();

  annotations_.clear();
}

CFieldRunners::EntranceCell *
CFieldRunners::
addEntrance(int r, int c)
{
  CellPos pos(r, c);

  auto *entrance = createEntrance(pos);

  entrance->setPos(pos);
  entrance->setFunction(CellFunction::ENTRANCE);

  entrances_.push_back(entrance);

  return entrance;
}

CFieldRunners::ExitCell *
CFieldRunners::
addExit(int r, int c)
{
  CellPos pos(r, c);

  auto *exit = createExit(pos);

  exit->setPos(pos);
  exit->setFunction(CellFunction::EXIT);

  exits_.push_back(exit);

  return exit;
}

bool
CFieldRunners::
loadMap(const std::string &filename)
{
  CFile file(filename);

  CFile::Lines lines;

  if (! file.toLines(lines)) {
    std::cerr << "Can't read file '" << filename << "'\n";
    return false;
  }

  auto nl = lines.size();

  if (nl < 2) {
    std::cerr << "Not enough lines in file '" << filename << "'\n";
    return false;
  }

  uint il = 0;

  //---

  std::string lastLine;
  int         lastLineNum { 0 };

  auto readLine = [&]() {
    lastLineNum = il;
    lastLine    = (il < nl ? lines[il++] : "");

    return lastLine;
  };

  auto errorMsg = [&](const std::string &msg) {
    std::cerr << "Error: " << msg << "\n";
    std::cerr << "  Line: '" << lastLine << "' (#" << lastLineNum << ")\n";
    return false;
  };

  //---

  int nc = -1;
  int nr = -1;

  // read def lines (to blank line)
  while (il < nl) {
    auto line = readLine();
    if (line == "") break;

    if (line[0] == '#') continue;

    std::vector<std::string> fields;
    CStrUtil::addFields(line, fields, "=");

    if (fields.size() != 2)
      return errorMsg("Invalid def line '" + line + "'");

    if      (fields[0] == "nc") {
      if (! CStrUtil::toInteger(fields[1], &nc))
        return errorMsg("Invalid integer '" + fields[1] + "'");
    }
    else if (fields[0] == "nr") {
      if (! CStrUtil::toInteger(fields[1], &nr))
        return errorMsg("Invalid integer '" + fields[1] + "'");
    }
    else if (fields[0] == "bgcolor") {
      std::vector<std::string> fields1;
      CStrUtil::addFields(fields[1], fields1, ",");

      if (fields1.size() != 3)
        return errorMsg("Invalid color '" + fields[1] + "'");

      int r, g, b;

      if (! CStrUtil::toInteger(fields1[0], &r) ||
          ! CStrUtil::toInteger(fields1[1], &g) ||
          ! CStrUtil::toInteger(fields1[2], &b))
        return errorMsg("Invalid color '" + fields[1] + "'");

      setBgColor(Color(r/255.0, g/255.0, b/255.0));
    }
    else if (fields[0] == "fgcolor") {
      std::vector<std::string> fields1;
      CStrUtil::addFields(fields[1], fields1, ",");

      if (fields1.size() != 3)
        return errorMsg("Invalid color '" + fields[1] + "'");

      int r, g, b;

      if (! CStrUtil::toInteger(fields1[0], &r) ||
          ! CStrUtil::toInteger(fields1[1], &g) ||
          ! CStrUtil::toInteger(fields1[2], &b))
        return errorMsg("Invalid color '" + fields[1] + "'");

      setFgColor(Color(r/255.0, g/255.0, b/255.0));
    }
    else if (fields[0] == "bg") {
      if      (fields[1] == "none"      ) setBgType(BgType::NONE);
      else if (fields[1] == "stone"     ) setBgType(BgType::STONE);
      else if (fields[1] == "grasslands") setBgType(BgType::GRASSLANDS);
      else if (fields[1] == "crystal"   ) setBgType(BgType::CRYSTAL);
      else if (fields[1] == "crossroads") setBgType(BgType::CROSSROADS);
      else if (fields[1] == "frostbite" ) setBgType(BgType::FROSTBITE);
      else if (fields[1] == "drylands"  ) setBgType(BgType::DRYLANDS);
      else if (fields[1] == "lavaflow"  ) setBgType(BgType::LAVAFLOW);
      else if (fields[1] == "skyway"    ) setBgType(BgType::SKYWAY);
      else if (fields[1] == "mudslide"  ) setBgType(BgType::MUDSLIDE);
      else return errorMsg("Invalid bg type '" + fields[1] + "'");
    }
    else if (fields[0] == "border") {
      if      (fields[1] == "none"      ) setBorderType(BorderType::NONE);
      else if (fields[1] == "grasslands") setBorderType(BorderType::GRASSLANDS);
      else if (fields[1] == "crystal"   ) setBorderType(BorderType::CRYSTAL);
      else if (fields[1] == "crossroads") setBorderType(BorderType::CROSSROADS);
      else if (fields[1] == "frostbite" ) setBorderType(BorderType::FROSTBITE);
      else if (fields[1] == "drylands"  ) setBorderType(BorderType::DRYLANDS);
      else if (fields[1] == "lavaflow"  ) setBorderType(BorderType::LAVAFLOW);
      else if (fields[1] == "skyway"    ) setBorderType(BorderType::SKYWAY);
      else if (fields[1] == "mudslide"  ) setBorderType(BorderType::MUDSLIDE);
      else return errorMsg("Invalid border type '" + fields[1] + "'");
    }
    else if (fields[0] == "max_level") {
      int maxLevels { 0 };

      if (! CStrUtil::toInteger(fields[1], &maxLevels))
        return errorMsg("Invalid integer '" + fields[1] + "'");

      if (maxLevels > 0)
        setMaxLevels(uint(maxLevels));
    }
    else if (fields[0] == "start_money") {
      int startMoney { 0 };

      if (! CStrUtil::toInteger(fields[1], &startMoney))
        return errorMsg("Invalid integer '" + fields[1] + "'");

      if (startMoney > 0)
        setStartMoney(uint(startMoney));
    }
    else if (fields[0] == "start_lives") {
      int startLives { 0 };

      if (! CStrUtil::toInteger(fields[1], &startLives))
        return errorMsg("Invalid integer '" + fields[1] + "'");

      if (startLives > 0)
        setStartLives(uint(startLives));
    }
    else
      return errorMsg("Invalid def name '" + fields[0] + "'");
  }

  if (nc <= 0 || nr <= 0) {
    std::cerr << "Invalid map size nc=" << nc << " nr=" << nr << "\n";
    return false;
  }

  //---

  // read tile lines (to blank line)
  using CharCellData = std::map<char, CellData>;

  CharCellData charCellData;

  while (il < nl) {
    auto line = readLine();
    if (line == "") break;

    if (line[0] == '#') continue;

    std::vector<std::string> fields;
    CStrUtil::addFields(line, fields, ",");

    int nf = int(fields.size());

    if (nf < 2)
      return errorMsg("Invalid tile line '" + line + "'");

    if (fields[0].size() != 1)
      return errorMsg("Invalid tile char '" + fields[0] + "'");

    // char,type,subtype,direction,function,id,attributs
    CellData cellData;

    // char
    cellData.c = fields[0][0];

    cellData.desc = line;

    // type
    if      (fields[1] == "border") cellData.type = CellType::BORDER;
    else if (fields[1] == "block" ) cellData.type = CellType::BLOCK;
    else if (fields[1] == "empty" ) cellData.type = CellType::EMPTY;
    else return errorMsg("Invalid tile cell type '" + fields[1] + "'");

    // subtype
    if (nf > 2) {
      if      (fields[2] == "none" ) cellData.subType = CellSubType::NONE;
      else if (fields[2] == "rock" ) cellData.subType = CellSubType::ROCK;
      else if (fields[2] == "grass") cellData.subType = CellSubType::GRASS;
      else if (fields[2] == "stone") cellData.subType = CellSubType::STONE;
      else if (fields[2] == "track") cellData.subType = CellSubType::TRACK;
      else if (fields[2] == "igloo") cellData.subType = CellSubType::IGLOO;
      else return errorMsg("Invalid tile sub cell type '" + fields[2] + "'");
    }

    // direction
    if (nf > 3) {
      if      (fields[3] == "none") cellData.direction = CellDirection::NONE;
      else if (fields[3] == "n"   ) cellData.direction = CellDirection::N;
      else if (fields[3] == "s"   ) cellData.direction = CellDirection::S;
      else if (fields[3] == "w"   ) cellData.direction = CellDirection::W;
      else if (fields[3] == "e"   ) cellData.direction = CellDirection::E;
      else if (fields[3] == "ne"  ) cellData.direction = CellDirection::NE;
      else if (fields[3] == "nw"  ) cellData.direction = CellDirection::NW;
      else if (fields[3] == "se"  ) cellData.direction = CellDirection::SE;
      else if (fields[3] == "sw"  ) cellData.direction = CellDirection::SW;
      else return errorMsg("Invalid tile direction '" + fields[3] + "'");
    }

    // function
    if (nf > 4) {
      if      (fields[4] == "none"    ) cellData.function = CellFunction::NONE;
      else if (fields[4] == "entrance") cellData.function = CellFunction::ENTRANCE;
      else if (fields[4] == "exit"    ) cellData.function = CellFunction::EXIT;
      else if (fields[4] == "epath"   ) cellData.function = CellFunction::EPATH;
      else return errorMsg("Invalid tile sub cell type '" + fields[4] + "'");
    }

    // id
    if (nf > 5) {
      cellData.id = fields[5];
    }

    // attributes
    if (nf > 6) {
      if (fields[6] != "none") {
        std::vector<std::string> fields1;
        CStrUtil::addFields(fields[6], fields1, ",");

        for (const auto &field1 : fields1) {
          if (field1 == "transparent") cellData.transparent = true;
          else return errorMsg("Invalid tile attribute '" + field1 + "'");
        }
      }
    }

    if (nf > 7) {
      return errorMsg("Invalid line length '" + std::to_string(nf) + "'");
    }

    charCellData[cellData.c] = cellData;
  }

  //---

  // read field lines (to blank line)
  bool blankFound = false;

  auto grid = new char [nr*nc];

  int k = 0;

  for (int i = 0; i < nr; ++i) {
    auto line = readLine();

    if (line == "") {
      blankFound = true;
      break;
    }

    auto len = line.size();

    if (int(len) != nc) {
      std::cerr << "Invalid line '" << line << "' len " << len << " expected " << nc << "'\n";
      return false;
    }

    std::cerr << line << " " << len << "\n";

    for (auto j = 0; j < nc; ++j) {
      grid[k++] = line[j];
    }
  }

  if (! blankFound) {
    if (il < nl) {
      auto line = readLine();

      if (line != "")
        return errorMsg("Missing blank line after cell grid");
    }
  }

  //---

  // read runner lines (to blank line)
  using CharRunner = std::map<char, RunnerType>;

  CharRunner charRunner;

  while (il < nl) {
    auto line = readLine();
    if (line == "") break;

    if (line[0] == '#') continue;

    std::vector<std::string> fields;
    CStrUtil::addFields(line, fields, ",");

    int nf = int(fields.size());

    if (nf < 2)
      return errorMsg("Invalid runner line '" + line + "'");

    // char
    char c = fields[0][0];

    // type
    RunnerType runnerType { RunnerType::NONE };

    if      (fields[1] == "soldier"   ) runnerType = RunnerType::SOLDIER;
    else if (fields[1] == "mercenary" ) runnerType = RunnerType::MERCENARY;
    else if (fields[1] == "motorbike" ) runnerType = RunnerType::MOTORBIKE;
    else if (fields[1] == "car"       ) runnerType = RunnerType::CAR;
    else if (fields[1] == "heavybike" ) runnerType = RunnerType::HEAVYBIKE;
    else if (fields[1] == "tank"      ) runnerType = RunnerType::TANK;
    else if (fields[1] == "helicopter") runnerType = RunnerType::HELICOPTER;
    else if (fields[1] == "plane"     ) runnerType = RunnerType::PLANE;
    else if (fields[1] == "train"     ) runnerType = RunnerType::TRAIN;
    else if (fields[1] == "blimp"     ) runnerType = RunnerType::BLIMP;
    else return errorMsg("Invalid runner type '" + fields[1] + "'");

    charRunner[c] = runnerType;
  }

  //---

  // read weapons lines (to blank line)
  Weapons weapons;

  while (il < nl) {
    auto line = readLine();
    if (line == "") break;

    if (line[0] == '#') continue;

    std::vector<std::string> fields;
    CStrUtil::addFields(line, fields, ",");

    for (const auto &field : fields) {
      if      (field == "gun"     ) weapons.push_back(CellType::GUN);
      else if (field == "glue"    ) weapons.push_back(CellType::GLUE);
      else if (field == "snowbomb") weapons.push_back(CellType::SNOWBOMB);
      else if (field == "missile" ) weapons.push_back(CellType::MISSILE);
      else if (field == "shotgun" ) weapons.push_back(CellType::SHOTGUN);
      else if (field == "zap"     ) weapons.push_back(CellType::ZAP);
      else if (field == "pulse"   ) weapons.push_back(CellType::PULSE);
      else if (field == "laser"   ) weapons.push_back(CellType::LASER);
      else if (field == "firebomb") weapons.push_back(CellType::FIREBOMB);
      else return errorMsg("Invalid weapon '" + field + "'");
    }
  }

  //---

  // read level lines (to blank line)
  LevelRunners levelRunners;

  int level = 1;

  while (il < nl) {
    auto line = readLine();
    if (line == "") break;

    if (line[0] == '#') continue;

    std::vector<std::string> fields;
    CStrUtil::addFields(line, fields, ",");

    for (const auto &field : fields) {
      std::vector<std::string> fields1;
      CStrUtil::addFields(field, fields1, ":");

      if (fields1.size() != 3)
        return errorMsg("Invalid level runner '" + field + "'");

      auto c        = fields1[0][0];
      auto entrance = fields1[1];

      int ticks;

      if (! CStrUtil::toInteger(fields1[2], &ticks))
        return errorMsg("Invalid level runner ticks '" + fields1[2] + "'");

      auto pr = charRunner.find(c);

      if (pr == charRunner.end())
        return errorMsg("Invalid runner char '" + fields1[0] + "'");

      RunnerData runnerData;

      runnerData.type       = (*pr).second;
      runnerData.entranceId = entrance;
      runnerData.ticks      = ticks;

      levelRunners[level].push_back(runnerData);
    }

    ++level;
  }

  //---

  // read annotations
  Annotations annotations;

  while (il < nl) {
    auto line = readLine();
    if (line == "") break;

    if (line[0] == '#') continue;

    std::vector<std::string> fields;
    CStrUtil::addFields(line, fields, ",");

    if (fields.size() != 5)
      return errorMsg("Invalid annotation line");

    int row, col, width, height;

    if (! CStrUtil::toInteger(fields[0], &row) ||
        ! CStrUtil::toInteger(fields[1], &col) ||
        ! CStrUtil::toInteger(fields[2], &width) ||
        ! CStrUtil::toInteger(fields[3], &height))
      return errorMsg("Invalid annotation line");

    auto id = fields[4];

    Annotation annotation;

    annotation.id     = id;
    annotation.row    = row;
    annotation.col    = col;
    annotation.width  = width;
    annotation.height = height;

    annotations.push_back(annotation);
  }

  //---

  if (il != nl)
    return errorMsg("End of file expected");

  //---

  // init field and runner data from file data
  initBuild();

  field_->resizeCells(nr, nc);

  k = 0;

  for (int r = 0; r < nr; ++r) {
    for (int c = 0; c < nc; ++c) {
      auto pos = CellPos(r, c);

      auto gc = grid[k++];

      auto pc = charCellData.find(gc);

      if (pc == charCellData.end()) {
        std::cerr << "Invalid map char '" << gc << "'\n";
        return false;
      }

      const auto &cellData = (*pc).second;

      auto *cell = createCell(cellData.type, pos);

      cell->setSubType(cellData.subType);
      cell->setDirection(cellData.direction);
      cell->setFunction(cellData.function);

      if (cellData.function == CellFunction::ENTRANCE || cellData.function == CellFunction::EXIT) {
        if (cellData.type != CellType::EMPTY)
          return errorMsg("Invalid Entrance/Exit");
      }

      if      (cellData.function == CellFunction::ENTRANCE) {
        auto *entrance = addEntrance(r, c);

        entrance->setId(cellData.id);
      }
      else if (cellData.function == CellFunction::EXIT) {
        auto *exit = addExit(r, c);

        exit->setId(cellData.id);
      }

      cell->setId(cellData.id);
      cell->setDesc(cellData.desc);

      if (cellData.transparent)
        cell->setTransparent(true);

      field_->setCell(pos, cell);
    }
  }

  //---

  weapons_ = weapons;

  levelRunners_ = levelRunners;

  annotations_ = annotations;

  //---

  // cleanup
  delete [] grid;

  //---

  player_->init();

  return true;
}

bool
CFieldRunners::
isEntrance(const CellPos &pos) const
{
  auto num_entrances = entrances_.size();

  for (uint i = 0; i < num_entrances; ++i) {
    if (entrances_[i]->getPos() == pos)
      return true;
  }

  return false;
}

bool
CFieldRunners::
isExit(const CellPos &pos) const
{
  auto num_exits = exits_.size();

  for (uint i = 0; i < num_exits; ++i) {
    if (exits_[i]->getPos() == pos)
      return true;
  }

  return false;
}

void
CFieldRunners::
emitSoldier(const std::string &entranceId)
{
  auto *soldier = createSoldier();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  soldier->setPos (startPos);
  soldier->setGoal(endPos);

  runners_.push_back(soldier);
}

void
CFieldRunners::
emitMercenary(const std::string &entranceId)
{
  auto *mercenary = createMercenary();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  mercenary->setPos (startPos);
  mercenary->setGoal(endPos);

  runners_.push_back(mercenary);
}

void
CFieldRunners::
emitMotorbike(const std::string &entranceId)
{
  auto *motorbike = createMotorbike();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  motorbike->setPos (startPos);
  motorbike->setGoal(endPos);

  runners_.push_back(motorbike);
}

void
CFieldRunners::
emitCar(const std::string &entranceId)
{
  auto *car = createCar();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  car->setPos (startPos);
  car->setGoal(endPos);

  runners_.push_back(car);
}

void
CFieldRunners::
emitHeavybike(const std::string &entranceId)
{
  auto *heavybike = createHeavybike();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  heavybike->setPos (startPos);
  heavybike->setGoal(endPos);

  runners_.push_back(heavybike);
}

void
CFieldRunners::
emitTank(const std::string &entranceId)
{
  auto *tank = createTank();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  tank->setPos (startPos);
  tank->setGoal(endPos);

  runners_.push_back(tank);
}

void
CFieldRunners::
emitHelicopter(const std::string &entranceId)
{
  auto *plane = createHelicopter();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  plane->setPos (startPos);
  plane->setGoal(endPos);

  runners_.push_back(plane);
}

void
CFieldRunners::
emitPlane(const std::string &entranceId)
{
  auto *plane = createPlane();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  plane->setPos (startPos);
  plane->setGoal(endPos);

  runners_.push_back(plane);
}

void
CFieldRunners::
emitTrain(const std::string &entranceId)
{
  auto *train = createTrain();

  //---

  CellPos startPos, endPos;
  bool    startPosSet { false }, endPosSet { false };

  int rows = getNumRows();
  int cols = getNumCols();

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      FieldCell *cell;
      getCell(CellPos(row, col), &cell);

      auto *emptyCell = dynamic_cast<EmptyCell *>(cell);
      if (! emptyCell) continue;

      if      (emptyCell->subType() == CellSubType::TRACK) {
        if      (emptyCell->function() == CellFunction::ENTRANCE) {
          if (! startPosSet) {
            startPos = emptyCell->getPos();

            if (emptyCell->id() == entranceId)
              startPosSet = true;
          }
        }
        else if (emptyCell->function() == CellFunction::EXIT) {
          if (! endPosSet ) {
            endPos = emptyCell->getPos();

            if (emptyCell->id() == entranceId)
              endPosSet = true;
          }
        }
      }
    }
  }

  train->setPos (startPos);
  train->setGoal(endPos);

  runners_.push_back(train);
}

void
CFieldRunners::
emitBlimp(const std::string &entranceId)
{
  auto *plane = createBlimp();

  std::string id = entranceId;

  auto startPos = getStartPos(id);
  auto endPos   = getEndPos  (id);

  plane->setPos (startPos);
  plane->setGoal(endPos);

  runners_.push_back(plane);
}

//---

void
CFieldRunners::
emitMissileBullet(const Point &point, RunnerCell *runner, Orient orient)
{
  auto *bullet = createMissileBullet(point, runner, orient);

  bullets_.push_back(bullet);
}

void
CFieldRunners::
emitPulseBullet(const Point &point, Orient orient)
{
  auto *bullet = createPulseBullet(point, orient);

  bullets_.push_back(bullet);
}

void
CFieldRunners::
emitLaserBullet(const Point &point, Orient orient)
{
  auto *bullet = createLaserBullet(point, orient);

  bullets_.push_back(bullet);
}

//---

CFieldRunners::CellPos
CFieldRunners::
getStartPos(std::string &id) const
{
  if (id != "") {
    for (auto *entrance : entrances_) {
      if (entrance->id() == id)
        return entrance->getPos();
    }
  }

  //---

  id = "";

  auto n = entrances_.size();

  if (n == 0) {
    int rows = field_->getNumRows();

    return CellPos(rows/2, 0);
  }

  auto *entrance = entrances_[uint(randInt(int(n)))];

  id = entrance->id();

  return entrance->getPos();
}

CFieldRunners::CellPos
CFieldRunners::
getEndPos(const std::string &id) const
{
  for (auto *exit : exits_) {
    if (id == "" || exit->id() == id)
      return exit->getPos();
  }

  int rows = field_->getNumRows();
  int cols = field_->getNumCols();

  return CellPos(rows/2, cols - 1);
}

void
CFieldRunners::
playerAddMoney(int money)
{
  player_->addMoney(money);
}

void
CFieldRunners::
playerAddScore(int score)
{
  player_->addScore(score);
}

void
CFieldRunners::
playerLoseLife()
{
  player_->loseLife();
}

CFieldRunners::RunnerCell *
CFieldRunners::
nearestRunner(const CellPos &loc, uint &dist) const
{
  int dx, dy;
  auto *runner = nearestRunner(loc, dist, dx, dy);

  return runner;
}

CFieldRunners::RunnerCell *
CFieldRunners::
nearestRunner(const CellPos &loc, uint &dist, int &dx, int &dy) const
{
  dist = 0;

  RunnerCell *minRunner = nullptr;
  CellPos     minPos;
  uint        minDist = 0;

  // find nearest enemy and if close enough rotate 1 turn to face
  uint numRunners = this->getNumRunners();
  if (numRunners == 0) return nullptr;

  for (uint i = 0; i < numRunners; ++i) {
    auto *runner = this->getRunner(i);

    const auto &pos = runner->getPos();

    uint dx1 = std::abs(pos.col - loc.col);
    uint dy1 = std::abs(pos.row - loc.row);

    uint dist1 = dx1 + dy1;

    if (! minRunner || dist1 < minDist) {
      minRunner = runner;
      minPos    = pos;
      minDist   = dist1;
    }
  }

  dx = minPos.col - loc.col; // -ve left, +ve right
  dy = loc.row - minPos.row; // -ve below, +ve above

  dist = std::max(abs(dx), abs(dy));

  return minRunner;
}

void
CFieldRunners::
update()
{
  if (isPaused() || isSettings())
    return;

  int nt = (isFastForward() ? 2 : 1);

  for (int t = 0; t < nt; ++t) {
    // remove done bullets
    bullets_.erase(
      std::remove_if(bullets_.begin(), bullets_.end(),
                     std::mem_fun(&Bullet::isDone)), bullets_.end());

    //------

    // remove done runners
    runners_.erase(
      std::remove_if(runners_.begin(), runners_.end(),
                     std::mem_fun(&RunnerCell::isDone)), runners_.end());

    if (tick_ > levelTicks_ && runners_.empty()) {
      if (level_ < maxLevels()) {
        ++level_;

        tick_       = 0;
        levelTicks_ = 100;

        std::cerr << "Level " << level_ << "\n";
      }
    }

    //------

    // update bullets
    for (auto *bullet : bullets_)
      bullet->update();

    // update runners
    for (auto *runner : runners_)
      runner->update();

    // update field
    field_->update();

    //------

    auto pl = levelRunners_.find(level_);

    if (pl != levelRunners_.end()) {
      for (const auto &runnerData : (*pl).second) {
        if (tick_ == runnerData.ticks) {
          emitRunner(runnerData.type, runnerData.entranceId);
        }
      }
    }
    else {
      if      (tick_ %  30 == 0) emitRunner(RunnerType::SOLDIER);
      else if (tick_ %  50 == 0) emitRunner(RunnerType::MERCENARY);
      else if (tick_ %  55 == 0) emitRunner(RunnerType::TRAIN);
      else if (tick_ %  70 == 0) emitRunner(RunnerType::CAR);
      else if (tick_ % 110 == 0) emitRunner(RunnerType::TANK);
      else if (tick_ % 130 == 0) emitRunner(RunnerType::PLANE);
    }

    //------

    ++tick_;
  }
}

void
CFieldRunners::
emitRunner(RunnerType type, std::string entranceId)
{
  switch (type) {
    case RunnerType::SOLDIER   : emitSoldier   (entranceId); break;
    case RunnerType::MERCENARY : emitMercenary (entranceId); break;
    case RunnerType::MOTORBIKE : emitMotorbike (entranceId); break;
    case RunnerType::CAR       : emitCar       (entranceId); break;
    case RunnerType::HEAVYBIKE : emitHeavybike (entranceId); break;
    case RunnerType::TANK      : emitTank      (entranceId); break;
    case RunnerType::HELICOPTER: emitHelicopter(entranceId); break;
    case RunnerType::PLANE     : emitPlane     (entranceId); break;
    case RunnerType::TRAIN     : emitTrain     (entranceId); break;
    case RunnerType::BLIMP     : emitBlimp     (entranceId); break;
    default: assert(false); break;
  }
}

void
CFieldRunners::
draw()
{
  Size windowSize;

  window_->getSize(windowSize);

  Size cellSize;

  cellSize.width  = windowSize.width /getNumCols();
  cellSize.height = windowSize.height/getNumRows();

  setCellSize(cellSize);

  //------

  field_->draw();

  //------

  for (auto *bullet : bullets_)
    bullet->draw();

  for (auto *runner : runners_)
    runner->draw();

  //------

  player_->draw();

  //------

  drawPausePlay();
  drawFastForward();
  drawSettings();

  drawWeaponPrices();

  //------

  field_->drawSelectedCell();
}

void
CFieldRunners::
drawHealthBar(int x, int y, double percent)
{
  int healthWidth  = 22;
  int healthHeight = 3;

  getWindow()->setForeground(Color(1, 0, 0));
  getWindow()->fillRectangle(x + 2, y, x + int(healthWidth*percent), y + healthHeight);

  getWindow()->setForeground(Color(1, 1, 1));
  getWindow()->drawRectangle(x + 2, y, x + healthWidth, y + healthHeight);
}

void
CFieldRunners::
drawMoney(int money)
{
  int xl = 8;

  getWindow()->drawText(xl, 16, CFieldRunners::toString(money).c_str());
}

void
CFieldRunners::
drawScore(int score)
{
  Size windowSize;
  getWindow()->getSize(windowSize);

  int xm = windowSize.width/2;

  getWindow()->drawText(xm, 16, CFieldRunners::toString(score).c_str());
}

void
CFieldRunners::
drawLives(int lives)
{
  Size windowSize;
  getWindow()->getSize(windowSize);

  int xr = windowSize.width - 32;

  getWindow()->drawText(xr, 16, CFieldRunners::toString(lives).c_str());
}

void
CFieldRunners::
drawLevel(int level)
{
  Size windowSize;
  getWindow()->getSize(windowSize);

  int xr = windowSize.width - 32;

  getWindow()->drawText(xr, 32, CFieldRunners::toString(level).c_str());
}

void
CFieldRunners::
drawPausePlay()
{
}

void
CFieldRunners::
drawFastForward()
{
}

void
CFieldRunners::
drawSettings()
{
}

void
CFieldRunners::
drawAnnotation(int, int, int, int, const std::string &)
{
}

void
CFieldRunners::
drawWeaponPrices()
{
  uint rows = getNumRows();
  uint cols = getNumCols();

  Size cellSize;
  getCellSize(cellSize);

  int w = cellSize.width;
  int h = cellSize.height;

  int x1 = int(cols)*cellSize.width  - int(getWeapons().size())*w;
  int y1 = int(rows)*cellSize.height - h;

  for (const auto &weapon : getWeapons()) {
    drawWeaponPrice(weapon, x1, y1, w, h);

    x1 += w;
  }
}

#include "images/gun_price_png.h"
#include "images/glue_price_png.h"
#include "images/missile_price_png.h"
#include "images/zap_price_png.h"

#include "images/gun_price_disable_png.h"
#include "images/glue_price_disable_png.h"
#include "images/missile_price_disable_png.h"
#include "images/zap_price_disable_png.h"

void
CFieldRunners::
drawWeaponPrice(CellType cellType, int x1, int y1, int w, int h)
{
  static bool    images_loaded;;
  static ImageId gun_price_image;
  static ImageId glue_price_image;
  static ImageId missile_price_image;
  static ImageId zap_price_image;
  static ImageId gun_price_disable_image;
  static ImageId glue_price_disable_image;
  static ImageId missile_price_disable_image;
  static ImageId zap_price_disable_image;

  if (! images_loaded) {
    images_loaded = true;

    auto *window = getWindow();

    gun_price_image     = window->loadImage(gun_price_data    , GUN_PRICE_DATA_LEN);
    glue_price_image    = window->loadImage(glue_price_data   , GLUE_PRICE_DATA_LEN);
    missile_price_image = window->loadImage(missile_price_data, MISSILE_PRICE_DATA_LEN);
    zap_price_image     = window->loadImage(zap_price_data    , ZAP_PRICE_DATA_LEN);

    gun_price_disable_image     =
     window->loadImage(gun_price_disable_data    , GUN_PRICE_DISABLE_DATA_LEN);
    glue_price_disable_image    =
     window->loadImage(glue_price_disable_data   , GLUE_PRICE_DISABLE_DATA_LEN);
    missile_price_disable_image =
     window->loadImage(missile_price_disable_data, MISSILE_PRICE_DISABLE_DATA_LEN);
    zap_price_disable_image     =
     window->loadImage(zap_price_disable_data    , ZAP_PRICE_DISABLE_DATA_LEN);
  }

  int money = getPlayer()->getMoney();
  int price = cellPrice(cellType);

  auto selectImage = [&](const ImageId &image, const ImageId &disable_image) {
    if (money >= price)
      getWindow()->drawImage(x1, y1, image);
    else
      getWindow()->drawImage(x1, y1, disable_image);
  };

  switch (cellType) {
    case CellType::GUN     : selectImage(gun_price_image    , gun_price_disable_image    ); break;
    case CellType::GLUE    : selectImage(glue_price_image   , glue_price_disable_image   ); break;
    case CellType::SNOWBOMB: selectImage(glue_price_image   , glue_price_disable_image   ); break;
    case CellType::MISSILE : selectImage(missile_price_image, missile_price_disable_image); break;
    case CellType::SHOTGUN : selectImage(missile_price_image, missile_price_disable_image); break;
    case CellType::ZAP     : selectImage(zap_price_image    , zap_price_disable_image    ); break;
    case CellType::PULSE   : selectImage(zap_price_image    , zap_price_disable_image    ); break;
    case CellType::LASER   : selectImage(zap_price_image    , zap_price_disable_image    ); break;
    case CellType::FIREBOMB: selectImage(zap_price_image    , zap_price_disable_image    ); break;
    default: assert(false); break;
 }

  if (getBuyCellType() == cellType) {
    getWindow()->setForeground(Color(1.0, 0.0, 0.0));
    getWindow()->drawRectangle(x1, y1, x1 + w, y1 + h);
  }
}

int
CFieldRunners::
cellPrice(CellType cellType) const
{
  switch (cellType) {
    case CellType::GUN     : return CFieldRunners::GunCell     ::buyPrice();
    case CellType::GLUE    : return CFieldRunners::GlueCell    ::buyPrice();
    case CellType::SNOWBOMB: return CFieldRunners::SnowbombCell::buyPrice();
    case CellType::MISSILE : return CFieldRunners::MissileCell ::buyPrice();
    case CellType::SHOTGUN : return CFieldRunners::ShotgunCell ::buyPrice();
    case CellType::ZAP     : return CFieldRunners::ZapCell     ::buyPrice();
    case CellType::PULSE   : return CFieldRunners::PulseCell   ::buyPrice();
    case CellType::LASER   : return CFieldRunners::LaserCell   ::buyPrice();
    case CellType::FIREBOMB: return CFieldRunners::FirebombCell::buyPrice();
    default: assert(false) ; return 0;
  }
}

//---

double
CFieldRunners::
orientAngle(Orient orient) const
{
  double da = M_PI/4.0;

  return orient*da;
}

//---

bool
CFieldRunners::
randBool(double weight)
{
  double r = (1.0*rand())/RAND_MAX;

  return (r <= weight);
}

int
CFieldRunners::
randInt(int n)
{
  int r = int(n*(1.0*rand())/RAND_MAX);

  return std::min(std::max(r, 0), n - 1);
}

std::string
CFieldRunners::
toString(int i)
{
  if (i < 0)
    return "-" + toString(-i);

  std::string str;

  int d = 1;

  while (d*10 < i)
    d *= 10;

  while (d >= 10) {
    int r1 = i % 10;

    char c = char('0' + r1);

    str = c + str;

    i /= 10;
    d /= 10;
  }

  char c = char('0' + i);

  str = c + str;

  return str;
}

//-----------

CFieldRunners::Player::
Player(CFieldRunners *fieldRunners) :
 fieldRunners_(fieldRunners)
{
  init();
}

void
CFieldRunners::Player::
init()
{
  money_ = fieldRunners_->startMoney();
  lives_ = fieldRunners_->startLives();
}

CFieldRunnersWindow *
CFieldRunners::Player::
getWindow() const
{
  return fieldRunners_->getWindow();
}

void
CFieldRunners::Player::
draw()
{
  getWindow()->setForeground(Color(1.0, 1.0, 1.0));

  fieldRunners_->drawMoney(getMoney());
  fieldRunners_->drawScore(getScore());
  fieldRunners_->drawLives(getLives());
  fieldRunners_->drawLevel(fieldRunners_->getLevel());
}

//-----------

#include "images/field_png.h"

CFieldRunners::Field::
Field(CFieldRunners *fieldRunners) :
 fieldRunners_(fieldRunners)
{
}

CFieldRunnersWindow *
CFieldRunners::Field::
getWindow() const
{
  return fieldRunners_->getWindow();
}

int
CFieldRunners::Field::
getNumRows() const
{
  uint rows, cols;

  cells_.getSize(rows, cols);

  return int(rows);
}

int
CFieldRunners::Field::
getNumCols() const
{
  uint rows, cols;

  cells_.getSize(rows, cols);

  return int(cols);
}

bool
CFieldRunners::Field::
isInside(const CellPos &pos) const
{
  return cells_.isInside(pos.row, pos.col);
}

void
CFieldRunners::Field::
getCell(const CellPos &pos, FieldCell **cell) const
{
  *cell = cells_.getCell(uint(pos.row), uint(pos.col));

  if (*cell)
    assert((*cell)->getPos() == pos);
}

void
CFieldRunners::Field::
setCell(const CellPos &pos, FieldCell *cell)
{
  assert(cell->getPos() == pos);

  delete cells_.getCell(uint(pos.row), uint(pos.col));

  cells_.setCell(uint(pos.row), uint(pos.col), cell);
}

void
CFieldRunners::Field::
build(uint rows, uint cols)
{
  resizeCells(rows, cols);

  for (uint row = 0; row < rows; ++row) {
    for (uint col = 0; col < cols; ++col) {
      auto pos = CellPos(int(row), int(col));

      FieldCell *cell;

      if (row <= 0 || row >= rows - 1 || col <= 0 || col >= cols - 1) {
        if (fieldRunners_->isEntrance(pos) || fieldRunners_->isExit(pos))
          cell = fieldRunners_->createCell(CellType::EMPTY, pos);
        else
          cell = fieldRunners_->createCell(CellType::BORDER, pos);
      }
      else {
#if 1
        bool is_block = CFieldRunners::randBool(0.05);

        if (is_block && col != 1 && col != cols - 2)
          cell = fieldRunners_->createCell(CellType::BLOCK, pos);
        else
          cell = fieldRunners_->createCell(CellType::EMPTY, pos);
#else
        cell = fieldRunners_->createCell(CellType::EMPTY, pos);
#endif
      }

      setCell(CellPos(row, col), cell);
    }
  }
}

void
CFieldRunners::Field::
resizeCells(uint rows, uint cols)
{
  cells_.setSize(rows, cols);
}

void
CFieldRunners::Field::
update()
{
  int rows = getNumRows();
  int cols = getNumCols();

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      FieldCell *cell;
      getCell(CellPos(row, col), &cell);
      if (! cell) continue;

      cell->update();
    }
  }
}

void
CFieldRunners::Field::
draw()
{
  static ImageId s_field_image;

  if (! s_field_image) {
    auto *window = fieldRunners_->getWindow();

    s_field_image = window->loadImage(field_data, FIELD_DATA_LEN);
  }

  getWindow()->drawImage(0, 0, s_field_image);

#if 0
  uint rows = getNumRows();
  uint cols = getNumCols();

  Size cellSize;
  fieldRunners_->getCellSize(cellSize);

  // draw background

  getWindow()->fill(Color(0.2, 0.8, 0.2));

  // draw grid
  Size windowSize;

  fieldRunners_->getSize(windowSize);

  getWindow()->setForeground(Color(0.2, 0.6, 0.2));

  for (uint row = 0; row <= rows; ++row)
    getWindow()->drawLine(0, row*cellSize.height, windowSize.width - 1, row*cellSize.height);

  for (uint col = 0; col <= cols; ++col)
    getWindow()->drawLine(col*cellSize.width , 0, col*cellSize.width , windowSize.height - 1);
#endif

  //------

  drawCells();

  drawAnnotations();
}

void
CFieldRunners::Field::
drawCells()
{
  uint rows = getNumRows();
  uint cols = getNumCols();

  Size cellSize;
  fieldRunners_->getCellSize(cellSize);

  // draw cells
  selectedCell_ = nullptr;

  int y = 0;

  for (uint row = 0; row < rows; ++row) {
    int x = 0;

    for (uint col = 0; col < cols; ++col) {
      FieldCell *cell;
      getCell(CellPos(row, col), &cell);
      if (! cell) continue;

      cell->setBBox(BBox(x, y, x + cellSize.width - 1, y + cellSize.height - 1));

      cell->draw();

      x += cellSize.width;

      if (cell->isSelected())
        selectedCell_ = cell;
    }

    y += cellSize.height;
  }
}

void
CFieldRunners::Field::
drawSelectedCell()
{
  if (! selectedCell_)
    return;

  drawSelected(selectedCell_);
}

void
CFieldRunners::Field::
drawSelected(FieldCell *cell)
{
  cell->drawSelected();
}

void
CFieldRunners::Field::
drawAnnotations()
{
  for (const auto &annotation : fieldRunners_->getAnnotations()) {
    fieldRunners_->drawAnnotation(annotation.row, annotation.col,
                                  annotation.width, annotation.height,
                                  annotation.id);
  }
}

//-----------

void
CFieldRunners::BlockCell::
draw()
{
  FieldCell *cell;
  fieldRunners()->getCell(getPos(), &cell);
  if (! cell) return;

  BBox bbox;
  cell->getBBox(bbox);

  int x, y;

  bbox.getLL(&x, &y);

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  getWindow()->setForeground(Color(0.2, 0.2, 0.2));
  getWindow()->fillRectangle(x, y, x + cellSize.width, y + cellSize.height);
}

//-----------

CFieldRunners::RunnerCell::
RunnerCell(CFieldRunners *fieldRunners) :
 Cell(fieldRunners, CellPos())
{
}

void
CFieldRunners::RunnerCell::
bulletDamage(uint bulletId, uint damage)
{
  auto p = bullets_.find(bulletId);

  if (p == bullets_.end()) {
    this->damage(damage);

    bullets_.insert(bulletId);
  }
}

void
CFieldRunners::RunnerCell::
damage(uint damage)
{
  if (isDying() || isDead())
    return;

  setHealth(getHealth() - damage);

  setDamaged(true);

  if (getHealth() <= 0) {
    setDying(true);

    setHealth(0);

    fieldRunners()->playerAddMoney(getDeathMoney());
    fieldRunners()->playerAddScore(getDeathScore());
  }
}

void
CFieldRunners::RunnerCell::
slowDown(int count)
{
  if (slow_ == 0)
    slow_ = count;
}

void
CFieldRunners::RunnerCell::
update()
{
  if (isDying()) {
    setDead(updateDying());
    return;
  }

  if (isAtGoal() || isDead())
    return;

  auto *nextCell = searchNext();

  if (nextCell) {
    const auto &nextPos = nextCell->loc;

    Size cellSize;
    fieldRunners()->getCellSize(cellSize);

    // move by speed in direction of next cell
    double speed = (slow_ > 0 ? getSpeed()/2 : getSpeed());

    auto pos = getPos();

  //int x = pos.col*cellSize.width  + dx_;
  //int y = pos.row*cellSize.height + dy_;

  //int x1 = nextPos.col*cellSize.width;
  //int y1 = nextPos.row*cellSize.height;

    if      (abs(pos.col - nextPos.col) > std::abs(pos.row - nextPos.row)) {
      // move horizontal (mostly horizontal)
      double d = (nextPos.col > pos.col ? speed : -speed);

      int d1 = (d > 0 ? int(d*cellSize.width + 0.5) : int(d*cellSize.width - 0.5));

      dx_   += d1;
      dist_ += d1;
    }
    else if (abs(pos.row - nextPos.row) > std::abs(pos.col - nextPos.col)) {
      // move vertical (mostly vertical)
      double d = (nextPos.row > pos.row ? speed : -speed);

      int d1 = (d > 0 ? int(d*cellSize.height + 0.5) : int(d*cellSize.height - 0.5));

      dy_   += d1;
      dist_ += d1;
    }
    else {
      // no move
    }

    // move to next col if delta greater than cell width
    if      (dx_ >= cellSize.width) {
      dx_ -= cellSize.width;

      ++pos.col;

      setPos(pos);
    }
    else if (dx_ <= -cellSize.width) {
      dx_ += cellSize.width;

      --pos.col;

      setPos(pos);
    }

    // move to next row if delta greater than cell height
    if      (dy_ >= cellSize.height) {
      dy_ -= cellSize.height;

      ++pos.row;

      setPos(pos);
    }
    else if (dy_ <= -cellSize.height) {
      dy_ += cellSize.height;

      --pos.row;

      setPos(pos);
    }

    // check at goal (exit)
    setAtGoal(pos == goal_);

    if (isAtGoal())
      fieldRunners()->playerLoseLife();
  }

  //---

  // decrement slow counter
  if (slow_ > 0)
    --slow_;
}

CFieldRunners::FieldCell *
CFieldRunners::RunnerCell::
searchNext() const
{
  SearchField searchField(fieldRunners());

  SearchField::NodeList pathNodes;

  if (searchField.search(getPos(), goal_, pathNodes)) {
    if (pathNodes.size() < 2)
      return nullptr;

    auto p = pathNodes.begin();

    ++p;

    return dynamic_cast<FieldCell *>(*p);
  }
  else
    return nullptr;
}

bool
CFieldRunners::RunnerCell::
updateDying()
{
  return true;
}

void
CFieldRunners::RunnerCell::
draw()
{
  int x, y;

  getXYPos(x, y);

  if (! isDying()) {
    int frame = getFrame();

    getWindow()->drawCellImage(x, y, getFrameImage(frame));

    if (isDamaged()) {
      drawHealthBar(x, y);

      //setDamaged(false);
    }
  }
  else {
    getWindow()->drawCellImage(x, y, getDeadImage());
  }
}

void
CFieldRunners::RunnerCell::
drawHealthBar(int x, int y)
{
  double percent =(1.0*getHealth())/getMaxHealth();

  fieldRunners()->drawHealthBar(x, y, percent);
}

ImageId
CFieldRunners::RunnerCell::
getDeadImage()
{
  return getFrameImage(0);
}

int
CFieldRunners::RunnerCell::
getFrame() const
{
  return int(dist_) % getNumFrames();
}

void
CFieldRunners::RunnerCell::
getXYPos(int &x, int &y) const
{
  FieldCell *cell;
  fieldRunners()->getCell(getPos(), &cell);
  if (! cell) return;

  BBox bbox;
  cell->getBBox(bbox);

  bbox.getLL(&x, &y);

  x += int(dx_);
  y += int(dy_);
}

//-----------

#include "images/soldier1_png.h"
#include "images/soldier2_png.h"
#include "images/soldier3_png.h"
#include "images/soldier4_png.h"
#include "images/soldier5_png.h"
#include "images/soldier6_png.h"
#include "images/dead_soldier1_png.h"
#include "images/dead_soldier2_png.h"
#include "images/dead_soldier3_png.h"
#include "images/dead_soldier4_png.h"
#include "images/dead_soldier5_png.h"
#include "images/dead_soldier6_png.h"

ImageId CFieldRunners::Soldier::images_[6];
ImageId CFieldRunners::Soldier::deadImages_[6];
bool    CFieldRunners::Soldier::imagesLoaded_;

CFieldRunners::Soldier::
Soldier(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners), deadFrame_(0)
{
  if (! imagesLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    images_[0] = window->loadImage(soldier1_data, SOLDIER1_DATA_LEN);
    images_[1] = window->loadImage(soldier2_data, SOLDIER2_DATA_LEN);
    images_[2] = window->loadImage(soldier3_data, SOLDIER3_DATA_LEN);
    images_[3] = window->loadImage(soldier4_data, SOLDIER4_DATA_LEN);
    images_[4] = window->loadImage(soldier5_data, SOLDIER5_DATA_LEN);
    images_[5] = window->loadImage(soldier6_data, SOLDIER6_DATA_LEN);

    deadImages_[0] = window->loadImage(dead_soldier1_data, DEAD_SOLDIER1_DATA_LEN);
    deadImages_[1] = window->loadImage(dead_soldier2_data, DEAD_SOLDIER2_DATA_LEN);
    deadImages_[2] = window->loadImage(dead_soldier3_data, DEAD_SOLDIER3_DATA_LEN);
    deadImages_[3] = window->loadImage(dead_soldier4_data, DEAD_SOLDIER4_DATA_LEN);
    deadImages_[4] = window->loadImage(dead_soldier5_data, DEAD_SOLDIER5_DATA_LEN);
    deadImages_[5] = window->loadImage(dead_soldier6_data, DEAD_SOLDIER6_DATA_LEN);

    imagesLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Soldier::
getFrameImage(int frame)
{
  return images_[frame];
}

bool
CFieldRunners::Soldier::
updateDying()
{
  ++deadFrame_;

  return (deadFrame_ >= 18);
}

ImageId
CFieldRunners::Soldier::
getDeadImage()
{
  return deadImages_[std::min(deadFrame_/3, 5)];
}

//-----------

#include "images/grenade1_png.h"
#include "images/grenade2_png.h"
#include "images/grenade3_png.h"
#include "images/grenade4_png.h"
#include "images/grenade5_png.h"
#include "images/grenade6_png.h"

ImageId CFieldRunners::Mercenary::images_[6];
bool    CFieldRunners::Mercenary::imagesLoaded_;

CFieldRunners::Mercenary::
Mercenary(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  if (! imagesLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    images_[0] = window->loadImage(grenade1_data, GRENADE1_DATA_LEN);
    images_[1] = window->loadImage(grenade2_data, GRENADE2_DATA_LEN);
    images_[2] = window->loadImage(grenade3_data, GRENADE3_DATA_LEN);
    images_[3] = window->loadImage(grenade4_data, GRENADE4_DATA_LEN);
    images_[4] = window->loadImage(grenade5_data, GRENADE5_DATA_LEN);
    images_[5] = window->loadImage(grenade6_data, GRENADE6_DATA_LEN);

    imagesLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Mercenary::
getFrameImage(int frame)
{
  return images_[frame];
}

//-----------

CFieldRunners::Motorbike::
Motorbike(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Motorbike::
getFrameImage(int /*frame*/)
{
  return -1;
}

//-----------

#include "images/car_png.h"

ImageId CFieldRunners::Car::image_;
bool    CFieldRunners::Car::imageLoaded_;

CFieldRunners::Car::
Car(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  if (! imageLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    image_ = window->loadImage(car_data, CAR_DATA_LEN);

    imageLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Car::
getFrameImage(int)
{
  return image_;
}

//-----------

CFieldRunners::Heavybike::
Heavybike(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Heavybike::
getFrameImage(int /*frame*/)
{
  return -1;
}

//-----------

#include "images/tank_png.h"

ImageId CFieldRunners::Tank::image_;
bool    CFieldRunners::Tank::imageLoaded_;

CFieldRunners::Tank::
Tank(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  if (! imageLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    image_ = window->loadImage(tank_data, TANK_DATA_LEN);

    imageLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Tank::
getFrameImage(int)
{
  return image_;
}

//-----------

#include "images/helicopter_png.h"

ImageId CFieldRunners::Helicopter::image_;
bool    CFieldRunners::Helicopter::imageLoaded_;

CFieldRunners::Helicopter::
Helicopter(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  if (! imageLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    image_ = window->loadImage(helicopter_data, HELICOPTER_DATA_LEN);

    imageLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Helicopter::
getFrameImage(int)
{
  return image_;
}

CFieldRunners::FieldCell *
CFieldRunners::Helicopter::
searchNext() const
{
  FieldCell *cell;

  fieldRunners()->getCell(goal_, &cell);

  return cell;
}

//-----------

#include "images/plane_png.h"

ImageId CFieldRunners::Plane::image_;
bool    CFieldRunners::Plane::imageLoaded_;

CFieldRunners::Plane::
Plane(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  if (! imageLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    image_ = window->loadImage(plane_data, PLANE_DATA_LEN);

    imageLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Plane::
getFrameImage(int)
{
  return image_;
}

CFieldRunners::FieldCell *
CFieldRunners::Plane::
searchNext() const
{
  FieldCell *cell;

  fieldRunners()->getCell(goal_, &cell);

  return cell;
}

//-----------

#include "images/train_png.h"

ImageId CFieldRunners::Train::image_;
bool    CFieldRunners::Train::imageLoaded_;

CFieldRunners::Train::
Train(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  if (! imageLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    image_ = window->loadImage(plane_data, TRAIN_DATA_LEN);

    imageLoaded_ = true;
  }

  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Train::
getFrameImage(int)
{
  return image_;
}

CFieldRunners::FieldCell *
CFieldRunners::Train::
searchNext() const
{
  FieldCell *cell;
  fieldRunners()->getCell(getPos(), &cell);

  if (visited_.find(cell) == visited_.end())
    visited_.insert(cell);

  int rows = fieldRunners()->getNumRows();
  int cols = fieldRunners()->getNumCols();

  for (int row = getPos().row - 1; row <= getPos().row + 1; ++row) {
    if (row < 0 || row >= rows) continue;

    for (int col = getPos().col - 1; col <= getPos().col + 1; ++col) {
      if (col < 0 || col >= cols) continue;

      FieldCell *cell1;
      fieldRunners()->getCell(CellPos(row, col), &cell1);

      auto *emptyCell = dynamic_cast<EmptyCell *>(cell1);

      if (emptyCell) {
        if (emptyCell->subType() == CellSubType::TRACK) {
          if (visited_.find(emptyCell) == visited_.end())
            return cell1;
        }
      }

      auto *blockCell = dynamic_cast<BlockCell *>(cell1);

      if (blockCell) {
        if (blockCell->subType() == CellSubType::TRACK) {
          if (visited_.find(blockCell) == visited_.end())
            return cell1;
        }
      }
    }
  }

  return nullptr;
}

//-----------

CFieldRunners::Blimp::
Blimp(CFieldRunners *fieldRunners) :
 RunnerCell(fieldRunners)
{
  setHealth(getMaxHealth());
}

ImageId
CFieldRunners::Blimp::
getFrameImage(int /*frame*/)
{
  return -1;
}

CFieldRunners::FieldCell *
CFieldRunners::Blimp::
searchNext() const
{
  FieldCell *cell;

  fieldRunners()->getCell(goal_, &cell);

  return cell;
}

//-----------

CFieldRunners::FieldCell::
FieldCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 Cell(fieldRunners, pos), Node(pos)
{
}

//-----------

CFieldRunners::Cell::
Cell(CFieldRunners *fieldRunners, const CellPos &pos) :
 fieldRunners_(fieldRunners), pos_(pos)
{
}

CFieldRunnersWindow *
CFieldRunners::Cell::
getWindow() const
{
  return fieldRunners()->getWindow();
}

//-----------

CFieldRunners::WeaponCell::
WeaponCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 FieldCell(fieldRunners, pos)
{
}

CFieldRunners::Orient
CFieldRunners::WeaponCell::
deltaToOrient4(int dx, int dy) const
{
  Orient orient = ORIENT_NONE;

  if (abs(dx) >= abs(dy)) {
    if (dx > 0) orient = ORIENT_E;
    else        orient = ORIENT_W;
  }
  else {
    if (dy > 0) orient = ORIENT_N;
    else        orient = ORIENT_S;
  }

  return orient;
}

CFieldRunners::Orient
CFieldRunners::WeaponCell::
deltaToOrient8(int dx, int dy) const
{
  Orient orient = ORIENT_NONE;

  if      (dx < 0) {
    if      (dy > 0) orient = ORIENT_NW;
    else if (dy < 0) orient = ORIENT_SW;
    else             orient = ORIENT_W;
  }
  else if (dx > 0) {
    if      (dy > 0) orient = ORIENT_NE;
    else if (dy < 0) orient = ORIENT_SE;
    else             orient = ORIENT_E;
  }
  else {
    if (dy > 0) orient = ORIENT_N;
    if (dy < 0) orient = ORIENT_S;
  }

  return orient;
}

void
CFieldRunners::WeaponCell::
setNewOrient(Orient newOrient)
{
  int orient1 = int(orient());
  int orient2 = int(orient());

  while (orient1 != newOrient && orient2 != newOrient) {
    orient1 = orient1 + 1; if (orient1 > 7) orient1 = 0;
    orient2 = orient2 - 1; if (orient2 < 0) orient2 = 7;
  }

  if (orient1 == newOrient)
    setOrient(Orient(int(orient()) < 7 ? int(orient()) + 1 : 0));
  else
    setOrient(Orient(int(orient()) > 0 ? int(orient()) - 1 : 7));
}

//-----------

#include "images/gun1_png.h"
#include "images/gun2_png.h"
#include "images/gun3_png.h"
#include "images/gun4_png.h"
#include "images/gun5_png.h"
#include "images/gun6_png.h"
#include "images/gun7_png.h"
#include "images/gun8_png.h"

ImageId CFieldRunners::GunCell::images_[8];
bool    CFieldRunners::GunCell::imagesLoaded_;

CFieldRunners::GunCell::
GunCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
  setOrient(ORIENT_E);

  if (! imagesLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    images_[0] = window->loadImage(gun1_data, GUN1_DATA_LEN);
    images_[1] = window->loadImage(gun2_data, GUN2_DATA_LEN);
    images_[2] = window->loadImage(gun3_data, GUN3_DATA_LEN);
    images_[3] = window->loadImage(gun4_data, GUN4_DATA_LEN);
    images_[4] = window->loadImage(gun5_data, GUN5_DATA_LEN);
    images_[5] = window->loadImage(gun6_data, GUN6_DATA_LEN);
    images_[6] = window->loadImage(gun7_data, GUN7_DATA_LEN);
    images_[7] = window->loadImage(gun8_data, GUN8_DATA_LEN);

    imagesLoaded_ = true;
  }
}

void
CFieldRunners::GunCell::
update()
{
  // find nearest enemy and if close enough rotate 1 turn to face
  uint dist; int dx, dy;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist, dx, dy);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  auto newOrient = deltaToOrient8(dx, dy);
  if (newOrient == ORIENT_NONE) return;

  if (orient() != newOrient)
    setNewOrient(newOrient);
  else
    minRunner->damage(getDamage());
}

void
CFieldRunners::GunCell::
draw()
{
  BBox bbox;
  getBBox(bbox);

  getWindow()->drawCellImage(bbox.xmin, bbox.ymin, images_[orient()]);
}

//-----------

#include "images/glue1_png.h"
#include "images/glue2_png.h"
#include "images/glue3_png.h"
#include "images/glue4_png.h"
#include "images/glue5_png.h"
#include "images/glue6_png.h"
#include "images/glue7_png.h"
#include "images/glue8_png.h"

ImageId CFieldRunners::GlueCell::images_[8];
bool    CFieldRunners::GlueCell::imagesLoaded_;

CFieldRunners::GlueCell::
GlueCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
  setOrient(ORIENT_E);

  if (! imagesLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    images_[0] = window->loadImage(glue1_data, GLUE1_DATA_LEN);
    images_[1] = window->loadImage(glue2_data, GLUE2_DATA_LEN);
    images_[2] = window->loadImage(glue3_data, GLUE3_DATA_LEN);
    images_[3] = window->loadImage(glue4_data, GLUE4_DATA_LEN);
    images_[4] = window->loadImage(glue5_data, GLUE5_DATA_LEN);
    images_[5] = window->loadImage(glue6_data, GLUE6_DATA_LEN);
    images_[6] = window->loadImage(glue7_data, GLUE7_DATA_LEN);
    images_[7] = window->loadImage(glue8_data, GLUE8_DATA_LEN);

    imagesLoaded_ = true;
  }
}

void
CFieldRunners::GlueCell::
update()
{
  // find nearest enemy and if close enough rotate 1 turn to face
  uint dist; int dx, dy;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist, dx, dy);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  auto newOrient = deltaToOrient8(dx, dy);
  if (newOrient == ORIENT_NONE) return;

  if (orient() != newOrient)
    setNewOrient(newOrient);
  else
    minRunner->slowDown(60);
}

void
CFieldRunners::GlueCell::
draw()
{
  getWindow()->drawCellImage(bbox_.xmin, bbox_.ymin, images_[orient()]);
}

//-----------

CFieldRunners::SnowbombCell::
SnowbombCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
}

void
CFieldRunners::SnowbombCell::
update()
{
}

void
CFieldRunners::SnowbombCell::
draw()
{
}

//-----------

#include "images/missile1_png.h"
#include "images/missile2_png.h"
#include "images/missile3_png.h"
#include "images/missile4_png.h"
#include "images/missile5_png.h"
#include "images/missile6_png.h"
#include "images/missile7_png.h"
#include "images/missile8_png.h"

ImageId CFieldRunners::MissileCell::images_[8];
bool    CFieldRunners::MissileCell::imagesLoaded_;

CFieldRunners::MissileCell::
MissileCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
  setOrient(ORIENT_E);

  if (! imagesLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    images_[0] = window->loadImage(missile1_data, MISSILE1_DATA_LEN);
    images_[1] = window->loadImage(missile2_data, MISSILE2_DATA_LEN);
    images_[2] = window->loadImage(missile3_data, MISSILE3_DATA_LEN);
    images_[3] = window->loadImage(missile4_data, MISSILE4_DATA_LEN);
    images_[4] = window->loadImage(missile5_data, MISSILE5_DATA_LEN);
    images_[5] = window->loadImage(missile6_data, MISSILE6_DATA_LEN);
    images_[6] = window->loadImage(missile7_data, MISSILE7_DATA_LEN);
    images_[7] = window->loadImage(missile8_data, MISSILE8_DATA_LEN);

    imagesLoaded_ = true;
  }
}

void
CFieldRunners::MissileCell::
update()
{
  // find nearest enemy and if close enough rotate 1 turn to face
  uint dist; int dx, dy;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist, dx, dy);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  auto newOrient = deltaToOrient8(dx, dy);
  if (newOrient == ORIENT_NONE) return;

  if (orient() != newOrient) {
    setNewOrient(newOrient);
  }
  else {
    if (reload() == 0) {
      Point point;

      fieldRunners()->posToPixel(loc, 0.5, 0.5, point);

      fieldRunners()->emitMissileBullet(point, minRunner, orient());

      setReload(12);
    }
  }

  if (reload() > 0)
    setReload(reload() - 1);
}

void
CFieldRunners::MissileCell::
draw()
{
  getWindow()->drawCellImage(bbox_.xmin, bbox_.ymin, images_[orient()]);
}

//-----------

CFieldRunners::ShotgunCell::
ShotgunCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
}

void
CFieldRunners::ShotgunCell::
update()
{
  // find nearest enemy
  uint dist;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  minRunner->damage(getDamage());
}

void
CFieldRunners::ShotgunCell::
draw()
{
}

//-----------

#include "images/zap_png.h"

ImageId CFieldRunners::ZapCell::image_;
bool    CFieldRunners::ZapCell::imageLoaded_;

CFieldRunners::ZapCell::
ZapCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
  if (! imageLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    image_ = window->loadImage(zap_data, ZAP_DATA_LEN);

    imageLoaded_ = true;
  }
}

void
CFieldRunners::ZapCell::
update()
{
  // find nearest enemy
  uint dist;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  minRunner->damage(getDamage());
}

void
CFieldRunners::ZapCell::
draw()
{
  getWindow()->drawCellImage(bbox_.xmin, bbox_.ymin, image_);
}

//-----------

CFieldRunners::PulseCell::
PulseCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
}

void
CFieldRunners::PulseCell::
update()
{
  // find nearest enemy
  uint dist; int dx, dy;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist, dx, dy);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  // must be save row/column
  if (dx != 0 && dy != 0)
    return;

  if (reload() == 0) {
    Point point;

    fieldRunners()->posToPixel(loc, 0.5, 0.5, point);

    fieldRunners()->emitPulseBullet(point, ORIENT_N);
    fieldRunners()->emitPulseBullet(point, ORIENT_S);
    fieldRunners()->emitPulseBullet(point, ORIENT_E);
    fieldRunners()->emitPulseBullet(point, ORIENT_W);

    setReload(12);
  }

  if (reload() > 0)
    setReload(reload() - 1);
}

void
CFieldRunners::PulseCell::
draw()
{
}

//-----------

CFieldRunners::LaserCell::
LaserCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
  setOrient(ORIENT_E);
}

void
CFieldRunners::LaserCell::
update()
{
  // find nearest enemy
  uint dist; int dx, dy;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist, dx, dy);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  // must be save row/column
  if (dx != 0 && dy != 0)
    return;

  auto newOrient = deltaToOrient4(dx, dy);
  if (newOrient == ORIENT_NONE) return;

  if (orient() != newOrient) {
    setNewOrient(newOrient);
  }
  else {
    if (reload() == 0) {
      Point point;

      fieldRunners()->posToPixel(loc, 0.5, 0.5, point);

      fieldRunners()->emitLaserBullet(point, orient());

      setReload(30);
    }
  }

  if (reload() > 0)
    setReload(reload() - 1);
}

void
CFieldRunners::LaserCell::
draw()
{
}

//-----------

CFieldRunners::FirebombCell::
FirebombCell(CFieldRunners *fieldRunners, const CellPos &pos) :
 WeaponCell(fieldRunners, pos)
{
}

void
CFieldRunners::FirebombCell::
update()
{
  // find nearest enemy
  uint dist;
  auto *minRunner = fieldRunners()->nearestRunner(loc, dist);
  if (! minRunner) return;

  // too far away
  if (dist > getRange())
    return;

  minRunner->damage(getDamage());
}

void
CFieldRunners::FirebombCell::
draw()
{
}

//-----------

CFieldRunners::Bullet::
Bullet(CFieldRunners *fieldRunners, const Point &point) :
 fieldRunners_(fieldRunners), point_(point)
{
  static uint s_bullet_id;

  id_ = ++s_bullet_id;
}

CFieldRunnersWindow *
CFieldRunners::Bullet::
getWindow() const
{
  return fieldRunners()->getWindow();
}

//-----------

#include "images/rocket1_png.h"
#include "images/rocket2_png.h"
#include "images/rocket3_png.h"
#include "images/rocket4_png.h"
#include "images/rocket5_png.h"
#include "images/rocket6_png.h"
#include "images/rocket7_png.h"
#include "images/rocket8_png.h"

ImageId CFieldRunners::MissileBullet::images_[8];
bool    CFieldRunners::MissileBullet::imagesLoaded_;

CFieldRunners::MissileBullet::
MissileBullet(CFieldRunners *fieldRunners, const Point &point, RunnerCell *runner, Orient orient) :
 Bullet(fieldRunners, point), runner_(runner), orient_(orient)
{
  if (! imagesLoaded_) {
    auto *window = this->fieldRunners()->getWindow();

    images_[0] = window->loadImage(rocket1_data, ROCKET1_DATA_LEN);
    images_[1] = window->loadImage(rocket2_data, ROCKET2_DATA_LEN);
    images_[2] = window->loadImage(rocket3_data, ROCKET3_DATA_LEN);
    images_[3] = window->loadImage(rocket4_data, ROCKET4_DATA_LEN);
    images_[4] = window->loadImage(rocket5_data, ROCKET5_DATA_LEN);
    images_[5] = window->loadImage(rocket6_data, ROCKET6_DATA_LEN);
    images_[6] = window->loadImage(rocket7_data, ROCKET7_DATA_LEN);
    images_[7] = window->loadImage(rocket8_data, ROCKET8_DATA_LEN);

    imagesLoaded_ = true;
  }
}

bool
CFieldRunners::MissileBullet::
isDone() const
{
  return (isAtGoal() || runner_->isDone());
}

void
CFieldRunners::MissileBullet::
update()
{
  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  const auto &pos = runner_->getPos();

  Point point;
  fieldRunners()->posToPixel(pos, point);

  point.x += cellSize.width /2;
  point.y += cellSize.height/2;

  int dx = (point.x - point_.x);
  int dy = (point.y - point_.y);

  int dhit = cellSize.width/2;

  if (abs(dx) < dhit && abs(dy) < dhit) {
    setAtGoal(true);

    runner_->bulletDamage(id(), getDamage());

    return;
  }

  double angle = atan2(dy, dx);

  double da = M_PI/16.0;

  Orient newOrient;

  if      (angle >  7*da) newOrient = ORIENT_W;
  else if (angle >  5*da) newOrient = ORIENT_SW;
  else if (angle >  3*da) newOrient = ORIENT_S;
  else if (angle >    da) newOrient = ORIENT_SE;
  else if (angle < -7*da) newOrient = ORIENT_W;
  else if (angle < -5*da) newOrient = ORIENT_NW;
  else if (angle < -3*da) newOrient = ORIENT_N;
  else if (angle <   -da) newOrient = ORIENT_NE;
  else                    newOrient = ORIENT_E;

  if (orient() != newOrient)
    setNewOrient(newOrient);

  int d = int(0.3*cellSize.width);

  switch (orient()) {
    case ORIENT_E : {                point_.x += d; break; }
    case ORIENT_NE: { point_.y -= d; point_.x += d; break; }
    case ORIENT_N : { point_.y -= d;                break; }
    case ORIENT_NW: { point_.y -= d; point_.x -= d; break; }
    case ORIENT_W : {                point_.x -= d; break; }
    case ORIENT_SW: { point_.y += d; point_.x -= d; break; }
    case ORIENT_S : { point_.y += d;                break; }
    case ORIENT_SE: { point_.y += d; point_.x += d; break; }
    default       : {                               break; }
  }
}

void
CFieldRunners::MissileBullet::
setNewOrient(Orient newOrient)
{
  int orient1 = int(orient());
  int orient2 = int(orient());

  while (orient1 != newOrient && orient2 != newOrient) {
    orient1 = orient1 + 1; if (orient1 > 7) orient1 = 0;
    orient2 = orient2 - 1; if (orient2 < 0) orient2 = 7;
  }

  if (orient1 == newOrient) setOrient(Orient(int(orient()) < 7 ? int(orient()) + 1 : 0));
  else                      setOrient(Orient(int(orient()) > 0 ? int(orient()) - 1 : 7));
}

void
CFieldRunners::MissileBullet::
draw()
{
  getWindow()->drawImage(point_.x, point_.y, images_[orient()]);
}

//-----------

CFieldRunners::PulseBullet::
PulseBullet(CFieldRunners *fieldRunners, const Point &point, Orient orient) :
 Bullet(fieldRunners, point), orient_(orient)
{
}

void
CFieldRunners::PulseBullet::
update()
{
  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  int d = int(0.3*cellSize.width);

  if      (orient() == ORIENT_N) point_.y += d;
  else if (orient() == ORIENT_S) point_.y -= d;
  else if (orient() == ORIENT_E) point_.x += d;
  else if (orient() == ORIENT_W) point_.x -= d;

  CellPos cellPos;
  fieldRunners()->pixelToPos(point_, cellPos);

  if (fieldRunners()->isInside(cellPos)) {
    CFieldRunners::RunnerArray runners;
    fieldRunners()->getCellRunners(cellPos, runners);

    for (auto *runner : runners)
      runner->bulletDamage(id(), getDamage());
  }
  else {
    done_ = true;
  }
}

void
CFieldRunners::PulseBullet::
draw()
{
}

//-----------

CFieldRunners::LaserBullet::
LaserBullet(CFieldRunners *fieldRunners, const Point &point, Orient orient) :
 Bullet(fieldRunners, point), orient_(orient)
{
  life_ = initLife();
}

void
CFieldRunners::LaserBullet::
update()
{
  if (life_ > 0)
    --life_;

  Size cellSize;
  fieldRunners()->getCellSize(cellSize);

  int dx = 0, dy = 0;

  if      (orient() == ORIENT_N) dy = -1;
  else if (orient() == ORIENT_S) dy =  1;
  else if (orient() == ORIENT_E) dx =  1;
  else if (orient() == ORIENT_W) dx = -1;

  CellPos cellPos;
  fieldRunners()->pixelToPos(point_, cellPos);

  auto cellPos1 = CellPos(cellPos.row + dy, cellPos.col + dx);

  while (fieldRunners()->isInside(cellPos1)) {
    CFieldRunners::RunnerArray runners;
    fieldRunners()->getCellRunners(cellPos1, runners);

    for (auto *runner : runners)
      runner->bulletDamage(id(), getDamage());

    cellPos1 = CellPos(cellPos1.row + dy, cellPos1.col + dx);
  }
}

void
CFieldRunners::LaserBullet::
draw()
{
}

//-----------

CFieldRunners::SearchField::
SearchField(CFieldRunners *fieldRunners) :
 fieldRunners_(fieldRunners)
{
}

double
CFieldRunners::SearchField::
pathCostEstimate(const CellPos &startLoc, const CellPos &endLoc)
{
  double dx = abs(endLoc.col - startLoc.col);
  double dy = abs(endLoc.row - startLoc.row);

  return dx + dy;
}

double
CFieldRunners::SearchField::
traverseCost(const CellPos &loc, const CellPos &newLoc)
{
  double dx = abs(loc.col - newLoc.col);
  double dy = abs(loc.row - newLoc.row);

  return dx + dy;
}

CFieldRunners::SearchField::NodeList
CFieldRunners::SearchField::
getNextNodes(Node *node) const
{
  NodeList nodes;

  auto addEmptyCell = [&](const CellPos &pos) {
    CFieldRunners::FieldCell *cell;
    fieldRunners()->getCell(pos, &cell);

    if (cell && ! cell->isSolid())
      nodes.push_back(cell);
  };

  //---

  auto &loc = node->loc;

  if (loc.col > 0)
    addEmptyCell(CellPos(loc.row, loc.col - 1));

  if (loc.row > 0)
    addEmptyCell(CellPos(loc.row - 1, loc.col));

  if (loc.row < fieldRunners()->getNumRows() - 1)
    addEmptyCell(CellPos(loc.row + 1, loc.col));

  if (loc.col < fieldRunners()->getNumCols() - 1)
    addEmptyCell(CellPos(loc.row, loc.col + 1));

  return nodes;
}

CFieldRunners::SearchField::Node *
CFieldRunners::SearchField::
lookupNode(const CellPos &loc) const
{
  CFieldRunners::FieldCell *cell;
  fieldRunners()->getCell(loc, &cell);
  return cell;
}

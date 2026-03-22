#ifndef FieldRunners_H
#define FieldRunners_H

#include <CFieldRunners.h>

namespace CQGLFieldRunners {

class Canvas;

class FieldRunners : public CFieldRunners {
 public:
  FieldRunners(Canvas *canvas);

  const CellPos &currentCell() const { return currentCell_; }
  void setCurrentCell(const CellPos &v) { currentCell_ = v; }

  Tank *createTank() override;

  void stateChanged() override;

 private:
  Canvas* canvas_ { nullptr };

  CellPos currentCell_;
};

}

#endif

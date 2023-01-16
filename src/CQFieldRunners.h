#include <QFrame>
#include <CQCanvas2D.h>

class QLabel;
class QPainter;
class QPainterPath;
class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;
class QTimer;

class CQFieldRunners;
class CQFieldRunnersCanvas;
class CQFieldRunnersWindow;

class Color;

class CQFieldRunnersApp : public QFrame {
  Q_OBJECT

 public:
  struct Pos {
    Pos() { }

    Pos(int row_, int col_) :
     row(row_), col(col_) {
    }

    int row { 0 };
    int col { 0 };

    friend bool operator==(const Pos &lhs, const Pos &rhs) {
      return (lhs.row == rhs.row && lhs.col == rhs.col);
    }

    friend bool operator!=(const Pos &lhs, const Pos &rhs) {
      return ! operator==(lhs, rhs);
    }
  };

 public:
  CQFieldRunnersApp();
 ~CQFieldRunnersApp();

  CQFieldRunners *fieldRunners() const { return fieldRunners_; }

  void draw(QPainter *painter);

  const Pos &currentPos() const { return currentPos_; }
  void setCurrentPos(const Pos &pos);

  void setStatus(const QString &text);

  void mousePress(QMouseEvent *event);
  void keyPress(QKeyEvent *event);

 private slots:
  void timerSlot();

 private:
  CQFieldRunnersCanvas*  canvas_       { nullptr };
  CQFieldRunnersWindow*  window_       { nullptr };
  CQFieldRunners*        fieldRunners_ { nullptr };
  QFrame*                status_       { nullptr };
  QLabel*                statusLabel_  { nullptr };
  QTimer*                timer_;
  Pos                    currentPos_;
};

class CQFieldRunnersCanvas : public CQCanvas2D {
  Q_OBJECT

 public:
  CQFieldRunnersCanvas(CQFieldRunnersApp *fieldRunners);
 ~CQFieldRunnersCanvas();

  CQFieldRunnersApp *app() const { return app_; }

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent (QMouseEvent *event) override;

  void keyPressEvent(QKeyEvent *event) override;

  void fill(QColor c);

  void setForeground(QColor fg);
  void setBackground(QColor fg);

  void drawPoint(int x, int y);

  void drawLine(int x1, int y1, int x2, int y2);

  void drawRectangle(int x1, int y1, int x2, int y2);

  void fillRectangle(int x1, int y1, int x2, int y2);

  void drawEllipse(int x1, int y1, int x2, int y2);

  void drawPath(const QPainterPath &path);

  void drawImage(int x, int y, const QImage &image);

  void setFont(const QFont &font);

  void drawContrastText(int x, int y, const QString &text, const QColor &bg, const QColor &fg);

  void drawText(int x, int y, const QString &text);
  void drawText(int x, int y, const char *text);

  QColor mapColor(const Color &color) const;

 private:
  CQFieldRunnersApp *app_ { nullptr };
};

#ifndef ParticleBuffer_H
#define ParticleBuffer_H

#include <CQGLUtil.h>

namespace CQGLFieldRunners {

class ParticleBuffer {
 public:
  struct Point {
    float x { 0.0 };
    float y { 0.0 };
    float z { 0.0 };

    Point() { }

    Point(float x, float y, float z) :
     x(x), y(y), z(z) {
    }
  };

  struct Color {
    float r { 0.0 };
    float g { 0.0 };
    float b { 0.0 };

    Color() { }

    Color(float r, float g, float b) :
     r(r), g(g), b(b) {
    }
  };

  using Points = std::vector<Point>;
  using Colors = std::vector<Color>;

 public:
  ParticleBuffer(QOpenGLShaderProgram *program=nullptr) {
    program_ = program;
  }

  const QOpenGLExtraFunctions *functions() const { return fn_; }
  void setFunctions(QOpenGLExtraFunctions *p) { fn_ = p; }

  double particleSize() const { return particleSize_; }
  void setParticleSize(double r) { particleSize_ = r; }

  const CQGLTexture *texture() const { return texture_; }
  void setTexture(CQGLTexture *p) { texture_ = p; }

  void init() {
    if (initialized_)
      return;

    initialized_ = true;

    // get program variables
    positionAttr_ = program_->attributeLocation("position");
    assert(positionAttr_ != -1);

    centerAttr_ = program_->attributeLocation("center");
    assert(centerAttr_ != -1);

    colorAttr_ = program_->attributeLocation("color");
    assert(colorAttr_ != -1);

    //---

    addParticleShape();

    //---

    // The VBO containing the positions and sizes of the particles
    fn_->glGenBuffers(1, &particlesPositionBuffer_);
    fn_->glBindBuffer(GL_ARRAY_BUFFER, particlesPositionBuffer_);
    // Initialize with empty (null) buffer : it will be updated later, each frame.
    fn_->glBufferData(GL_ARRAY_BUFFER, s_maxPoints*sizeof(Point),
                      nullptr, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    fn_->glGenBuffers(1, &particlesColorBuffer_);
    fn_->glBindBuffer(GL_ARRAY_BUFFER, particlesColorBuffer_);
    // Initialize with empty (null) buffer : it will be updated later, each frame.
    fn_->glBufferData(GL_ARRAY_BUFFER, s_maxPoints*sizeof(Color),
                      nullptr, GL_STREAM_DRAW);
  }

  void addParticleShape() {
    // static geometry for all particles
    static const GLfloat s_vertex_buffer_data[] = {
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
     -0.5f,  0.5f, 0.0f,
      0.5f,  0.5f, 0.0f,
    };

    fn_->glGenBuffers(1, &billboardVertexBuffer_);
    fn_->glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer_);
    fn_->glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertex_buffer_data),
                      s_vertex_buffer_data, GL_STATIC_DRAW);
  }

  void clear() {
    points_.clear();
    colors_.clear();
  }

  void addPoint(const CPoint3D &p) {
    addPoint(Point(p.x, p.y, p.z));
  }

  void addPoint(const Point &p) {
    points_.push_back(p);
  }

  void addColor(const CRGBA &c) {
    addColor(Color(c.getRed(), c.getGreen(), c.getBlue()));
  }

  void addColor(const Color &c) {
    colors_.push_back(c);
  }

  void draw(Camera *camera) {
    glDisable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);

    //---

    program_->bind();

    //---

    // camera projection
    program_->setUniformValue("projection", CQGLUtil::toQMatrix(camera->perspectiveMatrix()));

    // camera/view transformation
    program_->setUniformValue("view", CQGLUtil::toQMatrix(camera->viewMatrix()));

    program_->setUniformValue("model", CQGLUtil::toQMatrix(CMatrix3DH::identity()));

    program_->setUniformValue("cameraUp"   , CQGLUtil::toVector(camera->up  ()));
    program_->setUniformValue("cameraFront", CQGLUtil::toVector(camera->front()));
    program_->setUniformValue("cameraRight", CQGLUtil::toVector(camera->right()));

    program_->setUniformValue("particleSize", float(particleSize()));

    bool useTexture = !!texture_;

    program_->setUniformValue("useTexture", useTexture);
    program_->setUniformValue("textureId", 0);

    if (useTexture) {
      glActiveTexture(GL_TEXTURE0);
      texture_->bind();
    }

    //---

    auto n = points_.size();
    assert(colors_.size() == n);

    // Update the buffers that OpenGL uses for rendering.
    // There are much more sophisticated means to stream data from the CPU to the GPU,
    // but this is outside the scope of this tutorial.
    // http://www.opengl.org/wiki/Buffer_Object_Streaming
    fn_->glBindBuffer(GL_ARRAY_BUFFER, particlesPositionBuffer_);
    // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    fn_->glBufferData(GL_ARRAY_BUFFER, s_maxPoints*sizeof(Point), nullptr, GL_STREAM_DRAW);
    fn_->glBufferSubData(GL_ARRAY_BUFFER, 0, n*sizeof(Point), &points_[0]);

    fn_->glBindBuffer(GL_ARRAY_BUFFER, particlesColorBuffer_);
    // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    fn_->glBufferData(GL_ARRAY_BUFFER, s_maxPoints*sizeof(Color), nullptr, GL_STREAM_DRAW);
    fn_->glBufferSubData(GL_ARRAY_BUFFER, 0, n*sizeof(Color), &colors_[0]);

    // 1st attribute buffer : vertices
    fn_->glEnableVertexAttribArray(positionAttr_);
    fn_->glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer_);
    fn_->glVertexAttribPointer(
     positionAttr_,
     3,
     GL_FLOAT, // type
     GL_FALSE, // normalized?
     0,
     nullptr
    );

    // 2nd attribute buffer : positions of particles' centers
    fn_->glEnableVertexAttribArray(centerAttr_);
    fn_->glBindBuffer(GL_ARRAY_BUFFER, particlesPositionBuffer_);
    fn_->glVertexAttribPointer(
     centerAttr_,
     3,
     GL_FLOAT, // type
     GL_FALSE, // normalized?
     0,
     nullptr
    );

    // 3rd attribute buffer : particles' colors
    fn_->glEnableVertexAttribArray(colorAttr_);
    fn_->glBindBuffer(GL_ARRAY_BUFFER, particlesColorBuffer_);
    fn_->glVertexAttribPointer(
     colorAttr_,
     4,
     GL_FLOAT, // type
     GL_FALSE, // normalized?
     0,
     nullptr
    );

    fn_->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n);

    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when
    // rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml

    // particles vertices : always reuse the same 4 vertices -> 0
    fn_->glVertexAttribDivisor(positionAttr_, 0);
    // center per quad -> 1
    fn_->glVertexAttribDivisor(centerAttr_, 1);
    // color per quad -> 1
    fn_->glVertexAttribDivisor(colorAttr_, 1);

    // Draw the particules !
    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to :
    // for (i in n) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.
    fn_->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n);

    fn_->glBindBuffer(GL_ARRAY_BUFFER, 0);

    //---

    program_->release();

    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    glDepthMask(GL_TRUE);
  }

 private:
  /*static*/ size_t s_maxPoints { 50000 };

  QOpenGLShaderProgram *program_ { nullptr };

  QOpenGLExtraFunctions* fn_ { nullptr };

  bool initialized_ { false };

  GLint positionAttr_ { 0 };
  GLint centerAttr_   { 0 };
  GLint colorAttr_    { 0 };

  GLuint billboardVertexBuffer_   { 0 };
  GLuint particlesPositionBuffer_ { 0 };
  GLuint particlesColorBuffer_    { 0 };

  Points points_;
  Colors colors_;

  double particleSize_ { 0.3 };

  CQGLTexture* texture_ { nullptr };
};

}

#endif

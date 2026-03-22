#version 330 core

attribute highp vec3 position; // billboard points (xy) (1x1 centered at (0,0))
attribute highp vec3 center;   // particle position
attribute lowp  vec4 color;

uniform highp mat4 projection;
uniform highp mat4 view;
uniform highp mat4 model;

uniform highp vec3 cameraUp;
uniform highp vec3 cameraFront;
uniform highp vec3 cameraRight;

uniform highp float particleSize;

varying lowp  vec4 col;
varying highp vec2 texPos;

void main() {
  col = color;

  texPos = position.xy + vec2(0.5);

  // adjust x position to left (health > 0) or right (health < 0)
  float health = center.y;

  float px;
  if (health > 0) {
    px = health*((position.x + 1)/2); // 0 - health

    col = vec4(0, 1, 0, 1);
  } else {
    px = health*(1 - (position.x + 1)/2); // -health - 0 

    col = vec4(1, 0, 0, 1);
  }

  float xs = 0.25;
  float ys = 0.10;

  vec3 position1;

  vec3 right = vec3(1, 0, 0);
  vec3 front = vec3(0, 0, 1);
  vec3 up    = vec3(0, 1, 0);

  // adjust billboard to particle size oriented to camera
  //position1 = (cameraRight*px*particleSize) + (cameraFront*position.y*particleSize);
  position1 = (right*px*xs) + (up*position.y*ys);

  gl_Position = projection*view*model*vec4(vec3(center.x, 1.5, center.z) + position1, 1);
}

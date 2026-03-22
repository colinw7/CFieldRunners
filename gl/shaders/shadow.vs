#version 330 core

attribute highp vec3 position;
attribute highp vec3 center;
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

//vec3 position1 = (cameraRight*position.x*particleSize) + (cameraUp*position.y*particleSize);
  vec3 position1 = vec3(particleSize*position.x, 0, particleSize*position.y);

//gl_Position = (projection*view*model*center) + vec4(position1, 1);
//gl_Position = projection*view*model*center + particleSize*position;
  gl_Position = projection*view*model*vec4(center + position1, 1);
}

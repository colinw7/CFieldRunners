#version 330 core

attribute highp vec3 position; // particle square (-0.5 -> 0.5)
attribute highp vec3 center;   // particle position
attribute lowp  vec4 color;    // particle color

uniform highp mat4 projection;
uniform highp mat4 view;
uniform highp mat4 model;

uniform highp float particleSize;

varying lowp  vec4 col;
varying highp vec2 texPos;

void main() {
  col = color;

  texPos = position.xy + vec2(0.5); // 0-1

  vec3 position1 = vec3(center.y*position.x, 0, center.y*position.y);

  gl_Position = projection*view*model*vec4(vec3(center.x, 0.25, center.z) + position1, 1);
}

#version 330 core

varying lowp  vec4 col;
varying highp vec2 texPos;

uniform bool useTexture;
uniform sampler2D textureId;

void main() {
  float a = 0.0;
  if (useTexture) {
    vec4 tc = texture(textureId, texPos);
    a = tc.a;
  } else {
    // circle
    if (length(texPos - vec2(0.5, 0.5)) > 0.5) {
      a = 1.0;
    }
  }
  gl_FragColor = vec4(0,0,0,0.4*a);
}

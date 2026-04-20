#version 330 core

in vec3 FragPos;
in vec3 Color;
in vec2 TexCoords;

struct TextureData {
  bool      enabled;
  sampler2D texture;
};

uniform TextureData diffuseTexture;

uniform bool isWireframe;
uniform bool enabled;

void main() {
  if (! isWireframe) {
    float a = 1.0;

    if (! enabled) {
      a = 0.3;
    }

    if (diffuseTexture.enabled) {
      vec4 tc = texture(diffuseTexture.texture, TexCoords);

      if (tc.a < 0.1)
        discard;

      tc.a = tc.a*a;

      gl_FragColor = vec4(tc.a*tc.r, tc.a*tc.g, tc.a*tc.b, tc);
    }
    else {
      gl_FragColor = vec4(Color, a);
    }
  } else {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
}

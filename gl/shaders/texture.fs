#version 330 core

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D textureId;

const float offset = 1.0/300.0;

void main() {
  vec4 tc = texture(textureId, TexCoords);

  if (FragPos.y < -0.33 || FragPos.y > 0.33) {
    vec2 offsets[9] = vec2[](
      vec2(-offset,  offset), // top-left
      vec2( 0.0f,    offset), // top-center
      vec2( offset,  offset), // top-right
      vec2(-offset,  0.0f  ), // center-left
      vec2( 0.0f,    0.0f  ), // center-center
      vec2( offset,  0.0f  ), // center - right
      vec2(-offset, -offset), // bottom-left
      vec2( 0.0f,   -offset), // bottom-center
      vec2( offset, -offset)  // bottom-right    
    );

    /*
    float kernel[9] = float[](
      -1.0f, -2.0f, -1.0f,
      -1.0f,  9.0f, -1.0f,
      -1.0f, -2.0f, -1.0f
    );
    */
    float kernel[9] = float[](
      1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f,
      2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f,
      1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
      sampleTex[i] = vec3(texture(textureId, TexCoords + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
      col += sampleTex[i]*kernel[i];
    }

    float f = 1.0;

    if (FragPos.y < -0.33)
      f = (FragPos.y + 1.0)/0.66;
    else
      f = (1.0 - FragPos.y)/0.66;

    f = pow(f, 4);

    //gl_FragColor = vec4(f, f, f, 1);
    gl_FragColor = mix(vec4(col, 1.0), tc, f);
  }
  else {
    gl_FragColor = tc;
  }
}

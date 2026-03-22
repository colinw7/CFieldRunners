#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Color;
out vec2 TexCoords;

uniform mat4 model;

void main() {
  vec3 position = aPos;

  FragPos = vec3(model*vec4(position, 1.0));

  Color     = aColor;
  TexCoords = aTexCoords;

  gl_Position = vec4(FragPos, 1.0);
}

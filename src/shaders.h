#ifndef TRANSFORMER_SHADERS_H
#define TRANSFORMER_SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

const GLchar* vertexShaderSource =
"#version 330 core\n"
"        layout(location = 0) in vec3 vertexPosition;"
"layout(location = 1) in vec3 vertexNormal;"
"out vec3 Normal;"
"out vec3 FragPos;"
"uniform mat4 transformationMatrix;"
"uniform mat4 view;"
"uniform mat4 projection;"
"void main() {"
"    FragPos = vec3(transformationMatrix * vec4(vertexPosition, 1.0));"
"    Normal = mat3(transformationMatrix) * vertexNormal;"
"    gl_Position = projection * view * vec4(FragPos, 1.0);"
"}";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"        in vec3 Normal;"
"in vec3 FragPos;"
"out vec4 FragColor;"
"uniform vec3 lightPos;"
"uniform vec3 viewPos;"
"uniform vec3 lightColor;"
"void main() {"
"    float intensity = dot(normalize(Normal), normalize(lightPos - FragPos));"
"    float quantized = floor(intensity * 4.0) / 4.0;"
"    FragColor = vec4(lightColor * quantized, 1.0);"
"}";


#endif //TRANSFORMER_SHADERS_H

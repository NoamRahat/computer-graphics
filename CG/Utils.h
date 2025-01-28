#pragma once

#include <string>
#include <glm/glm.hpp>

void ConvertQuaternionToMatrix(const double quat[4], double mat[16]);
void displayMessage(const std::string& str);
std::wstring getOpenFileName();
glm::vec3 toScreenSpace(const glm::vec4& vertex, int screenWidth, int screenHeight);

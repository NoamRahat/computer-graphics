#pragma once

#include <string>
#include "../Obj Parser/wavefront_obj.h"

namespace MathUtils {
    void ConvertQuaternionToMatrix(const double quat[4], double mat[16]);
}

void displayMessage(const std::string& str);

namespace FileDialogUtils {
    std::wstring getOpenFileName();
}

namespace TransformationUtils {
    void transformToScreenSpace(Wavefront_obj& obj, int screenWidth, int screenHeight);
}

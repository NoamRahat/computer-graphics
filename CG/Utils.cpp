#include "Utils.h"
#include <windows.h>
#include <iostream>

// Convert quaternion to a 4x4 rotation matrix
void MathUtils::ConvertQuaternionToMatrix(const double quat[4], double mat[16]) {
    double yy2 = 2.0 * quat[1] * quat[1];
    double xy2 = 2.0 * quat[0] * quat[1];
    double xz2 = 2.0 * quat[0] * quat[2];
    double yz2 = 2.0 * quat[1] * quat[2];
    double zz2 = 2.0 * quat[2] * quat[2];
    double wz2 = 2.0 * quat[3] * quat[2];
    double wy2 = 2.0 * quat[3] * quat[1];
    double wx2 = 2.0 * quat[3] * quat[0];
    double xx2 = 2.0 * quat[0] * quat[0];
    
    mat[0] = -yy2 - zz2 + 1.0; mat[1] = xy2 + wz2; mat[2] = xz2 - wy2; mat[3] = 0;
    mat[4] = xy2 - wz2; mat[5] = -xx2 - zz2 + 1.0; mat[6] = yz2 + wx2; mat[7] = 0;
    mat[8] = xz2 + wy2; mat[9] = yz2 - wx2; mat[10] = -xx2 - yy2 + 1.0; mat[11] = 0;
    mat[12] = mat[13] = mat[14] = 0; mat[15] = 1;
}

std::wstring FileDialogUtils::getOpenFileName() {
    OPENFILENAME ofn = { 0 };
    TCHAR fileStr[10000] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileStr;
    ofn.nMaxFile = sizeof(fileStr) / sizeof(TCHAR) - 1;

    if (GetOpenFileName(&ofn)) {
        return fileStr;
    } else {
        std::cerr << "File selection canceled or failed." << std::endl;
        return L"";
    }
}

void TransformationUtils::transformToScreenSpace(Wavefront_obj& obj, int screenWidth, int screenHeight) {
    for (auto& vertex : obj.m_points) {
        vertex[0] = (vertex[0] + 1) * (screenWidth / 2.0);
        vertex[1] = (vertex[1] + 1) * (screenHeight / 2.0);
    }
}

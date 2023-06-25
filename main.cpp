#include <Eigen/Dense>
#include <iostream>

int main() {
    // 创建3x3的浮点型矩阵
    Eigen::Matrix3f matrix;
    matrix << 1, 2, 3,
            4, 5, 6,
            7, 8, 9;

    // 输出矩阵
    std::cout << "Matrix:\n" << matrix << std::endl;

    return 0;
}
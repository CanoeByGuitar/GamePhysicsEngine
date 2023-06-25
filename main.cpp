#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <Base/Log.h>
#include <random>

int main() {
    Eigen::VectorXf v = Eigen::Vector3f::Constant(1000, 1.f);
    TICK(eigen)
    for(int k = 0; k < 100000; k++){
        v  += Eigen::Vector3f::Constant(1000, 1.f);
    }
    TOCK(eigen)

    std::vector a(1000, 1.0f);
    TICK(stdvector)
    for(int k = 0; k < 100000; k++){
        for(auto& i : a){
            i += 1.0f;
        }
    }
    TOCK(stdvector)

    return 0;
}
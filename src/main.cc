/**
 * @file main.cc
 * @author Garrett Rhoads
 * @brief main file for renderer
 * @date 2025-03-15
 */

#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include "plotter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using Eigen::MatrixXf;
using Eigen::MatrixXi;
using Eigen::JacobiSVD;
using Eigen::ComputeThinU;
using Eigen::ComputeThinV;
using Eigen::Vector3f;
using namespace std;

void grayscale(unsigned char* image, const int& width, const int& height) {
    for (int i = 0; i < (height * width * 4); i += 4) {
        int avg = 0;
        for (int j = i; j < (i + 3); j++) {
            avg += image[j];
        }
        avg /= 3;
        for (int j = i; j < (i + 3); j++) {
            image[j] = avg;
        }
    }
}

MatrixXi to_matrix(unsigned char* image, const int& width, const int& height) {
    MatrixXi m(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << "i = " << i << "    j = " << j << endl;
            cout << "index = " << 4 * ((i * width) + j) << endl;
            m(i, j) = image[4 * ((i * width) + j)];
        }
    }
    return m;
}

int main() {
    unsigned char* image;
    int width;
    int height;
    int n;
    unsigned char* data = stbi_load("test.png", &width, &height, &n, 4);
    if (data != nullptr) {
        image = new unsigned char[width * height * 4];
        image = data;
    }

    stbi_image_free(data);
    cout << width << " " << height << endl;
    grayscale(image, width, height);
    MatrixXi m = to_matrix(image, width, height);
    cout << m << endl;
    stbi_write_png("test_output.png", width, height, 4, image, width * 4);
    
    //grayscale(image, width, height);
    //write(image, width, height);
    // MatrixXf m = MatrixXf::Random(3,2);
    // cout << "Here is the matrix m:" << endl << m << endl;
    // JacobiSVD<MatrixXf> svd;
    // svd.compute(m, ComputeThinU | ComputeThinV);
    // cout << "Its singular values are:" << endl << svd.singularValues() << endl;
    // cout << "Its left singular vectors are the columns of the thin U matrix:" << endl << svd.matrixU() << endl;
    // cout << "Its right singular vectors are the columns of the thin V matrix:" << endl << svd.matrixV() << endl;
    // Vector3f rhs(1, 0, 0);
    // cout << "Now consider this rhs vector:" << endl << rhs << endl;
    // cout << "A least-squares solution of m*x = rhs is:" << endl << svd.solve(rhs) << endl;
    return 0;
}
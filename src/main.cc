/**
 * @file main.cc
 * @author Garrett Rhoads
 * @brief main file for renderer
 * @date 2025-03-15
 */

#include <iostream>
#include <vector>
#include <cmath>
#include "Eigen/Dense"
#include "plotter.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using Eigen::MatrixXf;
using Eigen::MatrixXi;
using Eigen::JacobiSVD;
using Eigen::BDCSVD;
using Eigen::ComputeThinU;
using Eigen::ComputeThinV;
using Eigen::Vector3f;
using namespace std;

void grayscale(unsigned char* image, const int& width, const int& height) {
    for (int i = 0; i < (height * width * 3); i += 3) {
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

MatrixXd to_matrix_gray(const unsigned char* image, const int& width, const int& height) {
    MatrixXd m(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            m(i, j) = image[3 * ((i * width) + j)];
        }
    }
    return m;
}

int main() {
    unsigned char* image;
    int width;
    int height;
    int n;
    string input_filename = "test.png";
    unsigned char* data = stbi_load("test3.png", &width, &height, &n, 3);
    if (data != nullptr) {
        image = new unsigned char[width * height * 3];
        image = data;
    }
    stbi_image_free(data);
    cout << width << " x " << height << endl; 
    grayscale(image, width, height);

    MatrixXd m = to_matrix_gray(image, width, height);

    BDCSVD<MatrixXd> svd;
    svd.compute(m, ComputeThinU | ComputeThinV);
    MatrixXd U = svd.matrixU();
    MatrixXd V = svd.matrixV().transpose();

    // Reconstruct
    double t = 0.1;
    int k = svd.singularValues().rows() * t; 
    MatrixXd S = MatrixXd::Zero(k, k);
    MatrixXd U_img = MatrixXd::Zero(U.rows(), k);
    MatrixXd V_img = MatrixXd::Zero(k, V.cols());

    for (int i = 0; i < k; i++) {
        S(i, i) = svd.singularValues()(i);
        U_img.col(i) = U.col(i);
        V_img.row(i) = V.row(i);
    }

    MatrixXd img = U_img * S * V_img;
    unsigned char *reconstructed_image = new unsigned char[width * height * 3];
    for (int i = 0; i < img.rows(); i++) {
        for (int j = 0; j < img.cols(); j++) {
            unsigned char tmp_r, tmp_g, tmp_b, tmp;
            ((img(i, j) >= 0) && (img(i, j) < 256)) ? (tmp = img(i, j)) : ((img(i, j) < 0) ? (tmp = 0) : (tmp = 255));
            for (int idx = (3 * ((i * img.cols()) + j)); idx < (3 * ((i * img.cols()) + j)) + 3; idx++) {
                reconstructed_image[idx] = tmp;
            }
        }
    }
    string output = "output_test.png";
    stbi_write_png(output.c_str(), width, height, 3, reconstructed_image, width * 3);
    return 0;
}




// /**
//  * @file main.cc
//  * @author Garrett Rhoads
//  * @brief main file for renderer
//  * @date 2025-03-15
//  */

// #include <iostream>
// #include <vector>
// #include <cmath>
// #include "Eigen/Dense"
// #include "plotter.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

// using Eigen::MatrixXf;
// using Eigen::MatrixXi;
// using Eigen::JacobiSVD;
// using Eigen::BDCSVD;
// using Eigen::ComputeThinU;
// using Eigen::ComputeThinV;
// using Eigen::Vector3f;
// using namespace std;

// void grayscale(unsigned char* image, const int& width, const int& height) {
//     for (int i = 0; i < (height * width * 3); i += 3) {
//         int avg = 0;
//         for (int j = i; j < (i + 3); j++) {
//             avg += image[j];
//         }
//         avg /= 3;
//         for (int j = i; j < (i + 3); j++) {
//             image[j] = avg;
//         }
//     }
// }

// void rgb_split(const unsigned char *input, unsigned char *img_r, unsigned char *img_g, unsigned char *img_b, const int& width, const int& height) {
//     int color = 0;
//     for (int i = 0; i < (height * width * 3); i += 3) {
//         //cout << i % 3 << endl;
//         // if (color == 2) {
//         //     img_r[i] = 0;
//         //     img_g[i] = 0;
//         //     img_b[i] = input[i];
//         //     //cout << "blue" << " ";
//         // }
//         // if (color == 1) {
//         //     img_r[i] = 0;
//         //     img_g[i] = input[i];
//         //     img_b[i] = 0;
//         //     //cout << "green" << " ";
//         // }
//         // if (color == 0) {
//         //     img_r[i] = input[i];
//         //     img_g[i] = 0;
//         //     img_b[i] = 0;
//         //     //cout << "red" << " ";
//         // } 
//         // (color < 3) ? color++ : color = 0;
//         // if (color == 1) {
//         //     img_r[i] = 0;
//         //     img_g[i] = input[i];
//         //     img_b[i] = 0;
//         // }
//         //cout << "i = " << i << " ";

//         img_r[i + 0] = input[i + 0];
//         img_g[i + 1] = input[i + 1];
//         img_b[i + 2] = input[i + 2];
//         //cout << static_cast<unsigned int>(input[i]) << " ";
//     }
//     cout << endl;
// }

// MatrixXd to_matrix_gray(const unsigned char* image, const int& width, const int& height) {
//     MatrixXd m(height, width);
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             m(i, j) = image[3 * ((i * width) + j)];
//         }
//     }
//     return m;
// }

// MatrixXd to_matrix_red(const unsigned char* image, const int& width, const int& height) {
//     MatrixXd m(height, width);
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             m(i, j) = image[3 * ((i * width) + j)];
//         }
//     }
//     return m;
// }

// MatrixXd to_matrix_green(const unsigned char* image, const int& width, const int& height) {
//     MatrixXd m(height, width);
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             m(i, j) = image[3 * ((i * width) + j) + 1];
//         }
//     }
//     return m;
// }

// MatrixXd to_matrix_blue(const unsigned char* image, const int& width, const int& height) {
//     MatrixXd m(height, width);
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             m(i, j) = image[3 * ((i * width) + j) + 2];
//         }
//     }
//     return m;
// }

// double eigen_max(const MatrixXd& m) {
//     double max = 0;
//     for (int i = 0; i < m.rows(); i++) {
//         for (int j = 0; j < m.cols(); j++) {
//             if (abs(m(i, j)) > max) {
//                 max = m(i, j);
//             }
//         }
//     }
//     return max;
// }

// void to_image(const MatrixXd& m_r, const MatrixXd& m_g, const MatrixXd& m_b, unsigned char *image) {
//     double r_max = abs(eigen_max(m_r));
//     double g_max = abs(eigen_max(m_g));
//     double b_max = abs(eigen_max(m_b));
//     for (int i = 0; i < m_r.rows(); i++) {
//         for (int j = 0; j < m_r.cols(); j++) {
//             int k = (3 * ((i * m_r.cols()) + j));
//             image[k + 0] = static_cast<unsigned char>(abs(0xff * m_r(i, j) / r_max));
//             image[k + 1] = static_cast<unsigned char>(abs(0xff * m_g(i, j) / g_max));
//             image[k + 2] = static_cast<unsigned char>(abs(0xff * m_b(i, j) / b_max));
//         }
//     }
// }


// int main() {
//     unsigned char* image;
//     int width;
//     int height;
//     int n;
//     string input_filename = "test.png";
//     unsigned char* data = stbi_load("test2.png", &width, &height, &n, 3);
//     if (data != nullptr) {
//         image = new unsigned char[width * height * 3];
//         image = data;
//     }
//     stbi_image_free(data);
//     cout << width << " x " << height << endl; 
//     //grayscale(image, width, height);
//     for (int i = 0; i < (width * height * 3); i++) {
//         cout << static_cast<int>(image[i]) << " ";
//     }
//     unsigned char *image_r = new unsigned char[height * width * 3]();
//     unsigned char *image_g = new unsigned char[height * width * 3]();
//     unsigned char *image_b = new unsigned char[height * width * 3]();
//     rgb_split(image, image_r, image_g, image_b, width, height);

//     MatrixXd m_r = to_matrix_red(image_r, width, height);
//     MatrixXd m_g = to_matrix_green(image_g, width, height);
//     MatrixXd m_b = to_matrix_blue(image_b, width, height);

//     cout << m_r << endl << m_g << endl << m_b << endl;

//     BDCSVD<MatrixXd> svd_r;
//     BDCSVD<MatrixXd> svd_g;
//     BDCSVD<MatrixXd> svd_b;
//     svd_r.compute(m_r, ComputeThinU | ComputeThinV);
//     svd_g.compute(m_g, ComputeThinU | ComputeThinV);
//     svd_b.compute(m_b, ComputeThinU | ComputeThinV);

//     MatrixXd U_r = svd_r.matrixU();
//     MatrixXd V_r = svd_r.matrixV().transpose();

//     MatrixXd U_g = svd_g.matrixU();
//     MatrixXd V_g = svd_g.matrixV().transpose();

//     MatrixXd U_b = svd_b.matrixU();
//     MatrixXd V_b = svd_b.matrixV().transpose();

//     unsigned char* U_image = new unsigned char[U_r.rows() * U_r.cols() * 3];
//     unsigned char* V_image = new unsigned char[V_r.rows() * V_r.cols() * 3];
//     to_image(U_r, U_g, U_b, U_image);
//     to_image(V_r, V_g, V_b, V_image);
//     stbi_write_png("U.png", U_r.rows(), U_r.cols(), 3, U_image, U_r.rows() * 3);
//     stbi_write_png("V.png", V_r.rows(), V_r.cols(), 3, V_image, V_r.rows() * 3); 

//     //Reconstruct
//     double t = 0.3;
//     int k = svd_r.singularValues().rows() * t; 
//     MatrixXd S_r = MatrixXd::Zero(k, k);
//     MatrixXd S_g = MatrixXd::Zero(k, k);
//     MatrixXd S_b = MatrixXd::Zero(k, k);
//     MatrixXd U_img_r = MatrixXd::Zero(U_r.rows(), k);
//     MatrixXd V_img_r = MatrixXd::Zero(k, V_r.cols());
//     MatrixXd U_img_g = MatrixXd::Zero(U_g.rows(), k);
//     MatrixXd V_img_g = MatrixXd::Zero(k, V_g.cols());
//     MatrixXd U_img_b = MatrixXd::Zero(U_b.rows(), k);
//     MatrixXd V_img_b = MatrixXd::Zero(k, V_b.cols());

//     for (int i = 0; i < k; i++) {
//         S_r(i, i) = svd_r.singularValues()(i);
//         S_g(i, i) = svd_g.singularValues()(i);
//         S_b(i, i) = svd_b.singularValues()(i);
//         U_img_r.col(i) = U_r.col(i);
//         V_img_r.row(i) = V_r.row(i);
//         U_img_g.col(i) = U_g.col(i);
//         V_img_g.row(i) = V_g.row(i);
//         U_img_b.col(i) = U_b.col(i);
//         V_img_b.row(i) = V_b.row(i);
//     }
//     MatrixXd img_r = U_img_r * S_r * V_img_r;
//     MatrixXd img_g = U_img_g * S_g * V_img_g;
//     MatrixXd img_b = U_img_b * S_b * V_img_b;
//     unsigned char *reconstructed_image = new unsigned char[width * height * 3];
//     for (int i = 0; i < img_r.rows(); i++) {
//         for (int j = 0; j < img_r.cols(); j++) {
//             unsigned char tmp_r, tmp_g, tmp_b;
//             ((img_r(i, j) >= 0) && (img_r(i, j) < 256)) ? (tmp_r = img_r(i, j)) : ((img_r(i, j) < 0) ? (tmp_r = 0) : (tmp_r = 255));
//             ((img_g(i, j) >= 0) && (img_g(i, j) < 256)) ? (tmp_g = img_g(i, j)) : ((img_g(i, j) < 0) ? (tmp_g = 0) : (tmp_g = 255));
//             ((img_b(i, j) >= 0) && (img_b(i, j) < 256)) ? (tmp_b = img_b(i, j)) : ((img_b(i, j) < 0) ? (tmp_b = 0) : (tmp_b = 255));
//             // for (int idx = (3 * ((i * img_r.cols()) + j)); idx < (3 * ((i * img_r.cols()) + j)) + 3; idx++) {
//             //     reconstructed_image[idx] = tmp;
//             // }
//             int idx = (3 * ((i * img_r.cols()) + j));
//             reconstructed_image[idx + 0] = tmp_r;
//             reconstructed_image[idx + 1] = tmp_g;
//             reconstructed_image[idx + 2] = tmp_b;
//         }
//     }
//     stbi_write_png("output_test.png", width, height, 3, reconstructed_image, width * 3);
    
//     return 0;
// }
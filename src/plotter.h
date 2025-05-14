/**
 * @file plot.h
 * @author Garrett
 * @brief 
 */

#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <stdexcept>
#include "structs.h"
#include "Eigen/Dense"

using Eigen::MatrixXd;
using Eigen::Vector2d;
using namespace std;

const int CHANNELS = 3;

class Plot {
public:
    Plot();
    Plot(const int& img_width_res,  const int& img_height_res, 
          const double& plot_width, const double& plot_height);
    ~Plot();

    int get_img_width() const;
    int get_img_height() const;
    double get_plot_width() const;
    double get_plot_height() const;
    int get_img_size() const;
    uint8_t* get_img() const;
    
    Pixel get_pixel(const RectPoint& pt);
    Pixel get_pixel(const Vector2d& pt);
    Pixel get_pixel(const int& pix_x, const int& pix_y);
    void draw_pixel(const RectPoint& pt, const Pixel& px);
    void draw_pixel(const Vector2d& pt, const Pixel& px);
    void draw_pixel(const int& pix_x, const int& pix_y, const Pixel& px);
    void draw_pixel_additive(const RectPoint& pt, const Pixel& px);
    void draw_pixel_additive(const Vector2d& pt, const Pixel& px);
    void draw_line(RectPoint pt0, RectPoint pt1);
    void draw_line(Vector2d pt0, Vector2d pt1);
    void draw_line(RectPoint pt0, RectPoint pt1, const Pixel& px);
    void draw_line(Vector2d pt0, Vector2d pt1, const Pixel& px);
    void draw_line_additive(RectPoint pt0, RectPoint pt1, const Pixel& px);
    void draw_line_additive(Vector2d pt0, Vector2d pt1, const Pixel& px);

    void blackout();
    void whiteout();
    void bg_fill(const Pixel& px);

    Plot& operator-=(Plot& plot);
    Plot& operator+=(Plot& plot);
private:
    int _img_width = 100;
    int _img_height = 100;
    double _plot_width = 4.0;
    double _plot_height = 4.0; 
    //const int CHANNELS = 3;
    int _img_size = _img_width * _img_height * CHANNELS;
    
    uint8_t *_img = nullptr;
    uint8_t *_pix = _img;
};

#endif
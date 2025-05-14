/**
 * @file plot.cc
 * @author Garrett
 * @brief 
 */

#include <iostream>
#include <cmath>
#include <stdexcept>
#include "plotter.h"

using namespace std;

Plot::Plot() {}

Plot::Plot(const int& img_width_res, const int& img_height_res, 
             const double& plot_width, const double& plot_height) {
    _img_width = img_width_res;
    _img_height = img_height_res;
    _plot_width = plot_width;
    _plot_height = plot_height;
    _img_size = _img_width * _img_height * CHANNELS;
    _img = new uint8_t[_img_size];
}

Plot::~Plot() {
    delete[] _img;
}

int Plot::get_img_width() const { return _img_width; };
int Plot::get_img_height() const { return _img_height; };
double Plot::get_plot_width() const { return _plot_width; };
double Plot::get_plot_height() const { return _plot_height; };
int Plot::get_img_size() const { return _img_size; };
uint8_t* Plot::get_img() const { return _img; }

Pixel Plot::get_pixel(const RectPoint& pt) {
    Pixel px;
    int pix_x = (((pt.x / _plot_width) + 0.5) * (_img_width - 1) * CHANNELS);
    int pix_y = ((_img_height - 1) - (((pt.y / _plot_height) + 0.5) * (_img_height - 1)));
    _pix = _img + (((_img_width * CHANNELS * pix_y) + pix_x) - 
    ((_img_width * CHANNELS * pix_y) + pix_x) % CHANNELS);
    px.r = *(_pix + 0);
    px.g = *(_pix + 1);
    px.b = *(_pix + 2);

    return px;
}

Pixel Plot::get_pixel(const Vector2d& pt) {
    RectPoint rect_pt;
    rect_pt.x = pt(0);
    rect_pt.y = pt(1);
    return (get_pixel(pt));
}

Pixel Plot::get_pixel(const int& pix_x, const int& pix_y) {
    Pixel px;

    _pix = _img + (((_img_width * CHANNELS * pix_y) + (pix_x * CHANNELS)) - 
    ((_img_width * CHANNELS * pix_y) + (pix_x * CHANNELS)) % CHANNELS);
    px.r = *(_pix + 0);
    px.g = *(_pix + 1);
    px.b = *(_pix + 2);

    return px;
}

void Plot::draw_pixel(const RectPoint& pt, const Pixel& px) {
    int pix_x = (((pt.x / _plot_width) + 0.5) * (_img_width - 1) * CHANNELS);
    int pix_y = ((_img_height - 1) - (((pt.y / _plot_height) + 0.5) * (_img_height - 1)));
    _pix = _img + (((_img_width * CHANNELS * pix_y) + pix_x) - 
         ((_img_width * CHANNELS * pix_y) + pix_x) % CHANNELS);
    *(_pix + 0) = px.r;
    *(_pix + 1) = px.g;
    *(_pix + 2) = px.b;
}

void Plot::draw_pixel(const Vector2d& pt, const Pixel& px) {
    RectPoint rect_pt;
    rect_pt.x = pt(0);
    rect_pt.y = pt(1);
    draw_pixel(pt, px);
}

void Plot::draw_pixel(const int& pix_x, const int& pix_y, const Pixel& px) {
    _pix = _img + (((_img_width * CHANNELS * pix_y) + (pix_x * CHANNELS)) - 
         ((_img_width * CHANNELS * pix_y) + (pix_x * CHANNELS)) % CHANNELS);
    *(_pix + 0) = px.r;
    *(_pix + 1) = px.g;
    *(_pix + 2) = px.b;
}

void Plot::draw_pixel_additive(const RectPoint& pt, const Pixel& px) {
    Pixel this_px = get_pixel(pt);
    int r = this_px.r + px.r;
    int g = this_px.g + px.g;
    int b = this_px.b + px.b;

    if (r > 0xFF) { r = 0xFF; }
    if (g > 0xFF) { g = 0xFF; }
    if (b > 0xFF) { b = 0xFF; }

    Pixel new_pix;
    new_pix.r = r;
    new_pix.g = g;
    new_pix.b = b; 

    int pix_x = (((pt.x / _plot_width) + 0.5) * (_img_width - 1) * CHANNELS);
    int pix_y = ((_img_height - 1) - (((pt.y / _plot_height) + 0.5) * (_img_height - 1)));
    _pix = _img + (((_img_width * CHANNELS * pix_y) + pix_x) - 
         ((_img_width * CHANNELS * pix_y) + pix_x) % CHANNELS);
    *(_pix + 0) = new_pix.r;
    *(_pix + 1) = new_pix.g;
    *(_pix + 2) = new_pix.b;
}

void Plot::draw_pixel_additive(const Vector2d& pt, const Pixel& px) {
    RectPoint rect_pt;
    rect_pt.x = pt(0);
    rect_pt.y = pt(1);
    draw_pixel_additive(pt, px);
}

void Plot::draw_line(RectPoint pt0, RectPoint pt1) {
    Pixel px;
    px.r = 0;
    px.g = 0;
    px.b = 0;
    draw_line(pt0, pt1, px);
}

void Plot::draw_line(Vector2d pt0, Vector2d pt1) {
    RectPoint rect_pt0;
    RectPoint rect_pt1;
    rect_pt0.x = pt0(0);
    rect_pt0.y = pt0(1);
    rect_pt1.x = pt1(0);
    rect_pt1.y = pt1(1);
    draw_line(rect_pt0, rect_pt1);
}

void Plot::draw_line(Vector2d pt0, Vector2d pt1, const Pixel& px) {
    RectPoint rect_pt0;
    RectPoint rect_pt1;
    rect_pt0.x = pt0(0);
    rect_pt0.y = pt0(1);
    rect_pt1.x = pt1(0);
    rect_pt1.y = pt1(1);
    draw_line(rect_pt0, rect_pt1, px);
}

void Plot::draw_line_additive(RectPoint pt0, RectPoint pt1, const Pixel& px) {
    RectPoint pt;
    RectPoint swap;
    double dx = pt1.x - pt0.x;
    double dy = pt1.y - pt0.y;
    double m = dy / dx;
    
    if (abs(m) > 1) {
        if (pt0.y > pt1.y) {
            swap.x = pt0.x;
            pt0.x = pt1.x;
            pt1.x = swap.x;
            swap.y = pt0.y;
            pt0.y = pt1.y;
            pt1.y = swap.y;
        }
        for (pt.y = pt0.y; pt.y < pt1.y; pt.y += (1.0 / _img_width)) {
            pt.x = ((pt.y - pt0.y) / m) + pt0.x;
            draw_pixel_additive(pt, px);
        }
    } else {
        if (pt0.x > pt1.x) {
            swap.x = pt0.x;
            pt0.x = pt1.x;
            pt1.x = swap.x;
            swap.y = pt0.y;
            pt0.y = pt1.y;
            pt1.y = swap.y;
        }
        for (pt.x = pt0.x; pt.x < pt1.x; pt.x += (1.0 / _img_width)) {
            pt.y = m * (pt.x - pt0.x) + pt0.y;
            draw_pixel_additive(pt, px);
        }
    }
}

void Plot::draw_line(RectPoint pt0, RectPoint pt1, const Pixel& px) {
    RectPoint pt;
    RectPoint swap;
    double dx = pt1.x - pt0.x;
    double dy = pt1.y - pt0.y;
    double m = dy / dx;
    
    if (abs(m) > 1) {
        if (pt0.y > pt1.y) {
            swap.x = pt0.x;
            pt0.x = pt1.x;
            pt1.x = swap.x;
            swap.y = pt0.y;
            pt0.y = pt1.y;
            pt1.y = swap.y;
        }
        for (pt.y = pt0.y; pt.y < pt1.y; pt.y += (1.0 / _img_width)) {
            pt.x = ((pt.y - pt0.y) / m) + pt0.x;
            draw_pixel(pt, px);
        }
    } else {
        if (pt0.x > pt1.x) {
            swap.x = pt0.x;
            pt0.x = pt1.x;
            pt1.x = swap.x;
            swap.y = pt0.y;
            pt0.y = pt1.y;
            pt1.y = swap.y;
        }
        for (pt.x = pt0.x; pt.x < pt1.x; pt.x += (1.0 / _img_width)) {
            pt.y = m * (pt.x - pt0.x) + pt0.y;
            draw_pixel(pt, px);
        }
    }
}

void Plot::blackout() {
    for (_pix = _img; _pix < (_img + _img_size); _pix++) {
        *_pix = 0x00;
    }
}

void Plot::whiteout() {
    for (_pix = _img; _pix < (_img + _img_size); _pix++) {
        *_pix = 0xFF;
    }
}

void Plot::bg_fill(const Pixel& px) {
    for (_pix = _img; _pix < (_img + _img_size); _pix += CHANNELS) {
        *(_pix + 0) = px.r;
        *(_pix + 1) = px.g;
        *(_pix + 2) = px.b;
    }
}

Plot& Plot::operator-=(Plot& plot) {
    if (_img_size != plot.get_img_size()) {
        cout << "Images of different sizes" << endl;
        throw;
    }
    Pixel px_this;
    Pixel px_plot;
    for (int i = 0; i < _img_height; i++) {
        for (int j = 0; j < _img_width; j++) {
            px_this = this->get_pixel(j, i);
            px_plot = plot.get_pixel(j, i);
            int r = px_this.r - px_plot.r;
            int g = px_this.g - px_plot.g;
            int b = px_this.b - px_plot.b;

            if (r < 0) { r = 0; }
            if (g < 0) { g = 0; } 
            if (b < 0) { b = 0; }

            px_this.r = r;
            px_this.g = g;
            px_this.b = b;

            this->draw_pixel(j, i, px_this);                
        }
    }
    return *this;
}

Plot& Plot::operator+=(Plot& plot) {
    if (_img_size != plot.get_img_size()) {
        cout << "Images of different sizes" << endl;
        throw;
    }
    Pixel px_this;
    Pixel px_plot;
    for (int i = 0; i < _img_height; i++) {
        for (int j = 0; j < _img_width; j++) {
            px_this = this->get_pixel(j, i);
            px_plot = plot.get_pixel(j, i);
            int r = px_this.r + px_plot.r;
            int g = px_this.g + px_plot.g;
            int b = px_this.b + px_plot.b;

            if (r > 0xFF) { r = 0xFF; }
            if (g > 0xFF) { g = 0xFF; } 
            if (b > 0xFF) { b = 0xFF; }

            px_this.r = r;
            px_this.g = g;
            px_this.b = b;

            this->draw_pixel(j, i, px_this);                
        }
    }
    return *this;
}

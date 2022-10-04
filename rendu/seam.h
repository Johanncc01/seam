//
//  seam.h
//  SeamCarving
//
//  Created by Barbara Jobstmann on 15.06.21.
//
#pragma once

#include <limits>
#include <vector>

#include "seam_types.h"

// TASK 1: COLOR
double get_red(int rgb);
double get_green(int rgb);
double get_blue(int rgb);
double get_gray(int rgb);
int get_RGB(double red, double green, double blue);
int get_RGB(double gray);
GrayImage to_gray(const RGBImage &cimage);
RGBImage to_RGB(const GrayImage &gimage);

//  TASK 2: FILTER
inline void clamp(int &val, int max);

GrayImage filter(const GrayImage &gray, const Kernel &kernel);
GrayImage smooth(const GrayImage &gray);
GrayImage sobelX(const GrayImage &gray);
GrayImage sobelY(const GrayImage &gray);
GrayImage sobel(const GrayImage &gray);

//  TASK 3 NEW: SEAM

Graph create_graph(const GrayImage &gray);
Path shortest_path(Graph &graph, size_t from, size_t to);
Path find_seam(const GrayImage &energy);

// Provided functions
GrayImage highlight_seam(const GrayImage &gray, const Path &seam);
RGBImage highlight_seam(const RGBImage &image, const Path &seam);
GrayImage remove_seam(const GrayImage &energy, const Path &seam);
RGBImage remove_seam(const RGBImage &image, const Path &seam);

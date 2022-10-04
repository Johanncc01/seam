#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "seam_types.h"

bool exists(const std::string &name);

/*
 * Reads an image and returns a 2-dimensional vector with the RGB values
 * of the given image.
 */
RGBImage read_image(std::string name);

/*
 * Take a 2-dimensional vector with RGB values and write a png file.
 */
void write_image(const RGBImage &image, std::string name);

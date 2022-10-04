#include "helper.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CHANNEL_NUM 3

bool exists(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

/*
 * Reads an image and returns a 2-dimensional vector with the RGB values
 * of the given image.
 */
RGBImage read_image(std::string name)
{
    int width, height, bpp;

    if (!exists(name)) {
        std::cout << "Error: File " << name << " does not exist." << std::endl;
        return std::vector<std::vector<int>>();
    }

    std::cout << "Info: reading file " << name << std::endl;

    uint8_t *rgb_image = stbi_load(name.c_str(), &width, &height, &bpp, CHANNEL_NUM);
    std::vector<std::vector<int>> image = std::vector<std::vector<int>>(height, std::vector<int>(width));

    uint8_t *iterator = rgb_image;
    for (size_t i(0); i < height; ++i) {
        for (size_t j(0); j < width; ++j) {
            int rgb = 0;
            for (int c(CHANNEL_NUM - 1); c >= 0; --c) {
                int value = (int)*iterator;
                rgb = rgb + (value << (8 * c));
                iterator++;
            }
            image[i][j] = rgb;
        }
    }
    stbi_image_free(rgb_image);
    return image;
}

/*
 * Take a 2-dimensional vector with RGB values and write a png file.
 */
void write_image(const RGBImage &image, std::string name)
{
    std::cout << "Info: writing file " << name << std::endl;

    int height = (int)image.size();
    int width = (int)image[0].size();

    uint8_t *rgb_image;
    rgb_image = (uint8_t *)malloc(width * height * CHANNEL_NUM);

    uint8_t *iterator = rgb_image;
    for (size_t i(0); i < height; ++i) {
        for (size_t j(0); j < width; ++j) {
            int rgb = image[i][j];
            for (int c(CHANNEL_NUM - 1); c >= 0; --c) {
                int value = (rgb >> (8 * c)) & 0xFF;
                *iterator = value;
                iterator++;
            }
        }
    }
    stbi_write_png(name.c_str(), width, height, CHANNEL_NUM, rgb_image, width * CHANNEL_NUM);
    stbi_image_free(rgb_image);
}

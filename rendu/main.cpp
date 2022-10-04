//
//  main.cpp
//  SeamCarving
//
//  Created by Barbara Jobstmann on 15.06.21.
//
//

#include <cassert>
#include <iostream>
#include <tgmath.h>
#include <vector>

#include "helper.h"
#include "seam.h"
#include "unit_test.h"

void test_to_gray(std::string const& in_path);
void test_smooth(std::string const& in_path);
void test_sobel(std::string const& in_path);
void test_hightlight_seam(std::string const& in_path, int num);
void test_remove_seam(std::string const& in_path, int num);

int main(int argc, char **argv)
{
    run_unit_tests();

    // Initialize with a default value
    std::string in_path = "img/americascup.jpg";
    std::string out_path = "test.png";
    if (argc > 1 && argc <= 3) {
        // Change it if the user defined a image path
        in_path = argv[1];
    } else if (argc > 3) {
        std::cerr << "error: Too many arguments" << std::endl;
        std::cerr << "Usage:\n\t./prog [input_path] [output_path]" << std::endl;
        return -1;
    }

    // // Uncomment for testing different phases:
    // test_to_gray(in_path);
    // test_smooth(in_path);
    // test_sobel(in_path);
    // int num_seam(10); /* high value will slow things down */
    // test_hightlight_seam(in_path, num_seam);
    //  test_remove_seam(in_path, num_seam);

    return 0;
}

void test_to_gray(std::string const& in_path)
{
    RGBImage image(read_image(in_path));
    if (!image.empty()) {
        GrayImage gray_image(to_gray(image));
        write_image(to_RGB(gray_image), "test_grayed.png");
    }
}

void test_smooth(std::string const& in_path)
{
    RGBImage image(read_image(in_path));
    if (!image.empty()) {
        GrayImage gray_image(to_gray(image));
        GrayImage smoothed_image(smooth(gray_image));
        write_image(to_RGB(smoothed_image), "test_smoothed.png");
    }
};

void test_sobel(std::string const& in_path)
{
    RGBImage image(read_image(in_path));
    if (!image.empty()) {
        GrayImage gray_image(to_gray(image));
        GrayImage sobeled_image(sobel(gray_image));
        write_image(to_RGB(sobeled_image), "test_sobeled.png");
    }
};

void test_hightlight_seam(std::string const& in_path, int num)
{
    RGBImage image(read_image(in_path));
    if (!image.empty()) {
        GrayImage gray_image(to_gray(image));
        for (int i = 0; i < num; ++i) {
            GrayImage sobeled_image(sobel(smooth(gray_image)));
            Path seam = find_seam(sobeled_image);
            gray_image = highlight_seam(gray_image, seam);
        }
        write_image(to_RGB(gray_image), "test_highlighted_seam.png");
    }
}

void test_remove_seam(std::string const& in_path, int num)
{
    RGBImage image(read_image(in_path));
    if (!image.empty()) {
        for (int i = 0; i < num; ++i) {
            GrayImage gray_image(to_gray(image));
            GrayImage sobeled_image(sobel(smooth(gray_image)));
            Path seam = find_seam(sobeled_image);
            image = remove_seam(image, seam);
        }
        write_image(image, "test_removed_seam.png");
    }
};

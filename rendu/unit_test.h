#pragma once
#include <array>
#include <cmath> // std::fabs
#include <iostream> // std::cerr, std::endl
#include <limits> // std::numeric_limits
#include <tuple>

#include "helper.h"
#include "seam.h"

constexpr double EPSILON = 10e-6;

bool equal(double x, double y, double precision = EPSILON);

struct ColorInfo {
    std::string name;
    std::tuple<int, double, double, double, double> fields;
};

void print_header(std::string const& header);

void print_image_row(std::vector<double> const& row);

void print_image(GrayImage const& image);

void print_node(Node const& node) ;

void print_path(Path const& path);

void check_equal(double expected, double computed);

void check_equal(int expected, int computed);

void check_equal(Node const& expected, Node const& computed);

void check_equal(Graph const& expected, Graph const& computed);

void check_equal(Path const& expected, Path const& computed);

void check_equal(GrayImage const& expected, GrayImage const& computed);

void test_color();

void test_to_gray_2_2();

void test_to_gray_3_3();

void test_to_gray_4_4();

void test_to_gray_5_5();

void test_sobelX_1();

void test_sobelX_2();

void test_sobelX_3();

void test_sobelY_1();

void test_sobelY_2();

void test_sobelY_3();

void test_sobel_1();

void test_smooth_1();

void test_create_graph_1();

void test_create_graph_2();

void test_shortest_path_1();

void test_shortest_path_2();

void test_find_seam_1();

void run_unit_tests();

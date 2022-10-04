#include <array>
#include <cmath> // std::fabs
#include <iostream> // std::cerr, std::endl
#include <limits> // std::numeric_limits
#include <tuple>
#include <iomanip>
#include <bitset>

#include "helper.h"
#include "seam.h"
#include "unit_test.h"

using namespace std;

bool equal(double x, double y, double precision)
{
    return (std::fabs(x - y) < precision);
}
  
void print_header(std::string const& header)
{
    std::cerr << "---------------" << std::endl;
    std::cerr << header << std::endl;
    std::cerr << "---------------" << std::endl;
}

void print_image_row(std::vector<double> const& row)
{
    if (row.empty()) {
        std::cerr << "{ empty row }" << std::endl;
        return;
    }
    std::cerr << "{" << row.at(0);
    for (int pixel = 1; pixel < row.size(); ++pixel) {
        std::cerr << ", " << row.at(pixel);
    }
    std::cerr << "}";
}

void print_image(GrayImage const& image)
{
    if (image.empty()) {
        std::cerr << "{ empty image }" << std::endl;
        return;
    }
    std::cerr << "{";
    print_image_row(image.at(0));
    for (int row = 1; row < image.size(); ++row) {
        std::cerr << ", ";
        print_image_row(image.at(row));
    }
    std::cerr << "}" << std::endl;
}

void print_node(Node const& node) 
{
    std::cerr << "{{";
    if (!node.successors.empty()) {
        std::cerr << node.successors.at(0);
        for (int i = 1; i < node.successors.size(); ++i) {
            std::cerr << ", " << node.successors.at(i);
        }
    }
    std::cerr << "}, ";
    std::cerr << node.costs << ", " << node.distance_to_target << ", ";
    std::cerr << node.predecessor_to_target << "}" << std::endl;
}

void print_path(Path const& path)
{
    if (path.empty()) {
        std::cerr << "{ empty path }" << std::endl;
        return;
    }
    std::cerr << "{" << path.at(0);
    for (int i = 1; i < path.size(); ++i) {
        std::cerr << ", " << path.at(i);
    }
    std::cerr << "}" << std::endl;
}

void check_equal(double expected, double computed)
{
    if (equal(computed, expected)) {
        std::cerr << "[Passed]" << std::endl;
        return;
    }
    std::cerr << "[Failed]" << std::endl;
    std::cerr << "   expected: " << expected << std::endl;
    std::cerr << "   computed: " << computed << std::endl;
}

void check_equal(int expected, int computed)
{
    if (expected == computed) {
        std::cerr << "[Passed]" << std::endl;
        return;
    }
    std::cerr << "[Failed]" << std::endl;
    std::cerr << "   expected: " << expected << std::endl;
    std::cerr << "   computed: " << computed << std::endl;
}

void check_equal(Node const& expected, Node const& computed)
{
    bool mismatch = expected.successors != computed.successors;
    mismatch |= !equal(expected.costs, computed.costs);
    mismatch |= !equal(expected.distance_to_target, computed.distance_to_target);
    mismatch |= expected.predecessor_to_target != computed.predecessor_to_target;
    if (!mismatch) {
        std::cerr << "[Passed]" << std::endl;
        return;
    }
    std::cerr << "[Failed]" << std::endl;
    std::cerr << "   expected: "; print_node(expected);
    std::cerr << "   computed: "; print_node(computed);
}

void check_equal(Graph const& expected, Graph const& computed)
{
    if (expected.size() > computed.size()) {
        std::cerr << "[Failed] - Missing nodes" << std::endl;
        int i = 0u;
        while (i < computed.size()) {
            std::cerr << "   expected: "; print_node(expected.at(i));
            std::cerr << "   computed: "; print_node(computed.at(i));
            std::cerr << std::endl;
            i += 1;
        }
        while (i < expected.size()) {
            std::cerr << "   expected: "; print_node(expected.at(i));
            std::cerr << "   computed: missing" << std::endl;
            i += 1;
        }
        return;
    }
    if (expected.size() < computed.size()) {
        std::cerr << "[Failed] - Too many nodes" << std::endl;
        int i = 0u;
        while (i < expected.size()) {
            std::cerr << "   expected: "; print_node(expected.at(i));
            std::cerr << "   computed: "; print_node(computed.at(i));
            std::cerr << std::endl;
            i += 1;
        }
        while (i < computed.size()) {
            std::cerr << "   expected: -" << std::endl;
            std::cerr << "   computed: "; print_node(computed.at(i));
            std::cerr << std::endl;
            i += 1;
        }
        return;
    }
    for (int i = 0u; i < expected.size(); ++i) {
        check_equal(expected.at(i), computed.at(i));
    }
}

void check_equal(Path const& expected, Path const& computed)
{
    bool mismatch = expected.size() != computed.size();
    for (size_t i = 0u; !mismatch && (i < expected.size()); ++i) {
        if (computed.at(i) != expected.at(i)) {
            mismatch = true;
        }
    }

    if (!mismatch) {
        std::cerr << "[Passed]" << std::endl;
        return;
    }
    std::cerr << "[Failed]" << std::endl;
    std::cerr << "   expected: "; print_path(expected);
    std::cerr << "   computed: "; print_path(computed);
}

void check_equal(GrayImage const& expected, GrayImage const& computed)
{
    int const num_rows = expected.size();
    int const num_cols = expected.at(0).size();
    bool mismatch = num_rows != computed.size();
    mismatch = num_cols != computed.at(0).size();
    for (size_t row = 0u; !mismatch && (row < num_rows); ++row) {
        mismatch |= num_cols != computed.at(row).size();
        for (size_t col = 0u; !mismatch && (col < num_cols); ++col) {
            mismatch |= !equal(computed.at(row).at(col), expected.at(row).at(col));
        }
    }

    if (!mismatch) {
        std::cerr << "[Passed]" << std::endl;
        return;
    }
    std::cerr << "[Failed]" << std::endl;
    std::cerr << "   expected: "; print_image(expected);
    std::cerr << "   computed: "; print_image(computed);
}

void test_color()
{
    std::vector<ColorInfo> colors = {{
        /* Red */ {"Red", {0xFF0000, 1.0, 0.0, 0.0, 0.333333}},
        /* Green */ {"Green", {0x00FF00, 0.0, 1.0, 0.0, 0.333333}},
        /* Blue */ {"Blue", {0x0000FF, 0.0, 0.0, 1.0, 0.333333}},
        /* White */ {"White", {0xFFFFFF, 1.0, 1.0, 1.0, 1.0}},
        /* Black */ {"Black", {0x000000, 0.0, 0.0, 0.0, 0.0, }},
        // example of page 6:
        /* Light blue */ {"Light blue", {0x20c0ff, 0.1254902, 0.7529412, 1.0, 0.6261438}},
    }};

    print_header("test_color");
    for (ColorInfo color_info : colors) {
        std::cerr << "With color " << color_info.name << ":" << std::endl;
        int const color = std::get<0>(color_info.fields);
        std::cerr << "Testing get_red(): ";
        check_equal(get_red(color), std::get<1>(color_info.fields));
        std::cerr << "Testing get_green(): ";
        check_equal(get_green(color), std::get<2>(color_info.fields));
        std::cerr << "Testing get_blue(): ";
        check_equal(get_blue(color), std::get<3>(color_info.fields));
        std::cerr << "Testing get_gray(): ";
        check_equal(get_gray(color), std::get<4>(color_info.fields));
        std::cerr << std::endl;
    }

    std::cerr << "Testing get_RGB(): " << std::endl;
    check_equal(get_RGB(0.0, 0.0, 1.0), 255);
    check_equal(get_RGB(0.5), 8355711);
}

// example in page 6:
void test_to_gray_2_2()
{
    const RGBImage rgb_image({{0x20c0ff, 0x123456},
                              {0xffffff, 0x000000}});

    GrayImage expected({{0.62614375f, 0.20392157f},
                        {1.0f, 0.0f}});

    print_header("test_to_gray_2_2");
    GrayImage gray_image(to_gray(rgb_image));
    check_equal(expected, gray_image);
}

// example in page 12
void test_to_gray_3_3()
{
    const RGBImage rgb_image({{0x888888, 0x666666, 0xcccccc},
                              {0x000000, 0x111111, 0x222222},
                              {0xbbbbbb, 0xffffff, 0x666666}});
    GrayImage expected({{0.533333, 0.4, 0.8},
                        {0, 0.0666667, 0.133333},
                        {0.733333, 1, 0.4}});

    print_header("test_to_gray_3_3");
    GrayImage gray_image(to_gray(rgb_image));
    check_equal(expected, gray_image);
}

void test_to_gray_4_4()
{
    const RGBImage rgb_image({{0x0000FF, 0x00FF00, 0x0000FF, 0x00FF00},
                              {0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000},
                              {0x0000FF, 0x00FF00, 0x0000FF, 0x00FF00},
                              {0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000}});
    GrayImage expected({{0.333333, 0.333333, 0.333333, 0.333333},
                        {0.333333, 0.333333, 0.333333, 0.333333},
                        {0.333333, 0.333333, 0.333333, 0.333333},
                        {0.333333, 0.333333, 0.333333, 0.333333}});

    print_header("test_to_gray_4_4");
    GrayImage gray_image(to_gray(rgb_image));
    check_equal(expected, gray_image);
}

void test_to_gray_5_5()
{
    // Randomly generated:
    const RGBImage rgb_image({{0xbf83ed, 0x253a83, 0xa6ffd0, 0xe78deb, 0xef53be}, 
                              {0x1f7509, 0xbbe1fe, 0xc40123, 0x66e9df, 0x76fef9}, 
                              {0x31b342, 0x236b80, 0xcc3be3, 0x5c21e7, 0xebe9be}, 
                              {0x1a0eb3, 0x1be9bc, 0x4a4d26, 0x290f24, 0xe17cff}, 
                              {0xb3e312, 0xb625b3, 0xd1f260, 0xca12c2, 0xe68b59}});
    GrayImage expected({{{0.730719, 0.295425, 0.822222, 0.793464, 0.669281}, 
                         {0.205229, 0.870588, 0.303268, 0.729412, 0.811765},
                         {0.384314, 0.352941, 0.640523, 0.465359, 0.860131},
                         {0.286275, 0.585621, 0.247059, 0.120261, 0.789542}, 
                         {0.554248, 0.520261, 0.715033, 0.541176, 0.598693}}});

    print_header("test_to_gray_5_5");
    GrayImage gray_image(to_gray(rgb_image));
    check_equal(expected, gray_image);
}

void test_sobelX_1()
{
    GrayImage energy = {{0.0, 0.1, 0.2},
                        {0.5, 0.3, 0.4},
                        {0.8, 0.7, 0.6},
                        {0.9, 0.91, 0.92}};
    GrayImage expected({{0.1, 0.5, 0.4},
                        {-0.4, -0.2, 0.2},
                        {-0.39, -0.48, -0.09},
                        {-0.07, -0.14, -0.07}});

    print_header("test_sobelX_1");
    GrayImage filtered(sobelX(energy));
    check_equal(expected, filtered);
}

void test_sobelX_2()
{
    // Randomly generated:
    GrayImage energy = {{0.861324, 0.903912, 0.668282}, 
                        {0.430332, 0.476503, 0.242629},
                        {0.135265, 0.74065, 0.154726}};
    GrayImage expected = {{0.173936, -0.766829, -0.940765},
                          {0.740317, -0.548986, -1.2893},
                          {1.86233, -0.129319, -1.99165}};

    print_header("test_sobelX_2");
    GrayImage filtered(sobelX(energy));
    check_equal(expected, filtered);
}

void test_sobelX_3()
{
    // Randomly generated:
    GrayImage energy = {{0.768527, 0.0347588, 0.568311}, 
                        {0.0568832, 0.408555, 0.473698}, 
                        {0.80575, 0.223045, 0.956454}};
    GrayImage expected = {{-1.84963, -0.18383, 1.6658}, 
                          {-0.61313, 0.784119, 1.39725}, 
                          {-1.39644, 0.868928, 2.26537}};

    print_header("test_sobelX_3");
    GrayImage filtered(sobelX(energy));
    check_equal(expected, filtered);
}

void test_sobelY_1()
{
    GrayImage energy = {{0.0, 0.1, 0.2},
                        {0.5, 0.3, 0.4},
                        {0.8, 0.7, 0.6},
                        {0.9, 0.91, 0.92}};
    GrayImage expected({{1.7, 1.1, 0.8},
                        {3, 2.4, 1.8},
                        {1.81, 2.14, 2.17},
                        {0.51, 0.84, 1.17}});
    print_header("test_sobelY_1");
    GrayImage filtered(sobelY(energy));
    check_equal(expected, filtered);
}

void test_sobelY_2()
{
    GrayImage energy = {{0.810088, 0.124107, 0.261972},
                        {0.0635178, 0.0824675, 0.428621},
                        {0.312224, 0.876371, 0.0934941}};
    GrayImage expected = {{-2.28135, -0.663201, 0.458308}, 
                          {-0.741328, 0.838185, 0.246829},
                          {1.54002, 1.50139, -0.211478}};
    print_header("test_sobelY_2");
    GrayImage filtered(sobelY(energy));
    check_equal(expected, filtered);
}

void test_sobelY_3()
{
    GrayImage energy = {{0.690886, 0.897239, 0.0535633},
                        {0.292441, 0.655353, 0.347423}, 
                        {0.726867, 0.992577, 0.176277}};
    GrayImage expected = {{-1.43722, -0.588358, 0.639692},
                          {0.203281, 0.349371, 0.463479},
                          {1.6405, 0.937729, -0.176213}};
    print_header("test_sobelY_3");
    GrayImage filtered(sobelY(energy));
    check_equal(expected, filtered);
}

void test_sobel_1()
{
    GrayImage energy = {{0.0, 0.1, 0.2},
                        {0.5, 0.3, 0.4},
                        {0.8, 0.7, 0.6},
                        {0.9, 0.91, 0.92}};
    GrayImage expected({{1.70294, 1.2083, 0.894427},
                        {3.02655, 2.40832, 1.81108},
                        {1.85154, 2.19317, 2.17187},
                        {0.514782, 0.851587, 1.17209}});
    print_header("test_sobel_1");
    GrayImage filtered(sobel(energy));
    check_equal(expected, filtered);
}

void test_smooth_1()
{
    GrayImage energy = {{0.0, 0.1, 0.2},
                        {0.5, 0.3, 0.4},
                        {0.8, 0.7, 0.6},
                        {0.9, 0.91, 0.92}};
    GrayImage expected({{0.15, 0.19, 0.23},
                        {0.42, 0.39, 0.39},
                        {0.711, 0.673, 0.635},
                        {0.862, 0.847, 0.832}});
    print_header("test_smooth_1");
    GrayImage smoothed(smooth(energy));
    check_equal(expected, smoothed);
}

void test_create_graph_1()
{
    constexpr auto MAX_DIST = std::numeric_limits<double>::max();
    GrayImage gray = {{0.0, 0.1, 0.2, 0.3},
                      {0.4, 0.5, 0.6, 0.7}, 
                      {0.8, 0.9, 1.0, 1.1}};
    Graph expected;
    expected.push_back({{4, 5}, 0, MAX_DIST, 0});
    expected.push_back({{4, 5, 6}, 0.1, MAX_DIST, 0});
    expected.push_back({{5, 6, 7}, 0.2, MAX_DIST, 0});
    expected.push_back({{6, 7}, 0.3, MAX_DIST, 0});
    expected.push_back({{8, 9}, 0.4, MAX_DIST, 0});
    expected.push_back({{8, 9, 10}, 0.5, MAX_DIST, 0});
    expected.push_back({{9, 10, 11}, 0.6, MAX_DIST, 0});
    expected.push_back({{10, 11}, 0.7, MAX_DIST, 0});
    expected.push_back({{13}, 0.8, MAX_DIST, 0});
    expected.push_back({{13}, 0.9, MAX_DIST, 0});
    expected.push_back({{13}, 1.0, MAX_DIST, 0});
    expected.push_back({{13}, 1.1, MAX_DIST, 0});
    expected.push_back({{0, 1, 2, 3}, 0, MAX_DIST, 0});
    expected.push_back({{}, 0, MAX_DIST, 0});

    print_header("test_create_graph_1");
    Graph computed = create_graph(gray);
    check_equal(expected, computed);
}

void test_create_graph_2()
{
    // Randomly generated:
    constexpr auto MAX_DIST = std::numeric_limits<double>::max();
    GrayImage gray = {{0.1941, 0.61678, 0.165914, 0.9352, 0.705023}, 
                      {0.900316, 0.683907, 0.44065, 0.91574, 0.563579},
                      {0.791948, 0.871053, 0.737948, 0.172903, 0.751178},
                      {0.593102, 0.264439, 0.131972, 0.131486, 0.76045},
                      {0.147292, 0.519809, 0.16009, 0.0973893, 0.737221}};
    Graph expected;
    expected.push_back({{5, 6}, 0.1941, MAX_DIST, 0});
    expected.push_back({{5, 6, 7}, 0.61678, MAX_DIST, 0});
    expected.push_back({{6, 7, 8}, 0.165914, MAX_DIST, 0});
    expected.push_back({{7, 8, 9}, 0.9352, MAX_DIST, 0});
    expected.push_back({{8, 9}, 0.705023, MAX_DIST, 0});
    expected.push_back({{10, 11}, 0.900316, MAX_DIST, 0});
    expected.push_back({{10, 11, 12}, 0.683907, MAX_DIST, 0});
    expected.push_back({{11, 12, 13}, 0.44065, MAX_DIST, 0});
    expected.push_back({{12, 13, 14}, 0.91574, MAX_DIST, 0});
    expected.push_back({{13, 14}, 0.563579, MAX_DIST, 0});
    expected.push_back({{15, 16}, 0.791948, MAX_DIST, 0});
    expected.push_back({{15, 16, 17}, 0.871053, MAX_DIST, 0});
    expected.push_back({{16, 17, 18}, 0.737948, MAX_DIST, 0});
    expected.push_back({{17, 18, 19}, 0.172903, MAX_DIST, 0});
    expected.push_back({{18, 19}, 0.751178, MAX_DIST, 0});
    expected.push_back({{20, 21}, 0.593102, MAX_DIST, 0});
    expected.push_back({{20, 21, 22}, 0.264439, MAX_DIST, 0});
    expected.push_back({{21, 22, 23}, 0.131972, MAX_DIST, 0});
    expected.push_back({{22, 23, 24}, 0.131486, MAX_DIST, 0});
    expected.push_back({{23, 24}, 0.76045, MAX_DIST, 0});
    expected.push_back({{26}, 0.147292, MAX_DIST, 0});
    expected.push_back({{26}, 0.519809, MAX_DIST, 0});
    expected.push_back({{26}, 0.16009, MAX_DIST, 0});
    expected.push_back({{26}, 0.0973893, MAX_DIST, 0});
    expected.push_back({{26}, 0.737221, MAX_DIST, 0});
    expected.push_back({{0, 1, 2, 3, 4}, 0, MAX_DIST, 0});
    expected.push_back({{}, 0, MAX_DIST, 0});

    print_header("test_create_graph_2");
    Graph computed = create_graph(gray);
    check_equal(expected, computed);
}

void test_shortest_path_1()
{
    constexpr auto MAX_DIST = std::numeric_limits<double>::max();
    Graph graph;
    graph.push_back({{1, 2}, 0.5, MAX_DIST, 0});
    graph.push_back({{0, 3}, 1, MAX_DIST, 0});
    graph.push_back({{3}, 2, MAX_DIST, 0});
    graph.push_back({{2, 4}, 3, MAX_DIST, 0});
    graph.push_back({{4}, 4, MAX_DIST, 0});
    
    print_header("test_shortest_path_1");
    Path computed = shortest_path(graph, 0, 4);
    check_equal({1, 3}, computed);
}

void test_shortest_path_2()
{
    constexpr auto MAX_DIST = std::numeric_limits<double>::max();
    Graph graph;
    graph.push_back({{4, 5}, 0, MAX_DIST, 0});
    graph.push_back({{4, 5, 6}, 0.1, MAX_DIST, 0});
    graph.push_back({{5, 6, 7}, 0.2, MAX_DIST, 0});
    graph.push_back({{6, 7}, 0.3, MAX_DIST, 0});
    graph.push_back({{8, 9}, 0.4, MAX_DIST, 0});
    graph.push_back({{8, 9, 10}, 0.5, MAX_DIST, 0});
    graph.push_back({{9, 10, 11}, 0.6, MAX_DIST, 0});
    graph.push_back({{10, 11}, 0.7, MAX_DIST, 0});
    graph.push_back({{13}, 0.8, MAX_DIST, 0});
    graph.push_back({{13}, 0.9, MAX_DIST, 0});
    graph.push_back({{13}, 1.0, MAX_DIST, 0});
    graph.push_back({{13}, 1.1, MAX_DIST, 0});
    graph.push_back({{0, 1, 2, 3}, 0, MAX_DIST, 0});
    graph.push_back({{}, 0, MAX_DIST, 0});
    
    print_header("test_shortest_path_2");
    Path computed = shortest_path(graph, 0, 12);
    check_equal({}, computed);
    computed = shortest_path(graph, 0, 13);
    check_equal({4, 8}, computed);
}

void test_find_seam_1()
{
    GrayImage energy = {{0.0, 0.1, 0.2},
                        {0.5, 0.3, 0.4},
                        {0.8, 0.7, 0.6},
                        {0.9, 0.91, 0.92}};
    print_header("test_find_seam_1");
    Path x_coordinates(find_seam(energy));
    check_equal({0, 1, 2, 1}, x_coordinates);
}

void run_unit_tests() 
{
    test_color();
    test_to_gray_2_2();
    test_to_gray_3_3();
    test_to_gray_4_4();
    test_to_gray_5_5();
    //test_sobelX_1();
    //test_sobelX_2();
    //test_sobelX_3();
    //test_sobelY_1();
    //test_sobelY_2();
    //test_sobelY_3();
    //test_sobel_1();
    //test_smooth_1();
    //test_create_graph_1();
    //test_create_graph_2();
    //test_shortest_path_1();
    //test_shortest_path_2();
    //test_find_seam_1();
}

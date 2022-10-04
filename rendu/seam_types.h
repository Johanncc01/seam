#pragma once

#include <string>
#include <vector>

typedef std::vector<std::vector<int>> RGBImage;
typedef std::vector<std::vector<double>> GrayImage;
typedef std::vector<std::vector<double>> Kernel;
typedef std::vector<size_t> Path;

struct Node
{
    std::vector<size_t> successors;
    double costs;
    double distance_to_target;
    size_t predecessor_to_target;
};

typedef std::vector<Node> Graph;

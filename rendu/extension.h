#pragma once
#include "seam.h"
#include "seam_types.h"

/* A UTILISER POUR LE CODAGE EVENTUEL D'EXTENSIONS */

// 1) Fuctions used in task 3 : seam //

size_t get_id(size_t row , size_t col, size_t width);
size_t get_row(size_t id , size_t width);
size_t get_col(size_t id , size_t width);

// 2) Horizontal application of the algorithm //

Graph create_horizontal_graph(const GrayImage &gray);
Path shortest_horizontal_path(Graph &graph, size_t from, size_t to);
Path find_horizontal_seam(const GrayImage &gray);

void test_hightlight_horizontal_seam(std::string const& in_path, int num);
GrayImage highlight_horizontal_seam(const GrayImage &gray, const Path &seam);

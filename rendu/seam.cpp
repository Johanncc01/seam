#include <iostream>
#include <cassert>
#include <limits>
#include <tgmath.h>
#include <vector>
#include <algorithm>

#include "seam.h"
#include "extension.h"

using namespace std;


// ***********************************
// TASK 1: COLOR
// ***********************************

// Returns red component (in the scale 0.0-1.0) from given RGB color.
double get_red(int rgb)
{
    int x(rgb >> 16);
    x = x & 0b11111111;
    double red(x/255.0);
    return red;
}

// Returns green component (in the scale 0.0-1.0) from given RGB color.
double get_green(int rgb)
{
    int x(rgb >> 8);
    x = x & 0b11111111;
    double green(x/255.0);
    
    return green;
}

// Returns blue component (in the scale 0.0-1.0) from given RGB color.
double get_blue(int rgb)
{
    int x(rgb);
    x = x & 0b11111111;
    double blue(x/255.0);
    
    return blue;
}

// Returns the average of red, green and blue components from given RGB color. (Scale: 0.0-1.0)
double get_gray(int rgb)
{
    double red(get_red(rgb));
    double green(get_green(rgb));
    double blue(get_blue(rgb));
    double gray((blue+green+red)/3);    // Doing the average of the 3 values we got, the 3 colors.
    
    return gray;
}

// Returns the RGB value of the given red, green and blue components.
int get_RGB(double red, double green, double blue)
{
    int r(red*255);
    int b(blue*255);
    int g(green * 255);
    int RGB(0b00000000);
    RGB = RGB + r;
    RGB = RGB << 8;
    RGB = RGB + g;
    RGB = RGB << 8;
    RGB = RGB + b;

    return RGB;
}

// Returns the RGB components from given grayscale value (between 0.0 and 1.0).
int get_RGB(double gray)
{
    int g(gray*255);
    int gr(0b00000000);
    gr = gr + g;
    gr = gr << 8;
    gr = gr + g;
    gr = gr << 8;
    gr = gr + g;
    
    return gr ;
}

// Converts  RGB image to grayscale double image.
GrayImage to_gray(const RGBImage& cimage)
{
    const size_t line(cimage.size());
    const size_t col(cimage[0].size());
    GrayImage grimage(line, vector<double>(col));
    
    for (size_t i(0) ; i < line ; ++i ) {
        for (size_t j(0) ; j < col ; ++j) {
            double gr(get_gray(cimage[i][j]));
            grimage[i][j] = gr;
        }
    }

    return grimage;
}

// Converts grayscale double image to an RGB image.
RGBImage to_RGB(const GrayImage& gimage)
{
    const size_t line(gimage.size());
    const size_t col(gimage[0].size());
    RGBImage rgimage(line, vector<int>(col));
    
    for (size_t i(0) ; i < line ; ++i ) {
        for (size_t j(0) ; j < col ; ++j) {
            int rgb(get_RGB(gimage[i][j]));
            rgimage[i][j] = rgb;
        }
    }
    
    return rgimage;
}

// ***********************************
// TASK 2: FILTER
// ***********************************

// Get a pixel without accessing out of bounds
// return nearest valid pixel color
void clamp(long& val, long max)
{
    if (val < 0) {val = 0;}
    if (val >= max) {val = max;}
}

// Convolve a single-channel image with the given kernel.
GrayImage filter(const GrayImage &gray, const Kernel &kernel)
{
    GrayImage filteredgray(gray);
    const size_t hauteur_gray(gray.size());
    const size_t largeur_gray(gray[0].size());
    const size_t taille_kernel(kernel.size());
    
    const size_t max_index1(hauteur_gray-1);
    const size_t max_index2(largeur_gray-1);
    const long int indice_kernel(-(taille_kernel / 2)-1);
    long int indice_kernel1;
    long int indice_kernel2;
    
    for (size_t i(0) ; i < hauteur_gray ; ++i){                         // Browse through all the lines of pixels of gray
        for (size_t j(0) ; j < largeur_gray ; ++j){                     // Browse through all the columns of pixels of gray
            long double somme(0.0);
            indice_kernel1 = indice_kernel;
            for (size_t k(0) ; k < taille_kernel ; ++k) {               // Browse through all the lines of the kernel
                    indice_kernel2 = indice_kernel;
                    ++indice_kernel1;
                for (size_t c(0) ; c < taille_kernel ; ++c) {           // Browse through all the columns of the kernel
                    ++indice_kernel2;
                    long int index1(i + indice_kernel1);                // index1 and index2 are used to access all the adjacent elements of the pixel[i][j]
                    long int index2(j + indice_kernel2);
                    clamp(index1, max_index1);                          // Borderline cases
                    clamp(index2, max_index2);
                    somme += (kernel[k][c])*gray[index1][index2];
                    }
            }
         filteredgray[i][j] = somme;                                    // Assigns the sum to the pixel[i][j]
        }
    }
    return filteredgray;
}

// Smooth a single-channel image
GrayImage smooth(const GrayImage &gray)
{
    GrayImage smoothgray(gray);
    const Kernel smooth(
                  { {0.1 , 0.1 , 0.1},
                    {0.1 , 0.2 , 0.1},
                    {0.1 , 0.1 , 0.1} }
                  );

    smoothgray = filter(smoothgray , smooth);

    return {smoothgray};
}

// Compute horizontal Sobel filter

GrayImage sobelX(const GrayImage &gray)
{
    GrayImage sobel_x(gray);
    const Kernel sobel_1(
                  { {-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1} }
                    );

    sobel_x = filter(sobel_x, sobel_1);

    return sobel_x;
}

// Compute vertical Sobel filter

GrayImage sobelY(const GrayImage &gray)
{
    GrayImage sobel_y(gray);
    const Kernel sobel_2(
                  { {-1, -2, -1},
                    {0, 0, 0},
                    {1, 2, 1} }
                    );

    sobel_y = filter(sobel_y, sobel_2);

    return sobel_y;
}

// Compute the magnitude of combined Sobel filters

GrayImage sobel(const GrayImage &gray)
{
    const GrayImage sobel_x(sobelX(gray));
    const GrayImage sobel_y(sobelY(gray));
    GrayImage sobel_final(gray);

    for (size_t i(0); i < gray.size(); ++i) {
        for (size_t j(0); j < gray[0].size(); ++j) {
            sobel_final[i][j] = sqrt((sobel_x[i][j]*sobel_x[i][j])+(sobel_y[i][j]*sobel_y[i][j]));
        }
    }
    
    return sobel_final;
}

// ************************************
// TASK 3: SEAM
// ************************************

Graph create_graph(const GrayImage &gray)
{
    const long double INF(numeric_limits<double>::max());    // Declarating useful constants
    const size_t hauteur(gray.size());
    const size_t largeur(gray[0].size());
    const size_t maxId((hauteur*largeur)-1);
    const size_t startId(maxId + 1);                    // Calculating start and end ids from gray's size
    const size_t endId(maxId + 2);
    const size_t tailleId(endId+1);
    
    Graph graph(tailleId);                              // Creating an empty graph of desired size
    
    for(size_t id(0); id < tailleId ; ++id) {
        size_t row(get_row(id, largeur));               // Getting row and columns from id to deal with borderline cases
        size_t col(get_col(id, largeur));
        Path successors;
        
        if (id == startId){                                     // Initializing the starting node
            graph[id].costs = 0;
            for (size_t i(0); i < largeur; ++i){                // Adding all ids from row 0 as succesors of startId
                successors.push_back(get_id(0, i, largeur));
            }
            
        } else if (id == endId){                                // Initializing the ending node
            graph[id].costs = 0;                                // No successors because in last position
            
        } else {
            graph[id].costs = gray[row][col];       // Each point's cost corresponds to the value in the gray matrix corresponding
        
            if (row == hauteur-1){                                          // If the node is in the last row (successor = end node)
                successors.push_back(endId);
            } else if (col == 0){                                           // If the node is in the first column
                successors.push_back(get_id(row+1, col, largeur));
                successors.push_back(get_id(row+1, col+1, largeur));
            } else if (col == largeur-1){                                   // If the node is in the last column
                successors.push_back(get_id(row+1, col-1, largeur));
                successors.push_back(get_id(row+1, col, largeur));
            } else {                                                        // Regular cases
                successors.push_back(get_id(row+1, col-1, largeur));
                successors.push_back(get_id(row+1, col, largeur));
                successors.push_back(get_id(row+1, col+1, largeur));
            }
        }
        graph[id].distance_to_target = INF;
        graph[id].predecessor_to_target = 0;        // Properties common to all nodes, and applying manipulated vector (successors) to the struct
        graph[id].successors = successors;
    }
    return graph;
}

// Return shortest path from Node from to Node to
// The path does NOT include the from and to Node
Path shortest_path(Graph &graph, size_t from, size_t to)        // This fuction doesn't work (on Windows) without long doubles for costs and distance_to_target.
{
    Path pathfinder;
                    
    size_t startId(graph.size()-2);
    if (to == startId){                                         // Checking if startId is the end of desired path. If so, return an empty path, because no paths exist between them.
        return pathfinder;
    }
    
    graph[from].distance_to_target = graph[from].costs;         // Beginning of Dijkstra's algorithm
    bool modified(true);
    while (modified) {
        modified = false;
        for (size_t i(0) ; i < graph.size() ; ++i) {
            for (size_t j(0) ; j < (graph[i].successors).size() ; ++j) {
                    size_t id((graph[i]).successors[j]);
                if (graph[id].distance_to_target > graph[i].distance_to_target + (graph[id]).costs) {       // Condition to modify the value disance_to_target
                    (graph[id].distance_to_target) = (graph[i].distance_to_target + (graph[id]).costs);
                    (graph[id]).predecessor_to_target = i;                                                  // Assignment of new best predecessor
                    modified = true;
                    }
                }
            }
        }

    size_t index(to);
    while (index != from){                                  // Retrieving shortest path from the modified graph, using the bests predecessors
        index = ((graph[index]).predecessor_to_target);
        if (index != from) {
            pathfinder.push_back(index);
        }
    }
    
    reverse(pathfinder.begin(), pathfinder.end());         // As the path is found backwards, this instruction puts it the right way
    
    return pathfinder;
}


Path find_seam(const GrayImage &gray)
{
    Graph graph (create_graph(gray));                                           // Generate graph from gray image
    Path pathseeker (shortest_path(graph , graph.size()-2 , graph.size()-1));   // Finds the shortest path, from up (startId) to bottom (endId)
    Path seam;
    for (size_t i(0) ; i < pathseeker.size() ; ++i ) {
        seam.push_back(get_col(pathseeker[i],gray[0].size()));                  // Computes column for a path of ids to form the final seam
    }
    return seam;
}

// ***********************************
// TASK 3 provided functions
// Highlight or remove seam from RGB or gray image
// ***********************************

// Draw a seam on a gray image
// return a new gray image with the seam in black
GrayImage highlight_seam(const GrayImage &gray, const Path &seam)
{
    GrayImage result(gray);
    // Paint seam in black
    for (size_t row(0); row < seam.size(); ++row) {
        result[row][seam[row]] = 0;
    }
    return result;
}

// Draw a seam on an RGB image
// return a new RGB image with the seam in blue
RGBImage highlight_seam(const RGBImage &image, const Path &seam)
{
    RGBImage result(image);
    // Paint seam in blue
    for (size_t row(0); row < seam.size(); ++row) {
        result[row][seam[row]] = 0x000ff;
    }
    return result;
}

// Remove specified seam from a gray-scale image
// return the new gray image (width is decreased by 1)

GrayImage remove_seam(const GrayImage &gray, const Path &seam)
{
    GrayImage result(gray);
    for (size_t row(0); row < seam.size(); ++row) {
        result[row].erase(result[row].begin() + seam[row]);
    }
    return result;
}


// Remove specified seam from an RGB image
// return the new RGB image (width is decreased by 1)
RGBImage remove_seam(const RGBImage &image, const Path &seam)
{
    RGBImage result(image);
    for (size_t row(0); row < seam.size(); ++row) {
        result[row].erase(result[row].begin() + seam[row]);
    }
    return result;
}

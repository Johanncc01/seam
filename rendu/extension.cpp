#include "extension.h"
#include "seam.h"
#include "helper.h"
#include <algorithm>
using namespace std;

/* A UTILISER POUR LE CODAGE EVENTUEL D'EXTENSIONS */

// ************************************
// 1) Fuctions used in part 3 (seam)
// ************************************

size_t get_id(size_t row , size_t col, size_t width) {
return(row*width+col);
}

size_t get_row(size_t id , size_t width) {
return(id / width);
}

size_t get_col(size_t id , size_t width) {
return(id % width);
}

// *******************************************
// 2) Horizontal application of the algorithm
// *******************************************

// Create an horizontal graph instead, i.e. startId is on the left, and endId is on the right.
Graph create_horizontal_graph(const GrayImage &gray)
{
    const long double INF(numeric_limits<double>::max());
    const size_t hauteur(gray.size());
    const size_t largeur(gray[0].size());
    const size_t maxId((hauteur*largeur)-1);
    const size_t startId(maxId + 1);                    // Calculating start and end ids from gray's size
    const size_t endId(maxId + 2);
    const size_t tailleId(endId+1);
    
    Graph graph(tailleId);                              
    
    for(size_t id(0); id < tailleId ; ++id) {
        size_t row(get_row(id, largeur));               // Getting row and columns from id to deal with borderline cases
        size_t col(get_col(id, largeur));
        Path successors;
        
        if (id == startId){
            graph[id].costs = 0;
            for (size_t i(0); i < hauteur; ++i){                // Adding all ids from column 0 (instead of row 0) as successors of startId
                successors.push_back(get_id(i, 0, largeur));
            }
            
        } else if (id == endId){
            graph[id].costs = 0;                                // No successors because in last position
            
        } else {
            graph[id].costs = gray[row][col];
        
            if (col == largeur-1){                                          // If the node is in the last column (instead of row)
                successors.push_back(endId);
            } else if (row == 0){                                           // If the node is in the first row (instead of column)
                successors.push_back(get_id(row, col+1, largeur));
                successors.push_back(get_id(row+1, col+1, largeur));
            } else if (row == hauteur-1){                                   // If the node is in the last row (instead of column)
                successors.push_back(get_id(row-1, col+1, largeur));
                successors.push_back(get_id(row, col+1, largeur));
            } else {                                                        // Regular cases
                successors.push_back(get_id(row-1, col+1, largeur));
                successors.push_back(get_id(row, col+1, largeur));
                successors.push_back(get_id(row+1, col+1, largeur));
            }
        }
        graph[id].distance_to_target = INF;
        graph[id].predecessor_to_target = 0;        // Properties common to all nodes
        graph[id].successors = successors;
    }
    return graph;
}

// Return shortest path from Node from to Node to
// The path does NOT include the from and to Node
Path shortest_horizontal_path(Graph &graph, size_t from, size_t to)        // This fuction doesn't work (on Windows) without long doubles for costs and distance_to_target.
{
    Path pathfinder;
                    
    size_t startId(graph.size()-2);
    if (to == startId){
        return pathfinder;
    }
    
    graph[from].distance_to_target = graph[from].costs;         // Beginning of Dijkstra's algorithm
    bool modified(true);
    while (modified) {
        modified = false;
        for (size_t i(0) ; i < graph.size() ; ++i) {
            for (size_t j(0) ; j < (graph[i].successors).size() ; ++j) {
                    size_t id((graph[i]).successors[j]);
                if (graph[id].distance_to_target > graph[i].distance_to_target + (graph[id]).costs) {
                    (graph[id].distance_to_target) = (graph[i].distance_to_target + (graph[id]).costs);
                    (graph[id]).predecessor_to_target = i;
                    modified = true;
                    }
                }
            }
        }

    size_t index(to);
    while (index != from){                              // Retrieving shortest path from the modified graph, using the bests predecessors
        index = ((graph[index]).predecessor_to_target);
        if (index != from) {
            pathfinder.push_back(index);
        }
    }
    
    reverse(pathfinder.begin(), pathfinder.end());        // As the path is found backwards, this loop puts it the right way
    
    return pathfinder;
}

Path find_horizontal_seam(const GrayImage &gray)
{
    Graph graph (create_horizontal_graph(gray));                                           // Generate graph from gray image
    Path pathseeker (shortest_horizontal_path(graph , graph.size()-2 , graph.size()-1));   // Finds the shortest path, from left (startId) to right (endId)
    Path horizontal_seam;
    for (size_t i(0) ; i < pathseeker.size() ; ++i ) {
        horizontal_seam.push_back(get_row(pathseeker[i], gray[0].size()));                  // Computes rows for a path of ids to form the final seam
    }
    return horizontal_seam;
}


// *********************************
// Test functions for extension 2)
// *********************************


void test_hightlight_horizontal_seam(std::string const& in_path, int num)
{
    RGBImage image(read_image(in_path));
    if (!image.empty()) {
        GrayImage gray_image(to_gray(image));
        for (int i = 0; i < num; ++i) {
            GrayImage sobeled_image(sobel(smooth(gray_image)));
            Path seam = find_horizontal_seam(sobeled_image);                            // Adapted fuction calls
            gray_image = highlight_horizontal_seam(gray_image, seam);
        }
        write_image(to_RGB(gray_image), "test_highlighted_horizontal_seam.png");
    }
}


GrayImage highlight_horizontal_seam(const GrayImage &gray, const Path &seam)
{
    GrayImage result(gray);
    // Paint seam in black
    for (size_t col(0); col < seam.size(); ++col) {
        result[seam[col]][col] = 0;                                                     // Adapted assignment to modify rows instead of columns
    }
    return result;
}

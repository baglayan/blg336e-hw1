/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * Meriç Bağlayan
 * 150190056
*/

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
* * * * * * * * * * * * * * * * * */

#include <iostream>         
#include <vector>
#include <fstream>
#include <sstream>       
#include <stack>
#include <queue>
#include <chrono>       // For time measurement

/* INCLUDE ANY OTHER NECESSARY LIBRARIES HERE */
/* START YOUR CODE HERE */
#include <tuple>        // For std::tie
#include <numeric>      // For std::iota
#include <algorithm>    // For std::lower_bound

#define GRAY 0          // for bfs and dfs
#define BLACK -1
#define NODE make_pair(row, col)

#define RESOURCE_ID_MAX 5
/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */

/**
 * @brief Get the Map Dimensions object
 * 
 * @param map The map represented as a 2D vector of integers.
 * @return A tuple map dimensions (rows, columns).
 */
pair<int, int> getMapDimensions(vector<vector<int>> map)
{
    //      rows        columns
    return {map.size(), map[0].size()};
}

/**
 * @brief Get adjacent nodes of the current node,
 *        while considering the circular nature of the map.
 * 
 * @param node The node represented as a pair of integers.
 * @param rows The total number of rows in the map.
 * @param cols The total number of columns in the map.
 * @return A pair vector with the adjacent nodes.
 */
vector<pair<int,int>> getAdjacent(pair<int,int> node, int rows, int cols)
{
    pair<int,int> top, left, bottom, right;

    top = make_pair(node.first, ((node.second - 1) % cols + cols) % cols);
    left = make_pair(((node.first - 1) % rows + rows) % rows, node.second);
    bottom = make_pair(node.first, ((node.second + 1) % cols + cols) % cols);
    right = make_pair(((node.first + 1) % rows + rows) % rows, node.second);

    return {top, left, bottom, right};
}
/* END YOUR CODE HERE */



/**
 * Perform Depth-First Search (DFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>>& map, int row, int col, int resource) {

    /* START YOUR CODE HERE */
    int colonySize = 0;

    // Get the dimensions of the map.
    int rows, cols;
    tie(rows, cols) = getMapDimensions(map);

    // Auxiliary map to keep track of node status.
    vector<vector<int>> color;
    color = map;

    // Stack to keep track of nodes to be processed.
    stack<pair<int,int>> s;
    s.push(NODE);

    // While there are nodes to be processed:
    while(!s.empty())
    {
        // Get the top node of the stack.
        pair<int,int> u = s.top();
        s.pop();
        
        // If the node is visited or has a different resource type, skip it.
        if (color[u.first][u.second] <= GRAY || map[u.first][u.second] != resource)
        {
            continue;
        }

        // Color the current node as GRAY.
        // GRAY denotes that the node is visited but not yet fully processed.
        color[u.first][u.second] = GRAY;

        // Get adjacent nodes of the current node.
        vector<pair<int,int>> adj = getAdjacent(u, rows, cols);
        
        for (const auto &v : adj)
        {
            // If the adjacent node is not visited and has the same resource type:
            if(color[v.first][v.second] > GRAY && map[v.first][v.second] == resource)
            {
                // Push the adjacent node into to the stack.
                s.push(make_pair(v.first, v.second));
            }
        }

        // Color u as BLACK, as it is fully processed.
        color[u.first][u.second] = BLACK;
        map[u.first][u.second] = BLACK;

        // Increment the colony size.
        colonySize++;
    }

    return colonySize;
    /* END YOUR CODE HERE */

}


/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>>& map, int row, int col, int resource) {
    
    /* START YOUR CODE HERE */
    int colonySize = 0;

    // Get the dimensions of the map.
    int rows, cols;
    tie(rows, cols) = getMapDimensions(map);

    // Auxiliary map to keep track of node status.
    vector<vector<int>> color;
    color = map;

    // Color the current node as GRAY.
    // GRAY denotes that the node is visited but not yet fully processed.
    color[row][col] = GRAY;

    // Queue to keep track of nodes to be processed.
    queue<pair<int,int>> q;

    // Push the current node to the queue.
    q.push(NODE);

    // While there are nodes to be processed:
    while(!q.empty())
    {
        // Get the front node of the queue.
        pair<int,int> u = q.front();
        q.pop();

        // Get adjacent nodes of the current node.
        vector<pair<int,int>> adj = getAdjacent(u, rows, cols);

        for (const auto &v : adj)
        {
            // If the adjacent node is not visited and has the same resource type:
            if (color[v.first][v.second] > GRAY && map[v.first][v.second] == resource)
            {
                // Color the adjacent node as GRAY and push it to the queue.
                color[v.first][v.second] = GRAY;
                q.push(v);
            }
        }

        // Color u as BLACK, as it is fully processed.
        color[u.first][u.second] = BLACK;
        map[u.first][u.second] = BLACK;

        // Increment the colony size.
        colonySize++;
    }

    return colonySize;
    /* END YOUR CODE HERE */

}

/**
 * Finds the top-k largest colonies in a given map.
 *
 * @param map The map represented as a 2D vector of integers. Visited cells have the value 0.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>>& map, bool useDFS, unsigned int k) {

    auto start = high_resolution_clock::now();      // Start measuring time
    
    /* START YOUR CODE HERE */

    // Vector of pairs that will hold top k largest colonies.
    vector<pair<int, int>> result;

    // Return result as is (empty) if the map too is empty.
    if (map.empty())
    {
        return result;
    }

    // Allow specification of arbitrary number of resource types.
    vector<int> resourceTypes(RESOURCE_ID_MAX);

    // Fill up resourceTypes with increasing integers.
    iota(resourceTypes.begin(), resourceTypes.end(), 1);

    // Get the dimensions of the map.
    int rows, cols;
    tie(rows, cols) = getMapDimensions(map);

    for (const auto rt : resourceTypes)
    {
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                // If it's visited it won't have the resource type number anyway, as they start from 1
                if (map[row][col] == rt)
                {
                    // Calculate colony size using the appropriate algorithm specified from the command line.
                    const auto colonySize = useDFS ? dfs(map, row, col, rt) : bfs(map, row, col, rt);
                    
                    // Initially, result vector is empty, thus "sorted".
                    const auto resultItem = make_pair(colonySize, rt);

                    // Find the appropriate place to insert the new item.
                    const auto place = lower_bound
                    (
                        result.begin(), result.end(), resultItem,
                        [](pair<int, int> a, pair<int,int> b)
                        {
                            return
                            (
                                a.first == b.first
                                ? a.second < b.second
                                : a.first > b.first
                            );
                        }
                    );

                    // Insert new item to its appropriate place.
                    result.insert(place, resultItem);
                }
            }
        }
    }

    // Erase the trailing, unsolicited elements.
    if(k < result.size())
    {
        result.erase(result.begin() + k, result.end());
    }

    /* END YOUR CODE HERE */

    auto stop = high_resolution_clock::now();                       // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    /* START YOUR CODE HERE */
    return result;
    /* END YOUR CODE HERE */

}

// Main function
int main(int argc, char* argv[]) {

    // Check the command line arguments
    if (argc != 4) {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);

    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();   

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty()) {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}

/*  Travelling Salesman using Branching Nearest Neighbor
 *  MA-430 - Graph Theory and Combinatorics
 *  Kyle Shepard
 *  May 17, 2021
 */

#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <stdlib.h>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::tuple;

//define our maximum float macro as placeholder in minimum weight calculations
#define MAX_FLOAT 3.402823466e+38F

// pass 2 array by reference and randomly generate symmetric adjacency matrix for Kn graph
void generateMatrix(float** &mat, int size){

    mat = new float*[size]; // allocate memory for first layer of matrix
    float high = 25.0f; // upper bound for number generation
    float low = 5.0f; // lower bound for number generation
    srand((unsigned int)time(NULL)); // seed pseudo-random number generation

    // allocate memory for second dimension of matrix
     for(int i = 0; i < size; i++){
        mat[i] = new float[size]; // matrix is size (n x n)
     }

    // generate edge weights between vertices
    for(int i = 0; i < size; i++){

        mat[i][i] = 0.0f; // length to itself always zero
        for(int j = i + 1; j < size; j++){ // traverse over main diagonal

            float random = low + static_cast <float> ( (nearbyintf(rand() / static_cast <float> (RAND_MAX/(high - low)) * 10.0f)) / 10.0f ); // generate edge weight and round to one decimal place
            mat[i][j] = random; // apply edge weight
            mat[j][i] = random; // ...symmetrically
        }
    }
}

//returns the shortest path found using the nearest neighbor algorithm with branching for ties
vector<tuple<int, float>> nearestNeighborRecursive(float** &mat, int first, int v, vector<int> remaining){

    //no other options left
    if(remaining.size() == 0){

        return {{first, mat[first][v]}};
    }

    //find nearest neighbor(s)
    vector<int> nearest = {}; //vector in case of ties
    float min = MAX_FLOAT; //keep track of min value separately

    for(int i = 0; i < remaining.size(); i++){ //for all remaining vertices

        float weight = mat[remaining[i]][v]; //grab immediate cost of travelling to this vertex

        if(weight < min){ //if we have a new lowest candidate
            nearest = {remaining[i]}; //replace with new lowest cost vertex
            min = weight; //update weight to reflect new minimum
        } else if (weight == min){ //if there is a tie
            nearest.push_back(remaining[i]); //place it in a list with other matching cost branches
        }
        //else ignore because the weight is too large
    }

    //find lowest total weight of all ties. call function recursively
    vector<tuple<int, float>> shortestFromTies = {{-1, MAX_FLOAT}};

    for(int i = 0; i < nearest.size(); i++){ //for all ties

        vector<int> newRemaining = remaining; //temporary vector to hold potential remaining vertices if we choose this vertex
        newRemaining.erase(std::remove(newRemaining.begin(), newRemaining.end(), nearest[i]), newRemaining.end()); //remove this vertex from our temporary vector
        vector<tuple<int, float>> result = nearestNeighborRecursive(mat, first, nearest[i], newRemaining); //call nearest neighbor function recursively and store
        result.insert(result.begin(), {nearest[i], std::get<1>(result[0]) + min}); //place this vertex at the beginning of the sub-path with new total weight value

        if(std::get<1>(result[0]) < std::get<1>(shortestFromTies[0])){ //if tie still exists, choose path with lowest index vertex, which is the path calculated first, hence the less than
            shortestFromTies = result;
        }
    }

    return shortestFromTies;
}

//wrapper for nearest neighbor function setting up arguments
vector<tuple<int, float>> nearestNeighbor(float** &mat, int startVertex, int size){

    vector<int> remaining(size); // initialize vector of remaining vertices
    std::iota (std::begin(remaining), std::end(remaining), 0); // initialize with values 0 - size
    remaining.erase(remaining.begin() + startVertex); // remove start vertex

    return nearestNeighborRecursive(mat, startVertex, startVertex, remaining); //return result of algorithm
}

int main(){

    system("Color 01"); //for fun :)

    cout << endl
         << "-----------------------------------------------------" << endl
         << "| Branching Nearest Neighbor for Traveling Salesman |" << endl
         << "-----------------------------------------------------" << endl
         << "                                  | By Kyle Shepard |" << endl
         << "                                  -------------------" << endl
         << endl;

    // aribitrary size graph
    float** adjMat;

    //take in size of matrix from keyboard and check that it is a valid integer
    cout << "Input graph size:" << endl;
    int size = 0;
    cin >> size;

    while(size <=0 || !cin.good()){

        cin.clear();
        cin.ignore();
        cout << "Please put in an integer greater than zero" << endl;
        cin >> size;
    }
    cin.clear();
    cin.ignore();

    //create our matrix with custom size
    generateMatrix(adjMat, size);

    // print generated matrix
    cout << "Adjacency Matrix:\n";
    if(size <= 100){
        for(int i = 0; i < size; i++){
        cout << "| ";
            for(int j = 0; j < size; j++){
                cout << adjMat[i][j] << ", ";
            }
            cout << endl << endl;
        }
    } else {
        cout << "Matrix too large to print" << endl;
    }

    cout << "Proceeding to nearest neighbor..." << endl << endl;

    //get input for starting vertex from keyboard and check that it is a valid integer
    cout << "Input start vertex (-1 to try all options):" << endl;
    int startVertex = -1;
    cin >> startVertex;

    while(startVertex >= size || !cin.good()){
        cin.clear();
        cin.ignore();
        cout << "Please put in an integer between 0 and " << size << ", or -1 to test all vertices" << endl;
        cin >> startVertex;
    }

    //initialize placeholder path with very large weight to ensure any actual solution will overwrite it
    vector<tuple<int, float>> bestPath = {{-1, MAX_FLOAT}};

    if(startVertex < 0){ //test all possible start vertices

        //check progress (useful for large graphs)
        int percentage = 0;
        cout << "Progress:\n\t0%\n";

        //nearest neighbor starting at all vertices
        for(int i = 0; i < size; i++){

            //get best path starting from vertex i as "home city"
            vector<tuple<int, float>> path =  nearestNeighbor(adjMat, i, size);

            //check if solution is better than a previous solution (or placeholder)
            if(std::get<1>(path[0]) < std::get<1>(bestPath[0])){
                bestPath = path;
            }

            //calculate progress as integer (1-100)
            int progress = static_cast<int>(static_cast<float>(i) / static_cast<float>(size) * 100.0f);

            //if progress percentage value as integer changes, then update progress indicator
            if(progress > percentage){
                percentage = progress;
                cout << "\t" << percentage << "%" << endl;
            }
        }
    }
    else{ //test only the vertex defined by user input
        bestPath = nearestNeighbor(adjMat, startVertex, size);;
    }

    //output best found path (or the first one found of matching minimum total cost)
    cout << "Best path: " << endl;

    cout << "\t" << std::get<0>(bestPath[bestPath.size() - 1]);

    for(int j = 0; j < bestPath.size(); j++){

        cout << " -> " << std::get<0>(bestPath[j]);
    }

    cout << endl
         << "Total Cost:"
         << "\n\t" << std::get<1>(bestPath[0]) << endl;

    cout << endl
         << endl
         << "-------------" << endl
         << "| ~~ END ~~ |" << endl
         << "-------------" << endl
         << endl;

    return 0;
}

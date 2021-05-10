#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <numeric>
// #include <list>

using std::vector;
using std::cout;
using std::tuple;

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

vector<tuple<int, float>> nearestNeighbor(float** &mat, int first, int v, vector<int> remaining){

    return {{1, 5.5f}};

    if(remaining.size() == 0){
        return {{first, mat[first][v]}};
    }
}

int main(){

    cout <<
        "\n-----------------------------------------------------\n"
        "| Branching Nearest Neighbor for Traveling Salesman |\n"
        "-----------------------------------------------------\n\n";

    // aribitrary size graph
    float** adjMat;
    int size = 5;
    generateMatrix(adjMat, size);

    // print generated matrix
    cout << "Adjacency Matrix:\n";
    for(int i = 0; i < size; i++){
        cout << "| ";
        for(int j = 0; j < size; j++){
            cout << adjMat[i][j] << ", ";
        }
        cout << std::endl;
    }
    cout << "\n\n";

    //nearest neighbor starting at all vertices
    for(int i = 0; i < size; i++){

        vector<int> remaining(size); // initialize vector of remaining vertices
        std::iota (std::begin(remaining), std::end(remaining), 0); // initialize with values 0 - size
        remaining.erase(remaining.begin() + i); // remove start vertex

        vector<tuple<int, float>> result = nearestNeighbor(adjMat, i, i, remaining);
    }

    return 0;
}

#include <iostream>
#include <vector>

using std::vector;
using std::cout;

// //aribitrary size graph
vector<vector<double>> adjMat;

void dead(){

}

void generateMatrix(vector<vector<double>> &mat, int size){

    for(int i = 0; i < size; i++){
        mat[i][i] = 0.0;
        for(int j = i + 1; j < size; j++){
            double rand = 0.5;
            mat[i][j] = rand;
            mat[j][i] = rand;
        }
    }
}

int main(){
    int size = 5;
    generateMatrix(adjMat, size);

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cout << adjMat[i][j] << ", ";
        }
        cout << "\b\b" << std::endl;
    }

    std::cout << "Test";

    return 0;
}

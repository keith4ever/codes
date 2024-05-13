//
// Created by keith on 11/16/22.
//

#include <vector>
#include <iostream>

using namespace std;

class IntImage {
private:
    vector<vector<unsigned long long>> sum_image;
public:
    IntImage(vector<vector<int>>& matrix) {
        for(int i = 0; i < matrix.size(); i++){
            sum_image.push_back(vector<unsigned long long>());
            for(int j = 0; j < matrix[i].size(); j++){
                sum_image[i].push_back(0);
                if(i == 0){
                    if(j == 0)
                        sum_image[i][j] = matrix[i][j];
                    else
                        sum_image[i][j] = matrix[i][j] + sum_image[i][j-1];
                } else if (j ==0 ){
                    sum_image[i][j] = matrix[i][j] + sum_image[i-1][j];
                } else
                    sum_image[i][j] = matrix[i][j] + sum_image[i-1][j]
                            + sum_image[i][j-1] - sum_image[i-1][j-1];
                cout << "matrix[" << i << "][" << j << "] = " << sum_image[i][j] << endl;
            }
        }
    }

    ~IntImage(){
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        if(row1 >= row2 || col1 >= col2) return 0;
        int ver = row1 > 0 ? sum_image[row1-1][col2] : 0;
        int hor = col1 > 0 ? sum_image[row2][col1-1] : 0;
        int common = (row1 <= 0 || col1 <= 0)? 0 : sum_image[row1-1][col1-1];

        return (sum_image[row2][col2] - ver - hor + common);
    }
};

int main(){
    vector<vector<int>> input {
            {1,2,3,4},
            {2,3,4,5},
            {3,4,5,6},
            {4,5,6,7}
    };
    auto matrix = new IntImage(input);
    cout << matrix->sumRegion(2, 2, 3, 3) << endl;
}
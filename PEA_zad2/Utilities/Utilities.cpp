//
// Created by obied on 25.11.2024.
//

#include "Utilities.h"

std::pair<Matrix,int> reducedMatrix(const Matrix& matrix) {
    Matrix copy = matrix;
    const int size = copy.getSize();
    int reducedCost = 0;

    // Reduction cost for rows
    for(int i = 0; i < size; i++) {
        int  rowMin = INT_MAX;
        for(int j = 0; j < size; j++) {
            if(copy.getWeight(i, j) < rowMin && copy.getWeight(i,j) >=0) {
                rowMin = copy.getWeight(i, j);
            }
        }
        if(rowMin != INT_MAX && rowMin >= 0) {
            reducedCost += rowMin;
            for (int j = 0; j < size; j++) {
                if (copy.getWeight(i,j) != INT_MAX && copy.getWeight(i,j) >=0) {
                    copy.changeValue(i,j,copy.getWeight(i,j)-rowMin);
                }
            }
        }
    }
    // Reduction cost for columns
    for(int j = 0; j < size; j++) {
        int  colMin = INT_MAX;
        for(int i = 0; i < size; i++) {
            if(copy.getWeight(i, j) < colMin && copy.getWeight(i,j) >= 0) {
                colMin = copy.getWeight(i, j);
            }
        }
        if(colMin != INT_MAX && colMin >= 0) {
            reducedCost += colMin;
            for (int i = 0; i < size; i++) {
                if (copy.getWeight(i,j) != INT_MAX && copy.getWeight(i,j) >= 0) {
                    copy.changeValue(i,j,copy.getWeight(i,j)-colMin);
                }
            }
        }

    }
    return {copy, reducedCost};
}

#include <iostream>
#include "NNA.h"
#include <vector>

int NNA::NNAlgAS(const Matrix &matrix) { //asymmetrical
    int MINIMAL = INT_MAX;

    for(int i = 0; i < matrix.getSize(); i++){          //we start from every point
        std::vector<bool> visited = std::vector<bool>(matrix.getSize(), false);
        std::vector<int> path;

        int firstPoint = i;
        path.push_back(firstPoint);                     //we add the first point to the path
        visited[firstPoint] = true;                     //we set the first Point as visited

        int minimalIndex = 0;                           //index of the minimal path weight
        int current = 0;                                //current path weight
        int goVisit = firstPoint;

        while(path.size() < matrix.getSize()){
            int minimal = INT_MAX;                      //minimal path weight
            for(int j = 0; j < matrix.getSize(); j++){  //we visit other points and look for the smallest cost
                if(j != goVisit && !visited[j]){

                    if(matrix.getWeight(goVisit,j) < minimal){
                        minimal = matrix.getWeight(goVisit,j);
                        minimalIndex = j;
                    }
                    if(matrix.getWeight(j, goVisit) < minimal){
                        minimal = matrix.getWeight(goVisit,j);
                        minimalIndex = j;
                    }
                }
            }
            path.push_back(minimalIndex);
            visited[minimalIndex] = true;
            current += minimal;
            goVisit = minimalIndex;
        }
        current += matrix.getWeight(goVisit, firstPoint);

        if(current < MINIMAL){
            MINIMAL = current;
        }
    }


    return MINIMAL;

}

int NNA::NNAlg(const Matrix &matrix) { //symmetrical
    int MINIMAL = INT_MAX;

    for(int i = 0; i < matrix.getSize(); i++){          //we start from every point
        std::vector<bool> visited = std::vector<bool>(matrix.getSize(), false);
        std::vector<int> path;

        int firstPoint = i;
        path.push_back(firstPoint);                     //we add the first point to the path
        visited[firstPoint] = true;                     //we set the first Point as visited

        int minimalIndex = 0;                           //index of the minimal path weight
        int current = 0;                                //current path weight
        int goVisit = firstPoint;

        while(path.size() < matrix.getSize()){
            int minimal = INT_MAX;                      //minimal path weight
            for(int j = 0; j < matrix.getSize(); j++){  //we visit other points and look for the smallest cost
                if(j != goVisit && !visited[j]){

                    if(matrix.getWeight(goVisit,j) < minimal){
                        minimal = matrix.getWeight(goVisit,j);
                        minimalIndex = j;
                    }
                }
            }
            path.push_back(minimalIndex);
            visited[minimalIndex] = true;
            current += minimal;
            goVisit = minimalIndex;
        }
        current += matrix.getWeight(goVisit, firstPoint);

        if(current < MINIMAL){
            MINIMAL = current;
        }
    }


    return MINIMAL;

}

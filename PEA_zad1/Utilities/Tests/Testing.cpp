
#include "Testing.h"

#include <fstream>
#include <iostream>
#include <ostream>

#include "../../Algorytmy/BruteForceA.h"
#include "../../Algorytmy/NNA.h"
#include "../../Algorytmy/RandomA.h"
#include "C:\Users\obied\CLionProjects\PEA_zad1\Utilities\Macierz\Matrix.h"
#include  "C:\Users\obied\CLionProjects\PEA_zad1\Utilities\Timer\Stoper.h"

void Testing::run() {
    //robienie testów

        //zapisywanie wyników do excela
        //              time
        //sizes:   |  BF   |   NN  |   Rnd  |
        //   5:    |  -    |    -  |    -   |
        //   6:    |  -    |    -  |    -   |
        //   7:    |  -    |    -  |    -   |
        //   ...     ...       ...     ...
    Stoper stoper;
    std::ofstream fileToSave;
    if(fileToSave.good()) {
        std::cout << "File created" << std::endl;
        fileToSave.open("res_AS.xls",std::ios::app);

        int sizes[] = { 2,3, 4, 5, 6, 7, 8, 9, 10,11,12};
        fileToSave  << "asymetryczne \n";
        //fileToSave << "sizes: \t"<< "BF \t"<< "BFWC \t"<<"NN \t"<<"Rnd"<<"\tNNS"<<"\n";
        fileToSave << "sizes: \t"<<"NN \t"<<"\tNNS"<<"\n";
        for(int i : sizes) {
            Matrix matrix;
            matrix.generate(i);
            long long int sredBF=0;
            long long int sredBFWC=0;
            long long int sredNN=0;
            long long int sredNNS=0;
            long long int sredRd=0;
            fileToSave << i << "\t";

            for(int j = 0; j < 100; j++) {
                matrix.fillRandom();
                //matrix.fillRandomSymmetric();

                //Brute Force
                LARGE_INTEGER a = stoper.startTimer();
                const int cost = BruteForceA::bruteForce(matrix);
                LARGE_INTEGER b =stoper.stopTimer();
                sredBF += stoper.diffTime(a,b);

                //Brute Force WC
                a = stoper.startTimer();
                BruteForceA::bruteForceWCyc(matrix);
                b =stoper.stopTimer();
                sredBFWC += stoper.diffTime(a,b);

                //NN
                a = stoper.startTimer();
                NNA::NNAlgAS(matrix);
                b =stoper.stopTimer();
                sredNN += stoper.diffTime(a,b);

                //NNSymmetrical
                a = stoper.startTimer();
                NNA::NNAlg(matrix);
                b =stoper.stopTimer();
                sredNNS += stoper.diffTime(a,b);

                //Random
                //a = stoper.startTimer();
                //RandomA::randomPO(matrix,cost);
                //b =stoper.stopTimer();
                //sredRd += stoper.diffTime(a,b);

            }
            //std::cout << sredBF/100 << "\t"<< sredBFWC/100 << "\t" << sredNN/100 << "\t" << sredRd/100 << sredNNS/100 << "\n";
            fileToSave << sredBF/100 <<"\t"<< sredBFWC/100<< "\t" << sredNN/100 << "\t" <<"" <<"\t" << sredNNS/100 << "\n";
            //fileToSave << sredNN/100 <<"\t"<< sredNNS <<"\n";
            //std::cout << sredNNS;
            std::cout << std::endl;
        }
    }else {
        std::cout << "File not created" << std::endl;
        fileToSave.close();
    }
    fileToSave.close();

}

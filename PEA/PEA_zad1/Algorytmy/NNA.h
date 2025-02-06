
#ifndef PEA_ZAD1_NNA_H
#define PEA_ZAD1_NNA_H
#include "../Utilities/Macierz/Matrix.h"

class NNA {
public:
    static int NNAlg(const Matrix& matrix);
    static int NNAlgAS(const Matrix& matrix);
};


#endif //PEA_ZAD1_NNA_H

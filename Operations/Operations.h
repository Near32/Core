#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../EXP/EXP.h"
#include "../FUNC/FUNC.h"
#include "../VAR/VAR.h"
#include "../MAT/Mat.h"

//Mat<EXP> product(const Mat<EXP>& a, const Mat<EXP>& b);

Mat<EXP> product(const Mat<EXP>& a, const Mat<EXP>& b);
Mat<EXP> regwM(const Mat<EXP>& m);
void toString(const Mat<EXP>& m);
void evaluate(const Mat<EXP>& m);
Mat<EXP> rotation(const EXP& expvar, int axis);

Mat<EXP> derivateV(const Mat<EXP>& v, const VAR& var);
EXP* F2FDerivate(const EXP* f);
EXP* derivateREC(const EXP& exp, const VAR& var);
EXP derivateFUNCREC(const EXP& exp, const VAR& var);
EXP derivateEXPREC(const EXP& exp, const VAR& var);

#endif

#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../EXP/EXP.h"
#include "../FUNC/FUNC.h"
#include "../VAR/VAR.h"
#include "../MAT/Mat.h"

void toString(const Mat<EXP>& m);
void evaluate(const Mat<EXP>& m);

EXP* F2FDerivate(const EXP* f);
EXP* derivateREC(const EXP& exp, const VAR& var);
EXP derivateFUNCREC(const EXP& exp, const VAR& var);
EXP derivateEXPREC(const EXP& exp, const VAR& var);

#endif

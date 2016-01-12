#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../EXP/EXP.h"
#include "../FUNC/FUNC.h"
#include "../VAR/VAR.h"

EXP* F2FDerivate(const EXP* f);
EXP* derivateREC(const EXP& exp, const VAR& var);
EXP derivateFUNCREC(const EXP& exp, const VAR& var);
EXP derivateEXPREC(const EXP& exp, const VAR& var);

#endif

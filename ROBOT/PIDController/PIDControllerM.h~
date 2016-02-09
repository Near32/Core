#ifndef PIDcontroller_H
#define PIDcontroller_H
#include "../Mat/Mat.h"

template<typename T>
class PIDcontrollerM
{
    private :
    T Kp;
    T Ki;
    T Kd;
    
    Mat<T> err;
    Mat<T> err_old;
    Mat<T> err_sum;
    Mat<T> consigne;
    Mat<T> value;
    
    
    public :
    
    PIDcontrollerM();
    PIDcontrollerM(T Kp, T Ki, T Kd);
    ~PIDcontrollerM();
    void reset();
    void setConsigne(const Mat<T>& consigne);
    Mat<T> update(const Mat<T>& currentValue, T dt = (T)0.1);
    void setKp(T Kp);
    void setKi(T Ki);
    void setKd(T Kd);
    void set(T Kp, T Ki,T Kd);

    
};

#endif

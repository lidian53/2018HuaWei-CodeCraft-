
#include"point.h"
#include<iostream>
#include<vector>
#include<cmath>


class LeastSquare{
private:
	double a,b;
public:
	LeastSquare()  
    	{  
            a = 0.0;  
            b = 0.0;  
    	}  
	void Linearreg(VM_Num_Point* vmp,int n,double r)
	{
	    double t1=0,t2=0,t3=0,t4=0;
	    for(int i=0;i<n;i++)
	    {
		t1+=vmp[i].get_day()*vmp[i].get_day();
		t2+=vmp[i].get_day();
		t3+=vmp[i].get_day()*vmp[i].get_vm_num();
		t4+=vmp[i].get_vm_num();

	    }
	    a=(n*t3-t2*t4)/(r*n*n+n*t1-t2*t2);
	    b=(t1*t4-t2*t3+r*n*t4)/(t1*n-t2*t2+r*n*n);
	}
	double getA()
	{
	    return a;
	}
	
	double getB()
	{
	    return b;
	}
};

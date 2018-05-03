
#include"point.h"
#include<iostream>
#include<vector>
#include<cmath>


class LinearReg {
private:
	double a, b;
	double lasta, lastb;
	const double alpha = 0.0011;
public:
	LinearReg()
	{
		a = 0.0;
		b = 0.0;
	}
	void GradentDescent(VM_Num_Point* vmp, int n)
	{
		do {
			lasta = a;
			lastb = b;
			//首先更新a
			for (int i = 0; i < n; i++) {
				double hx = a + b*vmp[i].get_day();
				a = a + alpha*(vmp[i].get_vm_num()-hx);

			}
			//然后更新b
			for (int i = 0; i<n; i++)
			{
				double hx = a + b*vmp[i].get_day();
				b = b + alpha*(vmp[i].get_vm_num() - hx)*vmp[i].get_day();
			}

		} while (fabs(lasta - a)>1e-3&&fabs(lastb - b)>1e-3);//收敛条件
	}
	/*
	void show()
	{
		cout << "a: " << a << endl;
		cout << "b: " << b << endl;
	}
	*/
	double getA()
	{
		return a;
	}
	double getB()
	{
		return b;
	}
};


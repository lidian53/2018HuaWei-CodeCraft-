#ifndef POINT_H
#define POINT_H


class VM_Num_Point{
public:
	int day;
	int vm_num;
        VM_Num_Point()
        {
			day=0;
			vm_num=0;
	    }		
		VM_Num_Point(int day, int vm_num)
		{
			this->day = day;
			this->vm_num = vm_num;
		}
		int get_day()
		{
			return day;
		}
		int get_vm_num()
		{
			return vm_num;
		}
};


#endif

#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"


#include<stdlib.h>
//#include"train_interval_time.h"


void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename,int &train_data_length);



#endif

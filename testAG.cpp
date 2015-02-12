
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include "SC.h"
#define BSIZE 12
using namespace std;

int main(){
cout << "AG test! " << endl;
SC mySC(BSIZE);
int* test = (int*)malloc(sizeof(int) * 144);
int* test2 = (int*)malloc(sizeof(int) * 144);
int* test3 = (int*)malloc(sizeof(int) * 144);
for(int i = 0; i < 144; i++)
{
	test[i] = i;
	test2[i] = -1;
	test3[i] = -1;
}
for(int i = 0; i < 12; i++)
	for(int j = 0; j < 12; j++)
	{
		if(i+j < 12)
			test2[(i+j)*12+j] = test[i*12+j];
		else
			test2[(i+j-12)*12+j] = test[i*12+j];
	}
showArray(test, 144, 12, BSIZE);
cout << "===" << endl;
showArray(test2, 144, 12,BSIZE);
mySC.ag.getAddrArray(12, dia_h2d);
for(int i = 0; i < mySC.ag.arraySize; i++)
	cout << mySC.ag.srcArray[i] << " ";
cout << endl;
for(int i = 0; i < mySC.ag.arraySize; i++)
	cout << mySC.ag.dstArray[i] << " ";
cout << endl;
mySC.diaMap(test, test3, dia_h2d, 12);
showArray(test3, 144, 12, BSIZE);
return 0;
}

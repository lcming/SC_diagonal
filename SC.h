
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <iomanip>
using namespace std;

enum up_or_low{upper, lower};
enum mode{dia_h2d, dia_d2h};

class AG{
private:
	int tSize;
public:
	int* srcArray;
	int* dstArray;
	int arraySize;
	int ls;
	AG(int size);
	void getAddrArray(int matSize, int mode);
	
};
class SC{

private: 
	int bankSize;
	int** bank;
public:
	AG ag;
	SC(int size);
	
	void show();
	void diaRead(int* start, int row, int rr);
	void diaWrite(int* start, int col, int dis, int ul);

	void blkRead(int* start, int mode, int matSize);
	void blkWrite(int* start, int mode, int matSize);
	void blkWriteMainDia(int* start, int mode, int matSize);
	void directRead(int* start, int mode, int matSize);
	void directWrite(int* start, int mode, int matSize);

	void diaMap(int* src, int* dst, int mode, int matSize);


};

void showArray(int* a, int size, int endline, int block);

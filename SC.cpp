
#include "SC.h"
using namespace std;


	AG::AG(int size)
	{
		tSize = size;
	}
	void AG::getAddrArray(int matSize, int mode)
	{
		ls = matSize / tSize;
		arraySize = ls * ls; // dim square
		srcArray = (int*)malloc(sizeof(int) * arraySize);
		dstArray = (int*)malloc(sizeof(int) * arraySize);
		if(mode == dia_h2d)
		{
			int k = 0;
			int j, i, offset;
			for(i = 0; i < ls; i++) // handle read addr, upper
			{
				offset =  matSize*tSize*i; // find start point by offset
				for(j = 0; j+i < ls; j++)
				{
					srcArray[k] = offset; // read addr
					dstArray[k++] = offset + matSize*tSize*j; //write addr
					offset += tSize;
				}
			}
			for(i = 0; i < ls -1; i++)
			{
				offset =  matSize*tSize*(ls - 1 - i);
				offset += tSize * (i+1);
				for(j = 0; j+i < ls -1; j++)
				{
					srcArray[k] = offset; 
					dstArray[k++] = offset - matSize*tSize*(ls - i - j - 1);
					offset += tSize;
				}
			}
		
		}

		else if(mode == dia_d2h)
		{

		}
		else
		{
			cout << "addr gen exceptin" << endl;
			exit(1);
		}
	//	return addrArray;
	}

	
	SC::SC(int size): ag(size)
	{
		bankSize = size;
		bank = (int**)malloc(sizeof(int*) * size);
		for(int i = 0; i < size; i++)
			bank[i] = (int*)malloc(sizeof(int) * size);
	}
	
	void SC::show()
	{
		for(int i = 0; i < bankSize; i++)
		{
			for(int j = 0; j < bankSize; j++)
			{
				cout << setw(4) << bank[i][j] << " ";
			}
		cout << endl;
		}
	}
	void SC::diaRead(int* start, int row, int rr)
	{
		for(int i = 0; i < bankSize; i++)
		{
				bank[row][(i+rr)%bankSize] = *(start++);
		}
	}
	void SC::diaWrite(int* start, int col, int dis, int ul)
	{
		if(ul == upper)
		{
			start += dis;
			for(int i = dis; i < bankSize; i++)
			{
				*(start++) = bank[i][col];
			}
		}
		else if (ul == lower)
		{
			for(int i = 0; i < bankSize-dis; i++)
			{
				*(start++) = bank[i][col];
			}
		}

		else
		{
			cout << "dia write exception" << endl;
			exit(1);
		}
	}
	void SC::blkRead(int* start, int mode, int matSize)
	{
		if(mode == dia_h2d)
		{
			for(int i = 0; i < bankSize; i++)
			{
				diaRead(start, bankSize-1-i, i);
				start += matSize;
			}
		}
		else if(mode == dia_d2h)
		{

		}
		else
		{
			cout << "blk read exception" << endl;
			exit(1);
		}
	}
	void SC::blkWrite(int* start, int mode, int matSize)
	{
		if(mode == dia_h2d)
		{
			for(int i = 0; i < bankSize; i++)
			{
				diaWrite(start-(bankSize-1-i), i, bankSize-1-i, upper);
				start += matSize;
			}
			for(int i = 1; i < bankSize; i++)
			{
				diaWrite(start+i, i-1, i, lower);
				start += matSize;
			}
		}
		else if(mode == dia_d2h)
		{

		}
		else
		{
			cout << "blk write exception" << endl;
		}
	}
	void SC::directRead(int* start, int mode, int matSize)
	{
		if(mode == dia_h2d)
		{
			for(int i = 0; i < matSize; i++) // **
			{
				diaRead(start, matSize-1-i, i); // **
				start += matSize; // **
			}
		}
		else if(mode == dia_d2h)
		{

		}
		else
		{
			cout << "direct read exception" << endl;
			exit(1);
		}
	
	}
	void SC::directWrite(int* start, int mode, int matSize)
	{
		if(mode == dia_h2d)
		{
			for(int i = 0; i < matSize; i++) //**
			{
				diaWrite(start-(matSize-1-i), i, matSize-1-i, upper); //**
				start += matSize;
			}
			start -= matSize*matSize; //** go back
			for(int i = 1; i < matSize; i++) //**
			{
				diaWrite(start+i, i-1, i, lower);
				start += matSize;
			}
		}
		else if(mode == dia_d2h)
		{

		}
		else
		{
			cout << "direct write exception" << endl;
			exit(1);
		}
	}
	void SC::blkWriteMainDia(int* start, int mode, int matSize)
	{
		if(mode == dia_h2d)
		{
			for(int i = 0; i < bankSize; i++)
			{
				diaWrite(start-(bankSize-1-i), i, bankSize-1-i, upper);
				start += matSize;
			}
			start -= matSize*matSize;
			for(int i = 1; i < bankSize; i++)
			{
				diaWrite(start+i, i-1, i, lower);
				start += matSize;
			}
		}
		else if(mode == dia_d2h)
		{

		}
		else
		{
			cout << "blk write exception" << endl;
		}
	}
	
	void SC::diaMap(int* src, int* dst, int mode, int matSize)
	{
		if(matSize <= bankSize)
		{
			directRead(src, mode, matSize);
			directWrite(dst, mode, matSize);
		}
		else
		{
			ag.getAddrArray(matSize, mode);
			int cnt = 0;
			int target = ag.ls;
			for(int i = 0; i < ag.arraySize; i++)
			{
				cnt ++;
				blkRead(src+ag.srcArray[i], mode, matSize);
				if(cnt == target)
				{
					blkWriteMainDia(dst+ag.dstArray[i], mode, matSize);
					cnt = 0;
					target --;
				}
				else
						blkWrite(dst+ag.dstArray[i], mode, matSize);
			}
		}
	}
	



void showArray(int* a, int size, int endline, int block)
{
	for(int i = 0; i < size; i++)
	{
		if(i%(block*endline) == 0)
			cout << endl;
		if(i%endline == 0)
			cout << endl;
		if(i%block == 0)
		cout << "  ";
		cout << setw(4) <<a[i] << " ";
	}
	cout << endl;

}

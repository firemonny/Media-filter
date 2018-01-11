#include <iostream>
#include <fstream>
#include "Medianfilter.h"
int main() 
{
	Medianfilter Firsttest("First.bmp", "Output.bmp");
	Firsttest.applyfilter(3);
	Medianfilter Secondtest("First.bmp", "Output2.bmp");
	Secondtest.applyfilter(4);
	Medianfilter Thirdtest("First.bmp", "Output3.bmp");
	Thirdtest.applyfilter(5);


	return 0;
}
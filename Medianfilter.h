#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H
//Medianfilter recieve the file(.bit) and output bitmap(.bit) 
#include <iostream>
#include "fstream"
#include "string"

class Medianfilter
{
public:
	Medianfilter();
	//PROMISE: Initalialize a medianfilter class with empty inpufilename and empty outputfilename
	
	Medianfilter(std::string inputname, std::string outputname);
	//REQUIRE: 1. String of inputname 2. String of outputname
	//PROMISE: Initalialize a medianfilter class with string of infilename and string of outputfilename
	
	Medianfilter(Medianfilter&src);
	//PROMISE: Copy Constructor for medianfilter
	
	Medianfilter& operator=(const Medianfilter& src);
	//PROMISE: Assign Constructor for medianfilter
	
	~Medianfilter();
	//PROMIS: Destructor;
	
	void Set_Input_path(std::string inputname);
	//REQUIRE:string of input file path
	//PROMISE:change the input file path
	
	void Set_output_path(std::string outputname);
	//REQUIRE:string of output file path
	//PROMISE:change the output file path
	
	std::string Get_Input_path()const;
	//PROMISE: reurn the current input path
	
	std::string Get_Output_path()const;
	//PROMISE: reurn the current output path
	
	int applyfilter(unsigned int k);
	//REQUIRE:unisgned interger k. 
	//PROMISE: The function will open inputfile bitmap file and create a new bitmap filter 
	//The median filter is getting the  k*k nearby value. Sort the k*k set value return the median number
	//If the k*k is odd number. It will take average from the two median number.  
private:
	unsigned char median(unsigned char* src,unsigned int arrayrize); 

	std::string inputfilename;
	std::string outputfilename;
};

#endif // !MEDIANFILTER_H
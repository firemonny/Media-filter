#include "Medianfilter.h"



Medianfilter::Medianfilter() : inputfilename(0), outputfilename(0)
{
	;
}

Medianfilter::Medianfilter(Medianfilter&src)
{
	if (this != &src) 
	{
		inputfilename = src.inputfilename;
		outputfilename = src.outputfilename;
	}
}
Medianfilter::Medianfilter(std::string inputname, std::string outputname) 
{
	inputfilename = inputname;
	outputfilename = outputname;
 }
Medianfilter& Medianfilter::operator=(const Medianfilter& src)
{
	if (this != &src)
	{
		inputfilename = src.inputfilename;
		outputfilename = src.outputfilename;
	}
	return *this;
}

Medianfilter::~Medianfilter()
{
}

//function apply bubble sort in size of array[arraysize] output the median number
unsigned char Medianfilter::median(unsigned char* src, unsigned int arraysize) 
{
	//bubble sort for the array
	for (unsigned int i = 1; i < arraysize; i++)
	{
		for (unsigned int j = i; j<arraysize; j++)
			if (src[i - 1] > src[j])
			{
				unsigned char temp = src[i - 1];
				src[i - 1] = src[j];
				src[j] = temp;
			}
	}
	if ((arraysize % 2) == 1) { return src[arraysize / 2]; } //odd condition
	else
	{
		return (src[arraysize / 2] + src[arraysize / 2 - 1]) / 2; //even condition.
	}
}
void Medianfilter::Set_Input_path(std::string inputname) { inputfilename = inputname; }

void Medianfilter::Set_output_path(std::string outputname) { outputfilename = outputname; }

std::string Medianfilter::Get_Input_path()const { return inputfilename; }

std::string Medianfilter::Get_Output_path()const { return outputfilename; }


//applyfilter is the mainfunction for the median filter
//The function will check if the input file and output file was missing	.
//The median filter used 3 windows value to get the median value and output. 
//The median filter take input from  source bmp file apply median filter to output bmp file.
int Medianfilter::applyfilter(unsigned int k)
{//Checking the expect number of parameters
	if (inputfilename.empty())
	{
		std::cout << "The input filename is empty";
		return -1;
	}
	if (outputfilename.empty())
	{
		std::cout << "The output filename is empty";
		return -1;
	}
	//Initializing input and output files
	std::fstream ifile(inputfilename, std::ios_base::in | std::ios_base::binary);
	if (!ifile.is_open())
	{
		std::cout << "Error opening input file\n";
		return -1;
	}
	std::fstream ofile(outputfilename, std::ios_base::out | std::ios_base::binary);
	if (!ofile.is_open())
	{
		std::cout << "Error creating output file\n";
		return -1;
	}
	//Reading the main header (always 14 bytes)
	unsigned char headerData[14];
	ifile.seekg(0);
	ifile.read((char*)headerData, 14);
	unsigned int fileSize = *(int*)&headerData[2];
	unsigned int offset = *(int*)&headerData[10];

	//Reading image header (only the first twelve bytes -- header size, image width and image height)
	unsigned char imageheaderData[12];
	ifile.read((char*)imageheaderData, 12);
	unsigned int width = *(int*)&imageheaderData[4];
	unsigned int height = *(int*)&imageheaderData[8];

	//Validating the main header against image header information
	unsigned int pixelDataSize = fileSize - offset;
	if (pixelDataSize != width*height * 3)
	{
		std::cout << "This file is not a 24bit Bitmap file\n";
		return -1;
	}
	//Validating the image dimension if not even number return -1
	if (((width*height) % 2) != 0)
	{
		std::cout << "The file is not even number!\n";
		return -1;
	}
	if (k > width || k > height) 
	{
		std::cout << "The k window value exceed the imagine size. Please check" << std::endl;
		return -1;
	}
	//Reading input data into two sections: pixel and non-pixel related data
	unsigned char* nonPixelData = new unsigned char[offset];
	ifile.seekg(0);
	ifile.read((char*)nonPixelData, offset);
	unsigned char* pixelData = new unsigned char[pixelDataSize];
	ifile.read((char*)pixelData, pixelDataSize);
	ifile.close();

	//Applying filter to the data. 
	unsigned int rowSize = width * 3;
	unsigned int jCount = pixelDataSize / rowSize;
	unsigned int iCount = pixelDataSize / jCount;
	//create the constant for windowboundry
	unsigned int windowboundry = k/2;
	//create new container for median filter output file
	unsigned char* outputData = new unsigned char[pixelDataSize];
	for (int j = 0; j < jCount; j++)
	{
		//create the upper and lower window index
		int lowerb = j - windowboundry;
		int upperb = j + windowboundry;
		for (int i = 0; i<iCount; i += 3)
		{
			//create the left and right window index
			int leftb = i - 3*windowboundry;
			int rightb = i + 3*windowboundry;
			unsigned int index = j*iCount + i;
			//Need check if the value on the boundry. 
			//If the index on the boundry. The pixel will not apply on median function.
			//only return origin pixel color number
			if (leftb <0 || rightb>=iCount || lowerb<0 || upperb>=jCount)
			{
				outputData[index] = pixelData[index];
				outputData[index + 1] = pixelData[index + 1];
				outputData[index + 2] = pixelData[index + 2];				
			}
			else 
			{
				//create k*k array to store blue green red separtely
				unsigned char* blue = new unsigned char[k*k];
				unsigned char* green = new unsigned char[k*k];
				unsigned char* red = new unsigned char[k*k];
				unsigned int medianindex = lowerb*iCount + leftb;
				for (int row = 0; row < k; row++)
				{
					for (int column = 0; column < k; column++)
						blue[row*k + column] = pixelData[medianindex + column * 3 + row*iCount];
				}
				for (int row = 0; row < k; row++)
				{
					for (int column = 0; column < k; column++)
						green[row*k + column] = pixelData[(medianindex + column * 3 + row*iCount)+1];
				}
				for (int row = 0; row < k; row++)
				{
					for (int column = 0; column < k; column++)
						red[row*k + column] = pixelData[(medianindex + column * 3 + row*iCount) + 2];
				}
				//import the three array to the median filter function
				unsigned char outputblue = median(blue, k*k);   
				unsigned char outputgreen = median(green, k*k);
				unsigned char outputred = median(red, k*k);

				//output the number in the new outputfile
				outputData[index] = outputblue;
				outputData[index + 1] = outputgreen;
				outputData[index + 2] = outputred;

				//release the memory.
				delete[] blue;
				delete[] green;
				delete[] red;		
			}		
		}
	}

	//Writing both sections into the output file   
	ofile.seekg(0);
	ofile.write((char*)nonPixelData, offset);
	ofile.write((char*)outputData, pixelDataSize);
	ofile.close();

	//Releasing remaining allocated resources
	delete[] nonPixelData;
	delete[] pixelData;
	delete[]outputData;
	//return 1 with no error 
	return 1;
}
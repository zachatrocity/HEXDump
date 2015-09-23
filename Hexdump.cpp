// _   _              _                       
//| | | |            | |                      
//| |_| | _____  ____| |_   _ _ __ ___  _ __  
//|  _  |/ _ \ \/ / _` | | | | '_ ` _ \| '_ \ 
//| | | |  __/>  < (_| | |_| | | | | | | |_) |
//\_| |_/\___/_/\_\__,_|\__,_|_| |_| |_| .__/ 
//                                     | |    
//                                     |_|
// Program by Zach Russell


#include <iostream>
#include <iomanip>
#include <fstream>
#include <istream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::streamsize;
using std::ios;
using std::hex;
using std::uppercase;
using std::setw;
using std::setfill;

//Utility functions
string getFileNameWithExtentsion(string filepath, bool ext);
bool fileExists(const char * filename);


//Program functions
string getOutputFilePathNextToPath(string basePath);


//
// Main function
//
int main() {
	
	string inputFile;
	ifstream fin;

	cout << "Please enter file path to dump" << endl;
	getline(cin, inputFile);

	if (fileExists(inputFile.c_str())){
		//inputfile exsits create and open output file
		ofstream fout(getOutputFilePathNextToPath(inputFile));
		
		//open input file with ios::ate in order to find the length of the file
		fin.open(inputFile.c_str(), ios::in | ios::binary | ios::ate);
		//store the length of the file
		int numBytesInFile = fin.tellg();
		//move the pointer to the start of the file
		fin.seekg(0, ios::beg);

		int numBytesTraversed = 0;

		//variables used to store data and location
		unsigned char buffer[16];
		unsigned long memBlock = 0;
		
		
		//log the name of file we are dumping
		size_t found = inputFile.find_last_of("/\\");
		string inputName = inputFile.substr(found + 1);
		fout << "HEX DUMP FOR FILE: " + inputName << endl << endl;
		
		//tell cout how to handle the output
		fout << hex << setfill('0') << uppercase;
		//while the file is good
		while (fin.good() && numBytesTraversed < numBytesInFile)
		{
			//read the file 16 bytes at a time
			fin.read((char *)buffer, 16);

			//show the memory block;
			fout << setw(8) << memBlock << ": ";

			//loop through the first 16 bytes
			for (short i = 0; i < 16; i++)
			{
				if (numBytesTraversed < numBytesInFile)
				{
					fout << ' ' << setw(2) << int(buffer[i]); 
				}

				numBytesTraversed++;
			}

			//add space between hex and printable characters
			fout << "  ";

			//get printable characters
			if (numBytesTraversed <= numBytesInFile) //if i havent gone over the number of bytes in file
			{
				for (short x = 0; x < 16; x++)
				{
					if (isprint(buffer[x])) //if printable char, print it
						fout << buffer[x];
					else
						fout << ".";		//else print out a period
				}
			}
			else { //I've gone over the number of bytes

				//determine how many bytes were on the final line
				int numBytesOnFinalLine = (numBytesInFile % 16);

				//add white space to fill the remaining bytes
				for (short l = 0; l < (16 - numBytesOnFinalLine); l++)
				{
					fout << "  " << " ";
				}

				//output the ascii values for the bytes on the final line
				for (short x = 0; x < numBytesOnFinalLine; x++)
				{
					if (isprint(buffer[x])) //if printable char, print it
						fout << buffer[x];
					else
						fout << ".";		//else print out a period
				}
			}

			//move to the next block of memory
			memBlock += 16;

			//new line
			fout << endl;
		}
	}
	else {
		//no file at path entered
		cout << "File does not exists" << endl;
	}

	return 0;
}


//
//getOutputFilePathNextToPath function - returns the output file path next to the given parameter inputPath
// with the same name but the extension .dmp
string getOutputFilePathNextToPath(string inputPath)
{
	size_t found = inputPath.find_last_of("/\\");
	string outputPath = inputPath.substr(0, found) + "\\";  //append a back slash to the end of the path
	string outputName = inputPath.substr(found + 1);
	
	if (outputName.find(".")) // file name has extension
	{
		//strip off the extension and add .dmp
		size_t period = outputName.find_last_of(".");
		outputName = outputName.substr(0, period) + ".dmp";
	}
	else {
		//no file extension so just add .dmp
		outputName += ".dmp";
	}

	return outputPath + outputName;
}


//
//fileExists function - a utility function used to check if a file exists
//
bool fileExists(const char* filename)
{
	ifstream infile(filename);
	return (bool)infile;
}
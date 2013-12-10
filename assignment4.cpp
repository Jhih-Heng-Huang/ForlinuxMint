#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>

struct ineq
{
	double coefX;
	double coefY;
	double constant;
};

void readInput( std::vector<ineq>& buffer, std::string inputFile )
{
	std::ifstream file;
	ineq area;
	int numArea;

	file.open(inputFile.c_str());

	file >> numArea;

	for( int i = 0; i < numArea; ++i )
	{
		file >> area.coefX >> area.coefY >> area.constant;
		buffer.push_back(area);
	}

	file.close();
}



int main()
{
	std::vector<ineq> buffer;

	readInput(buffer,"input.txt");


	
	return 0;
}
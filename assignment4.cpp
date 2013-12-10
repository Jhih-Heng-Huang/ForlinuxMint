#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>

struct ineq
{
	int coefX;
	int coefY;
	int constant;
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

	for( size_t i = 0; i < buffer.size(); ++i )
	{
		std::cout << buffer[i].coefX << "\t" << buffer[i].coefY << "\t" << buffer[i].constant << std::endl;
	}
	
	return 0;
}
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>

// coefX * x + coefY * y <= constant
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

// pruning & search for 2D linear programming
double linearProg_2D_PS( std::vector<ineq>& buffer, double x_l, double x_r )
{
	return 0;
}

int main()
{
	std::vector<ineq> buffer;

	readInput(buffer,"input.txt");

	// pruning & search for 2D linear programming
	linearProg_2D_PS( buffer, -HUGE_VAL, HUGE_VAL );
	

	return 0;
}
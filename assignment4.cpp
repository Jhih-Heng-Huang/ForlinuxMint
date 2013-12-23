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

// data type of 3 classes of constraints
struct constraint
{
	std::vector<ineq> xAxisConstraints;
	std::vector<ineq> ubLines;
	std::vector<ineq> lbLines;
};

// divide the input to 3 classes: I^0, I^+, I^-
void readInput( constraint& buffer, std::string inputFile )
{
	std::ifstream file;
	ineq area;
	int numArea;

	file.open(inputFile.c_str());

	file >> numArea;

	for( int i = 0; i < numArea; ++i )
	{
		file >> area.coefX >> area.coefY >> area.constant;
		
		if( area.coefY == 0 )
		{
			buffer.xAxisConstraints.push_back(area);
		}
		else if( area.coefY > 0 )
		{
			buffer.ubLines.push_back(area);
		}
		else
		{
			buffer.lbLines.push_back(area);
		}
	}

	file.close();
}

// pruning & search for 2D linear programming
double linearProg_2D_PS( constraint& buffer, double x_l, double x_r )
{
	return 0;
}

int main()
{
	constraint buffer;

	// divide the input to 3 classes: I^0, I^+, I^-
	readInput(buffer,"input.txt");



	// pruning & search for 2D linear programming
	std::cout << linearProg_2D_PS( buffer, -HUGE_VAL, HUGE_VAL )  << std::endl;
	

	return 0;
}
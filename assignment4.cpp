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
	std::vector<ineq> ubLines;
	std::vector<ineq> lbLines;
	double u_1;
	double u_2;
};

// divide the input to 3 classes: I^0, I^+, I^-
void readInput( constraint& buffer, std::string inputFile )
{
	std::ifstream file;
	ineq area;
	int numArea;

	file.open(inputFile.c_str());

	file >> numArea;

	buffer.u_1 = -HUGE_VAL;
	buffer.u_2 = HUGE_VAL;

	for( int i = 0; i < numArea; ++i )
	{
		file >> area.coefX >> area.coefY >> area.constant;
		
		if( area.coefY == 0 )
		{
			if( area.coefX > 0 )
			{
				if( buffer.u_2 > (area.constant/area.coefX) )
				{
					buffer.u_2 = area.constant/area.coefX;
				}
			}
			else
			{
				if( buffer.u_1 < (area.constant/area.coefX) )
				{
					buffer.u_1 = area.constant/area.coefX;
				}
			}
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
double linearProg_2D_PS( constraint& buffer )
{
	// check whether the range of x-axis is feasible or not
	if( buffer.u_1 <= buffer.u_2 )
	{
		// find the intersection points of lines respectively in I^+ and I^-

		// find the median of x-axis of these intersection points

		// find the points alpha, beta, s_min, s_max, t_min, and t_max

		// check 6 cases

		// remove the unusable lines
	}
	else
	{
		std::cout << "no feasible solution" << std::endl;
		return 0;
	}

}

int main()
{
	constraint buffer;

	// divide the input to 3 classes: I^0, I^+, I^-
	readInput(buffer,"input.txt");

	std::cout << buffer.u_1 << "\t" << buffer.u_2 << std::endl;

	// pruning & search for 2D linear programming
	std::cout << linearProg_2D_PS( buffer )  << std::endl;
	

	return 0;
}
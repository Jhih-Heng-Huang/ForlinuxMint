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

struct  Coordinate
{
	double x;
	double y;
};

const int num = 5;

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

Coordinate getIntersection( const ineq& one, const ineq& theOther )
{
	Coordinate point;

	point.x = (one.constant * theOther.coefY - theOther.constant * one.coefY)/
				(one.coefX * theOther.coefY - theOther.coefX * one.coefY);

	point.y = (one.constant * theOther.coefX - theOther.constant * one.coefX)/
				(one.coefY * theOther.coefX - theOther.constant * one.coefX);

	return point;
}

double getSlope( const ineq& line )
{
	return -(line.coefX / line.coefY);
}

// only for 2D linear programming problem
void findIntersections( const std::vector<ineq>& buffer, std::vector<Coordinate>& point )
{
	Coordinate newPoint;

	
	for( size_t i = 1; i < buffer.size(); ++i )
	{
		// there is a intersection between two distinct lines
		if( getSlope(buffer[i-1]) != getSlope(buffer[i]) )
		{
			newPoint = getIntersection(buffer[i-1], buffer[i]);

			point.push_back(newPoint);
		}
	}


}

void swap( Coordinate& one, Coordinate& theOther )
{
	Coordinate tmp;

	tmp = one;
	one = theOther;
	theOther = tmp;
}

void groupBubbleSortOnX( std::vector<Coordinate>& buffer, const int front, const int rear )
{
	size_t size = rear - front;

	for( size_t count = 0; count < size; ++count )
	{
		for( size_t index = front+1; index < (rear - count); ++index )
		{
			if( buffer[index-1].x > buffer[index].x )
			{
				swap(buffer[index-1], buffer[index]);
			}
		}
	}

}

double findKthOnX( const std::vector<Coordinate>& point, const int kth, const int front, const int rear )
{
	std::vector<Coordinate> buffer;
	for( int i = front; i < rear; ++i )
	{
		buffer.push_back(point[i]);
	}

	if( buffer.size() <= num )
	{
		groupBubbleSortOnX(buffer,0,buffer.size());

		return buffer[kth-1].x;
	}
	else
	{
		// sort each group (O(n/5))
		size_t numGroup = (buffer.size() + num - 1)/num;


		for( size_t group = 0; group < numGroup; ++group )
		{
			if( (group + 1)*num <= buffer.size() )
			{
				groupBubbleSortOnX( buffer, group*num, (group + 1)*num );
			}
			else
			{
				groupBubbleSortOnX( buffer, group*num, buffer.size() );
			}
		}

		// find the median of each group and put into a new vector v (O(n/5))
		std::vector<Coordinate> v;
		for( size_t group = 0; group <= numGroup; ++group )
		{
			if( group*num + (num + 1)/2 < buffer.size() )
			{
				v.push_back(buffer[ group*num + (num + 1)/2 - 1 ]);
			}
		}

		double result = findKthOnX( v, (v.size() + 1)/2, 0, v.size() );
		v.clear();

		// to show that how big the median of the vector v in the buffer is and delete points which is smaller than it. (O(n))
		size_t count = 0;
		for( size_t index = 0; index < buffer.size(); ++index )
		{
			if( buffer[index].x <= result )
			{
				v.push_back(buffer[index]);
				buffer.erase(buffer.begin()+index);
				++count;
				--index;
			}
		}

		// which side of vector would you like to search? (T(3n/4))
		if( count > kth )
		{
			return findKthOnX( v, kth, 0, v.size() );
		}
		else if( count < kth )
		{
			return findKthOnX( buffer, kth - count, 0, buffer.size() );
		}
		else
		{
			return result;
		}

	}


}

void determineSlope( const std::vector<ineq>& buffer, const Coordinate& point, double& maxSlope, double& minSlope )
{
	double tmpY, tmpSlope;

	for( size_t i = 0; i < buffer.size(); ++i )
	{
		tmpY = (-buffer[i].coefX * point.x - buffer[i].constant) / buffer[i].coefY;

		if( tmpY == point.y )
		{
			tmpSlope = getSlope(buffer[i]);

			if( maxSlope < tmpSlope )
			{
				maxSlope = tmpSlope;
			}

			if( minSlope > tmpSlope )
			{
				minSlope = tmpSlope;
			}

		}
	}
}

void pruningLeft( constraint& buffer, const double& medianX )
{

}

void pruningRight( constraint& buffer, const double& medianX )
{

}

// pruning & search for 2D linear programming
double linearProg_2D_PS( constraint buffer )
{
	// check whether the range of x-axis is feasible or not
	if( buffer.u_1 <= buffer.u_2 )
	{
		std::vector<Coordinate> point;

		findIntersections(buffer.ubLines,point);

		findIntersections(buffer.lbLines,point);

		double medianX = findKthOnX( point, (point.size()+1)/2, 0, point.size() );

		// find the points alpha, beta, s_min, s_max, t_min, and t_max
		Coordinate alpha, beta;
		double max = -HUGE_VAL, min = HUGE_VAL;
		double s_max = -HUGE_VAL, s_min = HUGE_VAL;
		double t_max = -HUGE_VAL, t_min = HUGE_VAL;
		double tmpY, tmpSlope;
		alpha.x = medianX;
		beta.x = medianX;

		// determine the value of y axis of alpha , s_max and s_min
		for( size_t i = 0; i < buffer.lbLines.size(); ++i )
		{
			tmpY = ((-buffer.lbLines[i].coefX * alpha.x - buffer.lbLines[i].constant)/buffer.lbLines[i].coefY);

			if( max < tmpY )
			{
				max = tmpY;
			}
		}
		alpha.y = max;

		determineSlope( buffer.lbLines, alpha, s_max, s_min );

		// determinate the value of y axis of beta
		for( size_t i = 0; i < buffer.ubLines.size(); ++i )
		{
			tmpY = ((-buffer.ubLines[i].coefX * beta.x - buffer.ubLines[i].constant)/buffer.ubLines[i].coefY);

			if( min > tmpY )
			{
				min = tmpY;
			}
		}
		beta.y = min;

		determineSlope( buffer.ubLines, beta, t_max, t_min );

		// check 6 cases
		if( alpha.y <= beta.y )
		{
			if( s_max < 0 )
			{

			}
			else if( s_min > 0 )
			{

			}
			else
			{

			}
		}
		else
		{
			if( s_max < t_min )
			{

			}
			else if( s_min > t_max )
			{

			}
			else
			{
				std::cout << "no feasible solution" << std::endl;
				return 0;
			}
		}

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
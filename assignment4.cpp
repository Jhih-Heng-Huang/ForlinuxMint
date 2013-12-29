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
void findIntersections( std::vector<ineq>& buffer, std::vector<Coordinate>& point )
{
	Coordinate newPoint;

	
	for( size_t i = 1; i < buffer.size(); )
	{
		// there is a intersection between two distinct lines
		if( getSlope(buffer[i-1]) != getSlope(buffer[i]) )
		{
			newPoint = getIntersection(buffer[i-1], buffer[i]);

			point.push_back(newPoint);

			i = i + 2;
		}
		else
		{
			if( buffer[i].coefY > 0 )
			{
				// I^+
				if( (buffer[i-1].constant / buffer[i-1].coefY) < (buffer[i-1].constant / buffer[i-1].coefY) )
				{
					// line_{i - 1} belong to line_{i}
					buffer.erase( buffer.begin() + i );
				}
				else
				{
					// line_{i} belong to line_{i - 1}
					buffer.erase( buffer.begin() + i - 1 );	
				}
			}
			else
			{
				// I^-
				if( (buffer[i-1].constant / buffer[i-1].coefY) > (buffer[i-1].constant / buffer[i-1].coefY) )
				{
					// line_{i - 1} belong to line_{i}
					buffer.erase( buffer.begin() + i );
				}
				else
				{
					// line_{i} belong to line_{i - 1}
					buffer.erase( buffer.begin() + i - 1 );	
				}
			}
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
		tmpY = (buffer[i].constant - buffer[i].coefX * point.x) / buffer[i].coefY;

		// std::cout << point.y << "\t" << tmpY << "\n";

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
	Coordinate tmpPoint;

	// pruning the redundant lines in I^+
	for( size_t i = 1; i < buffer.ubLines.size(); ++i )
	{
		tmpPoint = getIntersection( buffer.ubLines[i-1], buffer.ubLines[i] );

		if( tmpPoint.x <= medianX )
		{
			if( getSlope(buffer.ubLines[i-1]) > getSlope(buffer.ubLines[i]) )
			{
				// pruning i-1
				buffer.ubLines.erase(buffer.ubLines.begin() + (i-1));
			}
			else if( getSlope(buffer.ubLines[i-1]) < getSlope(buffer.ubLines[i]) )
			{
				// pruning i
				buffer.ubLines.erase(buffer.ubLines.begin() + i);
			}
		}
	}

	// pruning the redundant lines in I^-
	for( size_t i = 0; i < buffer.lbLines.size(); ++i )
	{
		tmpPoint = getIntersection( buffer.lbLines[i-1], buffer.lbLines[i] );

		if( tmpPoint.x <= medianX )
		{
			if( getSlope(buffer.lbLines[i-1]) < getSlope(buffer.lbLines[i]) )
			{
				// pruning i-1
				buffer.lbLines.erase(buffer.lbLines.begin() + (i-1));
			}
			else if( getSlope(buffer.lbLines[i-1]) > getSlope(buffer.lbLines[i]) )
			{
				// pruning i
				buffer.lbLines.erase(buffer.lbLines.begin() + i);
			}
		}
	}
}

void pruningRight( constraint& buffer, const double& medianX )
{
	Coordinate tmpPoint;

	// pruning the redundant lines in I^+
	for( size_t i = 1; i < buffer.ubLines.size(); ++i )
	{
		tmpPoint = getIntersection( buffer.ubLines[i-1], buffer.ubLines[i] );

		if( medianX <= tmpPoint.x )
		{
			if( getSlope(buffer.ubLines[i-1]) < getSlope(buffer.ubLines[i]) )
			{
				// pruning i-1
				buffer.ubLines.erase(buffer.ubLines.begin() + (i-1));
			}
			else if( getSlope(buffer.ubLines[i-1]) > getSlope(buffer.ubLines[i]) )
			{
				// pruning i
				buffer.ubLines.erase(buffer.ubLines.begin() + i);
			}
		}
		
	}

	// pruning the redundant lines in I^-
	for( size_t i = 0; i < buffer.lbLines.size(); ++i )
	{
		tmpPoint = getIntersection( buffer.lbLines[i-1], buffer.lbLines[i] );

		if( medianX <= tmpPoint.x )
		{
			if( getSlope(buffer.lbLines[i-1]) > getSlope(buffer.lbLines[i]) )
			{
				// pruning i-1
				buffer.lbLines.erase(buffer.lbLines.begin() + (i-1));
			}
			else if( getSlope(buffer.lbLines[i-1]) < getSlope(buffer.lbLines[i]) )
			{
				// pruning i
				buffer.lbLines.erase(buffer.lbLines.begin() + i);
			}
		}
		
	}

}

bool isFeasible( const ineq& upperBound, const ineq& lowerBound )
{
	if( getSlope(upperBound) != getSlope(lowerBound) )
	{
		return true;
	}
	else
	{
		if( (upperBound.constant / upperBound.coefY) > (lowerBound.constant / lowerBound.coefY) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// pruning & search for 2D linear programming
double linearProg_2D_PS( constraint buffer )
{
	for( size_t i = 0; i < buffer.ubLines.size(); ++i )
	{
		std::cout << buffer.ubLines[i].coefX << "\t"
					<< buffer.ubLines[i].coefY << "\t"
					<< buffer.ubLines[i].constant << std::endl;
	}
	std::cout << "-------------------------" << std::endl;
	for( size_t i = 0; i < buffer.lbLines.size(); ++i )
	{
		std::cout << buffer.lbLines[i].coefX << "\t"
					<< buffer.lbLines[i].coefY << "\t"
					<< buffer.lbLines[i].constant << std::endl;
	}	
	std::cout << std::endl;

	// check whether the range of x-axis is feasible or not
	if( buffer.u_1 <= buffer.u_2 )
	{
		std::vector<Coordinate> point;

		findIntersections(buffer.ubLines,point);

		findIntersections(buffer.lbLines,point);

		// 

		if( point.size() == 0 )
		{
			if( buffer.ubLines.size() == 1 && buffer.lbLines.size() == 0 )
			{
				return -HUGE_VAL;
			}
			else if( buffer.ubLines.size() == 0 && buffer.lbLines.size() == 1 )
			{
				if( getSlope( buffer.lbLines[0] ) == 0 )
				{
					return (buffer.lbLines[0].constant / buffer.lbLines[0].coefY);
				}
				else
				{
					return -HUGE_VAL;
				}
			}
			else
			{
				if( isFeasible( buffer.ubLines[0], buffer.lbLines[0] ) )
				{
					if( getSlope(buffer.ubLines[0]) == getSlope(buffer.lbLines[0]) )
					{
						if( getSlope(buffer.ubLines[0]) == 0 )
						{
							return (buffer.ubLines[0].constant/ buffer.ubLines[0].coefY);
						}
						else
						{
							return -HUGE_VAL;
						}
					}
					else
					{
						if( getSlope(buffer.ubLines[0]) < getSlope(buffer.lbLines[0]) )
						{
							if( getSlope(buffer.lbLines[0]) > 0 )
							{
								return -HUGE_VAL;
							}
							else
							{
								Coordinate interPoint = getIntersection(buffer.ubLines[0], buffer.lbLines[0]);
								return interPoint.y;
							}
						}
						else
						{
							if( getSlope(buffer.lbLines[0]) < 0 )
							{
								return -HUGE_VAL;
							}
							else
							{
								Coordinate interPoint = getIntersection(buffer.ubLines[0], buffer.lbLines[0]);
								return interPoint.y;
							}
						}
					}
				}
				else
				{
					std::cout << "no feasible solution" << std::endl;
					return 0;
				}
			}
		}
		else
		{
			double medianX = findKthOnX( point, (point.size()+1)/2, 0, point.size() );

			std::cout << medianX << "\n";

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
				tmpY = (buffer.lbLines[i].constant - buffer.lbLines[i].coefX * alpha.x) / buffer.lbLines[i].coefY;

				if( max < tmpY )
				{
					max = tmpY;
				}
			}
			alpha.y = max;

			std::cout << alpha.x << "\t" << alpha.y << "\n";
			determineSlope( buffer.lbLines, alpha, s_max, s_min );

			// determinate the value of y axis of beta
			for( size_t i = 0; i < buffer.ubLines.size(); ++i )
			{
				tmpY = ((buffer.ubLines[i].constant - buffer.ubLines[i].coefX * beta.x)/buffer.ubLines[i].coefY);

				if( min > tmpY )
				{
					min = tmpY;
				}
			}
			beta.y = min;

			std::cout << beta.x << "\t" << beta.y << "\n";
			determineSlope( buffer.ubLines, beta, t_max, t_min );

			std::cout << "s_max = " << s_max << std::endl; 
			std::cout << "s_min = " << s_min << std::endl;
			std::cout << "t_max = " << t_max << std::endl;
			std::cout << "t_min = " << t_min << std::endl;

			// check 6 cases
			if( alpha.y <= beta.y )
			{
				if( s_max < 0 )
				{
					pruningLeft(buffer,medianX);
				}
				else if( s_min > 0 )
				{
					pruningRight(buffer,medianX);
				}
				else
				{
					// solution x = medianX
					return alpha.y;
				}
			}
			else
			{
				if( s_max < t_min )
				{
					pruningLeft(buffer,medianX);
				}
				else if( s_min > t_max )
				{
					pruningRight(buffer,medianX);
				}
				else
				{
					std::cout << "no feasible solution" << std::endl;
					return 0;
				}
			}

			return linearProg_2D_PS(buffer);
		}
		
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
	std::cout << "end  solution:" << linearProg_2D_PS( buffer ) << "\n";

	
	

	return 0;
}
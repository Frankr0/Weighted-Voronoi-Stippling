#include "SimplePolygon.h"

int main(int argc, char const *argv[]) {

	// vector<Point> v = {
	// 	Point(1, 1), Point(2, 2), Point(2, 3), Point(2, 4),
	// 	Point(3, 4), Point(4, 3), Point(4, 4), Point(4, 5),
	// 	Point(5, 5), Point(6, 2), Point(6, 4),
	// };

	vector<Point> v = {
		Point(1, 1), Point(1, 2), Point(1, 3), Point(1, 4),
		Point(4, 1), Point(4, 2), Point(4, 3), Point(4, 4),
	};

	// cout << *SimplePolygon<int>::findMax(v) << endl;
	SimplePolygon<int>::Genarate(v);
	return 0;
}
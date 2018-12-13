#include "SimplePolygon.h"

int main(int argc, char const *argv[]) {

	vector<Point> v = {
		Point(1, 1), Point(1, 2), Point(1, 3), Point(1, 4),
		Point(4, 1), Point(4, 2), Point(4, 3), Point(4, 4),
	};

	SimplePolygon::genarate<int>(v);
	return 0;
}

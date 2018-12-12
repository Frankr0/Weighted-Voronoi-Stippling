#include "PointPolygonTest.h"

int main(int argc, char const* argv[]) {
	vector<Point> v =  {
		Point(375, -792),
		Point(377, - 818),
		Point(413, 0),
		Point(412, 2),
		Point(357, 27),
		Point(343, 17),

	};

	cout << PointPolygonTest::isInside<int>(v, Point(0, 0)) << endl;
	cout << PointPolygonTest::isInside<int>(v, Point(500, 0)) << endl;
	cout << PointPolygonTest::isInside<int>(v, Point(370, 0)) << endl;

	return 0;
}

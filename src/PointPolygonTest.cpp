#include "PointPolygonTest.h"

int main(int argc, char const* argv[]) {
    vector<Point> v = {
        Point(10, 10),
        Point(10, -10),
        Point(-10, -10),
        Point(-10, 10),
    };

    cout << PointPolygonTest::isInside<int>(v, Point(10, 11)) << endl;

    return 0;
}

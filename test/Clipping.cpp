#include "opencv2/opencv.hpp"
#include <iostream>

#include "Clipping.h"

using namespace cv;
using namespace std;

int main(int argc, char const *argv[]) {

	Size size(100, 100);
	// vector<Point> v = Clipping::clipBound<int>(size, {Point(10, 10), Point(-10, 10), Point(-10, -10), Point(10, -10)});
	// vector<Point> v = Clipping::clipBound<int>(size, {Point(-10, 10), Point(20, 10), Point(10, -20), Point(-10, -10)});
	// vector<Point> v = Clipping::clipBound<int>(size, {Point(size.width -10, 10), Point(size.width + 20, 10), Point(size.width + 10, -20), Point(size.width -10, -10)});
	// vector<Point> v = Clipping::clipBound<int>(size, {Point(size.width - 10, size.height + 10), Point(size.width + 20, size.height + 10), Point(size.width + 10, size.height - 20), Point(size.width - 10, size.height - 10)});
	vector<Point> v = Clipping::clipBound<int>(size, {Point(-10, size.height + 10), Point(20, size.height + 10), Point(10, size.height - 20), Point(-10, size.height - 10)});

	for (std::vector<Point>::iterator i = v.begin(); i != v.end(); ++i) {
		cout << i->x << " " << i->y << endl;
	}

	return 0;
}

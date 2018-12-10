#include "opencv2/opencv.hpp"
#include <iostream>

#include "Utils.h"

using namespace cv;
using namespace std;

int main(int argc, char const *argv[]) {

    Size size(100, 100);
    vector<Point> v = Utils::clipBound<int>(size, {Point(10, 10), Point(-10, 10), Point(-10, -10), Point(10, -10)});
    for (std::vector<Point>::iterator i = v.begin(); i != v.end(); ++i) {
        cout << i->x << " " << i->y << endl;
    }

    return 0;
}

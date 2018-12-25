#include "CVT.h"

int main(int argc, char const *argv[]) {

	// Mat img = imread("../../data/500circle.jpg");
	// cvtColor(img, img, COLOR_BGR2GRAY);
	// img.convertTo(img, CV_8UC1);
	// img = ~img;

	// vector<Point> v = {Point(10, 10), Point(10, 250), Point(250, 270), Point(250, 10)};

	// CVT::calcDensityCentroPos(img, v);
	// waitKey(0);


	vector<Point> v = {Point2f(10, 10), Point2f(11, 10), Point2f(10, 11)};
	cout << CVT::calcCentroPos(v) << endl;
	return 0;
}
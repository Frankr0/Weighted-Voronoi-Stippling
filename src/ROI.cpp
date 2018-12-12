#include "ROI.h"


int main(int argc, char const *argv[]) {

	Mat img = imread("../../data/500circle.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);
	img.convertTo(img, CV_8UC1);
	img = ~img;

	vector<Point> v = {Point(10, 10), Point(10, 250), Point(250, 250), Point(250, 10)};
	Mat buf = ROI::crop(img, v);
	imshow("test", buf);

	waitKey(0);

	return 0;
}
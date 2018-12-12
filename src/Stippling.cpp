// Created by Frankro.

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#include "CVT.h"
using namespace cv;
using namespace std;


int main(int argc, char ** argv) {

	// Open Image.
	Mat img = imread("../../data/500circle.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);

	img = ~img;

	// To avoid the Density of dark region being too small.
	img = img * 254 / 255 + 1;

	// blur(img, img, Size(3, 3));
	const Size size = img.size();

	// Point Set.
	vector<Point2f> points;

	// Add Points.
	RNG rng;
	for (unsigned i = 0; i < 1000; ++i) {
		float x = rng.uniform(0, size.width);
		float y = rng.uniform(0, size.height);
		points.push_back(Point2f(x, y));
	}

	for (unsigned i = 0; i < 1000; ++i) {
		// Subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Subdiv Mat.
		Mat imgVoronoi = Mat::zeros(size.height, size.width, CV_8UC3);
		points = CVT::drawVoronoi(img, imgVoronoi, subdiv);

		// Show Image.
		waitKey(10);
		imshow( "imgVoronoi", imgVoronoi);

	}


	waitKey(0);

	return 0;
}

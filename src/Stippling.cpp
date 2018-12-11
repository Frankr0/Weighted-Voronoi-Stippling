// Created by Frankro.

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#include "Clipping.h"
using namespace cv;
using namespace std;

template<typename T>
inline bool is_valid(T x) {
	return x * 0.0 == 0.0;
}

Point2f calcCentroPos(const vector<Point> &facet) {
	Point2f centro(0.0f, 0.0f);
	Moments moment = moments(facet, false);
	centro = Point2f(moment.m10 / moment.m00, moment.m01 / moment.m00);
	return centro;
}

Point2f calcDensityCentroPos(const Mat & densityMap, const vector<Point> &facet) {
	Point2f centro(0.0f, 0.0f);
	Moments moment = moments(facet, false);
	centro = Point2f(moment.m10 / moment.m00, moment.m01 / moment.m00);
	return centro;
}

vector<Point2f> drawVoronoi(const Mat &input, Mat &output, Subdiv2D &subdiv) {

	vector<vector<Point2f> > facets;
	vector<Point2f> centers;
	vector<Point2f> centroPoints;
	subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

	vector<Point> ifacet;

	for ( size_t i = 0; i < facets.size(); i++ ) {

		ifacet.resize(facets[i].size());
		for ( size_t j = 0; j < facets[i].size(); j++ ) {
			ifacet[j] = facets[i][j];
		}

		Scalar color;
		color[0] = rand() & 255;
		color[1] = rand() & 255;
		color[2] = rand() & 255;
		fillConvexPoly(output, ifacet, color, 8, 0);

		polylines(output, ifacet, true, Scalar(), 1, CV_8S, 0);
		circle(output, centers[i], 3, Scalar(), cv::FILLED, CV_8S, 0);

		// Calculate Centroidal.
		ifacet = Clipping::clipBound<int>(output.size(), ifacet);
		Point2f centroPoint = calcCentroPos(ifacet);
		// Point2f centroPoint = calcDensityCentroPos(input, ifacet);
		circle(output, centroPoint, 3, Scalar(0, 255, 0), cv::FILLED, CV_8S, 0);
		centroPoints.push_back(centroPoint);
	}

	return centroPoints;
}


int main(int argc, char ** argv) {

	// Open Image.
	Mat img = imread("../../data/500test.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);
	blur(img, img, Size(3, 3));
	const Size size = img.size();

	// Point Set.
	vector<Point2f> points;


	// Add Points.
	RNG rng;
	for (unsigned i = 0; i < 100; ++i) {
		float x = rng.uniform(0, size.width);
		float y = rng.uniform(0, size.height);
		points.push_back(Point2f(x, y));
	}

	for (unsigned i = 0; i < 100; ++i) {

		// Subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Subdiv Mat.
		Mat imgVoronoi = Mat::zeros(size.height, size.width, CV_8UC3);
		points = drawVoronoi(img, imgVoronoi, subdiv);

		// Show Image.
		waitKey(10);
		imshow( "imgVoronoi", imgVoronoi);

	}

	waitKey(0);

	return 0;
}

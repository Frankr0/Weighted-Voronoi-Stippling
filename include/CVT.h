// Created by Frankro.

#ifndef CVT_H
#define CVT_H

#include "opencv2/opencv.hpp"
#include <iostream>

#include "Clipping.h"
#include "ROI.h"

using namespace cv;
using namespace std;


class CVT {
public:

	template<typename T>
	static Point2f calcCentroPos(const vector<Point_<T>> &facet) {
		Point2f centro(0.0f, 0.0f);
		Moments moment = moments(facet, false);
		centro = Point2f(moment.m10 / moment.m00, moment.m01 / moment.m00);
		return centro;
	}

	template<typename T>
	static Point2f calcDensityCentroPos(const Mat & densityMap, const vector<Point_<T>> &facet) {
		Point2f centro(0.0f, 0.0f);

		// Crop Region.
		Mat cropped = ROI::crop(densityMap, facet);
		Rect corppedRect = boundingRect(facet);
		cropped = cropped(corppedRect);

		// Expand the Influence of Difference.
		Mat powerCropped;
		cropped.convertTo(cropped, CV_32FC1);
		powerCropped = cropped.mul(cropped);
		powerCropped = powerCropped.mul(powerCropped);
		powerCropped = powerCropped.mul(powerCropped);

		Moments moment = moments(powerCropped, false);
		centro = Point2f(moment.m10 / moment.m00 + corppedRect.x, moment.m01 / moment.m00 + corppedRect.y);

		return centro;
	}

	static vector<Point2f> drawVoronoi(const Mat &input, Mat &output, Subdiv2D &subdiv) {

		vector<vector<Point2f> > facets;
		vector<Point2f> centers;
		vector<Point2f> centroPoints;
		subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

		vector<Point> ifacet;

		for ( size_t i = 0; i < facets.size(); i++ ) {

			ifacet.assign(facets[i].begin(), facets[i].end());

			// Calculate Centroidal.
			ifacet = Clipping::clipBound<int>(output.size(), ifacet);
			// Point2f centroPoint = calcCentroPos(ifacet);
			Point2f centroPoint = calcDensityCentroPos(input, ifacet);

			circle(output, centroPoint, 3, Scalar(0, 0, 0), cv::FILLED, CV_8S, 0);
			centroPoints.push_back(centroPoint);
		}

		return centroPoints;
	}

};



#endif // CVT_H

// Created by Frankro.

#ifndef CVT_H
#define CVT_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

#include "Clipping.h"
#include "ROI.h"

using namespace cv;
using namespace std;


class CVT {
public:
	template<typename T>
	static Point2f calcCentroid(const vector<Point_<T>> &facet) {
		Moments moment = moments(facet, false);

		// Guard against division by zero for degenerate regions.
		if (std::abs(moment.m00) < 1e-10) {
			return facet[0];
		}

		Point2f centroid(moment.m10 / moment.m00, moment.m01 / moment.m00);

		if (isnan(centroid.x)) {
			return facet[0];
		}

		return centroid;
	}

	template<typename T>
	static Point2f calcWeightedCentroid(const Mat & densityMap, const vector<Point_<T>> &facet) {
		// Crop density map to facet bounding box.
		Mat cropped = ROI::crop(densityMap, facet);
		Rect croppedRect = boundingRect(facet);
		cropped = cropped(croppedRect);

		// Raise density to higher power to amplify contrast.
		Mat weightedDensity;
		cropped.convertTo(cropped, CV_32FC1);
		weightedDensity = cropped.mul(cropped);
		weightedDensity = weightedDensity.mul(weightedDensity);
		weightedDensity = weightedDensity.mul(weightedDensity);

		Moments moment = moments(weightedDensity, false);

		// Guard against division by zero for degenerate regions.
		if (std::abs(moment.m00) < 1e-10) {
			return facet[0];
		}

		Point2f centroid(moment.m10 / moment.m00 + croppedRect.x, moment.m01 / moment.m00 + croppedRect.y);

		if (isnan(centroid.x)) {
			return facet[0];
		}

		return centroid;
	}

	static vector<Point2f> computeVoronoiCentroids(const Mat &input, Mat &output, Subdiv2D &subdiv, const int &pointSize) {

		vector<vector<Point2f> > facets;
		vector<Point2f> centers;
		vector<Point2f> centroids;
		subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

		vector<Point> clippedFacet;

		for ( size_t i = 0; i < facets.size(); i++ ) {

			clippedFacet.assign(facets[i].begin(), facets[i].end());

			// Clip Voronoi cell to image bounds and compute weighted centroid.
			clippedFacet = Clipping::clipToBounds<int>(output.size(), clippedFacet);
			// Point2f centroid = calcCentroid(clippedFacet);
			Point2f centroid = calcWeightedCentroid(input, clippedFacet);

			circle(output, centroid, pointSize, Scalar(0, 0, 0), cv::FILLED, cv::LINE_8, 0);
			centroids.push_back(centroid);
		}

		return centroids;
	}

};



#endif // CVT_H

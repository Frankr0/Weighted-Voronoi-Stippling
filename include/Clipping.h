// Created by Frankro.
// From https://en.wikipedia.org/wiki/Liang%E2%80%93Barsky_algorithm

#ifndef CLIPPING_H
#define CLIPPING_H

#include "opencv2/opencv.hpp"
#include <iostream>

#include "PointPolygonTest.h"
#include "SimplePolygon.h"

using namespace cv;
using namespace std;

class Clipping {
public:

	static double maxi(double arr[], size_t n) {
		double m = 0;
		for (size_t i = 0; i < n; ++i)
			if (m < arr[i])
				m = arr[i];
		return m;
	}

	static double mini(double arr[], size_t n) {
		double m = 1;
		for (size_t i = 0; i < n; ++i)
			if (m > arr[i])
				m = arr[i];
		return m;
	}

	template<typename T>
	static vector<Point_<T>> liangBarsky(Size size, Point_<T> pointA, Point_<T> pointB) {
		double xmin = 0;
		double ymin = 0;
		double xmax = size.width - 1;
		double ymax = size.height - 1;
		double x1 = pointA.x;
		double y1 = pointA.y;
		double x2 = pointB.x;
		double y2 = pointB.y;

		// defining variables
		vector<Point_<T>> v;

		double p1 = -(x2 - x1);
		double p2 = -p1;
		double p3 = -(y2 - y1);
		double p4 = -p3;
		double q1 = x1 - xmin;
		double q2 = xmax - x1;
		double q3 = y1 - ymin;
		double q4 = ymax - y1;
		double posarr[5], negarr[5];
		size_t posind = 1, negind = 1;
		posarr[0] = 1;
		negarr[0] = 0;

		if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0)) {
			// cout << "Line is parallel to clipping window!" << endl;
			return v;
		}

		if (p1 != 0) {
			double r1 = q1 / p1;
			double r2 = q2 / p2;
			if (p1 < 0) {
				negarr[negind++] = r1; // for negative p1, add it to negative array
				posarr[posind++] = r2; // and add p2 to positive array
			} else {
				negarr[negind++] = r2;
				posarr[posind++] = r1;
			}
		}
		if (p3 != 0) {
			double r3 = q3 / p3;
			double r4 = q4 / p4;
			if (p3 < 0) {
				negarr[negind++] = r3;
				posarr[posind++] = r4;
			} else {
				negarr[negind++] = r4;
				posarr[posind++] = r3;
			}
		}
		double xn1, yn1, xn2, yn2;
		double rn1, rn2;
		rn1 = maxi(negarr, negind); // maximum of negative array
		rn2 = mini(posarr, posind); // minimum of positive array

		if (rn1 > rn2)  { // reject
			return v;
		}

		xn1 = x1 + p2 * rn1;
		yn1 = y1 + p4 * rn1; // computing new points
		xn2 = x1 + p2 * rn2;
		yn2 = y1 + p4 * rn2;

		v.push_back(Point_<T>(xn1, yn1));
		v.push_back(Point_<T>(xn2, yn2));
		return v;
	}

	template<typename T>
	static vector<Point_<T>> clipBound(const Size &size, const vector<Point_<T>> &facet) {

		// Clipping Polygon.
		vector<Point_<T>> clipedPoly;
		vector<Point_<T>> clipedLine;
		for (auto i = facet.cbegin(); i != facet.cend(); ++i) {
			if (i == facet.end() - 1)
				clipedLine = liangBarsky(size, *i, *(facet.begin()) );
			else
				clipedLine = liangBarsky(size, *i, *(i + 1));

			clipedPoly.insert(clipedPoly.end(), clipedLine.begin(), clipedLine.end());
		}

		// Remove Outside Points.
		for (auto i = clipedPoly.begin(); i != clipedPoly.end(); ++i) {
			if (i->x < 0 || i->y < 0 || i->x > size.width - 1 || i->y > size.height - 1) {
				i = --clipedPoly.erase(i);
			}
		}

		// Add Corner Point.
		vector<Point_<T>> cornerPoints = {
			Point_<T>(0, 0),
			Point_<T>(0, size.height - 1),
			Point_<T>(size.width - 1, size.height - 1),
			Point_<T>(size.width - 1, 0)
		};

		for (auto i = cornerPoints.cbegin(); i != cornerPoints.cend(); ++i) {
			if (PointPolygonTest::isInside<T>(facet, *i))
				clipedPoly.push_back(*i);
		}

		// Remove Duplicate Point.
		sort(clipedPoly.begin(), clipedPoly.end(), [](Point_<T> a, Point_<T> b) {
			if (a.x == b.x)
				return a.y < b.y;
			return a.x < b.x;
		});
		auto end_unique = unique(clipedPoly.begin(), clipedPoly.end());
		clipedPoly.erase(end_unique, clipedPoly.end());

		// Genarate Simple Polygon.
		clipedPoly = SimplePolygon::genarate<T>(clipedPoly);

		return clipedPoly;

	}

};



#endif // CLIPPING_H
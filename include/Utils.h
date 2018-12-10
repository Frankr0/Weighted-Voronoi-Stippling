#ifndef UTILS_H
#define UTILS_H

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class Utils {
public:

	static double maxi(double arr[], int n) {
		double m = 0;
		for (int i = 0; i < n; ++i)
			if (m < arr[i])
				m = arr[i];
		return m;
	}

	static double mini(double arr[], int n) {
		double m = 1;
		for (int i = 0; i < n; ++i)
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
		int posind = 1, negind = 1;
		posarr[0] = 1;
		negarr[0] = 0;

		if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0)) {
			cout << "Line is parallel to clipping window!" << endl;
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

		// Add Corner Point.
		if(pointPolygonTest(facet, Point_<T>(0, 0), false)) {
			cout << "Bottom Left" << endl;
			clipedPoly.push_back(Point_<T>(0, 0));
		}
		// Remove Duplicate Point.


		return clipedPoly;

	}

};










#endif // UTILS_H





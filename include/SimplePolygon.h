// Created by Frankro.

#ifndef SIMPLEPOLYGON_H
#define SIMPLEPOLYGON_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <limits>
using namespace cv;
using namespace std;


class SimplePolygon {
public:

	template<typename T>
	static double calcDistanceSquare(const Point_<T> &p1, const Point_<T> &p2) {
		return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	}

	template<typename T>
	static double calcGrandient(const Point_<T> &p1, const Point_<T> &p2) {
		if (p1.x == p2.x) return p1.y < p2.y ? -DBL_MAX : DBL_MAX;
		return static_cast<double> ( 1.0 * (p2.y - p1.y) / (p2.x - p1.x) );
	}

	template<typename T>
	static typename vector<Point_<T> >::const_iterator
	findStartPoint (const vector<Point_<T> > &pset) {
		typename vector<Point_<T> >::const_iterator startPoint = pset.begin();
		for (typename vector<Point_<T> >::const_iterator i = pset.begin(); i != pset.end(); ++i) {

			if ( i->x > startPoint->x )
				startPoint = i;
			else if ( i->x == startPoint->x )
				startPoint = ( ( i->y < startPoint->y ) ?  i : startPoint );
		}
		return startPoint;
	}

	template<typename T>
	static vector<Point_<T>> genarate(const vector<Point_<T>> &pset) {
		auto startPoint = findStartPoint(pset);
		vector<pair<Point_<T>, double> > vppd;

		// Calculate Gradient.
		for (auto i = pset.begin(); i != pset.end(); ++i) {
			if (i == startPoint) continue;
			double gradient = calcGrandient(*startPoint, *i);
			vppd.push_back( pair<Point_<T>, double>(*i, gradient) );
		}

		// Sort by Gradient and Distance to startPoint Point.
		sort(vppd.begin(), vppd.end(), [startPoint](auto a, auto b) {
			if (a.second != b.second)
				return a.second < b.second;
			else
				return calcDistanceSquare(a.first, *startPoint) < calcDistanceSquare(b.first, *startPoint);
		});

		// Result Set.
		vector<Point_<T>> vp = {*startPoint};
		for (auto i = vppd.begin(); i != vppd.end(); ++i)
			vp.push_back(i->first);

		return vp;
	}
};



#endif

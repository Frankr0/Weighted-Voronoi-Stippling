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
	static double calcDistanceSquared(const Point_<T> &p1, const Point_<T> &p2) {
		return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	}

	template<typename T>
	static double calcSlope(const Point_<T> &p1, const Point_<T> &p2) {
		if (p1.x == p2.x) return p1.y < p2.y ? -DBL_MAX : DBL_MAX;
		return static_cast<double> ( 1.0 * (p2.y - p1.y) / (p2.x - p1.x) );
	}

	template<typename T>
	static typename vector<Point_<T> >::const_iterator
	findStartPoint (const vector<Point_<T> > &points) {
		typename vector<Point_<T> >::const_iterator startPoint = points.begin();
		for (typename vector<Point_<T> >::const_iterator it = points.begin(); it != points.end(); ++it) {

			if ( it->x > startPoint->x )
				startPoint = it;
			else if ( it->x == startPoint->x )
				startPoint = ( ( it->y < startPoint->y ) ?  it : startPoint );
		}
		return startPoint;
	}

	template<typename T>
	static vector<Point_<T>> generate(const vector<Point_<T>> &points) {
		auto startPoint = findStartPoint(points);
		vector<pair<Point_<T>, double> > pointSlopePairs;

		// Calculate slope from start point to each other point.
		for (auto it = points.begin(); it != points.end(); ++it) {
			if (it == startPoint) continue;
			double slope = calcSlope(*startPoint, *it);
			pointSlopePairs.push_back( pair<Point_<T>, double>(*it, slope) );
		}

		// Sort by slope, then by distance to start point.
		sort(pointSlopePairs.begin(), pointSlopePairs.end(), [startPoint](auto a, auto b) {
			if (a.second != b.second)
				return a.second < b.second;
			else
				return calcDistanceSquared(a.first, *startPoint) < calcDistanceSquared(b.first, *startPoint);
		});

		// Build result polygon.
		vector<Point_<T>> result = {*startPoint};
		for (auto it = pointSlopePairs.begin(); it != pointSlopePairs.end(); ++it)
			result.push_back(it->first);

		return result;
	}
};



#endif

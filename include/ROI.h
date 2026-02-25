// Created by Frankro.

#ifndef ROI_H
#define ROI_H

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class ROI {
public:
	template<typename T>
	static Mat crop(const Mat &image, const vector<Point_<T>> &region) {
		Mat mask = Mat::zeros(image.size(), CV_8UC1);
		Scalar maskFillColor(255);
		fillConvexPoly(mask, region, maskFillColor, cv::LINE_8, 0);

		// Apply mask to extract region.
		Mat maskedImage;
		bitwise_and(image, mask, maskedImage);
		return maskedImage;
	}
};



#endif // ROI_H

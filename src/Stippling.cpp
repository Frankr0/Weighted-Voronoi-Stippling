// Created by Frankro.

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>

#include <fstream>

#include "CVT.h"
using namespace cv;
using namespace std;

// Named constants for magic numbers.
static const int kBatchSize = 200;
static const int kDefaultEpochs = 100;
static const int kDefaultPointSize = 1;
static const int kDensityScaleNum = 254;
static const int kDensityScaleDen = 255;
static const int kDisplayDelayMs = 10;
static const Scalar kBackgroundColor(255, 255, 255);
static const Scalar kForegroundColor(0, 0, 0);

bool parseCommandLine(int argc, char ** argv, Mat &img, int &totalPoints, int &epochs, int &pointSize, bool &showProgress) {
	const String keys =
	    "{help h usage ? |      | print this message           }"
	    "{@image         |      | image for stippling          }"
	    "{N n number     |      | number of stipple points     }"
	    "{invert i       |      | invert image brightness      }"
	    "{epoch e        |      | number of iterations         }"
	    "{size s         |      | stipple point radius         }"
	    "{draw d         |      | show iteration progress      }"
	    ;

	CommandLineParser parser(argc, argv, keys);
	parser.about("Weighted Voronoi Rendering.");
	if (parser.has("help")) {
		parser.printMessage();
		return false;
	}

	// Load image.
	String imgPath = parser.get<String>(0);
	if (imgPath.empty()) {
		parser.printMessage();
		return false;
	}
	img = imread(imgPath);
	if (img.empty()) {
		cout << "Error: failed to load image." << endl;
		return false;
	}
	cvtColor(img, img, COLOR_BGR2GRAY);
	const Size size = img.size();

	// Invert brightness.
	if (parser.has("invert"))
		img = ~img;

	// Set total number of stipple points.
	totalPoints = (size.height + size.width) * 2;
	if (parser.has("n"))
		totalPoints = parser.get<int>("n");

	// Set number of iterations.
	epochs = kDefaultEpochs;
	if (parser.has("epoch"))
		epochs = parser.get<int>("epoch");
	if (epochs <= 0) {
		cout << "Error: epoch must be positive." << endl;
		return false;
	}

	// Set point size.
	pointSize = kDefaultPointSize;
	if (parser.has("size"))
		pointSize = parser.get<int>("size");
	if (pointSize <= 0) {
		cout << "Error: point size must be positive." << endl;
		return false;
	}

	// Show iteration progress.
	showProgress = false;
	if (parser.has("draw"))
		showProgress = true;

	// Check parser errors.
	if (!parser.check()) {
		parser.printErrors();
		return false;
	}

	return true;
}


void appendRandomPoints(RNG &rng, vector<Point2f> &points, Size size, int count) {
	for (int i = 0; i < count; ++i) {
		float x = rng.uniform((float)0, (float)size.width - 1);
		float y = rng.uniform((float)0, (float)size.height - 1);
		points.push_back(Point2f(x, y));
	}
}

int main(int argc, char ** argv) {

	RNG rng(time(0));
	Mat img;
	int totalPoints, epochs, pointSize;
	bool showProgress;

	if (!parseCommandLine(argc, argv, img, totalPoints, epochs, pointSize, showProgress)) {
		return -1;
	}

	const Size size = img.size();

	// Scale density to [1, 255] to avoid zero-weight regions.
	img = img * kDensityScaleNum / kDensityScaleDen + 1;

	// Point set.
	vector<Point2f> points;

	// Seed initial points.
	appendRandomPoints(rng, points, size, kBatchSize);

	Mat outputImage(size.height, size.width, CV_8UC3, kBackgroundColor);
	for (int i = 0; i < epochs; ++i) {

		// Gradually add more points.
		if (i < (totalPoints / kBatchSize))
			appendRandomPoints(rng, points, size, kBatchSize);

		// Remove out-of-bounds points.
		points.erase(
			std::remove_if(points.begin(), points.end(), [&size](const Point2f &p) {
				return p.x < 0 || p.x > size.width || p.y < 0 || p.y > size.height;
			}),
			points.end()
		);

		// Build Delaunay subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Compute Voronoi centroids and render stipples.
		outputImage = kBackgroundColor;
		points = CVT::computeVoronoiCentroids(img, outputImage, subdiv, pointSize);

		// Display intermediate result.
		if (showProgress) {
			waitKey(kDisplayDelayMs);
			imshow("Stippling", outputImage);
		}

		cout << "(" << i << "/" << epochs << ")" << endl;

	}
	cout << endl << "done." << endl;
	imshow("Stippling", outputImage);
	waitKey(0);

	imwrite("save.jpg", outputImage);

	return 0;
}

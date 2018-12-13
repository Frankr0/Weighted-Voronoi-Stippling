// Created by Frankro.

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#include "CVT.h"
using namespace cv;
using namespace std;

bool parserCommand(int argc, char ** argv, Mat &img , int &N, int &E, bool &drawRepeat) {
	const String keys =
	    "{help h usage ? |      | print this message     }"
	    "{@image         |      | image  for rendering   }"
	    "{N n number     |      | points number          }"
	    "{inverse i      |      | inverse image          }"
	    "{epoch e        |      | epochs number          }"
	    "{draw d         |      | draw result repeatedly }"
	    ;

	CommandLineParser parser(argc, argv, keys);
	parser.about("Weighted Voronoi Redering.");
	if (parser.has("help")) {
		parser.printMessage();
		return false;
	}

	// Open Image.
	String imgPath = parser.get<String>(0);
	if (imgPath.empty()) {
		parser.printMessage();
		return false;
	}
	img = imread(imgPath);
	if (img.empty()) {
		cout << "Error Loading File." << endl;
		return false;
	}
	cvtColor(img, img, COLOR_BGR2GRAY);
	const Size size = img.size();

	// Inverse.
	if (parser.has("inverse"))
		img = ~img;


	// Set Points Number.
	N = (size.height + size.width) * 2;
	if (parser.has("n"))
		N = parser.get<int>("n");

	// Set Epochs Number.
	E = 100;
	if (parser.has("epoch"))
		E = parser.get<int>("epoch");

	drawRepeat = false;
	if (parser.has("draw"))
		drawRepeat = true;

	// Check Parser Error.
	if (!parser.check()) {
		parser.printErrors();
		return false;
	}

	return true;
}


int main(int argc, char ** argv) {

	Mat img;
	int N, E;
	bool drawRepeat;

	if (!parserCommand(argc, argv, img, N, E, drawRepeat)) {
		return -1;
	}

	const Size size = img.size();

	// To avoid the Density of dark region being too small.
	img = img * 254 / 255 + 1;

	// Point Set.
	vector<Point2f> points;

	// Add Points.
	RNG rng;
	for (int i = 0; i < N; ++i) {
		float x = rng.uniform(0, size.width);
		float y = rng.uniform(0, size.height);
		points.push_back(Point2f(x, y));
	}

	Mat imgVoronoi(size.height, size.width, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < E; ++i) {
		// Subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Subdiv Mat.
		imgVoronoi = Scalar(255, 255, 255);
		points = CVT::drawVoronoi(img, imgVoronoi, subdiv);

		// Show Image.
		if (drawRepeat) {
			waitKey(10);
			imshow( "imgVoronoi", imgVoronoi);
			
		}

		cout << "!" << flush;

	}
	cout << endl << "done." << endl;
	imshow( "imgVoronoi", imgVoronoi);
	waitKey(0);

	return 0;
}

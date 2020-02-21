// Created by Frankro.

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#include <fstream>

#include "CVT.h"
using namespace cv;
using namespace std;

bool parserCommand(int argc, char ** argv, Mat &img , int &N, int &E,int &pointSize, bool &drawRepeat) {
	const String keys =
	    "{help h usage ? |      | print this message      }"
	    "{@image         |      | image for stippling     }"
	    "{N n number     |      | points amount           }"
	    "{inverse i      |      | inverse image           }"
	    "{epoch e        |      | epochs                  }"
	    "{size s         |      | point size              }"
	    "{draw d         |      | show iterate processing }"
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
	
	// Set Point Size.
	pointSize = 1;
	if (parser.has("size"))
		pointSize = parser.get<int>("size");

	// Show Processing.
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


void appendRandomPoint(RNG &rng, vector<Point2f> &points, Size size, int N) {
	for (int i = 0; i < N; ++i) {
		float x = rng.uniform((float)0, (float)size.width - 1);
		float y = rng.uniform((float)0, (float)size.height - 1);
		points.push_back(Point2f(x, y));
	}
}

int main(int argc, char ** argv) {

	RNG rng(time(0));
	Mat img;
	int N, E, pointSize;
	bool drawRepeat;

	if (!parserCommand(argc, argv, img, N, E,pointSize, drawRepeat)) {
		return -1;
	}

	const Size size = img.size();

	// To avoid the Density of dark region being too small.
	img = img * 254 / 255 + 1;

	// Point Set.
	vector<Point2f> points;

	// Add Points.
	appendRandomPoint(rng, points, size, 200);

	Mat imgVoronoi(size.height, size.width, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < E; ++i) {

		// Append Random Points.
		if (i < (N / 200))
			appendRandomPoint(rng, points, size, 200);

		// Remove Out of range Points.
		for (auto i = points.begin(); i != points.end(); ++i) {
			if (i->x > size.width || i->x < 0 || i->y > size.height || i->y < 0) {
				points.erase(i);
				i--;
			}
		}

		// Subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Subdiv Mat.
		imgVoronoi = Scalar(255, 255, 255);
		points = CVT::drawVoronoi(img, imgVoronoi, subdiv, pointSize);

		// Show Image.
		if (drawRepeat) {
			waitKey(10);
			imshow( "imgVoronoi", imgVoronoi);

		}

		cout << "(" << i << "/" << E << ")" << endl;

	}
	cout << endl << "done." << endl;
	imshow( "imgVoronoi", imgVoronoi);
	waitKey(0);

	imwrite("save.jpg", imgVoronoi);

	return 0;
}

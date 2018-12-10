#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void facetNormalize(const Size &size, vector<Point> &facet) {
	facet.push_back(facet[0]);
	for (auto pre = facet.begin(), curr = pre + 1; curr != facet.end(); ++pre, ++curr) {
		// Left Bound.
		if (pre->x * curr->x < 0) {
			// cout << "left bound." << endl;
			// Horizontal Line.
			if (pre->y == curr->y) {
				pre = facet.insert(curr, Point(0, curr->y));
				curr = pre + 1;
				// cout << "insert: (" << 0 << "," << curr->y << ")" << endl;
				continue;
			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufY = 1.0 * (-x2) / (x1 - x2) * (y1 - y2) + y2;

				pre = facet.insert(curr, Point(0, (int)bufY));
				curr = pre + 1;
				// cout << "insert: (" << 0 << "," << (int)bufY << ")" << endl;
				continue;
			}
		}

		// Right Bound.
		if ((pre->x - size.width) * (curr->x - size.width) < 0) {
			// cout << "right bound." << endl;
			// Horizontal Line.
			if (pre->y == curr->y) {
				pre = facet.insert(curr, Point(size.width - 1, curr->y));
				curr = pre + 1;
				// cout << "insert: (" << size.width - 1 << "," << curr->y << ")" << endl;
				continue;
			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufY = 1.0 * (size.width - x2) / (x1 - x2) * (y1 - y2) + y2;

				pre = facet.insert(curr, Point(size.width - 1, (int)bufY));
				curr = pre + 1;
				// cout << "insert: (" << size.width - 1 << "," << (int)bufY << ")" << endl;
				continue;
			}
		}

		// Bottom Bound;
		if (pre->y * curr->y < 0) {
			// cout << "bottom bound." << endl;
			// Horizontal Line.
			if (pre->x == curr->x) {
				pre = facet.insert(curr, Point(curr->x, 0));
				curr = pre + 1;
				// cout << "insert: (" << curr->x << "," << 0 << ")" << endl;
				continue;
			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufX = 1.0 * (-y2) / (y1 - y2) * (x1 - x2) + x2;

				pre = facet.insert(curr, Point((int)bufX, 0));
				curr = pre + 1;
				// cout << "insert: (" << (int)bufX << "," << 0 << ")" << endl;
				continue;
			}
		}

		// Top Bound;
		if ((pre->y - size.height) * (curr->y - size.height) < 0) {
			// cout << "top bound." << endl;
			// Horizontal Line.
			if (pre->x == curr->x) {
				pre = facet.insert(curr, Point(curr->x, size.height - 1));
				curr = pre + 1;
				// cout << "insert: (" << curr->x << "," << size.height - 1 << ")" << endl;
				continue;
			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufX = 1.0 * (size.height - y2) / (y1 - y2) * (x1 - x2) + x2;

				pre = facet.insert(curr, Point((int)bufX, size.height - 1));
				curr = pre + 1;
				// cout << "insert: (" << (int)bufX << "," << size.height - 1 << ")" << endl;
				continue;
			}
		}

	}

	facet.pop_back();

	bool top = false;
	bool bottom = false;
	bool left = false;
	bool right = false;
	for (auto i = facet.begin(); i != facet.end(); ) {
		if (i->x < 0) {
			// cout << "erase: (" << i->x << "," << i->y << ")" << endl;
			i = facet.erase(i);
			left = true;

		} else if (i->y < 0) {
			// cout << "erase: (" << i->x << "," << i->y << ")" << endl;
			i = facet.erase(i);
			bottom = true;
		} else if (i->x > size.width) {
			// cout << "erase: (" << i->x << "," << i->y << ")" << endl;
			i = facet.erase(i);
			right = true;
		} else if ( i->y > size.height) {
			// cout << "erase: (" << i->x << "," << i->y << ")" << endl;
			i = facet.erase(i);
			top = true;
		} else {
			++i;
		}
	}

	if (top && left)
		facet.push_back(Point(0, size.height - 1));
	if (top && right)
		facet.push_back(Point(size.width - 1, size.height - 1));
	if (bottom && right)
		facet.push_back(Point(size.width - 1, 0));
	if (bottom && left)
		facet.push_back(Point(0, 0));

	if(facet.size() < 3) {
		cout << "!!!!!!!!!!!" << endl;
	}
}

template<typename T>
inline bool is_valid(T x) {
	return x * 0.0 == 0.0;
}

// Point2f calcDensityCentroPos(const Mat &input, const vector<Point> &facet) {

// 	Point2f centro(0, 0);
// 	for (auto i = facet.begin(); i != facet.end(); ++i) {
// 		centro.x += i->x;
// 		centro.y += i->y;
// 	}

// 	centro.x = centro.x / facet.size();
// 	centro.y = centro.y  / facet.size();
// 	return centro;
// }

Point2f calcCentroPos(const vector<Point> &facet) {
	Point2f centro(0.0f, 0.0f);

	Moments moment = moments(facet, false);
	
	if (is_valid(moment.m10 / moment.m00) && is_valid(moment.m01 / moment.m00)) {
		centro = Point2f(moment.m10 / moment.m00, moment.m01 / moment.m00);
	} else {
		cout << "zero m00" << endl;
		for (auto i = facet.begin(); i != facet.end(); ++i) {
			centro.x = centro.x + i->x;
			centro.y = centro.y +  i->y;
		}
		cout << centro.x << " " <<  facet.size() << endl;
		centro.x = centro.x / facet.size();
		centro.y = centro.y  / facet.size();
	}

	if(centro.x > 500 || centro.y > 500) {
		for (auto i = facet.begin(); i != facet.end(); ++i) {
			cout << i->x << " " << i->y << endl;
		}
	}

	return centro;
}

vector<Point2f> drawVoronoi(const Mat &input, Mat &output, Subdiv2D &subdiv ) {

	vector<vector<Point2f> > facets;
	vector<Point2f> centers;
	vector<Point2f> centroPoints;
	subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

	vector<Point> ifacet;

	for ( size_t i = 0; i < facets.size(); i++ ) {

		ifacet.resize(facets[i].size());
		for ( size_t j = 0; j < facets[i].size(); j++ ) {
			ifacet[j] = facets[i][j];
		}

		Scalar color;
		color[0] = rand() & 255;
		color[1] = rand() & 255;
		color[2] = rand() & 255;
		fillConvexPoly(output, ifacet, color, 8, 0);

		polylines(output, ifacet, true, Scalar(), 1, CV_8S, 0);
		circle(output, centers[i], 3, Scalar(), cv::FILLED, CV_8S, 0);

		// Calculate Centroidal.
		facetNormalize(output.size(), ifacet);
		Point2f centroPoint = calcCentroPos(ifacet);
		// Point2f centroPoint = calcDensityCentroPos(input, ifacet);
		circle(output, centroPoint, 3, Scalar(0, 255, 0), cv::FILLED, CV_8S, 0);
		centroPoints.push_back(centroPoint);
	}

	return centroPoints;
}


int main(int argc, char ** argv) {

	// Open Image.
	Mat img = imread("../data/500test.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);
	blur(img, img, Size(3, 3));
	const Size size = img.size();

	// Point Set.
	vector<Point2f> points;


	// Add Points.
	RNG rng;
	for (unsigned i = 0; i < 100; ++i) {
		float x = rng.uniform(0, size.width);
		float y = rng.uniform(0, size.height);
		points.push_back(Point2f(x, y));
	}

	for (unsigned i = 0; i < 10; ++i) {

		// Subdivision.
		Rect rect(0, 0, size.width, size.height);
		Subdiv2D subdiv(rect);
		subdiv.insert(points);

		// Subdiv Mat.
		Mat imgVoronoi = Mat::zeros(size.height, size.width, CV_8UC3);
		points = drawVoronoi(img, imgVoronoi, subdiv);

		// Show Image.
		waitKey(10);
		imshow( "imgVoronoi", imgVoronoi);

	}

	waitKey(0);

	return 0;
}
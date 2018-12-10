#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void facetNormalize(const Size &size, vector<Point> &facet) {
	for (auto pre = facet.begin(), curr = pre + 1; curr != facet.end(); ++pre, ++curr) {
		// Left Bound.
		if (pre->x * curr->x < 0) {
			cout << "Left Bound" << endl;
			// Horizontal Line.
			if (pre->y == curr->y) {
				pre = facet.insert(curr, Point(0, curr->y));
				curr = pre + 1;

			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufY = 1.0 * (-x2) / (x1 - x2) * (y1 - y2) + y2;

				pre = facet.insert(curr, Point(0, (int)bufY));
				curr = pre + 1;
			}
		}

		// Right Bound.
		if ((pre->x - size.width) * (curr->x - size.width) < 0) {
			// Horizontal Line.
			if (pre->y == curr->y) {
				pre = facet.insert(curr, Point(size.width - 1, curr->y));
				curr = pre + 1;

			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufY = 1.0 * (size.width - x2) / (x1 - x2) * (y1 - y2) + y2;

				pre = facet.insert(curr, Point(size.width - 1, (int)bufY));
				curr = pre + 1;
			}
		}

		// Bottom Bound;
		if (pre->y * curr->y < 0) {
			// Horizontal Line.
			if (pre->x == curr->x) {
				pre = facet.insert(curr, Point(curr->x, 0));
				curr = pre + 1;

			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufX = 1.0 * (-y2) / (y1 - y2) * (x1 - x2) + x2;

				pre = facet.insert(curr, Point((int)bufX, 0));
				curr = pre + 1;
			}
		}

		// Top Bound;
		if ((pre->y - size.height) * (curr->y - size.height) < 0) {
			// Horizontal Line.
			if (pre->x == curr->x) {
				pre = facet.insert(curr, Point(curr->x, size.height - 1));
				curr = pre + 1;

			} else {
				// General Line.
				const int &x1 = pre->x;
				const int &y1 = pre->y;
				const int &x2 = curr->x;
				const int &y2 = curr->y;
				double bufX = 1.0 * (size.height - y2) / (y1 - y2) * (x1 - x2) + x2;

				pre = facet.insert(curr, Point((int)bufX, size.height - 1));
				curr = pre + 1;
			}
		}

	}

	bool top = false;
	bool bottom = false;
	bool left = false;
	bool right = false;
	for (auto i = facet.begin(); i != facet.end(); ) {
		if (i->x < 0) {
			i = facet.erase(i);
			left = true;
		} else if (i->y < 0) {
			i = facet.erase(i);
			bottom = true;
		} else if (i->x > size.width) {
			i = facet.erase(i);
			right = true;
		} else if ( i->y > size.height) {
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
}


int main(int argc, char ** argv) {
	vector<Point> v = {Point(10,10), Point(-20,20), Point(-10,-10)}
	vector<Point> c = facetNormalize(Size(100,100), v);

	for (std::vector<Point>::iterator i = c.begin(); i != c.end(); ++i) {
		cou << i->x << " " << i->y << endl;
	}
	return 0;
}
// Created by Frankro.
// From https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/

#ifndef POINT_POLYGON_TEST_H
#define POINT_POLYGON_TEST_H

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define INF 999999

class PointPolygonTest {
public:

    template<typename T>
    static T min(const T &a, const T &b) {
        return a < b ? a : b;
    }

    template<typename T>
    static T max(const T &a, const T &b) {
        return b < a ? a : b;
    }

    // Given three colinear points p, q, r, the function checks if
    // point q lies on line segment 'pr'
    template <typename T>
    static bool onSegment(Point_<T> p, Point_<T> q, Point_<T> r) {
        if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
            return true;
        return false;
    }

    // To find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are colinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    template <typename T>
    static int orientation(Point_<T> p, Point_<T> q, Point_<T> r) {
        T val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0)
            return 0;             // colinear
        return (val > 0) ? 1 : 2; // clock or counterclock wise
    }

    // The function that returns true if line segment 'p1q1'
    // and 'p2q2' intersect.
    template <typename T>
    static bool doIntersect(Point_<T> p1, Point_<T> q1, Point_<T> p2, Point_<T> q2) {
        // Find the four orientations needed for general and
        // special cases
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        // General case
        if (o1 != o2 && o3 != o4)
            return true;

        // Special Cases
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1
        if (o1 == 0 && onSegment(p1, p2, q1))
            return true;

        // p1, q1 and p2 are colinear and q2 lies on segment p1q1
        if (o2 == 0 && onSegment(p1, q2, q1))
            return true;

        // p2, q2 and p1 are colinear and p1 lies on segment p2q2
        if (o3 == 0 && onSegment(p2, p1, q2))
            return true;

        // p2, q2 and q1 are colinear and q1 lies on segment p2q2
        if (o4 == 0 && onSegment(p2, q1, q2))
            return true;

        return false; // Doesn't fall in any of the above cases
    }

    // Returns true if the point p lies inside the polygon[] with n vertices
    template <typename T>
    static bool isInside(std::vector<Point_<T>> polygon, Point_<T> p) {
        size_t n = polygon.size();
        // There must be at least 3 vertices in polygon[]
        if (n < 3)
            return false;

        // Fast Bound Box Test.
        T minX = polygon[0].x;
        T maxX = polygon[0].x;
        T minY = polygon[0].y;
        T maxY = polygon[0].y;
        for (auto i = polygon.begin(); i != polygon.end(); ++i) {
            Point_<T> q = *i;
            minX = min<T>(q.x, minX);
            maxX = max<T>(q.x, maxX);
            minY = min<T>(q.y, minY);
            maxY = max<T>(q.y, maxY);
        }
        
        if ( p.x < minX || p.x > maxX || p.y < minY || p.y > maxY ) {
            return false;
        }

        // Create a point for line segment from p to infinite
        Point_<T> extreme = {INF, p.y};

        // Count intersections of the above line with sides of polygon
        size_t count = 0, i = 0;
        do {
            size_t next = (i + 1) % n;

            // Check if the line segment from 'p' to 'extreme' intersects
            // with the line segment from 'polygon[i]' to 'polygon[next]'
            Point_<T> last(INF, INF);
            if (doIntersect(polygon[i], polygon[next], p, extreme)) {
                // If the point 'p' is colinear with line segment 'i-next',
                // then check if it lies on segment. If it lies, return true,
                // otherwise false
                if (orientation(polygon[i], p, polygon[next]) == 0)
                    return onSegment(polygon[i], p, polygon[next]);

                count++;
            }
            i = next;
        } while (i != 0);

        // Return true if count is odd, false otherwise
        return count & 1; // Same as (count%2 == 1)
    }
};

#endif // POINT_POLYGON_TEST_H

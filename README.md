# Weighted Voronoi Stippling
----------

This is an implementation of the following article using OpenCV:

> Weighted Voronoi Stippling, Adrian Secord. In: Proceedings of the 2nd International Symposium on Non-photorealistic Animation and Rendering. NPAR ’02. ACM, 2002, pp. 37– 43.

## Result
![result](https://frankr0.github.io/CV/Weighted-Voronoi-Stippling/result.jpg)

## Pre-requisites
This implementation has been written and tested on Manjaro using the following packages:
- GCC 8.2.1
- OpenCV 4.0.0

## Usage
```
 Usage: Stippling [params] image 

	-?, -h, --help, --usage (value:true)
		print this message
	-N, -n, --number
		points number
	-d, --draw
		draw result repeatedly
	-e, --epoch
		epochs number
	-i, --inverse
		inverse image

	image
		image  for redering
```
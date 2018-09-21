#pragma once
#include "avansvisionlib.h"
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv\cv.h>

	int startFase1();
	vector<vector<Point>>  MooreBoundry(Mat img);
	vector<Point> calculateContour(Point startPixel, Mat img, Mat contour);
	int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos);
	bool firstPixelOutofbounds(Point firstPixel, Mat img);

	vector<int> calculateBendingEnergy(Mat img);
	vector<vector<int>> calculateChainCode(Mat img);
	

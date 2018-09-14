#pragma once
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "avansvisionlib.h"
#include <opencv2/opencv.hpp>


	int startFase1();
	void MooreBoundry(Mat img);
	vector<Point> calculateContour(Point startPixel, Mat img, Mat contour);
	void calculateChainCode(Mat img);
	void calculateBendingEnergy(vector<vector<int>> chainCodes);
	int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos);
	void printImgData(Mat img);

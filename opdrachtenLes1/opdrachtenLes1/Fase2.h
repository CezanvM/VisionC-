#pragma once
#include "Fase1.h"


struct boundingBoxStruct {
	Point min = Point(99999, 99999);
	Point max = Point(0,0); 
};


int startFase2();
void boundingBox(Mat img);
int allBoundingBoxes(const vector<vector<Point>> & contours, vector<vector<Point>> bbs);

void drawBoundingBoxes(vector<boundingBoxStruct> boxes, Mat img);
vector<boundingBoxStruct> calculateBoundingBoxes(vector<vector<Point>> contours);
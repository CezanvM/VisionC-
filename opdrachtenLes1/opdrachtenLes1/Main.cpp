#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "avansvisionlib.h"
#include <opencv2/opencv.hpp>

void printImgData(Mat img);
Mat invertBits(Mat img);
void MooreBoundry(Mat img);


Point Offset[8] = {
	{ -1, -1 },   //  +----------+----------+----------+
{ 0, -1 },        //  |          |          |          |
{ 1, -1 },        //  |(x-1,y-1) | (x,y-1)  |(x+1,y-1) |
{ 1, 0 },         //  +----------+----------+----------+
{ 1, 1 },         //  |(x-1,y)   |  (x,y)   |(x+1,y)   |
{ 0, 1 },         //  |          |          |          |
{ -1, 1 },        //  +----------+----------+----------+
{ -1, 0 }         //  |          | (x,y+1)  |(x+1,y+1) |
};                //  |(x-1,y+1) |          |          |
				  //  +----------+----------+----------+

int main()
{

	Mat Numbers;
	Numbers = imread("res/rummikub0.jpg", CV_LOAD_IMAGE_COLOR);
	if (!Numbers.data)
	{
		cout << "Could not open or find the image" << std::endl;
		getchar();
		return -1;
	}


	Mat monsters; 
	monsters = imread("res/monsters0.jpg", CV_LOAD_IMAGE_COLOR);
	if (!monsters.data)
	{
		cout << "Could not open or find the image" << std::endl;
		getchar();
		return -1;
	}
	Mat gray_image_montser;
	cvtColor(monsters, gray_image_montser, CV_BGR2GRAY);


	Mat monstersTreshold;
	threshold(gray_image_montser, monstersTreshold, 120, 1, CV_THRESH_BINARY_INV);

	imshow("monsters", monstersTreshold * 255);
	//waitKey(0);
	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(Numbers, gray_image, CV_BGR2GRAY);
	//imshow("Original_grey", gray_image);
	//waitKey(0);

	//region of interest
	Mat roi = gray_image(Rect(30, 25, 980, 100));
	//imshow("regoin of interest img", roi);

	//Treshold 
	Mat treshhold_Img;
	threshold(roi, treshhold_Img, 120, 1, CV_THRESH_BINARY_INV);

	MooreBoundry(treshhold_Img);
	MooreBoundry(monstersTreshold);

	waitKey(0);
	getchar();

}

void calculateContour(Point startPixel, Mat img, Mat contour);

void MooreBoundry(Mat img)
{
	Mat contour = Mat::zeros(img.rows, img.cols, img.type());

	img.convertTo(img, CV_16S);
	Mat blobs = Mat::zeros(img.rows, img.cols, img.type());
	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(img, blobs, firstpixelVec2, posVec2, areaVec2);
	img.convertTo(img, CV_8U);
	

	for (int i = 0; i < firstpixelVec2.size(); i++) 
	{
		if (areaVec2[i] > 50)
		{
			calculateContour(Point(firstpixelVec2[i]->y, firstpixelVec2[i]->x), img, contour);
		}
	}

	imshow("Controur" , contour * 255);
	waitKey();

}

int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos);

void calculateContour(Point startPixel, Mat img, Mat contour)
{
	bool finished = false;
	uchar char1 = 1;
	int backTrackPos = 0;
	contour.at<uchar>(startPixel) = char1;
	Point currentPos = startPixel;
	Point lastPos;
	
	while (!finished)
	{
		cout << currentPos << endl;
		int index = 0;
		int whileindex = backTrackPos;
		while (index < 8)
		{
			if (img.at<uchar>(currentPos + Offset[whileindex]) == char1)
			{
				lastPos = currentPos;
				currentPos = currentPos + Offset[whileindex];
				backTrackPos = calculateBackTrackPos(img, currentPos, lastPos);
				contour.at<uchar>(lastPos) = char1;
				break;
			}

			whileindex++;
			if (whileindex > 7)
			{
				whileindex = 0;
			}
		}

		if (currentPos == startPixel)
			finished = true;
	}

}

int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos)
{
	int backtrackPos[8][3] = {
		// { xdif , ydif, backtrack Start pos}
	{ -1, 0, 1 },
	{ -1, -1, 1 },
	{ 0, -1, 3 },
	{ 1, 0, 5 },
	{ 0, 1, 7 },
	{ 1, 1, 5 },
	{1,-1,3},
	{-1,1,1}
	};

	int backtrackStartPos;
	int newBacktrackPos;
	Point pointdif = lastPos - currentPos;

	for (int i = 0; i < 8; i++)
	{
		if (pointdif.x == backtrackPos[i][0] && pointdif.y == backtrackPos[i][1])
		{
			backtrackStartPos = backtrackPos[i][2];

			break;
		}


	}

	int index = 0;
	int whileIndex = backtrackStartPos;

	while (index < 8)
	{
		//cout << whileIndex << endl;
		//cout << currentPos << endl;
		//cout << Offset[whileIndex] << endl;
		if (img.at<uchar>(currentPos + Offset[whileIndex]) == 0)
		{
			newBacktrackPos = whileIndex;
			break;
		}


		whileIndex++;
		if (whileIndex > 7)
			whileIndex = 0;
	}

	return newBacktrackPos;
}

void printImgData(Mat img)
{
	Mat float_IMG;
	img.convertTo(float_IMG, CV_32F);
	cout << float_IMG << endl;
}

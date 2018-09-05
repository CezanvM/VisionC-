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

	Mat image;
	image = imread("res/rummikub0.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		getchar();
		return -1;
	}
	//imshow("Original", image);
	//waitKey(0);
	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	//imshow("Original_grey", gray_image);
	//waitKey(0);

	//region of interest
	Mat roi = gray_image(Rect(30, 25, 980, 100));
	//imshow("regoin of interest img", roi);

	//Treshold 
	Mat treshhold_Img;
	Range range = Range(128, 255);
	threshold(roi, treshhold_Img, 120, 1, 0);
	//imshow("treshold", treshhold_Img );

	Mat Number9 = treshhold_Img(Rect(900, 0, 80, 80));
	// invert img
	Number9 = invertBits(Number9);
	imshow("Number", Number9 * 255);
	printImgData(Number9);

	//moore boundry tracking algrtithm 
	MooreBoundry(Number9);


	waitKey(0);
	getchar();



}


Point findFirstPixel(Mat img);
Mat calculateContour(Point startPixel, Mat img);

void MooreBoundry(Mat img)
{
	Point firstPixel = findFirstPixel(img);
	imshow("Contour", calculateContour(firstPixel, img) * 255);

}

int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos);

Mat calculateContour(Point startPixel, Mat img)
{
	bool finished = false;
	uchar char1 = 1;
	int backTrackPos = 0;
	Mat contour = Mat::zeros(img.cols, img.rows, img.type());
	contour.at<uchar>(startPixel) = char1;
	Point currentPos = startPixel;
	Point lastPos;

	while (!finished)
	{
		int index = 0;
		int whileindex = backTrackPos;
		while (index < 8)
		{
			if (img.at<uchar>(currentPos + Offset[whileindex]) == 1)
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

	return contour;
}

int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos)
{
	int backtrackPos[6][3] = {
		// { xdif , ydif, backtrack Start pos}
	{ -1, 0, 1 },
	{ -1, -1, 1 },
	{ 0, -1, 3 },
	{ 1, 0, 5 },
	{ 0, 1, 7 },
	{ 1, 1, 5 }
	};

	int backtrackStartPos;
	int newBacktrackPos;
	Point pointdif = lastPos - currentPos;

	for (int i = 0; i < 6; i++)
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

Point findFirstPixel(Mat img)
{
	for (int y = 0; y < img.cols; y++)
	{
		for (int x = 0; x < img.rows; x++)
		{
			if (img.at<uchar>(Point(x, y)) == 1)
			{
				cout << "first pixel found at [" << x << "," << y << "]" << endl;
				return Point(x, y);
			}

		}
	}
}

//void 


//
//void MooreRowCheck(Mat img, Mat boundry, int x, int y)
//{
//	uchar char1 = 1;
//
//	while (true)
//	{
//		if (img.at<uchar>(x - 1, y - 1) == 1)
//			boundry.at<uchar>(Point(y - 1, x - 1)) = char1;
//
//		else if (img.at<uchar>(x - 1, y ) == 1)
//			boundry.at<uchar>(Point(y - 1, x)) = char1;
//
//		else if (img.at<uchar>(x - 1, y + 1) == 1)
//			boundry.at<uchar>(Point(y - 1, x + 1)) = char1;
//
//		else if (img.at<uchar>(x, y + 1) == 1)
//			boundry.at<uchar>(Point(y, x + 1)) = char1;
//		else
//			break;
//
//		x++;
//		
//	}
//}



Mat invertBits(Mat img)
{
	Mat inverted_IMG;
	inverted_IMG = Mat::zeros(img.cols, img.rows, img.type());

	for (int x = 0; x < img.rows; x++)
	{
		for (int y = 0; y < img.cols; y++)
		{
			if (img.at<uchar>(x, y) == 1)
			{
				uchar char0 = 0;
				inverted_IMG.at<uchar>(Point(y, x)) = char0;
			}
			else
			{
				uchar char1 = 1;
				inverted_IMG.at<uchar>(Point(y, x)) = char1;
			}

		}
	}
	return inverted_IMG;

}


void printImgData(Mat img)
{
	Mat float_IMG;
	img.convertTo(float_IMG, CV_32F);
	cout << float_IMG << endl;
}

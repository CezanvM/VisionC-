#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "avansvisionlib.h"
#include <opencv2/opencv.hpp>

void printImgData(Mat img);
Mat invertBits(Mat img);
void MooreBoundry(Mat img);


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
//void MooreRowCheck(Mat img, Mat boundry, int x, int y);
//
void MooreBoundry(Mat img)
{
	//	Mat boundry = Mat::zeros(img.cols, img.rows, img.type());
	//
	//	for (int x = 0; x < img.rows; x++)
	//	{
	//		for (int y= 0; y < img.cols; y++)
	//		{
	//
	//			if (img.at<uchar>(x, y) == 1)
	//			{
	//				uchar char1 = 1;
	//				boundry.at<uchar>(Point(x, y)) = char1;
	//				MooreRowCheck(img, boundry, x, y);
	//				break;
	//
	//			}
	//
	//		}
	//	}
	//
	//	imshow("Boundry", boundry * 255);
	//
}


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

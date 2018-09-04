#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "avansvisionlib.h"

int main ()
{

	Mat image;
	image = imread("res/rummikub0.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		getchar();
		return -1;
	}
	imshow("Original", image);
	waitKey(0);
	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	imshow("Original_grey", gray_image);
	waitKey(0);

	
	
	Mat treshhold_Img;
	Range range = Range(128, 255);
	

	threshold(gray_image, treshhold_Img, 128, 255, 3);

	imshow("treshold", treshhold_Img * 255);
	waitKey(0);
	getchar();



}
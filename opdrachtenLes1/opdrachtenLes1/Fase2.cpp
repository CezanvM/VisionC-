#include "Fase2.h"


// load testImage
int startFase2()
{

	cout << "start fase 2" << endl;

	Mat testImage;
	testImage = imread("res/rummikub0.jpg", CV_LOAD_IMAGE_COLOR);
	if (!testImage.data)
	{
		cout << "Could not open or find the image" << endl;
		getchar();
		return -1;
	}
	Mat testImage_Gray;
	cvtColor(testImage, testImage_Gray, CV_BGR2GRAY);
	Mat testImage_threshold;
	threshold(testImage_Gray, testImage_threshold, 120, 1, CV_THRESH_BINARY_INV);


	boundingBox(testImage_threshold);

	cout << "end fase2 \n\n\n" << endl;
}

void boundingBox(Mat img)
{
	vector<vector<Point>> contours;
	contours = MooreBoundry(img);

	vector<boundingBoxStruct> boundingBoxes = calculateBoundingBoxes(contours);
	
	drawBoundingBoxes(boundingBoxes, img);
	imshow("boundingBox", img * 255);
				
	// cut image by bounding box
	
}

void drawBoundingBoxes(vector<boundingBoxStruct> boxes, Mat img)
{
	for (int boxindex = 0; boxindex < boxes.size(); boxindex++)
	{
		cv::rectangle(img, boxes[boxindex].min, boxes[boxindex].max, cv::Scalar(255,0,0));

	}

}

vector<boundingBoxStruct> calculateBoundingBoxes(vector<vector<Point>> contours)
{
	vector<boundingBoxStruct> boundingBoxses;
	for (int contoursIndex = 0; contoursIndex < contours.size(); contoursIndex++)
	{
		boundingBoxStruct boundingboxStruct;
		for (int pointIndex = 0; pointIndex < contours[contoursIndex].size(); pointIndex++)
		{
			Point currentPoint = contours[contoursIndex][pointIndex];

			if (currentPoint.x < boundingboxStruct.min.x)
				boundingboxStruct.min.x = currentPoint.x;
			else if (currentPoint.x > boundingboxStruct.max.x)
				boundingboxStruct.max.x = currentPoint.x;


			if (currentPoint.y < boundingboxStruct.min.y)
				boundingboxStruct.min.y = currentPoint.y;
			else if (currentPoint.y > boundingboxStruct.max.y)
				boundingboxStruct.max.y = currentPoint.y;

		}
		boundingBoxses.push_back(boundingboxStruct);
	}

	return boundingBoxses;
}


int allBoundingBoxes(const vector<vector<Point>> & contours, vector<vector<Point>> bbs)
{
	
	//foreach contour calculate max and min 
	// calculate position of the box
	// add box to bbs 
	// debug show image; 



	return 0;
}

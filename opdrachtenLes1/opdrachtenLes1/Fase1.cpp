#include "Fase1.h"


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

int startFase1()
{
	cout << "start fase1" << endl;
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

	Mat gray_image;
	cvtColor(Numbers, gray_image, CV_BGR2GRAY);

	Mat roi = gray_image(Rect(30, 25, 980, 100));
	Mat treshhold_Img;
	threshold(roi, treshhold_Img, 120, 1, CV_THRESH_BINARY_INV);

	MooreBoundry(treshhold_Img);
	MooreBoundry(monstersTreshold);

	cout << "end fase1 \n\n\n" << endl;
}

vector<vector<Point>> MooreBoundry(Mat img)
{
	Mat contour = Mat::zeros(img.rows, img.cols, img.type());

	img.convertTo(img, CV_16S);
	Mat blobs = Mat::zeros(img.rows, img.cols, img.type());
	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;

	vector<vector<Point>> contourVec;

	labelBLOBsInfo(img, blobs, firstpixelVec2, posVec2, areaVec2);
	img.convertTo(img, CV_8U);

	for (int i = 0; i < firstpixelVec2.size(); i++)
	{
		if (areaVec2[i] > 75)
		{
			vector<Point> contourvector = calculateContour(Point(firstpixelVec2[i]->y, firstpixelVec2[i]->x), img, contour);
				if(!contourvector.empty())
					contourVec.push_back(contourvector);
		}
	}
	calculateBendingEnergy(img);
	double id = generateRandomValue(0, 99);
	std::ostringstream strs;
	strs << "Contour";
	strs << id;
	std::string str = strs.str();
	imshow(str, contour * 255);
	return contourVec;

}
vector<Point> calculateContour(Point startPixel, Mat img, Mat contour)
{
	bool finished = false;
	uchar char1 = 1;
	int backTrackPos = 0;
	contour.at<uchar>(startPixel) = char1;
	Point currentPos = startPixel;
	Point lastPos;
	vector<Point> contourVec;

	if (firstPixelOutofbounds(startPixel, img))
	{
		return contourVec;
	}
	while (!finished)
	{
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
				contourVec.push_back(lastPos);
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
	return contourVec;
}

bool firstPixelOutofbounds(Point firstPixel, Mat img)
{
	if (firstPixel.x == 0 || firstPixel.y == 0)
		return true;
	if (firstPixel.x >= img.cols || firstPixel.y >= img.rows)
		return true;

	return false;
}

vector<vector<int>> calculateChainCode(Mat img)
{
	CvChain* chain = 0;
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	cvFindContours(&IplImage(img), storage, (CvSeq**)(&chain), sizeof(*chain), CV_RETR_EXTERNAL, CV_CHAIN_CODE);
	vector<vector<int>> chaincode;

	for (; chain != NULL; chain = (CvChain*)chain->h_next)
	{
		CvSeqReader reader;
		int i, total = chain->total;
		cvStartReadSeq((CvSeq*)chain, &reader, 0);

		if (total > 50)
		{
			vector<int> singleChainCode;
			for (i = 0; i < total; i++)
			{
				char code;
				CV_READ_SEQ_ELEM(code, reader);
				singleChainCode.push_back(code);
			}
			chaincode.push_back(singleChainCode);
		}
	}
	return chaincode;
}




vector<int> calculateBendingEnergy(Mat img)
{
	vector<int> bendingEnergies;
	vector<vector<int>> chainCodes = calculateChainCode(img);
	for (int x = 0; x < chainCodes.size(); x++)
	{
		int bendingEnergy = 0;

		for (int y = 0; y < chainCodes.at(x).size(); y++)
		{
			int ClockWiseIndex = 0;
			int counterClockwise = 0;
			int currentNumber = chainCodes.at(x).at(y);
			int NextNumber = chainCodes.at(x).at(y + 1);
			bool stepping = true;

			while ((currentNumber + ClockWiseIndex) % 8 != NextNumber)
			{
				ClockWiseIndex++;
			}
			int currentNumberFixed = (currentNumber - counterClockwise);
			while (stepping)
			{
				if (currentNumberFixed < 0)
					currentNumberFixed = 7;

				if (currentNumberFixed == NextNumber)
				{
					stepping = false;
					break;
				}
				currentNumberFixed--;
				counterClockwise++;
			}


			if (ClockWiseIndex < counterClockwise)
				bendingEnergy += 1 * ClockWiseIndex;
			else
				bendingEnergy += 1 * counterClockwise;

			if (chainCodes.at(x).size() == y + 2)
				break;

		}
		bendingEnergies.push_back(bendingEnergy);
		cout << "bending energy is: " << bendingEnergy << endl;
	}
	return bendingEnergies;
}

int calculateBackTrackPos(Mat img, Point currentPos, Point lastPos)
{
	int backtrackPos[8][3] = {
	{ -1, 0, 1 },
	{ -1, -1, 1 },
	{ 0, -1, 3 },
	{ 1, 0, 5 },
	{ 0, 1, 7 },
	{ 1, 1, 5 },
	{1,-1,3},
	{-1,1,7}
	};
	int backtrackSize = 8;

	int backtrackStartPos;
	int newBacktrackPos;
	Point pointdif = lastPos - currentPos;

	for (int i = 0; i < backtrackSize; i++)
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

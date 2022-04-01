



#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
using namespace cv;
using namespace std;
Mat imgOriginal;
Mat imgHSV;
Mat imgThresholded;
int main(int argc, char** argv)
{
	VideoCapture cap(0);

	if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", WINDOW_AUTOSIZE);

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	createTrackbar("LowH", "Control", &iLowH, 179);
	createTrackbar("HighH", "Control", &iHighH, 179);
	createTrackbar("LowS", "Control", &iLowS, 255); )
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255); 
	createTrackbar("HighV", "Control", &iHighV, 255);

	while (true)
	{


		bool bSuccess = cap.read(imgOriginal); 

		if (!bSuccess) 
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
        resize(imgOriginal, imgOriginal, Size(300,300));
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		vector<vector<Point> > contours;
		vector<Vec4i>hierarchy;
		findContours(imgThresholded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		vector<Moments> mu(contours.size());
		cout << contours.size() << endl;
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i]);
		}
		vector<Point2f> mc(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			int mcenterx = mu[i].m10 / mu[i].m00;
			int mcentery = mu[i].m01 / mu[i].m00;
			mc[i] = Point2f((mcenterx), (mcentery));
			drawContours(imgOriginal, contours, i, Scalar(0, 255, 0), 2, 0);
			circle(imgOriginal, Point(mcenterx, mcentery),3, Scalar(0, 0, 255),3, LINE_AA, 0);
            putText(imgOriginal,("x:"+to_string(mcenterx)+"-y:"+to_string(mcentery)),Point(50,50),FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,0),3,LINE_AA);

		}

		imshow("Thresholded Image", imgThresholded);
		imshow("Original", imgOriginal); 

		if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}

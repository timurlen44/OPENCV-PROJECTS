/*#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace std;
using namespace cv;

Mat orgImg;
Mat src_gray;

Mat video;
VideoCapture cap(0);
int main()
{
    while(1){
	cap >> orgImg;
	//orgImg = imread("eyes.jpg");
	imshow("ss", orgImg);

	cvtColor(orgImg, src_gray, COLOR_BGR2GRAY);
	GaussianBlur(src_gray, src_gray, Size(3, 3), 2, 2);

	vector<Vec3f> circles;


	HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows / 8, 30, 80);


	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(orgImg, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		circle(orgImg, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}



	imshow("Hough Circle Transform Demo", orgImg);


        waitKey(1);


	}
	return 0;
}
*/



/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat hsv, imthresh, im, b;
Mat video;
VideoCapture cap(0);

int iLowH = 0;
int iHighH = 179;

int iLowS = 0;
int iHighS = 34;

int iLowV = 109;
int iHighV = 255;





void track(int, void*) {

	return;
}



int main()
{
	cap >> video;
	namedWindow("Control", WINDOW_AUTOSIZE);


	createTrackbar("LowH", "Control", &iLowH, 255, track);
	createTrackbar("LowS", "Control", &iLowS, 255, track);
	createTrackbar("LowV", "Control", &iLowV, 255, track);

	createTrackbar("HighH", "Control", &iHighH, 255, track);


	createTrackbar("HighS", "Control", &iHighS, 255, track);


	createTrackbar("HighV", "Control", &iHighV, 255, track);
	double rates;
	int s = 5;
	while (true) {
		cap >> video;
		resize(video, video, Size(300, 300));


		GaussianBlur(video, b, Size(3,3), 0);
		cvtColor(b, hsv, COLOR_BGR2GRAY, 0);

		inRange(hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imthresh);

		erode(imthresh, imthresh, getStructuringElement(MORPH_ELLIPSE, Size(s,s)));
		dilate(imthresh, imthresh, getStructuringElement(MORPH_ELLIPSE, Size(s, s)));

		dilate(imthresh, imthresh, getStructuringElement(MORPH_ELLIPSE, Size(s, s)));
		erode(imthresh, imthresh, getStructuringElement(MORPH_ELLIPSE, Size(s, s)));

		dilate(imthresh, imthresh, getStructuringElement(MORPH_ELLIPSE, Size(s, s)));
		erode(imthresh, imthresh, getStructuringElement(MORPH_ELLIPSE, Size(s, s)));

		vector<vector<Point> > contours;
		vector<Vec4i>hierarchy;

		//findContours(imthresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(imthresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		vector<Moments> mu(contours.size());
		cout << contours.size() << endl;
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i]);
		}
		vector<Point2f> mc(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			float mcenterx = mu[i].m10 / mu[i].m00;
			float mcentery = mu[i].m01 / mu[i].m00;
			mc[i] = Point2f((mcenterx), (mcentery));
			drawContours(video, contours, i, Scalar(0, 255, 0), 2, 0);
			circle(video, Point(mcenterx, mcentery),3, Scalar(0, 0, 255),3, LINE_AA, 0);


		}
		imshow("Video", video);
		imshow("Renk", imthresh);


		//waitKey(100);

		if (waitKey(1) == 27) break;
	}

	return 0;
}
*/



#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
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
	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);
	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	while (true)
	{


		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
        resize(imgOriginal, imgOriginal, Size(300,300));
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		vector<vector<Point> > contours;
		vector<Vec4i>hierarchy;

		//findContours(imthresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
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

		imshow("Thresholded Image", imgThresholded); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}

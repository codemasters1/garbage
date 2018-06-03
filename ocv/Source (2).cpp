#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using std::string;
using std::vector;
using cv::VideoCapture;
using cv::Mat;
using cv::Point;
using cv::Scalar;
using cv::namedWindow;
using cv::createTrackbar;
using cv::waitKey;


int main(int argc, char** argv)
{
	VideoCapture capture = VideoCapture(CV_CAP_ANY);

	Mat frame, img, hsv_img, binary;
	vector<Mat> hsv_split;

	int thr = 100;
	namedWindow("bin", 0);
	createTrackbar("ub", "bin", &thr, 255, NULL);

	while (waitKey(100) != 'q')
	{
		capture >> frame;
		frame.copyTo(img);

		cvtColor(img, hsv_img, CV_BGR2HSV);
		split(hsv_img, hsv_split);

		GaussianBlur(hsv_split[1], hsv_split[1], cv::Size(21, 21), 4.0);

		threshold(hsv_split[1], binary, thr, 255, cv::THRESH_BINARY);

		vector<vector<Point>> contours;
		findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (auto c : contours)
		{
			vector<Point> approx;
			approxPolyDP(c, approx, arcLength(Mat(c), true) * 0.02, true);

			if (/**/1)
			{
				//std::cout << "test\n";
				putText(img, std::to_string(approx.size()), approx.at(0), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 200, 200), 4);
			}
		}

		imshow("org", img); 
		imshow("bin", binary);
	}

	capture.release();

	return 0;
}
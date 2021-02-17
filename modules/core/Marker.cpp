#include "Marker.h"

void CMarker::markFaces(Mat& img, const std::vector<ptr_face_t>& vFaces)
{
	for (auto face : vFaces) {
		// ------ PUT YOUR CODE HERE -------
		// 1. Draw all faces using face->getArea();
		// 2. Print the text using face->getText();
		// 3. Print face id using face->getId();
	}
}

void CMarker::markPoints(Mat& img, const std::vector<Point2f>& vPoints, Scalar color)
{
	for (auto& point : vPoints) 
		circle(img, point, 3, color, 2);
}

void CMarker::markVecOFF(Mat& img, const Mat& hFlow, const Mat& vFlow)
{
	// ------ PUT YOUR CODE HERE -------
}

void CMarker::markGUI(Mat& img)
{
	// ------ PUT YOUR CODE HERE -------
	// Implement yout ouw GUI
	// Show fps
	for (int y = 0; y < img.rows; y++) {
		Vec3b* ptr = img.ptr<Vec3b>(y);
		for (int x = 0; x < img.cols; x++) {
			float k = static_cast<float>(x) / img.cols;
			ptr[x] = Vec3b(static_cast<byte>(k * 255), 0, 255 - static_cast<byte>(k * 255));
		}
	}
	circle(img, Point(img.cols / 2, img.rows / 2), 50, CV_RGB(100, 255, 100), 5);
	GaussianBlur(img, img, Size(17, 17), 50);
	putText(img, "HCI", Point(100, 100), FONT_HERSHEY_SIMPLEX, 2, CV_RGB(255, 255, 255), 5);
}

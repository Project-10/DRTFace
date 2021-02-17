#pragma once

#include "types.h"
#include "Face.h"

class CMarker {
public:
	/** 
	* @brief Draws faces into the image
	*/
	DllExport static void markFaces(Mat& img, const std::vector<ptr_face_t>& vFaces);
	
	/**
	* @brief Draws the points (e.g. from GoodFeaturesToTrack) into the image
	*/
	DllExport static void markPoints(Mat& img, const std::vector<Point2f>& vPoints, Scalar color = CV_RGB(255, 165, 0));
	
	/**
	* @brief Draws vectors of the vector Optical Flow Field () into the image
	*/
	DllExport static void markVecOFF(Mat& img, const Mat& hFlow, const Mat& vFlow);
	
	/**
	* @brief Draws GUI
	*/
	DllExport static void markGUI(Mat& img);
};


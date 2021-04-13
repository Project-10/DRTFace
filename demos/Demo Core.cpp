#include <iostream>
#include "drtface.h"
#include <sys/wait.h>
#include <opencv2/dnn.hpp>

using namespace std;
using namespace drtf;
using namespace dnn;
using namespace cv::dnn;

static string class_name[] = {"Angry","Disgust","Fear","Happy","Neutral","Sad","Surprise"};
CascadeClassifier face_cascade(dataPath + "haarcascade_frontalface_default.xml");
string path = "../../model/frozen_models/frozen_graph.pb";
Net net = readNetFromTensorflow(path);

std::vector<ptr_face_t> faceDetector(const Mat& img) {
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	equalizeHist(img_gray, img_gray);
	std::vector<ptr_face_t> res;
	
	std::vector<Rect> vObjects;
	face_cascade.detectMultiScale(img_gray, vObjects);
	
	for (auto object: vObjects)
		res.push_back(std::make_shared<CFace>(object));
	
	return res;
}

std::string get_label(Net net, Mat frame){
	Mat frame_32F;
     frame.convertTo(frame_32F,CV_32FC1);
     Mat blob = blobFromImage(1-frame_32F/255.0,
                              1.0,
                              Size(48,48),
                              Scalar(0,0,0));
     net.setInput(blob);
     Mat out = net.forward();
     Point maxclass;
     minMaxLoc(out, NULL, NULL, NULL, &maxclass);
     return class_name[maxclass.x];
}

void assign_labels(Mat& img, const std::vector<ptr_face_t>& vFaces){
	for (const auto& face : vFaces){
		Mat grey;
		cv::cvtColor(img(face->getArea()), grey, COLOR_BGR2GRAY);
		face->setText(get_label(net, grey));
	}
}

int main() {

	std::vector<Point2f> vPoints;
	std::vector<ptr_face_t> vpFaceses;
	
	namedWindow("Camera");

	// mouse callback function which fills vPoints with coordinates of mouse clicks
	setMouseCallback("Camera", [] (int event, int x, int y, int flags, void* userdata) {
		if (userdata && event == EVENT_LBUTTONDOWN) {
			std::vector<Point2f> *pvPoints = (std::vector<Point2f> *) userdata;
			pvPoints->push_back(Point2f(static_cast<float>(x), static_cast<float>(y)));
		}
	}, (void*) &vPoints);
	
	CCameraCantroller controller(16);

	controller.init();
	controller.start();
	
	// Main loop
	Mat img;
	for(;;) {
		img = controller.getFrame();

		if (!img.empty()) {

			vpFaceses = faceDetector(img);
			assign_labels(img, vpFaceses);
			CMarker::markFaces(img, vpFaceses);
			
			// vPoints = good features to track ()
			// CMarker::markPoints(img, vPoints);
			// { hFlow, vFlow } = calculate optical flow (vPoints)
			// CMarker::markVecOFF(mask, hFlow, vFlow);
			
			//CMarker::markGUI(mask);

			imshow("Camera", img);
		}
		int key = waitKey(5);
		if (key == 27 || key == 'q') break;
	}

	controller.stop();
	return 0;
}


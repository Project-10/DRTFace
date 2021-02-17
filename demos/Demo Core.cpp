#include "drtface.h"

using namespace drtf;

CascadeClassifier face_cascade(dataPath + "haarcascade_frontalface_default.xml");

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
	

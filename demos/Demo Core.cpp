#include "drtface.h"

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
	Mat img, mask;
	float attenuation = 0.5f;
	for(;;) {
		img = controller.getFrame();

		if (!img.empty()) {
			if (mask.empty()) mask = Mat(img.size(), img.type());
			mask.setTo(0);

			// ------ PUT YOUR CODE HERE -------
			// vpFaceses = detect faces();
			CMarker::markFaces(mask, vpFaceses);
			
			// ------ PUT YOUR CODE HERE -------
			// vPoints = good features to track ()
			CMarker::markPoints(mask, vPoints);
			// { hFlow, vFlow } = calculate optical flow (vPoints)
			// CMarker::markVecOFF(mask, hFlow, vFlow);
			
			CMarker::markGUI(mask);
			

			add(img, attenuation * mask, img);

			imshow("Camera", img);
		}
		int key = waitKey(5);
		if (key == 27 || key == 'q') break;
		if (key == 'a') attenuation *= 1.1f;
		if (key == 'z') attenuation *= 0.9f;
	}

	controller.stop();
	return 0;
}
	

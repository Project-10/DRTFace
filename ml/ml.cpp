#include "drtface.h"
#include <string>
#include <ctime>
#include <random>
#include <sstream>
#include <algorithm>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>


using namespace std;
using namespace ml;
// using namespace cv::xfeatures2d;

struct struc_data {
	Mat image;
	String label;
};

//load dataset
vector<struc_data> load_data(String path){

	vector <String> files;
	vector <struc_data> dataset;

	//get filenames of all images
	glob (path + "/*.jpg", files, true);
	
	//load each image and label
	size_t n_files = files.size();

	for (size_t i=0; i<n_files; i++){
		Mat image =imread(files[i]);

		//extract label from filepath
		vector <String> tokens;
		stringstream temp(files[i]);
		string line;
		while(std::getline(temp,line,'/'))
			tokens.push_back(line);
		String label = tokens.end()[-2];

		//add image and label to dataset
		dataset.push_back({image, label});
	}

	//shuffle dataset
	random_device rd;
	mt19937 g(rd());
	shuffle(dataset.begin(), dataset.end(), g);

	return dataset;
}

//resize images
vector <Mat> resize_images(vector <Mat> images, int s){
	vector <Mat> res;
	size_t n = images.size();
	for (size_t i = 0; i < n; i++){
		Mat resized_im;
		resize(images[i], resized_im, Size(s,s));
		res.push_back(resized_im);
	}
	return res;
}

//prepare labels for training/testing
vector<int> map_to_int(vector<String> labels, vector <String> classes){
	vector<int> int_labels;
	int count = 0;
	for (size_t i = 0; i < labels.size(); i++){
		for (int j = 0; j < classes.size(); j++){
			if (labels[i] == classes[j]){
				int_labels.push_back(j);
				count++;
				break;
			}
		}
	}
	return int_labels;
}

void dlib_point2cv_Point(dlib::full_object_detection& S,vector<Point>& L, double scale){
    for(unsigned int i = 0; i<S.num_parts();++i)
        L.push_back(Point(S.part(i).x()*(1/scale),S.part(i).y()*(1/scale)));
}

vector<dlib::rectangle> getFaceRects(Mat & img){
	vector<Mat> faces;
	dlib::frontal_face_detector hogFaceDetector = dlib::get_frontal_face_detector();
	//convert cv img to dlib img
	dlib::cv_image<dlib::bgr_pixel> dimg(img);
	//detect faces
	return hogFaceDetector(dimg);
}

vector<Mat> getFaces(Mat & img){
	vector<Mat> faces;
	//detect faces
	vector<dlib::rectangle> faceRects = getFaceRects(img);
	for ( size_t i = 0; i < faceRects.size(); i++ ){
		int x1 = faceRects[i].left();
		int y1 = faceRects[i].top();
		int x2 = faceRects[i].right();
		int y2 = faceRects[i].bottom();
		Rect face(Point(x1, y1), Point(x2, y2));
		faces.push_back(img(face));
	}
	return faces;
}

Mat getFace(Mat & img){
	vector<dlib::rectangle> faceRects = getFaceRects(img);
	if (faceRects.size() == 0){
		return img;
	}
	else {
		assert(faceRects.size() == 1);
		int x1 = faceRects[0].left();
		int y1 = faceRects[0].top();
		int x2 = faceRects[0].right();
		int y2 = faceRects[0].bottom();
		if (x1 < 0) x1 = 0;
		if (y1 < 0) y1 = 0;
		if (x2 >= img.cols) x2 = img.cols;
		if (y2 >= img.rows) y2 = img.rows;
		Rect face(Point(x1, y1), Point(x2, y2));
		return img(face);
	}
}

vector<Point> getLandmark(Mat & face){
	dlib::shape_predictor predictor;
	dlib::deserialize(dataPath + "shape_predictor_68_face_landmarks.dat") >> predictor;
	vector<dlib::rectangle> faceRects = getFaceRects(face);
	if (faceRects.size() == 0){
		dlib::rectangle faceRect(300,300);
		dlib::cv_image<dlib::bgr_pixel> dimg(face);
		dlib::full_object_detection lm_d = predictor(dimg, faceRect);
		vector<Point> lm_cv;
		dlib_point2cv_Point(lm_d,lm_cv,1);
		return lm_cv;
	}
	else{
		dlib::cv_image<dlib::bgr_pixel> dimg(face);
		dlib::full_object_detection lm_d = predictor(dimg, faceRects[0]);
		vector<Point> lm_cv;
		dlib_point2cv_Point(lm_d,lm_cv,1);
		return lm_cv;
	}

}

vector<vector<Point>> getLandmarks(vector<Mat> & faces){
	vector<vector<Point>> landmarks;
	for (int i = 0; i < faces.size(); i++)
		landmarks.push_back(getLandmark(faces[i]));
	return landmarks;
}

float euclideanDist(Point & a, Point & b){
	Point df = a - b;
	return (sqrt(df.x*df.x + df.y*df.y));
}

vector<float> getDistances(vector<Point> & landmarks){
	vector<float> distances;
	distances.push_back(euclideanDist(landmarks[39], landmarks[41]));
	distances.push_back(euclideanDist(landmarks[45], landmarks[47]));
	distances.push_back(euclideanDist(landmarks[20], landmarks[39]));
	distances.push_back(euclideanDist(landmarks[24], landmarks[45]));
	distances.push_back(euclideanDist(landmarks[22], landmarks[28]));
	distances.push_back(euclideanDist(landmarks[23], landmarks[28]));
	distances.push_back(euclideanDist(landmarks[25], landmarks[28]));
	distances.push_back(euclideanDist(landmarks[20], landmarks[28]));
	distances.push_back(euclideanDist(landmarks[28], landmarks[29]));
	distances.push_back(euclideanDist(landmarks[41], landmarks[49]));
	distances.push_back(euclideanDist(landmarks[47], landmarks[55]));
	distances.push_back(euclideanDist(landmarks[49], landmarks[55]));
	distances.push_back(euclideanDist(landmarks[63], landmarks[67]));
	distances.push_back(euclideanDist(landmarks[63], landmarks[49]));
	distances.push_back(euclideanDist(landmarks[63], landmarks[55]));
	distances.push_back(euclideanDist(landmarks[52], landmarks[58]));
	distances.push_back(euclideanDist(landmarks[34], landmarks[52]));
	distances.push_back(euclideanDist(landmarks[32], landmarks[36]));
	return distances;
}

vector<vector<float>> getFeatureVectors(vector<Mat> & images){
	cout<<"Get feature vectors of "<<images.size()<<" images."<<endl;

	vector<vector<float>> feature_vecs;
	for(size_t k = 0; k < images.size(); k++){
		Mat face = getFace(images[k]);
		vector<Point> lm = getLandmark(face);
		feature_vecs.push_back(getDistances(lm));
	}
	return feature_vecs;
}

int main(){
	vector<struc_data> train = load_data(dataPath + "fer2013_1/train/");
	vector<struc_data> test = load_data(dataPath + "fer2013_1/test/");

	//split into images and labels
	vector<Mat> train_images;
	vector<String> train_labels_n;
	size_t n = train.size();
	for (size_t i = 0; i < n; i++){
		train_images.push_back(train[i].image);
		train_labels_n.push_back(train[i].label);
	}

	vector<Mat> test_images;
	vector<String> test_labels_n;
	size_t m = test.size();
	for (size_t i = 0; i < m; i++){
		test_images.push_back(test[i].image);
		test_labels_n.push_back(test[i].label);
	}

	//prepare images
	train_images = resize_images(train_images, 400);

	vector<vector<float>> feature_vecs = getFeatureVectors(train_images);
	Mat train_data = Mat(feature_vecs.size(), feature_vecs[0].size(), CV_32FC1);
	for (size_t i = 0; i < feature_vecs.size(); i++) {
		for (size_t j = 0; j < feature_vecs[i].size(); j++) {
			train_data.at<double>(i, j) = feature_vecs[i][j];
		}
	}
	cout<<train_data.rows<<endl;

	test_images = resize_images(test_images, 400);
	vector<vector<float>> test_feature_vecs = getFeatureVectors(test_images);
	Mat test_data = Mat(test_feature_vecs.size(), test_feature_vecs[0].size(), CV_32FC1);
	for (size_t i = 0; i < test_feature_vecs.size(); i++) {
		for (size_t j = 0; j < test_feature_vecs[i].size(); j++) {
			test_data.at<double>(i, j) = test_feature_vecs[i][j];
		}
	}
	cout<<test_feature_vecs[0][0]<<endl;
	cout<<test_data.rows<<endl;

	// prepare labels
	vector<String> classes = {"neutral", "angry", "sad", "happy", "disgust", "fear", "surprise"};
	int nclasses = classes.size();
	vector<int> train_vectors = map_to_int(train_labels_n, classes);
	Mat train_labels = Mat(train_vectors);
	// train_labels.convertTo(train_labels, CV_32FC1);
    Mat train_classes = Mat::zeros(train_labels.rows, nclasses, CV_32FC1);
    for(int i=0; i<train_classes.rows; i++)
		train_classes.at<float>(i, train_labels.at<int>(i)) = 1.f;
	train_classes.convertTo(train_classes, CV_32FC1);

	vector<int> test_vectors = map_to_int(test_labels_n, classes);
	Mat test_labels = Mat(test_vectors);
	// test_labels.convertTo(test_labels, CV_32FC1);
    Mat test_classes = Mat::zeros(test_labels.rows, nclasses, CV_32FC1);
    for(int j=0; j<test_classes.rows; j++){
		test_classes.at<float>(j, test_labels.at<int>(j)) = 1.f;
	}
	test_classes.convertTo(test_classes, CV_32FC1);
	cout<<test_classes.row(0)<<endl;
	cout<<test_data.row(0)<<endl;

	Ptr<ml::TrainData> tdata = ml::TrainData::create(train_data, ml::ROW_SAMPLE, train_classes);
	cout<<"classifier"<<endl;
	// set up mlp
	Mat_<int> layers(6, 1);
	layers(0) = train_data.cols;
	layers(1) = nclasses * 4;
	layers(2) = nclasses * 8;
	layers(3) = nclasses * 16;
	layers(4) = nclasses * 4;
	layers(5) = nclasses;
	Ptr<ANN_MLP> mlp = ANN_MLP::create();
	mlp->setLayerSizes(layers);
	mlp->setActivationFunction(ANN_MLP::SIGMOID_SYM);
	mlp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 10, 0.001));
	mlp->setTrainMethod(ANN_MLP::RPROP);
	
	cout<<"train"<<endl;
	cout<<tdata->getNVars()<<" "<<tdata->getResponses().cols<<endl;
	mlp->train(tdata);
	mlp->save(dataPath + "emotion_detection_mlp.dat");
	cout<<"train finished"<<endl;
	Mat confusion(nclasses,nclasses,CV_32S, Scalar(0));

	// run tests on validation set:
    for(int i=0; i<test_data.rows; i++) {
        int pred  = mlp->predict(test_data.row(i), noArray());
		Mat predic;
		mlp->predict(test_data.row(i), predic);
		float* row = test_classes.ptr<float>(i);
        int truth = distance(row,find(row, row + 7, 1));
        confusion.at<int>(pred, truth) ++;
    }

    Mat correct = confusion.diag();
    float accuracy = sum(correct)[0] / sum(confusion)[0];
	cout << "accuracy: " << accuracy << endl;
    cout << "confusion:\n" << confusion << endl;


	// Mat img = imread(dataPath + "faces.jpg");

	// vector<Mat> faces = getFaces(img);
	// vector<vector<Point>> landmarks = getLandmarks(faces);
	// vector<vector<float>> distances;

	// for (int i = 0; i < landmarks.size(); i++)
	// 	distances.push_back(getDistances(landmarks[i]));

	// cout<<distances[0].size()<<" "<<distances[1].size()<<" "<<distances[2].size()<<endl;

	return 0;
}

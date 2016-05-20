#include <opencv2\highgui\highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2\gpu\gpu.hpp"
#include <opencv2\gpu\gpu.hpp>
#include <vector>

#include <opencv2/opencv.hpp>  

/*
https://chrisjmccormick.wordpress.com/2013/01/24/opencv-sift-tutorial/
http://docs.opencv.org/2.4/doc/user_guide/ug_features2d.html
http://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O
http://www.codeproject.com/Tips/656906/Bag-of-Features-Descriptor-on-SURF-and-ORB-Feature
http://www.pyimagesearch.com/2014/03/03/charizard-explains-describe-quantify-image-using-feature-vectors/
http://www.pyimagesearch.com/2014/01/27/hobbits-and-histograms-a-how-to-guide-to-building-your-first-image-search-engine-in-python/
http://docs.opencv.org/2.4/modules/gpu/doc/object_detection.html
http://study.marearts.com/2014/04/the-example-source-code-of-2d-vector.html
http://study.marearts.com/2014/04/example-source-code-of-extract-hog.html
http://stackoverflow.com/questions/11626140/extracting-hog-features-using-opencv
http://study.marearts.com/2015/02/extracting-two-hog-feature-and.html
http://stackoverflow.com/questions/21833103/extracting-hog-features-using-opencv
http://stackoverflow.com/questions/22886789/hog-feature-implementation-with-svm-in-matlab
http://www.di.ens.fr/willow/events/cvml2013/materials/slides/tuesday/Tue_bof_summer_school_paris_2013.pdf
http://www.vlfeat.org/overview/hog.html
https://github.com/DaHoC/trainHOG/wiki/trainHOG-Tutorial#The_source_code
http://answers.opencv.org/question/640/extracting-hog-features/
http://answers.opencv.org/question/10374/how-to-training-hog-and-use-my-hogdescriptor/
http://hogprocessing.altervista.org/
http://ask.programmershare.com/12421_11643028/
http://slazebni.cs.illinois.edu/
http://answers.opencv.org/question/877/how-to-match-2-hog-for-object-detection/
http://docs.opencv.org/2.4/modules/gpu/doc/object_detection.html
http://docs.opencv.org/2.4/modules/ml/doc/neural_networks.html
http://stackoverflow.com/questions/13351913/opencv-hog-feature-data-layout?rq=1
http://stackoverflow.com/questions/31111979/hog-features-for-object-detection-using-gpu-in-opencv?rq=1
http://stackoverflow.com/questions/10769519/svm-classifier-based-on-hog-features-for-object-detection-in-opencv
https://github.com/LihO/SVMLightClassifier
https://github.com/DaHoC/trainHOG/wiki/trainHOG-Tutorial#The_source_code
http://study.marearts.com/search/label/HOG%20feature
http://study.marearts.com/2014/04/example-source-code-hog-feature-to.html
http://study.marearts.com/2014/04/to-test-svm-trained-data-is-whether.html
http://study.marearts.com/search/label/Data
http://study.marearts.com/2014/11/opencv-svm-learning-method-and-xml.html
http://study.marearts.com/2014/04/to-test-svm-trained-data-is-whether.html
http://study.marearts.com/2014/04/example-source-code-hog-feature-to.html
http://docs.opencv.org/2.4/doc/tutorials/ml/introduction_to_svm/introduction_to_svm.html#introductiontosvms
http://docs.opencv.org/2.4/modules/ml/doc/support_vector_machines.html
http://docs.opencv.org/2.4/doc/tutorials/ml/non_linear_svms/non_linear_svms.html
http://docs.opencv.org/2.4/doc/tutorials/ml/introduction_to_svm/introduction_to_svm.html#introductiontosvms
http://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_ml/py_svm/py_svm_basics/py_svm_basics.html
//neuronal networks
http://www.codeproject.com/Articles/16447/Neural-Networks-on-C
http://stackoverflow.com/questions/100469/neural-network-example-in-net
http://www.codeproject.com/Articles/14188/Brainnet-A-Neural-Netwok-Project-With-Illustrati#1.1 Introduction To This Article Series
http://www.codeproject.com/Articles/14188/Brainnet-A-Neural-Netwok-Project-With-Illustrati#1.1 Introduction To This Article Series
http://www.codeproject.com/Articles/14188/Brainnet-A-Neural-Netwok-Project-With-Illustrati#1.1 Introduction To This Article Series
http://www.codeproject.com/Articles/19323/Image-Recognition-with-Neural-Networks
http://www.robosoup.com/2008/09/single-layer-perceptron.html
http://www.cs.columbia.edu/~kathy/cs4701/documents/jason_svm_tutorial.pdf
http://www.craigsprogramming.com/2014/01/simple-c-artificial-neural-network.html
https://visualstudiomagazine.com/Articles/2013/08/01/Neural-Network-Back-Propagation-Using-C.aspx?Page=1
https://visualstudiomagazine.com/articles/2015/04/01/back-propagation-using-c.aspx
http://www.codeproject.com/Articles/16419/AI-Neural-Network-for-beginners-Part-of


*/

using namespace std;
using namespace cv;



//opens an image saved in a specified location
//press any key to close the image window

#define dim 109

void writeToFile(char fileName[dim], vector < vector < float > > descriere);
vector < vector < float > > Mat2DToArray(int row, int col, Mat M2);
void svmTraining();
void predict();
int main()
{

	char SaveHogPositiveFileName[dim] = "Positive.xml";
	char SaveHogNegativeFileName[dim] = "Negative.xml";
	char SaveHogRateFileName[dim] = "Rate.xml";

	string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";
	string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";

	vector < vector < float > > descrierePozitiv;
	vector < vector < float > > descriereNegativ;
	vector < vector < float > > descriereRate;
	for (int i = 0; i < 19; ++i)
	{
		//citim cate o poza
		fileN = fisierSeparat + std::to_string(i + 1) + ".png";
		Mat img1 = imread(fileN);
		if (img1.empty())
		{
			cout << " Citire incorecta ";
			return 0;
		}
		Mat img1_gray;
		cvtColor(img1, img1_gray, CV_RGB2GRAY);

		Mat r_img1_gray;
		resize(img1_gray, r_img1_gray, Size(64, 8));

		HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
		vector< float> descriptorsValues1;
		vector< Point> locations1;

		d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
		if (i <= 6)
		{
			descrierePozitiv.push_back(descriptorsValues1);
		}
		else
		if(i > 6 && i <=12)
		{
			descriereNegativ.push_back(descriptorsValues1);
		}
		else
		{
			descriereRate.push_back(descriptorsValues1);
		}

		imshow("picture", img1_gray);
		waitKey(0);
		img1.release();
		img1_gray.release();
		r_img1_gray.release();
	}

	writeToFile(SaveHogPositiveFileName, descrierePozitiv);
	writeToFile(SaveHogNegativeFileName, descriereNegativ);
	writeToFile(SaveHogRateFileName, descriereRate);

	svmTraining();

	predict();
	int rz = 1;

	/*

	Mat queryImg = imread("C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCV_SIFT\\Images\\frame_18_whole_card.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat trainImg = imread("C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCV_SIFT\\Images\\frame_20.png", CV_LOAD_IMAGE_GRAYSCALE);



	if (queryImg.empty() || trainImg.empty())
	{
		printf("Can't read one of the images\n");
		return -1;
	}

	//SurfFeatureDetector detector(150);
	FastFeatureDetector detector(25);
	//SiftFeatureDetector detector(400);
	vector<KeyPoint> queryKeypoints, trainKeypoints;
	detector.detect(queryImg, queryKeypoints);
	detector.detect(trainImg, trainKeypoints);
	printf("Found %d and %d keypoints.\n", queryKeypoints.size(), trainKeypoints.size());

	// Compute the SIFT feature descriptors for the keypoints.
	// Multiple features can be extracted from a single keypoint, so the result is a
	// matrix where row 'i' is the list of features for keypoint 'i'.

	//SurfDescriptorExtractor extractor;
	SiftDescriptorExtractor extractor;
	Mat queryDescriptors, trainDescriptors;
	extractor.compute(queryImg, queryKeypoints, queryDescriptors);
	extractor.compute(trainImg, trainKeypoints, trainDescriptors);

	// Print some statistics on the matrices returned.
	Size size = queryDescriptors.size();
	printf("Query descriptors height: %d, width: %d, area: %d, non-zero: %d\n",
		size.height, size.width, size.area(), countNonZero(queryDescriptors));

	size = trainDescriptors.size();
	printf("Train descriptors height: %d, width: %d, area: %d, non-zero: %d\n",
		size.height, size.width, size.area(), countNonZero(trainDescriptors));

	// For each of the descriptors in 'queryDescriptors', find the closest
	// matching descriptor in 'trainDescriptors' (performs an exhaustive search).
	// This seems to only return as many matches as there are keypoints. For each
	// keypoint in 'query', it must return the descriptor which most closesly matches a
	// a descriptor in 'train'?
	BFMatcher matcher(NORM_L2);
	vector<DMatch> matches;
	matcher.match(queryDescriptors, trainDescriptors, matches);

	printf("Found %d matches.\n", matches.size());

	// Draw the results. Displays the images side by side, with colored circles at
	// each keypoint, and lines connecting the matching keypoints between the two
	// images.
	namedWindow("matches", 1);
	Mat img_matches;
	drawMatches(queryImg, queryKeypoints, trainImg, trainKeypoints, matches, img_matches);
	imshow("matches", img_matches);
	waitKey(0);
	cin.get();


	/*IplImage* img = cvLoadImage("E:\\Poze\\acm bucuresti\\iulica.jpg");
	cvNamedWindow("Example1", 10);
	cvShowImage("Example1", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example1");
	*/
	return 0;
}
//#include <opencv2\highgui\highgui.hpp>
//#include <vector>
//using namespace std;
//
//
//displays video from a webcam
//0 is built-in, 1 is external
//press esc key to close
//
//
//int main()
//{
//	cvNamedWindow("Streaming", CV_WINDOW_AUTOSIZE);
//	
//	CvCapture* capture = cvCreateCameraCapture(0); //0 = built-in, 1 = external
//	IplImage* frame;
//	while (1) {
//		frame = cvQueryFrame(capture);
//		if (!frame) break;
//		cvShowImage("Streaming", frame);
//		char c = cvWaitKey(33);
//		if (c == 27) break;
//	}
//	cvReleaseCapture(&capture);
//	cvDestroyWindow("Streaming");
//	return 0;
//}

void writeToFile(char fileName[dim], vector<vector<float>> descriere)
{
	FileStorage hogXml(fileName, FileStorage::WRITE); //FileStorage::READ  
																//2d vector to Mat  
	int row = descriere.size(), col = descriere[0].size();
	printf("col=%d, row=%d\n", row, col);
	Mat M(row, col, CV_32F);
	//save Mat to XML  
	for (int i = 0; i < row; ++i)
		memcpy(&(M.data[col * i * sizeof(float)]), descriere[i].data(), col*sizeof(float));
	//write xml  
	write(hogXml, "Descriptor_of_images", M);

	write(hogXml, "Descriptor", descriere);
	//write(hogXml, "locations", v_locations);
	hogXml.release();
}

void svmTraining()
{
	FileStorage read_PositiveXml("Positive.xml", FileStorage::READ);
	FileStorage read_NegativeXml("Negative.xml", FileStorage::READ);
	FileStorage read_RateXml("Rate.xml", FileStorage::READ);

	//Positive Mat
	Mat pMat;
	read_PositiveXml["Descriptor_of_images"] >> pMat;
	//Read Row, Cols
	int pRow, pCol;
	pRow = pMat.rows; pCol = pMat.cols;

	//Negative Mat
	Mat nMat;
	read_NegativeXml["Descriptor_of_images"] >> nMat;
	//Read Row, Cols
	int nRow, nCol;
	nRow = nMat.rows; nCol = nMat.cols;


	//Rate Mat
	Mat rMat;
	read_RateXml["Descriptor_of_images"] >> rMat;
	//Read Row, Cols
	int rRow, rCol;
	rRow = rMat.rows;
	rCol = rMat.cols;

	//Rows, Cols printf
	printf("   pRow=%d pCol=%d, nRow=%d nCol=%d rRow=%d rCol=%d\n", pRow, pCol, nRow, nCol, rRow, rCol);
	//release
	read_PositiveXml.release();
	//release
	read_NegativeXml.release();
	/////////////////////////////////////////////////////////////////////////////////
	//release
	read_RateXml.release();
	/////////////////////////////////////////////////////////////////////////////////

	//Make training data for SVM
	/////////////////////////////////////////////////////////////////////////////////
	printf("2. Make training data for SVM\n");
	//descriptor data set
	Mat PN_Descriptor_mtx(pRow + nRow + rRow, pCol, CV_32FC1); //in here pCol and nCol is descriptor number, so two value must be same;
	memcpy(PN_Descriptor_mtx.data, pMat.data, sizeof(float) * pMat.cols * pMat.rows);
	int startP = sizeof(float) * pMat.cols * pMat.rows;
	memcpy(&(PN_Descriptor_mtx.data[startP]), nMat.data, sizeof(float) * nMat.cols * nMat.rows);
	int startN = sizeof(float) * nMat.cols * nMat.rows + startP;
	memcpy(&(PN_Descriptor_mtx.data[startN]), rMat.data, sizeof(float) * rMat.cols * rMat.rows);

	//data labeling
	Mat labels(pRow + nRow + rRow, 1, CV_32FC1, Scalar(-1.0));
	labels.rowRange(0, pRow) = Scalar(1.0);
	labels.rowRange(pRow + nRow, pRow + nRow + rRow) = Scalar(2.0);
	for (int i = 0; i < pRow + nRow + rRow; ++i)
		cout << labels.at<float>(i, 0) << '\n';
	/////////////////////////////////////////////////////////////////////////////////

	//Set svm parameter
	/////////////////////////////////////////////////////////////////////////////////
	printf("4. SVM training\n");
	CvSVM svm;
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000000, 1e-6);
	/////////////////////////////////////////////////////////////////////////////////

	//Training
	/////////////////////////////////////////////////////////////////////////////////
	svm.train(PN_Descriptor_mtx, labels, Mat(), Mat(), params);

	//Trained data save
	/////////////////////////////////////////////////////////////////////////////////
	printf("5. SVM xml save\n");
	svm.save("trainedSVM.xml");

	// FileStorage hogXml("testXML.xml", FileStorage::WRITE); //FileStorage::READ
	// write(hogXml, "Data", PN_Descriptor_mtx);
	// write(hogXml, "Label", labels);
	// hogXml.release();
}

vector < vector < float > > Mat2DToArray(int row, int col, Mat M2)
{
	//Mat to 2D vector
	//copy from Mat to 2d Vector
	vector < vector < float > > vv_Test2;
	for (int i = 0; i< row; ++i)
	{
		vector< float > temp;
		int start = col * i * sizeof(float);
		int end = start + col*sizeof(float) - 1;
		temp.assign((float*)(&(M2.data[start])), (float*)(&(M2.data[end])));
		vv_Test2.push_back(temp);
	}

	return vv_Test2;

}

void predict()
{
	CvSVM svm;
	svm.load("trainedSVM.xml");

	string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";
	string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";

	int poz = 0, neg = 0, rate =0;
	for (int i = 0; i < 3; ++i)
	{
		//citim cate o poza
		fileN = fisierSeparat + std::to_string(i + 1) + ".png";

		Mat img1 = imread(fileN);
		if (img1.empty())
		{
			cout << " Citire incorecta";
			return;
		}
		Mat img1_gray;
		cvtColor(img1, img1_gray, CV_RGB2GRAY);

		Mat r_img1_gray;
		resize(img1_gray, r_img1_gray, Size(64, 8));

		HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
		vector< float> descriptorsValues1;
		vector< Point> locations1;

		d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
		Mat fm = Mat(descriptorsValues1);

		//Classification whether data is positive or negative
		int result = svm.predict(fm);
		if (result == 1)
		{
			++poz;
		}
		else if(result == -1)
		{
			++neg;
		}
		else
		{
			++rate;
		}
		//cout << fileN << " " << result << '\n';

		imshow("picture", img1_gray);
		waitKey(0);
	}

	cout << poz << " " << neg << " " << rate << '\n';
	cin.get();
}

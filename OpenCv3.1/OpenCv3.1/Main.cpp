#include <opencv2\highgui\highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/ml/ml.hpp"
#include <iostream>
#include <vector>
#include <fstream>

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
using namespace cv::ml;



//opens an image saved in a specified location
//press any key to close the image window

#define dim 109

void writeToFile(char fileName[dim], vector < vector < float > > descriere);
vector < vector < float > > Mat2DToArray(int row, int col, Mat M2);
void svmTraining();
void predict();
float matrix[180][3780];
int rows, cols;

int main(int argc, char* argv[])
{

	char picturesPath[dim];
	char inputUser[dim];
	for (int i = 0; i < argc; ++i)
	{
		if (i == 1)
		{
			strcpy(inputUser, argv[i]);
		}
		else
			if (i == 2)
			{
				strcpy(picturesPath, argv[i]);
			}
		cout << argv[i] << " " << '\n';
	}
	while (1)
	{
		char ch; //= inputUser[0];
				 //cout << ch << " ";
		cout << "Alege o optiune:" << '\n';
		cout << "1. Citeste fotografii si salveaza continutul lor in fisiere" << '\n';
		cout << "2. Antreneaza reteaua" << '\n';
		cout << "3. Predictie" << '\n';
		cout << "4. Spune din ce categorie face parte o poza" << '\n';
		cout << "5. Exit" << '\n';
		cin >> ch;

		if (ch == '1')
		{

			ofstream out("fisier.out");
			out << 180 << '\n';
			char SaveHogPositiveFileName[dim] = "Mingi.xml";
			char SaveHogNegativeFileName[dim] = "Bere.xml";
			char SaveHogRateFileName[dim] = "Rate.xml";
			char SaveHogDogsFileName[dim] = "Dogs.xml";
			char SaveHogMotoFileName[dim] = "Moto.xml";/*
													   char SaveHogDubaiFileName[dim] = "Dubai.xml";*/

			string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\bigdata\\pic";
			string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\bigdata\\pic";

			vector < vector < float > > descriereMingi;
			vector < vector < float > > descriereBere;
			vector < vector < float > > descriereRate;
			vector < vector < float > > descreireDogs;
			vector < vector < float > > descriereMoto;/*
													  vector < vector < float > > descriereDubai;*/

			for (int i = 0; i < 180; ++i)
			{
				cols = 0;
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
				resize(img1_gray, r_img1_gray, Size(64, 128));

				HOGDescriptor d1(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
				vector< float> descriptorsValues1;
				vector< Point> locations1;

				d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
				for (int it = 0; it < descriptorsValues1.size(); ++it)
				{
					matrix[rows][cols++] = descriptorsValues1[it];
					out << descriptorsValues1[it] << " ";
				}
				rows++;
				out << '\n';


				if (i <= 35)
				{
				//	Pozeclasa["minge"].push_back(fileN);
					descriereMingi.push_back(descriptorsValues1);
				}
				else
					if (i >= 36 && i <= 71)
					{
					//	Pozeclasa["bere"].push_back(fileN);
						descriereBere.push_back(descriptorsValues1);
					}
					else
						if (i >= 72 && i <= 107)
						{
						//	Pozeclasa["rate"].push_back(fileN);
							descriereRate.push_back(descriptorsValues1);
						}
						else
							if (i >= 108 && i <= 143)
							{
							//	Pozeclasa["caini"].push_back(fileN);
								descreireDogs.push_back(descriptorsValues1);
							}
							else
								if (i >= 144 && i <= 179)
								{
							//		Pozeclasa["moto"].push_back(fileN);
									descriereMoto.push_back(descriptorsValues1);
								}
				/*else
				{
				Pozeclasa["dubai"].push_back(fileN);
				descriereDubai.push_back(descriptorsValues1);
				}*/

				imshow("picture", img1_gray);
				cvDestroyWindow("picture");
				waitKey(0);
				img1.release();
				img1_gray.release();
				r_img1_gray.release();
			}

			writeToFile(SaveHogPositiveFileName, descriereMingi);
			writeToFile(SaveHogNegativeFileName, descriereBere);
			writeToFile(SaveHogRateFileName, descriereRate);
			writeToFile(SaveHogDogsFileName, descreireDogs);
			writeToFile(SaveHogMotoFileName, descriereMoto);/*
			writeToFile(SaveHogDubaiFileName, descriereDubai);*/
			//writeClaseToFile(Pozeclasa);

			cout << "Operation completed" << '\n';
			cin.get();
		}
		else
			if (ch == '2')
			{
				//svmHelper.trainingSVM();
				svmTraining();
				cout << "Operation completed" << '\n';
				cin.get();
			}
			else
				if (ch == '3')
				{
					predict();
					//svmHelper.predictPhoto();
					cout << "Operation completed" << '\n';
					cin.get();
				}
				else
					if (ch == '4')
					{

						string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\";
						string fis = fileN + "pic10.png";  //fileN + string(picturesPath);

						cout << fis << '\n';
						//predictAnImage(fis);
					}
					else
					{
						/*cout << "Comanda eronata" << ch << '\n';
						cin.get();*/
						break;
					}
	}
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
	FileStorage read_PositiveXml("Mingi.xml", FileStorage::READ);
	FileStorage read_NegativeXml("Bere.xml", FileStorage::READ);
	FileStorage read_RateXml("Rate.xml", FileStorage::READ);
	FileStorage read_DogsXml("Dogs.xml", FileStorage::READ);
	FileStorage read_MotoXml("Moto.xml", FileStorage::READ);/*
															FileStorage read_DubaiXml("Dubai.xml", FileStorage::READ);*/

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


	//Dogs Mat
	Mat dMat;
	read_DogsXml["Descriptor_of_images"] >> dMat;
	int dRow, dCol;
	dRow = dMat.rows;
	dCol = dMat.cols;

	//Moto Mat
	Mat mMat;
	read_MotoXml["Descriptor_of_images"] >> mMat;
	int mRow, mCol;
	mRow = mMat.rows;
	mCol = mMat.cols;

	//Dubai Mat
	/*Mat dubMat;
	read_MotoXml["Descriptor_of_images"] >> dubMat;
	int dubRow, dubCol;
	dubRow = dubMat.rows;
	dubCol = dubMat.cols;*/


	//Rows, Cols printf
	printf("   pRow=%d pCol=%d, nRow=%d nCol=%d rRow=%d rCol=%d dRow=%d dCol=%d mRow=%d mCol=%d\n",
		pRow, pCol, nRow, nCol, rRow, rCol, dRow, dCol, mRow, mCol);
	//release
	read_PositiveXml.release();
	//release
	read_NegativeXml.release();
	/////////////////////////////////////////////////////////////////////////////////
	//release
	read_RateXml.release();
	/////////////////////////////////////////////////////////////////////////////////
	//release
	read_DogsXml.release();
	/////////////////////////////////////////////////////////////////////////////////
	//release
	read_MotoXml.release();
	/////////////////////////////////////////////////////////////////////////////////
	//release
	//read_DubaiXml.release();
	/////////////////////////////////////////////////////////////////////////////////

	//Make training data for SVM
	/////////////////////////////////////////////////////////////////////////////////
	printf("2. Make training data for SVM\n");
	//descriptor data set
	//mingi
	Mat PN_Descriptor_mtx(pRow + nRow + rRow + dRow + mRow, pCol, CV_32FC1); //in here pCol and nCol is descriptor number, so two value must be same;
	memcpy(PN_Descriptor_mtx.data, pMat.data, sizeof(float) * pMat.cols * pMat.rows);
	//bere
	int startP = sizeof(float) * pMat.cols * pMat.rows;
	memcpy(&(PN_Descriptor_mtx.data[startP]), nMat.data, sizeof(float) * nMat.cols * nMat.rows);
	//rate
	int startN = sizeof(float) * nMat.cols * nMat.rows + startP;
	memcpy(&(PN_Descriptor_mtx.data[startN]), rMat.data, sizeof(float) * rMat.cols * rMat.rows);
	//dogs
	int startR = sizeof(float) * rMat.cols * rMat.rows + startN;
	memcpy(&(PN_Descriptor_mtx.data[startR]), dMat.data, sizeof(float) * dMat.cols * dMat.rows);
	//moto
	int startM = sizeof(float) * dMat.cols * dMat.rows + startR;
	memcpy(&(PN_Descriptor_mtx.data[startM]), mMat.data, sizeof(float) * mMat.cols * mMat.rows);
	//dubai
	/*int startDub = sizeof(float) * mMat.cols * mMat.rows + startM;
	memcpy(&(PN_Descriptor_mtx.data[startDub]), dubMat.data, sizeof(float) * dubMat.cols * dubMat.rows);
	*/
	//data labeling
	int laba[180];
	int dimlaba = 0;
	Mat labels(pRow + nRow + rRow + dRow + mRow, 1, CV_32FC1, Scalar(-1.0));
	labels.rowRange(0, pRow) = Scalar(1.0);
	labels.rowRange(pRow + nRow, pRow + nRow + rRow) = Scalar(2.0);
	labels.rowRange(pRow + nRow + rRow, pRow + nRow + rRow + dRow) = Scalar(3.0);
	labels.rowRange(pRow + nRow + rRow + dRow, pRow + nRow + rRow + dRow + mRow) = Scalar(4.0);/*
	labels.rowRange(pRow + nRow + rRow + dRow + mRow, pRow + nRow + rRow + dRow + mRow + dubRow) = Scalar(5.0);*/
	for (int i = 0; i < pRow + nRow + rRow + dRow + mRow; ++i)
	{
		laba[dimlaba++] = labels.at<float>(i, 0);
		cout << labels.at<float>(i, 0) << '\n';
	}
	/////////////////////////////////////////////////////////////////////////////////


	//Set svm parameter
	/////////////////////////////////////////////////////////////////////////////////
	printf("4. SVM training\n");
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100000, 1e-6));
/*
	CvSVM svm;
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000000, 1e-6);
	*/
	/////////////////////////////////////////////////////////////////////////////////

	//Training
	/////////////////////////////////////////////////////////////////////////////////

	int sz = pRow + nRow + rRow + dRow + mRow;
	int labels2[4] = { 1, -1, -1, -1 };
	Mat labelsMat(180, 1, CV_32SC1, laba);
	float trainingData[4][2] = { { 501, 10 },{ 255, 10 },{ 501, 255 },{ 10, 501 } };
	Mat trainingDataMat(180, 3780, CV_32FC1, matrix);
	svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
	//svm.train(PN_Descriptor_mtx, labels, Mat(), Mat(), params);

	//Trained data save
	/////////////////////////////////////////////////////////////////////////////////
	printf("5. SVM xml save\n");
	svm->save("trainedSVM.xml");

	//predictieeeeeeeeeeeeeee
	/*string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";
	string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";

	int poz = 0, neg = 0, rate = 0, dogs = 0, moto = 0;
	for (int i = 0; i < 16; ++i)
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
		//resize(img1_gray, r_img1_gray, Size(64, 8));
		resize(img1_gray, r_img1_gray, Size(64, 128));

		//HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
		HOGDescriptor d1(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
		vector< float> descriptorsValues1;
		vector< Point> locations1;

		d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
		Mat fm = Mat(descriptorsValues1);

		//Classification whether data is positive or negative
		int result = svm->predict(fm);
		if (result == 1)
		{//minge
			++poz;
		}
		else if (result == -1)
		{
			//bere
			++neg;
		}
		else
			if (result == 2)
			{
				//rate
				++rate;
			}
			else
				if (result == 3)
				{
					//dogs
					++dogs;
				}
				else
					if (result == 4)
					{
						//moto
						++moto;
					}
		//cout << fileN << " " << result << '\n';

		imshow("picture", img1_gray);
		cvDestroyWindow("picture");
		waitKey(0);
	}

	cout << " Mingi: " << poz << " Bere: " << neg << " Rate: " << rate << " Dogs: " << dogs << " " << " Moto: " << moto << '\n';
	cin.get();
	*/
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
	//CvSVM svm;

	Ptr<SVM> svm = SVM::create();
	svm = StatModel::load<SVM>("trainedSVM.xml");
	/*svm.load("trainedSVM.xml");*/


	string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";
	string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";

	int poz = 0, neg = 0, rate = 0,  dogs = 0, moto = 0;
	for (int i = 0; i < 16; ++i)
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
		//resize(img1_gray, r_img1_gray, Size(64, 8));
		resize(img1_gray, r_img1_gray, Size(64, 128));

		//HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
		HOGDescriptor d1(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
		vector< float> descriptorsValues1;
		vector< Point> locations1;

		d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
		Mat fm = Mat(descriptorsValues1);
		Mat res;

		//Classification whether data is positive or negative
		int result = svm->predict(fm,res);
		if (result == 1)
		{//minge
			++poz;
		}
		else if (result == -1)
		{
			//bere
			++neg;
		}
		else
			if (result == 2)
			{
				//rate
				++rate;
			}
			else
				if (result == 3)
				{
					//dogs
					++dogs;
				}
				else
				if (result == 4)
				{
						//moto
					++moto;
				}
		//cout << fileN << " " << result << '\n';

		imshow("picture", img1_gray);
		cvDestroyWindow("picture");
		waitKey(0);
	}

	cout << " Mingi: " << poz << " Bere: " << neg << " Rate: " << rate << " Dogs: " << dogs << " " << " Moto: " << moto << '\n';
	cin.get();
}

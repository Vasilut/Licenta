#include <opencv2\highgui\highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include "opencv2\gpu\gpu.hpp"
#include <opencv2\gpu\gpu.hpp>

#include <opencv2/opencv.hpp>  

using namespace std;
using namespace cv;

class SvmHelper
{
public:

	void trainingSVM()
	{
		FileStorage read_PositiveXml("Mingi.xml", FileStorage::READ);
		FileStorage read_NegativeXml("Bere.xml", FileStorage::READ);
		FileStorage read_RateXml("Rate.xml", FileStorage::READ);
		FileStorage read_DogsXml("Dogs.xml", FileStorage::READ);
		FileStorage read_MotoXml("Moto.xml", FileStorage::READ);
		
		//balls Mat
		Mat pMat;
		read_PositiveXml["Descriptor_of_images"] >> pMat;
		//Read Row, Cols
		int pRow, pCol;
		pRow = pMat.rows; pCol = pMat.cols;

		//beer Mat
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

		
		//Rows, Cols printf
		//printf("   pRow=%d pCol=%d, nRow=%d nCol=%d rRow=%d rCol=%d dRow=%d dCol=%d mRow=%d mCol=%d\n",
			//pRow, pCol, nRow, nCol, rRow, rCol, dRow, dCol, mRow, mCol);
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
		
		//data labeling
		Mat labels(pRow + nRow + rRow + dRow + mRow, 1, CV_32FC1, Scalar(-1.0));
		labels.rowRange(0, pRow) = Scalar(1.0);
		labels.rowRange(pRow + nRow, pRow + nRow + rRow) = Scalar(2.0);
		labels.rowRange(pRow + nRow + rRow, pRow + nRow + rRow + dRow) = Scalar(3.0);
		labels.rowRange(pRow + nRow + rRow + dRow, pRow + nRow + rRow + dRow + mRow) = Scalar(4.0);/*
		labels.rowRange(pRow + nRow + rRow + dRow + mRow, pRow + nRow + rRow + dRow + mRow + dubRow) = Scalar(5.0);*/
		//for (int i = 0; i < pRow + nRow + rRow + dRow + mRow; ++i)
			//cout << labels.at<float>(i, 0) << '\n';
		/////////////////////////////////////////////////////////////////////////////////

		//Set svm parameter
		/////////////////////////////////////////////////////////////////////////////////
		printf("4. SVM training\n");
		CvSVM svm;
		CvSVMParams params;
		params.svm_type = CvSVM::C_SVC;
		params.kernel_type = CvSVM::LINEAR;
		params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100000, 1e-6);
		/////////////////////////////////////////////////////////////////////////////////

		//Training
		/////////////////////////////////////////////////////////////////////////////////
		svm.train(PN_Descriptor_mtx, labels, Mat(), Mat(), params);

		//Trained data save
		/////////////////////////////////////////////////////////////////////////////////
		printf("5. SVM xml save\n");
		svm.save("trainedSVM.xml");
	}

	void predictPhoto()
	{
		ofstream out("datetest.out");
		CvSVM svm;
		svm.load("trainedSVM.xml");

		string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";
		string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic";

		out << 16 << '\n';
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
			resize(img1_gray, r_img1_gray, Size(64, 8));

			HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
			vector< float> descriptorsValues1;
			vector< Point> locations1;

			d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
			Mat fm = Mat(descriptorsValues1);
			for (int it = 0; it < descriptorsValues1.size(); ++it)
			{
				out << descriptorsValues1[it] << " ";
			}
			out << '\n';

			//Classification whether data is positive or negative
			int result = svm.predict(fm);
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
};

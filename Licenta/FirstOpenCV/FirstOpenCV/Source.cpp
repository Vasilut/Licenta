#include <opencv2\highgui\highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2\gpu\gpu.hpp"
#include <opencv2\gpu\gpu.hpp>
#include <vector>
#include <fstream>
#include <map>
#include "SvmHelper.h"

#include <opencv2/opencv.hpp>  

using namespace std;
using namespace cv;


//opens an image saved in a specified location
//press any key to close the image window

#define dim 109

map < int, string > clasa;
map < string, vector < string > > Pozeclasa;


void writeToFile(char fileName[dim], vector < vector < float > > descriere);
vector < vector < float > > Mat2DToArray(int row, int col, Mat M2);
void predictAnImage(string fileName);
void showAllPicturesFromACategory(string category);
void writeClaseToFile(map < string, vector < string > > files);
void initializeClase();
void readAllClasses();
void extractFeaturesFromPicture(string fileName);
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
		//cout << argv[i] << " " << '\n';
	}
	SvmHelper svmHelper;
	initializeClase();
	//while (1)
	//	{
	char ch = inputUser[0];
	//cout << ch << " ";
	cout << "Alege o optiune:" << '\n';
	cout << "1. Citeste fotografii si salveaza continutul lor in fisiere" << '\n';
	cout << "2. Antreneaza reteaua" << '\n';
	cout << "3. Predictie" << '\n';
	cout << "4. Spune din ce categorie face parte o poza" << '\n';
	cout << "5. Extrage caracteristicile pentru o poza" << '\n';
	cout << "6. Exit" << '\n';
	//	cin >> ch;

	if (ch == '1')
	{

		ofstream out("fisier.out");
		out << 180 << '\n';
		char SaveHogPositiveFileName[dim] = "Mingi.xml";
		char SaveHogNegativeFileName[dim] = "Bere.xml";
		char SaveHogRateFileName[dim] = "Rate.xml";
		char SaveHogDogsFileName[dim] = "Dogs.xml";
		char SaveHogMotoFileName[dim] = "Moto.xml";

		string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\bigdata\\pic";
		string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\bigdata\\pic";

		vector < vector < float > > descriereMingi;
		vector < vector < float > > descriereBere;
		vector < vector < float > > descriereRate;
		vector < vector < float > > descreireDogs;
		vector < vector < float > > descriereMoto;

		for (int i = 0; i < 180; ++i)
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
			for (int it = 0; it < descriptorsValues1.size(); ++it)
			{
				out << descriptorsValues1[it] << " ";
			}
			out << '\n';

			if (i <= 35)
			{
				Pozeclasa["minge"].push_back(fileN);
				descriereMingi.push_back(descriptorsValues1);
			}
			else
				if (i >= 36 && i <= 71)
				{
					Pozeclasa["bere"].push_back(fileN);
					descriereBere.push_back(descriptorsValues1);
				}
				else
					if (i >= 72 && i <= 107)
					{
						Pozeclasa["rate"].push_back(fileN);
						descriereRate.push_back(descriptorsValues1);
					}
					else
						if (i >= 108 && i <= 143)
						{
							Pozeclasa["caini"].push_back(fileN);
							descreireDogs.push_back(descriptorsValues1);
						}
						else
							if (i >= 144 && i <= 179)
							{
								Pozeclasa["moto"].push_back(fileN);
								descriereMoto.push_back(descriptorsValues1);
							}

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
		writeToFile(SaveHogMotoFileName, descriereMoto);
		writeClaseToFile(Pozeclasa);

		cout << "Operation completed" << '\n';
		cin.get();
	}
	else
		if (ch == '2')
		{
			svmHelper.trainingSVM();
			cout << "Operation completed" << '\n';
			cin.get();
		}
		else
			if (ch == '3')
			{
				svmHelper.predictPhoto();
				cout << "Operation completed" << '\n';
				cin.get();
			}
			else
				if (ch == '4')
				{

					string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\";
					string fis = /*fileN + "pic10.png";*/  fileN + string(picturesPath);

					cout << fis << '\n';
					predictAnImage(fis);
				}
				else
					if (ch == '5')
					{
						string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\";
						string fis = /*fileN + "pic10.png";*/  fileN + string(picturesPath);

						cout << fis << '\n';
						extractFeaturesFromPicture(fis);
					}
				else
				{
					cout << "Comanda eronata" << ch << '\n';
					cin.get();
					//break;
				}
	//}
	return 0;
}

void writeToFile(char fileName[dim], vector<vector<float>> descriere)
{
	FileStorage hogXml(fileName, FileStorage::WRITE); //FileStorage::READ  
																//2d vector to Mat  
	int row = (int)descriere.size(), col = (int)descriere[0].size();
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

vector < vector < float > > Mat2DToArray(int row, int col, Mat M2)
{
	//Mat to 2D vector
	//copy from Mat to 2d Vector
	vector < vector < float > > vv_Test2;
	for (int i = 0; i < row; ++i)
	{
		vector< float > temp;
		int start = col * i * sizeof(float);
		int end = start + col*sizeof(float) - 1;
		temp.assign((float*)(&(M2.data[start])), (float*)(&(M2.data[end])));
		vv_Test2.push_back(temp);
	}

	return vv_Test2;

}


void predictAnImage(string fileName)
{
	CvSVM svm;
	svm.load("trainedSVM.xml");
	string fisier = string(fileName);
	Mat img1 = imread(fileName);
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
	imshow("picture", img1_gray);
	waitKey(0);

	string clasaResult = clasa[result];
	cout << "Poza scanata este : " << clasaResult;
	showAllPicturesFromACategory(clasaResult);

}

void showAllPicturesFromACategory(string category)
{
	if (Pozeclasa.size() == 0)
	{
		readAllClasses();
	}
	ofstream g("predictie.txt");
	g << category << '\n';
	/*
		for (const auto &myPair : Pozeclasa)
		{
			cout << myPair.first << " ";
		}*/
	vector < string > foldersPath = Pozeclasa[category];
	for (int i = 0; i < foldersPath.size(); ++i)
	{
		string imgPath = foldersPath[i];
		g << imgPath << '\n';
		/*Mat img1 = imread(imgPath);
		if (img1.empty())
		{
			cout << " Citire incorecta";
			return;
		}
		imshow("picture", img1);
		waitKey(0);*/
	}
}

void writeClaseToFile(map<string, vector<string> > files)
{
	ofstream fout("clases.out");
	fout << files.size() << '\n';
	for (const auto &myPair : Pozeclasa)
	{
		fout << myPair.first << " ";
		int sizeOfCurrentClass = (int)Pozeclasa[myPair.first].size();
		fout << sizeOfCurrentClass << '\n';
		vector < string > objectsFromCurrentClass = myPair.second;
		for (vector < string >::iterator it = objectsFromCurrentClass.begin(); it != objectsFromCurrentClass.end(); ++it)
		{
			fout << *it << " " << '\n';
		}
	}

}

void initializeClase()
{
	clasa[1] = "minge";
	clasa[-1] = "bere";
	clasa[2] = "rate";
	clasa[3] = "caini";
	clasa[4] = "moto";

}

void readAllClasses()
{
	//we need to take the info from "classes.out"
	//first line
	//number of classes
	ifstream in("clases.out");
	int numberOfClasses;
	in >> numberOfClasses;
	//next will have "numberOfClasses" lines with the next format:
	//name of the class and the number of picturesPath from that class
	for (int i = 1; i <= numberOfClasses; ++i)
	{
		string nameOfClass;
		int numberOfPictures;
		vector < string > name;
		in >> nameOfClass >> numberOfClasses;
		in.get();
		for (int j = 1; j <= numberOfClasses; ++j)
		{
			char arr[dim];
			in.getline(arr, dim);
			string rez(arr);
			name.push_back(rez);
		}
		Pozeclasa[nameOfClass] = name;
	}
}

void extractFeaturesFromPicture(string fileName)
{
	ofstream out("annfeature.out");

	Mat img1 = imread(fileName);
	if (img1.empty())
	{
		cout << " Citire incorecta ";
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
	for (int it = 0; it < descriptorsValues1.size(); ++it)
	{
		out << descriptorsValues1[it] << " ";
	}

	imshow("picture", img1_gray);
	cvDestroyWindow("picture");
	waitKey(0);
	img1.release();
	img1_gray.release();
	r_img1_gray.release();
}

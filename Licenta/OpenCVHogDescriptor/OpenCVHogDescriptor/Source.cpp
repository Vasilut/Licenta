#include <iostream>
#include <string>
#include < stdio.h>  
#include <opencv2/opencv.hpp>  


using namespace std;
using namespace cv;

#define dim 109

Mat get_hogdescriptor_visual_image(Mat& origImg,
	vector< float>& descriptorValues,
	Size winSize,
	Size cellSize,
	int scaleFactor,
	double viz_factor)
{
	Mat visual_image;
	resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));
	cvtColor(visual_image, visual_image, CV_GRAY2BGR);


	int gradientBinSize = 9;
	// dividing 180° into 9 bins, how large (in rad) is one bin?  
	float radRangeForOneBin = 3.14 / (float)gradientBinSize;

	// prepare data structure: 9 orientation / gradient strenghts for each cell  
	int cells_in_x_dir = winSize.width / cellSize.width;
	int cells_in_y_dir = winSize.height / cellSize.height;
	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter = new int*[cells_in_y_dir];
	for (int y = 0; y < cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x = 0; x < cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin = 0; bin < gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;
		}
	}

	// nr of blocks = nr of cells - 1  
	// since there is a new block on each cell (overlapping blocks!) but the last one  
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell  
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	for (int blockx = 0; blockx < blocks_in_x_dir; blockx++)
	{
		for (int blocky = 0; blocky < blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...  
			for (int cellNr = 0; cellNr < 4; cellNr++)
			{
				// compute corresponding cell nr  
				int cellx = blockx;
				int celly = blocky;
				if (cellNr == 1) celly++;
				if (cellNr == 2) cellx++;
				if (cellNr == 3)
				{
					cellx++;
					celly++;
				}

				for (int bin = 0; bin < gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[descriptorDataIdx];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;

				} // for (all bins)  


				  // note: overlapping blocks lead to multiple updates of this sum!  
				  // we therefore keep track how often a cell was updated,  
				  // to compute average gradient strengths  
				cellUpdateCounter[celly][cellx]++;

			} // for (all cells)  


		} // for (all block x pos)  
	} // for (all block y pos)  


	  // compute average gradient strengths  
	for (int celly = 0; celly < cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx < cells_in_x_dir; cellx++)
		{

			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction  
			for (int bin = 0; bin < gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}


	cout << "descriptorDataIdx = " << descriptorDataIdx << endl;

	// draw cells  
	for (int celly = 0; celly < cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx < cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;

			int mx = drawX + cellSize.width / 2;
			int my = drawY + cellSize.height / 2;

			rectangle(visual_image,
				Point(drawX*scaleFactor, drawY*scaleFactor),
				Point((drawX + cellSize.width)*scaleFactor,
					(drawY + cellSize.height)*scaleFactor),
				CV_RGB(100, 100, 100),
				1);

			// draw in each cell all 9 gradient strengths  
			for (int bin = 0; bin < gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?  
				if (currentGradStrength == 0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

				float dirVecX = cos(currRad);
				float dirVecY = sin(currRad);
				float maxVecLen = cellSize.width / 2;
				float scale = viz_factor; // just a visual_imagealization scale,  
										  // to see the lines better  

										  // compute line coordinates  
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visual_imagealization  
				line(visual_image,
					Point(x1*scaleFactor, y1*scaleFactor),
					Point(x2*scaleFactor, y2*scaleFactor),
					CV_RGB(0, 0, 255),
					1);

			} // for (all bins)  

		} // for (cellx)  
	} // for (celly)  


	  // don't forget to free memory allocated by helper data structures!  
	for (int y = 0; y < cells_in_y_dir; y++)
	{
		for (int x = 0; x < cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visual_image;

}

int main()
{

	//char SaveHogDescrFileName[dim] = "Positive.xml";
	//
	//vector < vector < float > > descriere;
	//int fileNumber = 5;
	//for (int i = 0; i < fileNumber; ++i)
	//{
	//	string s = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";
	//	std::string nr = std::to_string(i + 1);
	//	s = s + nr + ".png";
	//	Mat img1 = imread(s);
	//	if (img1.empty())
	//	{
	//		cout << "Eroare la imaginea " << i + 1 << '\n';
	//		return 0;
	//	}

	//	Mat img1_gray;
	//	cvtColor(img1, img1_gray, CV_RGB2GRAY);

	//	Mat r_img1_gray;
	//	resize(img1_gray, r_img1_gray, Size(64, 8));


	//	HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
	//	vector< float> descriptorsValues1;
	//	vector< Point> locations1;
	//	d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
	//	descriere.push_back(descriptorsValues1);

	//	//release procedure

	//	waitKey(0);
	//	img1.release();
	//	img1_gray.release();
	//	r_img1_gray.release();
	//	d1.~HOGDescriptor();
	//}

	char SaveHogDesFileName[100] = "Positive.xml";
	string fileN = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";
	string fileN2 = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";
	string fisierSeparat = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";
	vector < vector < float > > descriere;
	for (int i = 0; i < 5; ++i)
	{
		//citim cate o poza
		if (i == 0)
		{
			fileN = fisierSeparat + "1.png";
		}
		else
		if (i == 1)
			{
				fileN = fisierSeparat + "2.png";
			}
		else
		if (i == 2)
		{
				fileN = fisierSeparat + "3.png";
		}
		else
		if (i == 3)
		{
			    fileN = fisierSeparat + "4.png";
		}
		else
		{
			fileN = fisierSeparat + "5.png";
		}

		Mat img1 = imread(fileN);

		Mat img1_gray;
		cvtColor(img1, img1_gray, CV_RGB2GRAY);

		Mat r_img1_gray;
		resize(img1_gray, r_img1_gray, Size(64, 8));

		HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
		vector< float> descriptorsValues1;
		vector< Point> locations1;
		d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
		descriere.push_back(descriptorsValues1);

		imshow("picture", img1_gray);
		waitKey(0);
		img1.release();
		img1_gray.release();
		r_img1_gray.release();
	}

	FileStorage hogXml(SaveHogDesFileName, FileStorage::WRITE); //FileStorage::READ  
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


	cout << "Program exixted succesfully...";
	cin.get();
	/*
	Mat img1 = imread("C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic1.png");
	Mat img2 = imread("C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic2.png");

	Mat img1_gray;
	cvtColor(img1, img1_gray, CV_RGB2GRAY);

	Mat img2_gray;
	cvtColor(img2, img2_gray, CV_RGB2GRAY);

	Mat r_img1_gray;
	resize(img1_gray, r_img1_gray, Size(64, 8));
	Mat r_img2_gray;
	resize(img2_gray, r_img2_gray, Size(64, 8));

	HOGDescriptor d1(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);
	HOGDescriptor d2(Size(64, 8), Size(8, 8), Size(4, 4), Size(4, 4), 9);

	vector< float> descriptorsValues1;
	vector< Point> locations1;
	d1.compute(r_img1_gray, descriptorsValues1, Size(0, 0), Size(0, 0), locations1);
	vector< float> descriptorsValues2;
	vector< Point> locations2;
	d2.compute(r_img2_gray, descriptorsValues2, Size(0, 0), Size(0, 0), locations2);

	//desenaammmmm
	Mat A(descriptorsValues1.size(), 1, CV_32FC1);
	//copy vector to mat
	memcpy(A.data, descriptorsValues1.data(), descriptorsValues1.size()*sizeof(float));
	//create Mat
	Mat B(descriptorsValues2.size(), 1, CV_32FC1);
	//copy vector to mat
	memcpy(B.data, descriptorsValues2.data(), descriptorsValues2.size()*sizeof(float));


	/////////////////////////
	//sum( sqrt( (A.-B)^2 ) )
	Mat C = A - B;
	C = C.mul(C);
	cv::sqrt(C, C);
	cv::Scalar rr = cv::sum(C);
	float rrr = rr(0);
	cout << "Distance: " << rrr << endl;


	//hog visualization
	Mat r1 = get_hogdescriptor_visual_image(r_img1_gray, descriptorsValues1, Size(64, 8), Size(4, 4), 10, 3);
	Mat r2 = get_hogdescriptor_visual_image(r_img2_gray, descriptorsValues2, Size(64, 8), Size(4, 4), 10, 3);

	imshow("hog visualization1", r1);
	imshow("hog visualization2", r2);

	waitKey(0);

	/*HOGDescriptor hog;
	vector<float> ders;
	vector<Point>locs;

	hog.blockSize = Size(16,16);
	hog.cellSize = Size(4,4);
	hog.blockStride = Size(8,8);
	Mat img2, img_gray2;
	Mat Hogdfeat;
	img2 = imread("C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic1.png", 1);
	cvtColor(img2, img_gray2, CV_RGB2GRAY);
	hog.compute(img_gray2, ders, Size(32, 32), Size(0, 0), locs);
	Hogdfeat.create(ders.size(), 1, CV_32FC1);
	for (int i = 0; i<ders.size(); i++)
	{
		Hogdfeat.at<float>(i, 0) = ders.at(i);

	}
*/

/*char FullFileName[100];
char FirstFileName[100] = "C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\pic";

int FileNum = 5;

vector< vector < float> > v_descriptorsValues;
vector< vector < Point> > v_locations;

for (int i = 0; i < FileNum; ++i)
{
	sprintf_s(FullFileName, "%s%d.png", FirstFileName, i + 1);
	printf("%s\n", FullFileName);

	Mat img, img_gray;
	img = imread(FullFileName,1);

	if (img.empty())
	{
		cout << "esti prost";
		return 0;
	}

	resize(img, img, Size(63, 126));
	//gray
	cvtColor(img, img_gray, CV_RGB2GRAY);

	HOGDescriptor *hog = new HOGDescriptor(cvSize(63, 126), cvSize(18, 18),
		cvSize(3, 3), cvSize(6, 6), 9);
	//HOGDescriptor d(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);
	vector< float> descriptorsValues;
	vector< Point> locations;
	hog->compute(img_gray, descriptorsValues, Size(1, 1), Size(0, 0));
	//d.compute(img_gray, descriptorsValues, Size(1, 1), Size(1, 1), locations);
	v_descriptorsValues.push_back(descriptorsValues);
	v_locations.push_back(locations);

	imshow("picture", img);
	waitKey(0);
	img.release();
	img_gray.release();
}

//FileStorage hogXml(SaveHogDesFileName, FileStorage::WRITE); //FileStorage::READ
//															//2d vector to Mat
//int row = v_descriptorsValues.size(), col = v_descriptorsValues[0].size();
//printf("col=%d, row=%d\n", row, col);
//Mat M(row, col, CV_32F);
////save Mat to XML
//for (int i = 0; i< row; ++i)
//	memcpy(&(M.data[col * i * sizeof(float)]), v_descriptorsValues[i].data(), col*sizeof(float));
////write xml
//write(hogXml, "Descriptor_of_images", M);

//write(hogXml, "Descriptor", v_descriptorsValues );
//write(hogXml, "locations", v_locations );
//hogXml.release();


//cout << "Program exixted succesfully...";
//cin.get();
*/

	return 0;
}
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using namespace cv;

static void help(char* progName)
{
	cout << endl
		<< "This program shows how to filter images with mask:"
		<< "the write it yourself and rhe filter2d way." << endl
		<< "Usage: "
		<< progName << "[image_path -- deafault ../data/lena.jpg] [G -- grayscale]" << endl << endl;
}

const int w = 400;
void Sharpen(const Mat& myImage, Mat& Result);

namespace
{
	int alpha = 100;
	int beta = 100;
	int gamma_cor = 100;
	Mat img_original, img_corrected, img_gamma_corrected;

	void basicLinearTransform(const Mat &img, const double alpha_, const int beta_)
	{
		Mat res;
		img.convertTo(res, -1, alpha_, beta_);

		hconcat(img, res, img_corrected);
	}
	
	void gammaCorrection(const Mat &img, const double gamma_)
	{
		CV_Assert(gamma_ >= 0);

		Mat lookUpTable(1, 256, CV_8U);
		uchar* p = lookUpTable.ptr();
		for (int i = 0; i < 256; ++i)
		{
			p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
		}
		Mat res = img.clone();
		LUT(img, lookUpTable, res);

		hconcat(img, res, img_gamma_corrected);
	}

	void on_linear_transform_alpha_trackbar(int, void*)
	{
		double alpha_value = alpha / 100.0;
		int beta_value = beta - 100;
		basicLinearTransform(img_original, alpha_value, beta_value);
	}

	void on_linear_transform_beta_trackbar(int, void *)
	{
		double alpha_value = alpha / 100.0;
		int beta_value = beta - 100;
		basicLinearTransform(img_original, alpha_value, beta_value);
	}

	void on_gamma_correction_trackbar(int, void*)
	{
		double gamma_value = gamma_cor / 100.0;
		gammaCorrection(img_original, gamma_value);
	}
}


void MyEllipse(Mat img, double angle);
void MyFilledCircle(Mat img, Point center);
void MyPolygon(Mat img);
void MyLine(Mat img, Point start, Point end);

int main(int argc, char* argv[])
{
	//Drawing
	

	char atom_window[] = "Drawing 1: Atom";
	char rook_window[] = "Drawing 2: Rook";
	Mat atom_image = Mat::zeros(w, w, CV_8UC3);
	Mat rook_image = Mat::zeros(w, w, CV_8UC3);
	MyEllipse(atom_image, 90);
	MyEllipse(atom_image, 0);
	MyEllipse(atom_image, 45);
	MyEllipse(atom_image, -45);
	MyFilledCircle(atom_image, Point(w / 2, w / 2));
	MyPolygon(rook_image);
	rectangle(rook_image,
		Point(0, 7 * w / 8),
		Point(w, w),
		Scalar(0, 255, 255),
		FILLED,
		LINE_8);
	MyLine(rook_image, Point(0, 15 * w / 16), Point(w, 15 * w / 16));
	MyLine(rook_image, Point(w / 4, 7 * w / 8), Point(w / 4, w));
	MyLine(rook_image, Point(w / 2, 7 * w / 8), Point(w / 2, w));
	MyLine(rook_image, Point(3 * w / 4, 7 * w / 8), Point(3 * w / 4, w));
	imshow(atom_window, atom_image);
	moveWindow(atom_window, 0, 200);
	imshow(rook_window, rook_image);
	moveWindow(rook_window, w, 200);
	waitKey();
	//changing constrast & brightness

	/*String imageName = "Lena.png";
	if (argc > 1)
	{
		imageName = argv[1];
	}

	img_original = imread(imageName);
	img_corrected = Mat(img_original.rows, img_original.cols * 2, img_original.type());
	img_gamma_corrected = Mat(img_original.rows, img_original.cols * 2, img_original.type());

	hconcat(img_original, img_original, img_corrected);
	hconcat(img_original, img_original, img_gamma_corrected);

	namedWindow("Brightness and contrast adjustments", WINDOW_AUTOSIZE);
	namedWindow("Gamma correction", WINDOW_AUTOSIZE);

	createTrackbar("Alpha gain (contrast)", "Brightness and contrast adjustments", &alpha, 500, on_linear_transform_alpha_trackbar);
	createTrackbar("Beta bias (brightness)", "Brightness and contrast adjustments", &beta, 200, on_linear_transform_beta_trackbar);
	createTrackbar("Gamma correction", "Gamma correction", &gamma_cor, 200, on_gamma_correction_trackbar);

	while (true)
	{
		imshow("Brightness and contrast adjustments", img_corrected);
		imshow("Gamma correction", img_gamma_corrected);

		int c = waitKey(30);
		if (c == 27)
			break;
	}*/
	//Linear transform

	/*double alpha = 1.0;
	int beta = 0;
	String imageName = "Lena.png";
	if (argc > 1)
	{
		imageName = argv[1];
	}
	Mat image = imread(imageName);
	Mat new_image = Mat::zeros(image.size(), image.type());

	cout << "Basic Linear Transforms" << endl;
	cout << "-----------------------" << endl;
	cout << "* Enter the alpha value [1.0 - 3.0]: "; cin >> alpha;
	cout << "* Enter the beta value [0 - 100]: "; cin >> beta;

	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			for (int c = 0; c < 3; c++)
				new_image.at<Vec3b>(y, x)[c] =
				saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);
		}
	}
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	namedWindow("New Image", WINDOW_AUTOSIZE);

	imshow("Original Image", image);
	imshow("New Image", new_image);
	waitKey();*/ 
	/*double alpha = 0.5, beta, input;
	Mat src1, src2, dst;
	cout << "Simple Linear Blender" << endl;
	cout << "---------------------" << endl;
	cout << "* Enter alpha [0.0 - 1.0]:	" << endl;
	cin >> input;
	if (input >= 0 && input <= 1)
		alpha = input;
	src1 = imread("LinuxLogo.jpg");
	src2 = imread("WindowsLogo.jpg");
	
	if (src1.empty())
	{
		cout << "Error loading src1\n";
		return -1;
	}
	if (src2.empty())
	{
		cout << "Error loading src2\n";
		return -1;
	}
	beta = (1.0 - alpha);
	addWeighted(src1, alpha, src2, beta, 0.0, dst);
	imshow("Linear Blend", dst);
	waitKey();*/

	// Mat operations

	/*const char* filename = argc >= 2 ? argv[1] : "Lena.png";
	Mat img = imread(filename, IMREAD_COLOR);
	namedWindow("Input", WINDOW_AUTOSIZE);
	imshow("Input", img);
	
	Scalar intensity = img.at<uchar>(0, 0);
	Vec3b intensity1 = img.at<Vec3b>(0, 0);
	cout << "intensity : " << intensity << "  1:" << intensity1 << endl;
	uchar blue = intensity1.val[0];
	uchar green = intensity1.val[1];
	uchar red = intensity1.val[2];
	cout << intensity.val[0] << "  " << intensity.val[1] << " " << intensity.val[2] << endl
		<< "blue: " << blue << "green: " << green << "red:	" << red;
	Mat sobelx;
	Sobel(img, sobelx, CV_32F, 1, 0);
	namedWindow("Sobel", WINDOW_AUTOSIZE);
	imshow("Sobel", sobelx);
	Rect r(20, 20, 100, 100);
	Mat smallImg = img(r);
	namedWindow("SmallImage", WINDOW_AUTOSIZE);
	imshow("SmallImage", smallImg);
	waitKey();*/

	//Mask operations

	/*help(argv[0]);
	const char* filename = argc >= 2 ? argv[1] : "Lena.png";
	
	Mat src, dst0, dst1;
	if (argc >= 3 && !strcmp("G", argv[2]))
		src = imread(filename, IMREAD_GRAYSCALE);
	else
		src = imread(filename, IMREAD_COLOR);

	if (src.empty())
	{
		cerr << "Can't open image [" << filename << "]" << endl;
		return -1;
	}

	namedWindow("Input", WINDOW_AUTOSIZE);
	namedWindow("Output", WINDOW_AUTOSIZE);

	imshow("Input", src);
	double t = (double)getTickCount();

	Sharpen(src, dst0);
	
	t = ((double)getTickCount() - 1) / getTickFrequency();
	cout << "Hand written function time passed in seconds: " << t << endl;

	imshow("Output", dst0);
	waitKey();

	Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0,
		-1, 5, -1,
		0, -1, 0);
	t = (double)getTickCount();

	filter2D(src, dst1, src.depth(), kernel);
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "Built-in filter2D time passed in secconds: " << t << endl;

	imshow("Output", dst1);

	waitKey();*/
	return 0;
}

void MyEllipse(Mat img, double angle)
{
	int thickness = 2;
	int lineType = 8;
	ellipse(img,
		Point(w / 2, w / 2),
		Size(w / 4, w / 16),
		angle,
		0,
		360,
		Scalar(255, 0, 0),
		thickness,
		lineType);
}
void MyFilledCircle(Mat img, Point center)
{
	circle(img,
		center,
		w / 32,
		Scalar(0, 0, 255),
		FILLED,
		LINE_8);
}

void MyPolygon(Mat img)
{
	int lineType = LINE_8;
	Point rook_points[1][20];
	rook_points[0][0] = Point(w / 4, 7 * w / 8);
	rook_points[0][1] = Point(3 * w / 4, 7 * w / 8);
	rook_points[0][2] = Point(3 * w / 4, 13 * w / 16);
	rook_points[0][3] = Point(11 * w / 16, 13 * w / 16);
	rook_points[0][4] = Point(19 * w / 32, 3 * w / 8);
	rook_points[0][5] = Point(3 * w / 4, 3 * w / 8);
	rook_points[0][6] = Point(3 * w / 4, w / 8);
	rook_points[0][7] = Point(26 * w / 40, w / 8);
	rook_points[0][8] = Point(26 * w / 40, w / 4);
	rook_points[0][9] = Point(22 * w / 40, w / 4);
	rook_points[0][10] = Point(22 * w / 40, w / 8);
	rook_points[0][11] = Point(18 * w / 40, w / 8);
	rook_points[0][12] = Point(18 * w / 40, w / 4);
	rook_points[0][13] = Point(14 * w / 40, w / 4);
	rook_points[0][14] = Point(14 * w / 40, w / 8);
	rook_points[0][15] = Point(w / 4, w / 8);
	rook_points[0][16] = Point(w / 4, 3 * w / 8);
	rook_points[0][17] = Point(13 * w / 32, 3 * w / 8);
	rook_points[0][18] = Point(5 * w / 16, 13 * w / 16);
	rook_points[0][19] = Point(w / 4, 13 * w / 16);
	const Point* ppt[1] = { rook_points[0] };
	int npt[] = { 20 };
	fillPoly(img,
		ppt,
		npt,
		1,
		Scalar(255, 255, 255),
		lineType);
}
void MyLine(Mat img, Point start, Point end)
{
	int thickness = 2;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(0, 0, 0),
		thickness,
		lineType);
}

void Sharpen(const Mat& myImage, Mat& Result)
{
	CV_Assert(myImage.depth() == CV_8U);

	const int nChannels = myImage.channels();
	Result.create(myImage.size(), myImage.type());
	
	for (int j = 1; j < myImage.rows - 1; ++j)
	{
		const uchar* previous = myImage.ptr<uchar>(j - 1);
		const uchar* current = myImage.ptr<uchar>(j);
		const uchar* next = myImage.ptr<uchar>(j + 1);

		uchar* output = Result.ptr<uchar>(j);

		for (int i = nChannels; i < nChannels*(myImage.cols - 1); ++i)
		{
			*output++ = saturate_cast<uchar>(5 * current[i]
				- current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]);
		}
	}
	Result.row(0).setTo(Scalar(0));
	Result.row(Result.rows - 1).setTo(Scalar(0));
	Result.col(0).setTo(Scalar(0));
	Result.col(Result.cols - 1).setTo(Scalar(0));
}
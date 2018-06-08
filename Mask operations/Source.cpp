#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
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

void Sharpen(const Mat& myImage, Mat& Result);

int main(int argc, char* argv[])
{
	double alpha = 1.0;
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
	waitKey();
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
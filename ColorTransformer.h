#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
using namespace std;
using namespace cv;

/*
	Hàm tính histogram cho ảnh 1 kênh màu 8 bit
	(Ảnh nhiều kênh màu phải thực chia ra từng kênh màu riêng rẽ)

	Tham so :
		sourceImage : ảnh ban đầu
		histogram: histogram của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
int CalcHistogram(const Mat& sourceImage, Mat& histogram)
{
	if (sourceImage.channels() != 1)
		return 0;
	int sizeHist = 256;
	histogram = Mat(1, sizeHist, CV_32FC1, Scalar(0));
	for (int r = 0; r < sourceImage.rows; r++)
	{
		for (int c = 0; c < sourceImage.cols; c++)
		{
			uint8_t pixelValue = sourceImage.at<uint8_t>(r, c);
			histogram.at<float>(pixelValue) += 1;
		}
	}
	return 1;
}

/*
	Hàm tính histogram cho ảnh 1 kênh màu 8 bit
	(Ảnh nhiều kênh màu phải thực chia ra từng kênh màu riêng rẽ)

	Tham so :
		sourceImage : ảnh ban đầu
		histogram: histogram của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
void DrawHistogram(const Mat& histogram, Mat& histImage, int B, int G, int R, int trans)
{
	int width = histImage.cols;
	int height = histImage.rows;
	normalize(histogram, histogram, 0, height, NORM_MINMAX, -1, Mat());
	int bin = width / 256;
	for (int i = 0; i < 256; i++)
	{
		line(histImage, Point(bin*i + trans, height - (int)histogram.at<float>(i)),
			Point(bin*i + trans, height),
			Scalar(B, G, R), 1, 8, 0);
	}
}

class ColorTransformer
{
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b);

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
		c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/	
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c);
	
	
	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage);
	
	/*
	Hàm so sánh hai ảnh
	Tham so :
		image1 : ảnh thứ nhất
		image2 : ảnh thứ hai
	Hàm trả về:
		độ đo sự tương đồng giữa hai ảnh
	*/
	float CompareImage(const Mat& image1, Mat& image2);
	
	
	ColorTransformer();
	~ColorTransformer();
};

ColorTransformer::ColorTransformer()
{}

ColorTransformer::~ColorTransformer()
{}

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	int nr = sourceImage.rows; // number of lines
	int nc = sourceImage.cols * sourceImage.channels();

	uchar lookup[256];
	for (int i = 0; i < 256; i++)
		lookup[i] = i + b;

	for (int y = 0; y < nr; y++) {
		const uchar* datasrc = sourceImage.ptr<uchar>(y);
		uchar* datades = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < nc; x++)
		{
			datades[x] = lookup[datasrc[x]];
		}
	}

	if (sourceImage.empty() || destinationImage.empty())
		return 0;
	else
		return 1;
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	if (sourceImage.channels() != 3 && sourceImage.channels() != 1)
		return 0;
	if (sourceImage.channels() == 3)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC3, Scalar(0));
		for (int i = 0; i < sourceImage.rows; i++)
		{
			for (int j = 0; j < sourceImage.cols; j++)
			{
				float newB = sourceImage.at<Vec3b>(i, j)[0] * c;
				float newG = sourceImage.at<Vec3b>(i, j)[1] * c;
				float newR = sourceImage.at<Vec3b>(i, j)[2] * c;
				if (newB > 255)
					newB = 255;
				if (newG > 255)
					newG = 255;
				if (newR = 255)
					newR = 255;

				if (newB < 0)
					newB = 0;
				if (newG < 0)
					newG = 0;
				if (newR < 0)
					newR = 0;

				destinationImage.at<Vec3b>(i, j)[0] = (uint8_t)newB;
				destinationImage.at<Vec3b>(i, j)[1] = (uint8_t)newG;
				destinationImage.at<Vec3b>(i, j)[2] = (uint8_t)newR;
			}
		}
	}
	else if (sourceImage.channels() == 1)
	{
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		for (int i = 0; i < sourceImage.rows; i++)
		{
			for (int j = 0; j < sourceImage.cols; j++)
			{
				float newValue = sourceImage.at<uint8_t>(i, j) * c;
				if (newValue > 255)
					newValue = 255;

				if (newValue < 0)
					newValue = 0;

				destinationImage.at<uint8_t>(i, j) = (uint8_t)newValue;
			}
		}
	}
	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{

	int nl = sourceImage.rows; // number of lines
	int nc = sourceImage.cols * sourceImage.channels();
	Mat H(1, 255, CV_8UC3, Scalar::all(0));
	Mat T(1, 255, CV_8UC3, Scalar::all(0));
	Mat TR(1, 255, CV_8UC3, Scalar::all(0));

	// tinh luoc do do xam cua f
	CalcHistogram(sourceImage, H);

	// tinh luoc do do xam tich luy cua f
	T.at<uchar>(0, 0) = H.at<uchar>(0, 0);
	TR.at<uchar>(0, 0) = round(T.at<uchar>(0, 0) * 224 / (nl*nc));
	for (int p = 1; p < 255; p++) {
		T.at<uchar>(0, p) = T.at<uchar>(0, p - 1) + H.at<uchar>(0, p);
		TR.at<uchar>(0, p) = round(T.at<uchar>(0, p) * 224 / (nl*nc));
	}

	// tao anh ket qua
	for (int j = 0; j < nl; j++) {
		// lấy địa chỉ của dòng thứ j
		uchar* data = destinationImage.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {
			// xử lý trên mỗi pixel
			Scalar intensity = sourceImage.at<uchar>(j, i);
			int fxy = intensity[0];
			data[i] = TR.at<uchar>(0, fxy);
		} // end of line
	}
	return 0;
}




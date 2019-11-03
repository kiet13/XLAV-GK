#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
using namespace std;
using namespace cv;
class Converter
{
	
public:
	Converter();
	~Converter();
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
	sourceImage: ảnh input
	destinationImage: ảnh output	
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage);	

	/*
	Hàm chuyển đổi không gian màu của ảnh từ GrayScale sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage);
	

	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2HSV(const Mat& sourceImage, Mat& destinationImage);
	
	
	
	/*
	Hàm chuyển đổi không gian màu của ảnh từ HSV sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int HSV2RGB(const Mat& sourceImage, Mat& destinationImage);
	
	
	

	/*
	Hàm chuyển đổi không gian màu của ảnh
	sourceImage: ảnh input
	destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
	type: loại chuyển đổi
		0,1: chuyển từ RGB sang GrayScale và ngược lại
		2,3: chuyển từ RGB sang HSV và ngược lại		
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
	*/
	int Convert(Mat& sourceImage, Mat& destinationImage, int type);
	int Convert(IplImage* sourceImage, IplImage* destinationImage, int type);
	
};

Converter::Converter()
{}

Converter::~Converter()
{}


int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.channels() != 3)
		return 1;
	for (int r = 0; r < sourceImage.rows; r++)
	{
		for (int c = 0; c < sourceImage.cols; c++)
		{
			auto blue = sourceImage.at<Vec3b>(r, c)[0];
			auto green = sourceImage.at<Vec3b>(r, c)[1];
			auto red = sourceImage.at<Vec3b>(r, c)[2];
			destinationImage.at<uint8_t>(r, c) = (uint8_t)(0.114*blue + 0.587*green + 0.299*red);
		}
	}
	return 0;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.channels() != 3)
		return 1;

	for (int r = 0; r < sourceImage.rows; r++)
	{
		for (int c = 0; c < sourceImage.cols; c++)
		{
			uint8_t blue = sourceImage.at<Vec3b>(r, c)[0];
			uint8_t green = sourceImage.at<Vec3b>(r, c)[1];
			uint8_t red = sourceImage.at<Vec3b>(r, c)[2];

			uint8_t V = max(max(blue, green), red);
			float S = 0;
			if (V != 0)
				S = (float)(V-min(min(blue, green), red)) / V;
			int H;
			if (V == red)
				H = 60 * (green - blue) / (V - min(min(blue, green), red));
			else if (V == green)
				H = 120 + 60 * (blue - red) / (V - min(min(blue, green), red));
			else if (V == blue)
				H = 240 + 60 * (red - green) / (V - min(min(blue, green), red));

			if (H < 0)
				H += 360;

			S *= 255;
			H /= 2;
			destinationImage.at<Vec3b>(r, c)[0] = H;
			destinationImage.at<Vec3b>(r, c)[1] = (uint8_t)S;
			destinationImage.at<Vec3b>(r, c)[2] = V;
		}
	}
	return 0;
}


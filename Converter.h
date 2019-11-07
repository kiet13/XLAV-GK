#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
	//Kiểm tra ảnh input có rỗng ko?
	if (sourceImage.empty())
		return 1;

	//Kiểm tra ảnh input phải là ảnh RGB ko?
	CV_Assert(sourceImage.type() == CV_8UC3);

	//Tạo ảnh KQ
	destinationImage.create(sourceImage.size(), CV_8UC1);

	//Xử lý từng pixel
	for (int j = 0; j < sourceImage.rows; j++)
	{
		const uchar* src_ptr = sourceImage.ptr<uchar>(j);
		uchar* dst_ptr = destinationImage.ptr<uchar>(j);

		//Gray = 0.082*Blue + 0.6095*Green + 0.3086*Red 
		for (int i = 0; i < sourceImage.cols; i++)
		{
			dst_ptr[i] = (uchar)(src_ptr[0] * 0.0820f + src_ptr[1] * 0.6094f + src_ptr[2] * 0.3086f);
			src_ptr += 3;
		}

	}

	return 0;
}

inline int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage)
{
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

			uint8_t Value = max(max(blue, green), red);
			float Saturation = 0;
			if (Value != 0)
				Saturation = (float)(Value - min(min(blue, green), red)) / Value;
			int Hue;
			if (Value == red)
				Hue = 60 * (green - blue) / (Value - min(min(blue, green), red));
			else if (Value == green)
				Hue = 120 + 60 * (blue - red) / (Value - min(min(blue, green), red));
			else if (Value == blue)
				Hue = 240 + 60 * (red - green) / (Value - min(min(blue, green), red));
			if (Hue < 0)
				Hue += 360;

			Saturation *= 255;
			Hue /= 2;
			destinationImage.at<Vec3b>(r, c)[0] = Hue;
			destinationImage.at<Vec3b>(r, c)[1] = (uint8_t)Saturation;
			destinationImage.at<Vec3b>(r, c)[2] = Value;
		}
	}
	return 0;
}

inline int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.channels() != 3)
		return 1;
	for (int r = 0; r < sourceImage.rows; r++)
	{
		for(int c=0;c <sourceImage.cols;c++)
		{
			int Hue = sourceImage.at<Vec3b>(r, c)[0];
			double Saturation = sourceImage.at<Vec3b>(r, c)[1];
			double Value = sourceImage.at<Vec3b>(r, c)[2];

			double C = Saturation * Value;
			double X = C * (1 - abs(fmod(Hue / 60.0, 2) - 1));
			double m = Value - C;
			uint8_t R, G, B;
			if (Hue >= 0 && Hue < 60)
			{
				R = C;
				G = X;
				B = 0;
			}
			else if (Hue >= 60 && Hue < 120)
			{
				R = X;
				G = C;
				B = 0;
			}
			else if (Hue >= 120 && Hue < 180)
			{
				R = 0;
				G = C;
				B = X;
			}
			else if (Hue >= 180 && Hue < 240)
			{
				R = 0;
				G = X;
				B = C;
			}
			else if (Hue >= 240 && Hue < 300)
			{
				R = X;
				G = 0;
				B = C;
			}
			else
			{
				R = C;
				G = 0;
				B = X;
			}
			destinationImage.at<Vec3b>(r, c)[0] = (uint8_t)(R + m) * 255;
			destinationImage.at<Vec3b>(r, c)[1] = (uint8_t)(G + m) * 255;
			destinationImage.at<Vec3b>(r, c)[2] = (uint8_t)(B + m) * 255;
		}
	}
	
	return 0;
}

inline int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	switch (type) {
	case 0:
		return RGB2GrayScale(sourceImage, destinationImage);
	case 1:
		return GrayScale2RGB(sourceImage, destinationImage);
	case 2:
		return RGB2HSV(sourceImage, destinationImage);
	case 3:
		return HSV2RGB(sourceImage, destinationImage);
	default:
		return 1;
	}
	return 0;
}


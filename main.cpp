
#include "Converter.h"
#include "ColorTransformer.h"
#include "EdgeDetector.h"
#include "GeometricTransformer.h"
#include "Blur.h"
# define M_PI           3.14159265358979323846  /* pi */

int main(int argc, char *argv[])
{
	try
	{
		 // Get input path
		char* fileinput = argv[2];
		Mat origin = imread(fileinput, CV_LOAD_IMAGE_ANYCOLOR); // Original image
		Converter converter;
		ColorTransformer tranform;
		if (strcmp(argv[1], "--rgb2gray") == 0)
		{
			//Show Original image
			imshow("Original Image", origin);

			Mat rgb2gray;
			//Xử lý ảnh bằng hàm tự viết
			if (converter.Convert(origin, rgb2gray, 0) == 1) {
				cout << "Error ...";
			};
			imshow("Result Image", rgb2gray);

			//Dùng công cụ open CV tạo ra ảnh ví dụ
			Mat exam;
			cvtColor(origin, exam, CV_RGB2GRAY);
			imshow("Exam Image", exam);
		}
		else if (strcmp(argv[1], "--gray2rgb") == 0)
		{
			//Show Original image
			imshow("Original Image", origin);

			Mat gray2rgb;
			//Xử lý ảnh bằng hàm tự viết
			if (converter.Convert(origin, gray2rgb, 2) == 1) {
				cout << "Error ...";
			};
			imshow("Result Image", gray2rgb);

			//Dùng công cụ open CV tạo ra ảnh ví dụ
			Mat exam;
			cvtColor(origin, exam, CV_GRAY2BGR);
			imshow("Exam Image", exam);
		}
		else if (strcmp(argv[1], "--rgb2hsv") == 0)
		{
			//Show Original image
			imshow("Original Image", origin);

			Mat rgb2hsv;
			//Xử lý ảnh bằng hàm tự viết
			if (converter.Convert(origin, rgb2hsv, 2) == 1) {
				cout << "Error ...";
			};
			imshow("Result Image", rgb2hsv);

			//Dùng công cụ open CV tạo ra ảnh ví dụ
			Mat exam;
			cvtColor(origin, exam, CV_BGR2HSV);
			imshow("Exam Image", exam);
		}
		else if (strcmp(argv[1], "--hsv2rgb") == 0)
		{
			//Show Original image
			imshow("Original Image", origin);

			Mat hsv2rgb;
			//Xử lý ảnh bằng hàm tự viết
			if (converter.Convert(origin, hsv2rgb, 3) == 1) {
				cout << "Error ...";
			};
			imshow("Result Image", hsv2rgb);

			//Dùng công cụ open CV tạo ra ảnh ví dụ
			Mat exam;
			cvtColor(origin, exam, CV_HSV2BGR);
			imshow("Exam Image", exam);
		}
		else if (strcmp(argv[1], "--bright") == 0)
		{
			Mat changedBrightness;
			short b = (short)atoi(argv[3]);
			tranform.ChangeBrighness(origin, changedBrightness, b);
			imshow("Origin", origin);
			imshow("Changed brightness", changedBrightness);
		}
		else if (strcmp(argv[1], "--contrast") == 0)
		{
			Mat changedContrast;
			float c = (float)atof(argv[3]);
			tranform.ChangeContrast(origin, changedContrast, c);
			imshow("Origin", origin);
			imshow("Changed contrast", changedContrast);
		}
		else if (strcmp(argv[1], "--drawHist") == 0)
		{
			int numBins = 256;
			if (origin.channels() == 3)
			{
				Mat splitChannels[3];
				split(origin, splitChannels);
				Mat b_hist, g_hist, r_hist;
				CalcHistogram(splitChannels[0], b_hist);
				CalcHistogram(splitChannels[1], g_hist);
				CalcHistogram(splitChannels[2], r_hist);

				// draw the histogram for R, G and B
				int hist_w = 768; int hist_h = 600;
				int bin_w = hist_w / numBins;

				Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

				DrawHistogram(b_hist, histImage, 255, 0, 0, 0);
				DrawHistogram(g_hist, histImage, 0, 255, 0, 1);
				DrawHistogram(r_hist, histImage, 0, 0, 255, 2);
				namedWindow("Histogram of RGB image", CV_WINDOW_AUTOSIZE);
				imshow("Histogram", histImage);
			}
			else if (origin.channels() == 1)
			{
				Mat hist;
				CalcHistogram(origin, hist);
				// draw the histogram for R, G and B
				int hist_w = 768; int hist_h = 600;
				int bin_w = hist_w / numBins;

				Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

				DrawHistogram(hist, histImage, 255, 0, 0, 0);
				namedWindow("Histogram of gray scale image", CV_WINDOW_AUTOSIZE);
				imshow("Histogram", histImage);
			}
		}
		else if (strcmp(argv[1], "--prewitt") == 0)
		{
<<<<<<< HEAD
			Mat grayOrigin;
			converter.RGB2GrayScale(origin, grayOrigin);
			// Edge dectection by Prewitt
			EdgeDetector prewitt;
			Mat destinationImage;
			prewitt.DetectEdge(grayOrigin, destinationImage, 3, 3, 2);
			imshow("Original", origin);
			imshow("Detect edge by prewitt", destinationImage);

		}
		else if (strcmp(argv[1], "--laplace") == 0)
		{
				Mat grayOrigin;
				converter.RGB2GrayScale(origin, grayOrigin);
				EdgeDetector laplace;
				Mat destiantionImage;
				laplace.DetectEdge(grayOrigin, destiantionImage, 3, 3, 3);
				imshow("Original", origin);
				imshow("Detect edge by laplace", destiantionImage);
		}
		else if (strcmp(argv[1], "--rotateN") == 0)
		{
		imshow("Original", origin);
		PixelInterpolate* interpolator = NULL;

		if (strcmp(argv[3], "--bl") == 0)
			interpolator = new BilinearInterpolate;
		else if (strcmp(argv[3], "--nn") == 0)
			interpolator = new NearestNeighborInterpolate;

		}
		else if (strcmp(argv[1], "--sobel") == 0)
		{
			Mat grayOrigin;
			converter.RGB2GrayScale(origin, grayOrigin);
			// Edge dectection by Prewitt
			EdgeDetector sobel;
			Mat destinationImage;
			sobel.DetectEdge(grayOrigin, destinationImage, 3, 3, 1);
			imshow("Original", origin);
			imshow("Detect edge by sobel", destinationImage);
		}

		if (interpolator != NULL)
		{
			float angle = (float)atof(argv[4]);
			angle = M_PI * (angle / 180);
			GeometricTransformer transformer;
			Mat rotatedImage(origin.size(), origin.type(), Scalar(0));
			transformer.RotateUnkeepImage(origin, rotatedImage, angle, interpolator);
			imshow("Rotate", rotatedImage);
		}

		/*Mat exam = origin.clone();
		Point2f center(origin.cols / 2, origin.rows / 2);
		Mat matRot = getRotationMatrix2D(center, angle, 1);
		warpAffine(origin, exam, matRot, origin.size());
		imshow("Exam", exam);*/
		}
		else if (strcmp(argv[1], "--mean") == 0)
		{
		//Hiện ảnh gốc
		imshow("Original", origin);

		//Xử lý tham số truyền vào
		int kWidth = int(atof(argv[3]));
		int kHeigth = int(atof(argv[4]));

		//Xử lý bằng Blur
		Mat mean;
		Blur blur;
		blur.BlurImage(origin, mean, kWidth, kHeigth, 0);

		//Hiện ảnh kết quả
		imshow("Result (Mean)", mean);

		}
		else if (strcmp(argv[1], "--median") == 0)
		{
		//Hiện ảnh gốc
		imshow("Original", origin);

		//Xử lý tham số truyền vào
		int kWidth = int(atof(argv[3]));
		int kHeigth = int(atof(argv[4]));

		////Xử lý bằng Blur
		Mat median;
		Blur blur;
		if (blur.BlurImage(origin, median, kWidth, kHeigth, 1) == 1)
			cout << "Error..." << endl;

		////Hiện ảnh kết quả
		imshow("Result (Median)", median);
		}
		else if (strcmp(argv[1], "--gauss") == 0)
		{
		//Hiện ảnh gốc
		imshow("Original", origin);

		//Xử lý tham số truyền vào
		int kWidth = int(atof(argv[3]));
		int kHeigth = int(atof(argv[4]));

		//Xử lý bằng Blur
		Mat mean;
		Blur blur;
		blur.BlurImage(origin, mean, kWidth, kHeigth, 2);

		//Hiện ảnh kết quả
		imshow("Result (Gauss)", mean);
=======
			if (origin.channels() == 1)
			{
				// Edge dectection by Prewitt
				EdgeDetector prewitt;
				Mat destinationImage;
				prewitt.DetectEdge(origin, destinationImage, 3, 3, 2);
				imshow("Original", origin);
				imshow("Detect edge by prewitt", destinationImage);
>>>>>>> 7e8bc2b159d4ce1fd9df64469d2f827323b7884d

			}
		}
		else if (strcmp(argv[1], "--zoom") == 0)
		{
			PixelInterpolate* interpolator = NULL;

			if (strcmp(argv[3], "--bl") == 0)
				interpolator = new BilinearInterpolate;
			else if (strcmp(argv[3], "--nn") == 0)
				interpolator = new NearestNeighborInterpolate;

			if (interpolator != NULL)
			{
				float sx = (float)atof(argv[4]);
				// float sy = (float)atof(argv[5]);
				GeometricTransformer transformer;
				Mat zoomedImage = Mat(int(origin.rows * sx), int(origin.cols * sx), origin.type(), Scalar(0));
				transformer.Scale(origin, zoomedImage, sx, sx, interpolator);

				imshow("Original", origin);
				imshow("Zoom", zoomedImage);
			}

		}
		else if (strcmp(argv[1], "--flip") == 0)
		{
			PixelInterpolate* interpolator = NULL;
			if (strcmp(argv[3], "--bl") == 0)
			{
				interpolator = new BilinearInterpolate;
			}
			else if (strcmp(argv[3], "--nn") == 0)
			{
				interpolator = new NearestNeighborInterpolate;
			}
			if (interpolator != NULL)
			{
				int Ox = atoi(argv[4]);
				int Oy = atoi(argv[5]);
				GeometricTransformer transformer;
				Mat FlipedImage = Mat(int(origin.rows * Ox), int(origin.cols * Oy), origin.type(), Scalar(0));
				transformer.Flip(origin, FlipedImage, Ox, Oy, interpolator);
				
				imshow("Original", origin);
				imshow("Fliped Image", FlipedImage);
			}
		}
		else
		{
			cout << "Invalid command!!!" << endl;
		}
		waitKey(0);
	}
	catch (cv::Exception &e)
	{
		cerr << e.msg << endl;
	}
	return 0;
}


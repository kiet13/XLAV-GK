#include "Converter.h"
#include "ColorTransformer.h"
#include "EdgeDetector.h"
#include "GeometricTransformer.h"

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

			Mat rgb2gray;
			//Xử lý ảnh bằng hàm tự viết
			if (converter.Convert(origin, rgb2gray, 2) == 1) {
				cout << "Error ...";
			};
			imshow("Result Image", rgb2gray);

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
			Mat grayOrigin;
			converter.RGB2GrayScale(origin, grayOrigin);
			// Edge dectection by Prewitt
			EdgeDetector prewitt;
			Mat destinationImage;
			prewitt.DetectEdge(grayOrigin, destinationImage, 3, 3, 2);
			imshow("Original", origin);
			imshow("Detect edge by prewitt", destinationImage);

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
				float sy = (float)atof(argv[5]);
				GeometricTransformer transformer;
				Mat zoomedImage = Mat(int(origin.rows * sy), int(origin.cols * sx), origin.type(), Scalar(0));
				transformer.Scale(origin, zoomedImage, sx, sy, interpolator);

				imshow("Original", origin);
				imshow("Zoom", zoomedImage);
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


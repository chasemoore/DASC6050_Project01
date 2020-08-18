// opencv-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Checks cv::String vector for valid image extension
bool Check_ext(const cv::String filename)
{
    size_t pos = filename.rfind('.');
    if (pos == string::npos)
        // path to file does not contain an image
    
        return false;

    string ext = filename.substr(pos+1);

    if (ext == "jpg" || ext == "jpeg" || ext == "gif" || ext == "png")
        // path to file contains an image
        return true;

    return false;
}

// this readImages is based on an DFS, we're going to try BFS

void readImages(String path, vector<cv::String> &fn, vector<cv::Mat> &images){
    // current path = ^^^
    glob(path , fn, false);
    size_t count = fn.size();
    for (size_t i = 0; i < count; i++)
	{
        if (Check_ext(fn[i]) == true){
            images.push_back(imread(fn[i]));
        }
        else {
            fn.pop_back(); //fix later
            // modify filename path
            // current path . append /subdirectory ***
            // newPAth = ksd;fjaks
            // old_path
            readImages(path, fn, images);
        }
		
	}
    
}

int main()
{
    String path = "/Users/ericthomas/Desktop/dasc_project01/goya/*.jpg";

	/* change this to your image path (0 = false, 1 = true)  
    // next three lines test filename extensions
    cv::String testPath = "/Users/ericthomas/Desktop/dasc_project01/Goya/goat.jpg";
    std::cout << Check_ext(testPath);
    std::cout << "test";
    return 0;
     end test */

	vector<cv::String> fn;
	glob(path , fn, false);
	vector<Mat> images;

	//number of png files in images folder
	size_t count = fn.size(); 

	//make sure we're getting these images
	//printf("%lu", count);

	for (size_t i = 0; i < count; i++)
	{
		images.push_back(imread(fn[i]));
	}
	
	namedWindow("Image Browser", WINDOW_AUTOSIZE);

	//Display the images in the directory one per key press
    for (int i = 0; i < images.size(); i++){
        imshow("Image Browser", images[i]);
	    waitKey(0);
    }
	// imshow("Image Browser", images[0]);
	// waitKey(0);

    //TODO: Display multiple images in a display window.
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


/**Graveyard

//Mat image = Mat::zeros(300, 600, CV_8UC3);
	//circle(image, Point(250, 150), 100, Scalar(0, 255, 128), -100);
	//circle(image, Point(350, 150), 100, Scalar(255, 255, 255), -100);
	//imshow("Display Window", image);
	//waitKey(0);



cv::Mat makeCanvas(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows) {
    int N = vecMat.size();
    nRows = nRows > N ? N : nRows;
    int edgeThickness = 10;
    int imagesPerRow = ceil(double(N) / nRows);
    int resizeHeight = floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
    int maxRowLength = 0;

    std::vector<int> resizeWidth;
    for (int i = 0; i < N;) {
        int thisRowLen = 0;
        for (int k = 0; k < imagesPerRow; k++) {
            double aspectRatio = double(vecMat[i].cols) / vecMat[i].rows;
            int temp = int(ceil(resizeHeight * aspectRatio));
            resizeWidth.push_back(temp);
            thisRowLen += temp;
            if (++i == N) break;
        }
        if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength) {
            maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
        }
    }
    int windowWidth = maxRowLength;
    cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, Scalar(0, 0, 0));

    for (int k = 0, i = 0; i < nRows; i++) {
        int y = i * resizeHeight + (i + 1) * edgeThickness;
        int x_end = edgeThickness;
        for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
            int x = x_end;
            cv::Rect roi(x, y, resizeWidth[k], resizeHeight);
            cv::Size s = canvasImage(roi).size();
            // change the number of channels to three
            cv::Mat target_ROI(s, CV_8UC3);
            if (vecMat[k].channels() != canvasImage.channels()) {
                if (vecMat[k].channels() == 1) {
                    cv::cvtColor(vecMat[k], target_ROI, CV_GRAY2BGR);
                }
            }
            else {
                vecMat[k].copyTo(target_ROI);
            }
            cv::resize(target_ROI, target_ROI, s);
            if (target_ROI.type() != canvasImage.type()) {
                target_ROI.convertTo(target_ROI, canvasImage.type());
            }
            target_ROI.copyTo(canvasImage(roi));
            x_end += resizeWidth[k] + edgeThickness;
        }
    }
    return canvasImage;
}

*/
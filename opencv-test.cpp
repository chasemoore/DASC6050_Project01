// opencv-test.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <opencv2/core/core.hpp>
#Chase Moore
#Eric Thomas
#Todd Vlk

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    string path = "C:/Users/willi/Desktop/Grad/Fall2020/DASC6050/images";

    vector<cv::String> fn;
    glob(path, fn, false);
    

    //number of png files in images folder
    size_t count = fn.size();

    //hold the resized images 
    vector<Mat> resizedImages(count);

        //import numpy as np
        //import cv2 as cv
        //img = cv.imread('messi5.jpg', 0)
        //rows, cols = img.shape
        //M = np.float32([[1, 0, 100], [0, 1, 50]] )
        //dst = cv.warpAffine(img, M, (cols, rows))
        //cv.imshow('img', dst)
        //cv.waitKey(0)
        //cv.destroyAllWindows()

    //argument 2 and 3
    size_t x = 7;
    size_t y = 7;

    //make all images the same size
    for (size_t i = 0; i < count; i++)
    {
        //([[1,0,100], [0,1,50]])
        //warpAffine(imread(fn[i], 0),,imread(fn[i]).shape);
        resize(imread(fn[i]), resizedImages[i], Size(x*100, y*100), 0, 0, INTER_AREA);
    }

    //scales the browser based off the images
    namedWindow("Image Browser", WINDOW_AUTOSIZE);

    //Display the images in the directory one per key press
    for (int i = 0; i < resizedImages.size(); i++) {
        imshow("Image Browser", resizedImages[i]);
        waitKey(0);
    }

    return 0;
}



/**Graveyard
*/


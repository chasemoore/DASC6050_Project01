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

//scales the image based upon the dimensions of src image.
Mat scaleImage(Mat src, size_t scale)
{
    Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(src.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, src.rows - 1.f);

    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, 0.f);
    dstTri[1] = Point2f(src.cols * scale, src.rows * 0.0f);
    dstTri[2] = Point2f(src.cols * 0.0f, src.rows * scale);

    Mat warp_mat = getAffineTransform(srcTri, dstTri);
    Mat warp_dst = Mat::zeros(src.rows, src.cols, src.type());

    warpAffine(src, warp_dst, warp_mat, warp_dst.size() * (int)scale);

    return warp_dst;

}


int main(int argc, char** argv)
{

    string path = "C:/Users/willi/Desktop/Grad/Fall2020/DASC6050/images";

    vector<cv::String> fn;
    glob(path, fn, false);
    

    //number of png files in images folder
    size_t count = fn.size();

    //hold the resized images 
    vector<Mat> resizedImages(count);

     

    //how big are we scaling the images
    size_t scale = 1.0f;

    //make all images the same size
    for (size_t i = 0; i < count; i++) 
        resizedImages.push_back(imread(fn[i]));
   

    //Display the images in the directory one per key press
    for (int i = 0; i < resizedImages.size(); i++) {

        //Make sure we're actually displaying an image.
        if (resizedImages[i].cols > 0 && resizedImages[i].rows > 0)
        {
            imshow("Scaled", scaleImage(resizedImages[i], scale));
            waitKey(0);
        }
 
    }
    
    //waitKey(0);
    return 0;
}



/**Graveyard
* 
* 
* Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(src.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, src.rows - 1.f);
    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, src.rows * 0.33f);
    dstTri[1] = Point2f(src.cols * 0.85f, src.rows * 0.25f);
    dstTri[2] = Point2f(src.cols * 0.15f, src.rows * 0.7f);
    Mat warp_mat = getAffineTransform(srcTri, dstTri);
    Mat warp_dst = Mat::zeros(src.rows, src.cols, src.type());
    warpAffine(src, warp_dst, warp_mat, warp_dst.size());
    Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
    double angle = -50.0;
    double scale = 0.6;
    Mat rot_mat = getRotationMatrix2D(center, angle, scale);
    Mat warp_rotate_dst;
    warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
    imshow("Source image", src);
    imshow("Warp", warp_dst);
    imshow("Warp + Rotate", warp_rotate_dst);
    waitKey();
* 
*   //import numpy as np
     //import cv2 as cv
     //img = cv.imread('messi5.jpg', 0)
     //rows, cols = img.shape
     //M = np.float32([[1, 0, 100], [0, 1, 50]] )
     //dst = cv.warpAffine(img, M, (cols, rows))
     //cv.imshow('img', dst)
     //cv.waitKey(0)
     //cv.destroyAllWindows()

      // resize(imread(fn[i]), resizedImages[i], Size(x*100, y*100), 0, 0, INTER_AREA);

       Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(src.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, src.rows - 1.f);

    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, 0.f);
    dstTri[1] = Point2f(src.cols * 2.0f, src.rows * 0.0f);
    dstTri[2] = Point2f(src.cols * 0.0f, src.rows * 2.0f);

    Mat warp_mat = getAffineTransform(srcTri, dstTri);
    Mat warp_dst = Mat::zeros(src.rows, src.cols, src.type());

    warpAffine(src, warp_dst, warp_mat, warp_dst.size() * 2);


*/


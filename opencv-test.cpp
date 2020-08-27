// opencv-test.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <queue>
//#include <dirent.h>
#include <sys/types.h>

using namespace cv;
using namespace std;

Mat scaleImage(Mat src, size_t scale);
Mat makeCanvas(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows);
bool Check_ext(string filename);


//scales the image based upon the dimensions of src image.
Mat scaleImage(Mat src, size_t scale)
{
    Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(src.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, src.rows - 1.f);

    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, 0.f);
    dstTri[1] = Point2f(src.cols * 0.25f, src.rows * 0.0f);
    dstTri[2] = Point2f(src.cols * 0.0f, src.rows * 0.25f);

    Mat warp_mat = getAffineTransform(srcTri, dstTri);
    Mat warp_dst = Mat::zeros(src.rows, src.cols, src.type());

    warpAffine(src, warp_dst, warp_mat, warp_dst.size() * (int)scale);

    return warp_dst;

}


  Mat makeCanvas(vector <Mat>& vecMat, int windowHeight, int nRows) {
    
    int width = nRows;
    int N = vecMat.size();
    nRows = nRows > N ? N : nRows;
    double magicNum = 2.0;
    int edgeThickness = 10;
    int imagesPerRow = ceil(double(N) / nRows);
    int resizeHeight = floor(magicNum * ((floor(double(windowHeight - edgeThickness) / nRows)) / magicNum)) - edgeThickness;
    
    int maxRowLength = 0;
    std::vector<int> resizeWidth;
    for (int i = 0; i < N;) 
    {
        int thisRowLen = 0;
        for (int k = 0; k < imagesPerRow; k++) 
        {
           
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
    int windowWidth = width;
    cout << "windowWidth -> " << windowWidth << endl;

    cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, Scalar(0, 0, 0));
    cout << canvasImage.rows << endl << canvasImage.cols << endl;


    for (int k = 0, i = 0; i < nRows; i++) {
        int y = i * resizeHeight + (i + 1) * edgeThickness;
        int x_end = edgeThickness;
        cout << "resize Width -> " << resizeWidth[k] << endl;

        for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
            int x = x_end;
            Rect roi(x, y, resizeWidth[k], resizeHeight);
            if (x > y || resizeWidth[k] > resizeHeight)
            {
                x == y;
                resizeWidth[k] == resizeHeight;
                Rect roi(x, y, resizeWidth[k], resizeHeight);
            }
            cout << "rect roi -> " << roi << endl;



            Size s = canvasImage(roi).size();
            cout << "here1" << endl;




            // change the number of channels to three
            Mat target_ROI(s, CV_8UC3);
            if (vecMat[k].channels() != canvasImage.channels()) {
                if (vecMat[k].channels() == 1) {
                    cv::cvtColor(vecMat[k], target_ROI, CV_32FC4);
                }
            }
            else {
                vecMat[k].copyTo(target_ROI);
            }
            cv::resize(target_ROI, target_ROI, s);
            if (target_ROI.type() != canvasImage.type()) {
                target_ROI.convertTo(target_ROI, canvasImage.type());
            }
            cout << "here2" << endl;
            target_ROI.copyTo(canvasImage(roi));
            
           x_end += resizeWidth[k] + edgeThickness;
           
        }
        
    }
    //cout << "returning";
    return canvasImage;
}

  // Checks cv::String vector for valid image extension.
// Helper Function.
  bool Check_ext(string filename)
  {
      size_t pos = filename.rfind('.');
      if (pos == string::npos) {
          // File path is not an image.
          return false;
      }
      string ext = filename.substr(pos + 1);

      if (ext == "jpg" || ext == "jpeg" || ext == "gif" || ext == "png") {
          // File path is an image
          return true;
      }
      return false;
  }




int main(int argc, char** argv)
{
    
    //string path = "C:/Users/willi/Desktop/Grad/Fall2020/DASC6050/images";
    //string path = "C:\\Users\\willi\\Desktop\\Grad\\Fall2020\\DASC6050\\images";
    string path = argv[1];
    
    //"C:/Users/willi/Desktop/Grad/Fall2020/DASC6050/images"

    vector<cv::String> fn;
    glob(path, fn, false);

   
    

    //number of png files in images folder
    size_t count = fn.size();

    //hold the resized images 
    vector<Mat> resizedImages(count);

     

    //how big are we scaling the images
    size_t scale = 0.5f;
    int x = 1;
    int y = 1;
    cv::Mat rsImg;

    //make all images the same size
    for (size_t i = 0; i < count; i++) {
        //if (Check_ext(fn[i]))
            resizedImages.push_back(imread(fn[i]));
    }
   
    //Resized canvas begin

    //if (resizedImages[i].cols > 0 && resizedImages[i].rows > 0)
   //{
   
   
   
    //}

    //Resized cavnsas end

    //Display the images in the directory one per key press
    for (size_t i = 0; i < resizedImages.size(); i++) {

        //Make sure we're actually displaying an image.
        if (resizedImages[i].cols > 0 && resizedImages[i].rows > 0)
       {
            vector<Mat> image;
            image.push_back( resizedImages[i]);
            imshow("image", makeCanvas(image, 1000,1000));
           // rsImg = scaleImage(resizedImages[i], scale);
            //resize(imread(fn[i]), rsImg, Size(x * 100, y * 100), 0, 0, INTER_AREA);
            //imshow("Scaled", rsImg);
            waitKey(0);
            cout <<"index i -> " <<  i << endl;
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

`
*/


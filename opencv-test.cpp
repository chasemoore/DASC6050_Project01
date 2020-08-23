// opencv-test.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <queue>
#include <dirent.h>
#include <sys/types.h>


using namespace cv;
using namespace std;

bool Check_ext(string filename);
void listFiles(const char *path, vector<cv::Mat> &images);

// string path = "/Users/ericthomas/Desktop/dasc_project01/goya/";

int main(int argc, char *argv[])
{
    // Variable declaration for the root directory path and image vector.
    char path[100];
    vector<Mat> images;
    
    // Conditional statements to determine the meaning of entered program args.
    if(argc < 2 || argc > 4){
        std::cout << "Too many/few arguments.";
        return 0;
    } else if(argc == 2){
        strcpy(path, argv[1]);
    } else {
        // TODO
        // 4 arguments: do something with -r and -c arguments in regards to
        // window size and transformations.
    }

    // listFiles takes a directory path as a char array as well as a vector
    // of type cv::Mat and returns the vector full of images from directories.
    listFiles(path, images);

    // Display the images in the image vector one at a time, per key press.
	namedWindow("Image Browser", WINDOW_AUTOSIZE);
    int index = 0;


    while(true){
        imshow("Image Browser", images[index]);
        // Fill out while loop to dispay image, read user input, determine where
        // p, n or exit was pressed.
    }
    
    // Delete this for loop once the proper while loop above is filled out.
    for (int i = 0; i < images.size(); i++){
        imshow("Image Browser", images[i]);
	    waitKey(0);
    }
	return 0;
}


// Function to list files obtained from a directory path.
void listFiles(const char *path, vector<cv::Mat> &images)
{
    char fileName[255];
    char directoryPath[255];
    strncpy(directoryPath, path, 254);
    string str(directoryPath);
    string fullPath = str;
    vector<cv::String> fnames; // vector to return to calling function.
    struct dirent *dp;
    DIR *dir = opendir(path);

    // Unable to open directory stream
    if (!dir) 
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        // Check to determine whether the current directory pointer is a 
        // directory or a file.
        if (dp->d_type == DT_DIR){
            // Char array conversion for comparison purposes.
            strncpy(fileName, dp->d_name, 254);
            string str(fileName);
            string oneDotDir = ".";
            string twoDotDir = "..";
            // Check to make sure directory doesn't point to a parent directory
            // or to the current directory.
            if(str != oneDotDir && str != twoDotDir){
                // Directory Path is a valid subdirectory.
                // Explore this "node" directory in DFS algorithm manner.
                string temp_fn = fullPath + "/" + str;
                char newPath[255];
                strcpy(newPath, temp_fn.c_str());
                listFiles(newPath, images);
            }
        }
        else {
            // Directory pointer was not a directory but instead a file.
            // Char array conversion for comparison purposes.
            strncpy(fileName, dp->d_name, 254);
            string str(fileName);
            // Check to see if the file has the proper extension for image media.
            if(Check_ext(str)){
                string temp_fn = fullPath + "/" + str;
                // File passes the check and is added to the image vector.
                images.push_back(imread(temp_fn));
            }
        }
        
    }

    // Close directory stream
    closedir(dir);
    return;
}

// Checks cv::String vector for valid image extension.
// Helper Function.
bool Check_ext(string filename)
{
    size_t pos = filename.rfind('.');
    if (pos == string::npos){
        // File path is not an image.
        return false;
    }
    string ext = filename.substr(pos+1);

    if (ext == "jpg" || ext == "jpeg" || ext == "gif" || ext == "png"){
        // File path is an image
        return true;
    }
    return false;
}

// Graveyard below (may contain information on window sizing -see below for more information).








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
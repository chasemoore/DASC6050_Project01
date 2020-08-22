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

// Function to list files obtained from a directory path.
void listFiles(const char *path)
{
    char str1[] = ".";
    char str2[] = ". .";
    char fileName[255];
    vector<cv::String> fnames; // vector to return to calling function.
    struct dirent *dp;
    DIR *dir = opendir(path);

    // Unable to open directory stream
    if (!dir) 
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (dp->d_type == DT_DIR){
            // Convert directory name (char arr)
            // to string for string concatentation
            // and string comparison
            strncpy(fileName, dp->d_name, 254);
            string str(fileName);
            string oneDotDir = ".";
            string twoDotDir = "..";
            if(str != oneDotDir && str != twoDotDir){
                // Do NOT add to stack of paths
                std::cout << "Good directory! ";
                std::cout << str << "\n";
            }
            else {
                std::cout << "Bad Directory! ";
                std::cout << str << "\n";
            }
            
        }
        else {
            // dir read was not a directory
            // handle as a file
            // check extension
            // Convert file name (char arr)
            // to string for string concatentation
            // and string comparison
            strncpy(fileName, dp->d_name, 254);
            string str(fileName);
            if(Check_ext(str)){
                std::cout << "Valid Image! " << str << "\n";
            }
            else {
                std::cout << "Garbage: " << str << "\n";
            }
        }
        // printf("%s\n", dp->d_name);
    }

    // Close directory stream
    closedir(dir);
    return; // returned vector.
}

// Checks cv::String vector for valid image extension
bool Check_ext(string filename)
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

// this readImages is based on an BFS, we're going to try BFS

void readImages(cv::String path, vector<cv::Mat> &images){
    // Create stack for path names
    vector<cv::String> fnames;
    glob(path , fnames, false);
    // Do while stack not empty
    
    while(fnames.size() > 1){
        cv::String temp_fn;
        // Grab path name from stack and store in temp
        temp_fn = fnames.back();
        std::cout << temp_fn << "\n"; 
        // Remove path name from stack
        fnames.pop_back();
        // Check to see if pathname folder or image
        if (Check_ext(temp_fn)){
            std::cout << "Hello\n";
            images.push_back(imread(temp_fn));
        }
        else {
            std::cout << temp_fn << "\n";
            // waitKey(0);
           // TODO grab substring after last '/' character in filename var temp.
           // append to end of current path as new path var.
            cv::String folder_name;
            // Grab index of first char in the folder name.
            // Append folder name to current path.
            folder_name = path.substr(path.find_last_of("/")+1);
            cv::String temp_path;
            temp_path = path.append(folder_name);
            std::cout << temp_path;
            readImages(temp_path, images);

        }
    }
    return;
}

int main()
{

    // Store hard-coded root directory path into the path variable.
    cv::String path = "/Users/ericthomas/Desktop/dasc_project01/goya/";

    /*
    char path[100];

    // Input path from user
    printf("Enter path to list files: ");
    scanf("%s", path);

    listFiles(path);

    return 0;
    */

    
	vector<cv::String> fn;
    // Grab all file names as directory paths from the root folder.
	glob(path , fn, false);
    

    // Declare vector of image files.
	vector<Mat> images;

    /* Output test to determine how many file pathnames were read.
     std::cout << fn.size() << "\n";
    for (int i = 0; i <= fn.size(); i++){
        std::cout << fn[i] << "\n";
    }
    */

    // Call readImages function to read filenames from each directory.
    readImages(path, images);
    
	namedWindow("Image Browser", WINDOW_AUTOSIZE);

	// Display the images in the directory one per key press
    for (int i = 0; i < images.size(); i++){
        imshow("Image Browser", images[i]);
	    waitKey(0);
    }
	
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
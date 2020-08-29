//Chase Moore
//Todd VLK
//Eric Thomas

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <queue>
#include <opencv2/dirent.h>
#include <dirent.h>
#include <sys/types.h>
#include <conio.h>


using namespace cv;
using namespace std;

Mat makeCanvas(vector <Mat>& vecMat, int windowHeight, int nRows);
Mat scaleImage(Mat src, size_t scale);
bool Check_ext(string filename);
void listFiles(const char* path, vector<cv::Mat>& images, vector<String>& fn, vector<String>& fp);
Mat metadata(Mat& img, string fname, string fpath);


// string path = "/Users/ericthomas/Desktop/dasc_project01/goya/";



Mat makeCanvas(vector <Mat>& vecMat, int windowHeight, int nRows) {
    Mat img = vecMat[0];
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
    //cout << "windowWidth -> " << windowWidth << endl;

    cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, Scalar(0, 0, 0));
    cout << canvasImage.rows << endl << canvasImage.cols << endl;
    cout << img.rows << endl << img.cols << endl;
    int imgRows = img.rows;
    int imgCols = img.cols;
    int tempR = img.rows;
    int tempC = img.cols;
    float scaler = 0.99f;

    //check if image can fit canvas
    if (imgRows > canvasImage.rows || imgCols > canvasImage.cols)
    {
        //keep scaling the rows and col down to fit the canvas 
        while (tempR > canvasImage.rows || tempC > canvasImage.cols)
        {
          
            tempR = tempR * scaler; //scaling down the rows
            tempC = tempC * scaler; //scaling down the cols
        }
        Rect roi(0, 0, tempC, tempR);
        Size s = canvasImage(roi).size();
        Mat target_ROI(s, CV_8UC3);

        img.copyTo(target_ROI);
        cv::resize(target_ROI, target_ROI, s);
        target_ROI.convertTo(target_ROI, canvasImage.type());
        target_ROI.copyTo(canvasImage(roi));
    }
    else
    {
        Rect roi(0, 0, img.cols, img.rows);
        Size s = canvasImage(roi).size();
        Mat target_ROI(s, CV_8UC3);
        img.copyTo(target_ROI);
        cv::resize(target_ROI, target_ROI, s);
        target_ROI.convertTo(target_ROI, canvasImage.type());
        target_ROI.copyTo(canvasImage(roi));
    }
       
    return canvasImage;
}

Mat metadata(Mat& img, string fname, string fpath)
{
    string rows = to_string(img.rows);
    string cols = to_string(img.cols);
    string aspectRatio = (string)rows + " X " + cols;
    string size = to_string(img.dims);
    putText(img, aspectRatio, Point2f(10, 30), FONT_HERSHEY_COMPLEX, 0.75, Scalar(0, 0, 255, 3), 1);
    putText(img,size, Point2f(10, 70), FONT_HERSHEY_COMPLEX, 0.75, Scalar(73, 0, 255, 3), 1);
    putText(img, fname, Point2f(10, 110), FONT_HERSHEY_COMPLEX, 0.75, Scalar(73, 0, 255, 3), 1);
    putText(img, fpath, Point2f(10, 150), FONT_HERSHEY_COMPLEX, .4, Scalar(73, 0, 255, 3), 1);
    cout << fname << endl << fpath << endl << aspectRatio << endl << size;
    return img;


}

//scales the image based upon the dimensions of src image.
//This function isn't really needed
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

// Function to list files obtained from a directory path.
void listFiles(const char* path, vector<cv::Mat>& images, vector<String>& fn, vector<String>& fp)
{
    
    char fileName[255];
    char directoryPath[255];
    strncpy(directoryPath, path, 254);
    string str(directoryPath);
    string fullPath = str;
    vector<cv::String> fnames; // vector to return to calling function.
    struct dirent* dp;
    DIR* dir = opendir(path);
    cout << "in listFiles path -> " << fullPath << endl;
    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        cout << "we're in the directory " << endl;
        // Check to determine whether the current directory pointer is a 
        // directory or a file.
        if (dp->d_type == DT_DIR) {
            // Char array conversion for comparison purposes.
            strncpy(fileName, dp->d_name, 254);
            string str(fileName);
            string oneDotDir = ".";
            string twoDotDir = "..";
            // Check to make sure directory doesn't point to a parent directory
            // or to the current directory.
            if (str != oneDotDir && str != twoDotDir) {
                // Directory Path is a valid subdirectory.
                // Explore this "node" directory in DFS algorithm manner.
                string temp_fn = fullPath + "/" + str;
                char newPath[255];
                strcpy(newPath, temp_fn.c_str());
                listFiles(newPath, images,fn,fp);
            }
        }
        else {
            
            // Directory pointer was not a directory but instead a file.
            // Char array conversion for comparison purposes.
            strncpy(fileName, dp->d_name, 254);
            string str(fileName);
            // Check to see if the file has the proper extension for image media.
            if (Check_ext(str)) {
                string temp_fn = fullPath + "/" + str;
                fp.push_back(temp_fn);
                fn.push_back(str);
                cout << temp_fn << endl;
                // File passes the check and is added to the image vector.
                images.push_back(imread(temp_fn));
            }
        }

    }

    // Close directory stream
    closedir(dir); 
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

int main(int argc, char* argv[])
{
    // Variable declaration for the root directory path and image vector.
    char path[100];
    vector<Mat> images;
    vector<String> fileNames;
    vector<String> filePaths;
    string sr, sc;
    int r, c;

    // Conditional statements to determine the meaning of entered program args.
    if (argc < 2 || argc > 4 || argc == 3) {
        std::cout << "Too many/few arguments.";
        return 0;
    }

    if (argc == 2) {

        strcpy(path, argv[1]);
        // listFiles takes a directory path as a char array as well as a vector
        // of type cv::Mat and returns the vector full of images from directories.
        listFiles(path, images, fileNames, filePaths);

        int index = 0;

        while (true)
        {
            //Make sure we're actually displaying an image.
            if (images[index].cols > 0 && images[index].rows > 0)
            {
                int key = waitKey();
                cout << key << endl;
                Mat metaImg = metadata(images[index], fileNames[index], filePaths[index]);
                imshow("Browser", metaImg);
                if (key == 27) // For ESC
                {
                    cout << "exiting program" << endl;
                    break;
                }
                else if (key == 110) {
                    if (index != images.size() - 1) {
                        index++;
                        imshow("Browser", metaImg);
                        cout << "N index -> " << index << endl;

                    }
                }
                else if (key == 112) {
                    if (index > 0)
                    {
                        index--;
                        imshow("Browser", metaImg);
                        cout << "P index -> " << index << endl;

                    }

                }
                
            }
        }

        return 0;
    }
    if (argc == 4) {
        strcpy(path, argv[1]);
        r = stoi(argv[2]);
        c = stoi(argv[3]);
        int index = 0;

        // listFiles takes a directory path as a char array as well as a vector
        // of type cv::Mat and returns the vector full of images from directories.
        cout << "listingFiles" << endl;
        listFiles(path, images, fileNames, filePaths);
        while (true)
        {
            //Make sure we're actually displaying an image.
            if (images[index].cols > 0 && images[index].rows > 0)
            {
                int key = waitKey();
                cout << key << endl;
                Mat metaImg = metadata(images[index], fileNames[index], filePaths[index]);
                vector<Mat> temp;
                temp.push_back(metadata(metaImg, fileNames[index], filePaths[index]));
                imshow("Browser", makeCanvas(temp, c, r));
                if (key == 27) // For ESC
                {
                    cout << "exiting program" << endl;
                    break;
                }
                else if (key == 110) {
                    if (index != images.size() - 1) {
                        index++;
                        imshow("Browser", makeCanvas(temp, c, r));
                        cout << "N index -> " << index << endl;
                    }
                }
                else if (key == 112) {
                    if (index > 0)
                    {
                        index--;
                        imshow("Browser", makeCanvas(temp, c, r));
                        cout << "P index -> " << index << endl;

                    }
                }
            }

        }
        return 0;
    }

}

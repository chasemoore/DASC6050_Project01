# DASC6050_DisplayBrowser
Instructions to run the program.
Be sure if you’re in a UNIX / LINUX operating system you comment out the #include opencv2/dirent.h and uncomment the #include <dirent.h>.
If you’re on Windows you must get dirent.h and put it in the openCV directory for it to work.

#Requirements
-your path must not contain a space.
-your path must contain atleast one image.

You use the following arguments:
./NameOfFile {path_to_root_dir} -> This will display the program without a resized canvas.
./NameOfFile {path_to_root_dir} {num cols} {num rows} -> This will display the program with a resizes canvas.

if you use too many or too few arguments the program wouldn't run.

The metadata for the image will be grilled into the image.

You can navigate through the pictures using the left and right arrow keys.  If you’re ready to exit image browser simply hit ESC key. 

If you have any questions or issues email us.


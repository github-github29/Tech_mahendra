#include <cv.h>
#include <highgui.h>

#include <iostream>

int main(int argc, char* argv[])
{
    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened())
    {
        std::cout << "!!! Failed to open file: " << argv[1] << std::endl;
        return -1;
    }

    cv::Mat frame;
    for(;;)
    {

        if (!cap.read(frame))             
            break;

        cv::imshow("window", frame);

        char key = cvWaitKey(10);
        if (key == 27) // ESC
            break;
    }

    return 0;
}

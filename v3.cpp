#include <cv.h>
#include <highgui.h>

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    // Load input video
    cv::VideoCapture input_cap(argv[1]);
    if (!input_cap.isOpened())
    {
        std::cout << "!!! Input video could not be opened" << std::endl;
        return -1;
    }

    // Setup output video
cout<<"ko";
    cv::VideoWriter output_cap("out.avi", 
                               input_cap.get(CV_CAP_PROP_FOURCC),
                               input_cap.get(CV_CAP_PROP_FPS),
                               cv::Size(input_cap.get(CV_CAP_PROP_FRAME_WIDTH), input_cap.get(CV_CAP_PROP_FRAME_HEIGHT))); 
cout<<"hi";                         
    if (!output_cap.isOpened())
    {
        std::cout << "!!! Output video could not be opened" << std::endl;
        return -1;
    }

    // Loop to read from input and write to output
    cv::Mat frame;
    while (true)
    {       
        if (!input_cap.read(frame))             
            break;

        output_cap.write(frame);
    }

    input_cap.release();
    output_cap.release();

    return 0;
}

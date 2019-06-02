#include <cv.h>
#include <highgui.h>
#include <iostream>


const char* ref_window = "Draw rectangle to select template";
std::vector<cv::Point> rect_points;


void mouse_callback(int event, int x, int y, int flags, void* param)
{
    if (!param)
        return;

    cv::Mat* ref_img = (cv::Mat*) param;

    // Upon LMB click, store the X,Y coordinates to define a rectangle.
    // Later this info is used to set a ROI in the reference image.
    switch (event)
    {
        case CV_EVENT_LBUTTONDOWN:
        {
            if (rect_points.size() == 0)
                rect_points.push_back(cv::Point(x, y));
        }
        break;

        case CV_EVENT_LBUTTONUP:
        {
            if (rect_points.size() == 1)
                rect_points.push_back(cv::Point(x, y));
        }
        break;

        default:
        break;
    }

    if (rect_points.size() == 2)
    {
        cv::rectangle(*ref_img, 
                      rect_points[0], 
                      rect_points[1], 
                      cv::Scalar(0, 255, 0),
                      2);

        cv::imshow(ref_window, *ref_img);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <image>" << std::endl;
        return -1;
    }

    cv::Mat source = cv::imread(argv[1]);   // original image
    if (source.empty())
    {
        std::cout << "!!! Failed to load source image." << std::endl;
        return -1;
    }

    // For testing purposes, our template image will be a copy of the original.
    // Later we will present it in a window to the user, and he will select a region 
    // as a template, and then we'll try to match that to the original image.

    cv::Mat reference = source.clone(); 

    cv::namedWindow(ref_window, CV_WINDOW_AUTOSIZE);
    cv::setMouseCallback(ref_window, mouse_callback, (void*)&reference);

    cv::imshow(ref_window, reference);
    cv::waitKey(0);

    if (rect_points.size() != 2)
    {
        std::cout << "!!! Oops! You forgot to draw a rectangle." << std::endl;
        return -1;
    }

    // Create a cv::Rect with the dimensions of the selected area in the image
    cv::Rect template_roi = cv::boundingRect(rect_points);

    // Create THE TEMPLATE image using the ROI from the rectangle
    cv::Mat template_img = cv::Mat(source, template_roi);

    // Create the result matrix
    int result_cols =  source.cols - template_img.cols + 1;
    int result_rows = source.rows - template_img.rows + 1;
    cv::Mat result;

    // Do the matching and normalize
    cv::matchTemplate(source, template_img, result, CV_TM_CCORR_NORMED);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    /// Localizing the best match with minMaxLoc
    double min_val = 0, max_val = 0; 
    cv::Point min_loc, max_loc, match_loc;
    int match_method = CV_TM_CCORR_NORMED;
    cv::minMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc, cv::Mat());

    // When using CV_TM_CCORR_NORMED, max_loc holds the point with maximum 
    // correlation.
    match_loc = max_loc; 

    // Draw a rectangle in the area that was matched
    cv:rectangle(source, 
                 match_loc, 
                 cv::Point(match_loc.x + template_img.cols , match_loc.y + template_img.rows), 
                 cv::Scalar(255, 0, 0), 2, 8, 0 );

    imshow("Template Match:", source);
    cv::waitKey(0);

    return 0;
}

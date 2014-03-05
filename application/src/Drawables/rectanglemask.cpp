#include "rectanglemask.hpp"

RectangleMask::RectangleMask()
{
    finished = false;
}

RectangleMask::~RectangleMask()
{
}

// Click
void RectangleMask::press(cv::Point pi1){
    p[0] = pi1;
    p[1] = pi1;
}

void RectangleMask::release(cv::Point pi2){
    p[1] = pi2;
    finished = true;
}

// Draw
void RectangleMask::draw(cv::Mat& frame){
    if(!isVisible())
        return;

    cv::rectangle(frame, p[0], p[1], cv::Scalar( 0, 0, 255));
}

void RectangleMask::move(cv::Point point){
    p[1] = point;
}

bool RectangleMask::isDone(){
    return finished;
}

std::string RectangleMask::getDescription(){
    return std::string("mask");
}

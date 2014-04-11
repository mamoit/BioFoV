#include "Frame.hpp"

/*******************************************************************************
 * Constructors
 ******************************************************************************/
Frame::Frame(){
	vid = NULL;
}

Frame::Frame(Video *source_vid, cv::Mat shot, std::string path){
    // use image address as filename to ensure there are no clashes
    const void * address = static_cast<const void*>(this);
    std::stringstream ss;
    ss << address;
    filename = path + ss.str() + IMG_EXT;

    vid = source_vid;
    frameNumber = vid->getFramePos();

    cv::imwrite( filename, shot );

    activeDrawable = NULL;
}

/*******************************************************************************
 * Destructor
 ******************************************************************************/
Frame::~Frame(){
    remove(filename.c_str());
}

/*******************************************************************************
 * Functions
 ******************************************************************************/
void Frame::setImage(cv::Mat shot){
    imwrite( filename, shot);
}

cv::Mat Frame::getImage(){
    cv::Mat image = cv::imread(filename);
    return image;
}

unsigned int Frame::getNumber(){
    return frameNumber;
}


bool Frame::check_cap(){
    return true;
}

bool Frame::setFramePos(double /*frameNum - unused*/){
    return true;
}

double Frame::getFramePos(){
    return 0;
}

bool Frame::getFrame(cv::Mat &frame) {
    frame = getImage();
    applyDrawables(frame);
    return true;
}

bool Frame::getPrevFrame(cv::Mat &frame){
    getFrame(frame);
    return true;
}

bool Frame::getNextFrame(cv::Mat &frame){
    getFrame(frame);
    return true;
}

Frame *Frame::getCurrentFrameRef(){
    return this;
}

double Frame::getFrameInt(){
    return 0;
}

double Frame::getFPS(){
    return 0;
}

double Frame::getLengthTime(){
    return 0;
}

double Frame::getLengthFrames(){
    return 1;
}

uint Frame::getCurrentFrameNumber(){
    return frameNumber;
}

void Frame::mousePressEvent(cv::Point point){
    if (activeDrawable != NULL)
        activeDrawable->press(point);
}

void Frame::mouseReleaseEvent(cv::Point point){
    if (activeDrawable != NULL){
        activeDrawable->release(point);
        if (activeDrawable->isDone()){
            activeDrawable = NULL;
            updateValues();
        }
    }
}

void Frame::mouseMoveEvent(cv::Point point){
    if (activeDrawable != NULL)
        activeDrawable->move(point);
}

void Frame::save(std::string fname){
    cv::Mat buff;
    getFrame(buff);
    cv::imwrite( fname + ".png", buff);
}

TStrDoubleMap Frame::getValues(){
    TStrDoubleMap ret;
    TStrDoubleMap temp;
    for (std::deque<Drawable*>::iterator iter = drawers.begin();
         iter != drawers.end();
         iter ++) {
        if((*iter)->isDone()){
            temp = (*iter)->getValues();
            for (TStrDoubleMap::iterator iter2 = temp.begin();
                 iter2 != temp.end();
                 iter2 ++){
                ret.push_back(*iter2);
            }
        }
    }
    return ret;
}

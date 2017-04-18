#pragma once

#include "ofxThreadedImageLoader.h"
#include "ofMain.h"

class ofxVideoBufferPlayer
{
public:
    ofxVideoBufferPlayer();
    ofxVideoBufferPlayer(const ofxVideoBufferPlayer& player);
    virtual ~ofxVideoBufferPlayer();
    void setup(int fps);
    bool load(string foldername);
    void update();
    void draw(int x, int y, int w,int h);
    bool isLoaded();
    void setSpeed(float speed);
    void setVideoFrameRate(float fps);
    void setLoopInterval(float start, float end);
    unsigned int getTotalFrames();
    ofPixels & getPixels();

    void loadingFinished(bool &e);

    float width;
    float height;
    unsigned int totalFrames;
    unsigned int framesLoaded;

protected:

//        ofGstVideoPlayer video;
//        vector<ofTexture*>	frames;

    ofxThreadedImageLoader loader;
    vector<ofImage> frames;


    float playPosition;
    float startFrame;
    float endFrame;
    float loopSize;
    float startLoop;
    float endLoop;
    int frameRate;
    float currentFrame;
    float desiredFrameRate;
    float speed;

    bool bLoaded;
    int videoPlayerWidth;
    int videoPlayerHeight;

private:
    inline void frameCalculations();


};

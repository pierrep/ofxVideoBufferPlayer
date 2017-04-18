#include "ofxVideoBufferPlayer.h"

ofxVideoBufferPlayer::ofxVideoBufferPlayer()
{
    /* init variables*/
    frameRate = 30;
    currentFrame = 0;
    startFrame = 0;
    desiredFrameRate = 30.0f;
    playPosition = 0;
    speed = 0.0f;
    loopSize = 0;
    startLoop = 0;
    endLoop = 0;

    framesLoaded = 0;
    bLoaded = false;
}

ofxVideoBufferPlayer::ofxVideoBufferPlayer(const ofxVideoBufferPlayer& player)
{
    /* init variables*/
    frameRate = player.frameRate;
    currentFrame = player.currentFrame;
    startFrame = player.startFrame;
    desiredFrameRate = player.desiredFrameRate;
    playPosition = player.playPosition;
    speed = player.speed;
    loopSize = player.loopSize;

    framesLoaded = player.framesLoaded;
    bLoaded = player.bLoaded;
}

//--------------------------------------------------------------
ofxVideoBufferPlayer::~ofxVideoBufferPlayer()
{
    loader.stopThread();
    currentFrame = 0;
    //ofRemoveListener(loader.loaderEmpty, this, &ofxVideoBufferPlayer::loadingFinished);
}


//--------------------------------------------------------------
void ofxVideoBufferPlayer::setup(int fps)
{
    frameRate = fps;
    //ofAddListener(loader.loaderEmpty, this, &ofxVideoBufferPlayer::loadingFinished);
}

//--------------------------------------------------------------
void ofxVideoBufferPlayer::loadingFinished(bool& e)
{
    framesLoaded++;
    cout << "framesLoaded = " << framesLoaded << endl;

    if(framesLoaded == totalFrames)
    {
        cout << "Loading finished" << endl;
        loopSize = totalFrames;
        bLoaded = true;
        loader.stopThread();

        for(unsigned int i = 0; i < totalFrames; ++i)
        {

        }
    }
}

//--------------------------------------------------------------
bool ofxVideoBufferPlayer::load(string _folder)
{
    ofDirectory folder;
    folder.allowExt("jpg");
    folder.listDir(_folder);
    folder.sort();
    totalFrames = folder.size();

    if(!folder.exists())
    {
        ofLogError("ofxVideoBufferPlayer") << "Error couldn't load movie, folder doesn't exist";
        return false;
    }

    if(totalFrames == 0)
    {
        ofLogError("ofxVideoBufferPlayer") << "Error couldn't load movie, 0 files found";
        return false;
    }

    ofLogNotice() << "Loading " << totalFrames << " frames" << endl;
    frames.resize(totalFrames);
    for(unsigned int i = 0; i < totalFrames; ++i)
    {
        loader.loadFromDisk(frames[i], folder.getPath(i));
    }

    folder.close();
    bLoaded = true;
    loopSize = totalFrames-1;
    endLoop = loopSize;
    return true;
}

//--------------------------------------------------------------
bool ofxVideoBufferPlayer::isLoaded()
{
    return bLoaded;

}

void ofxVideoBufferPlayer::setLoopInterval(float start, float end)
{
    startLoop = start;
    endLoop = end;
}

//--------------------------------------------------------------
void ofxVideoBufferPlayer::update()
{
    if(!bLoaded) return;

    frameCalculations();

    //cout << "current frame = " << currentFrame << endl;
}

//--------------------------------------------------------------
void ofxVideoBufferPlayer::draw(int x,int y,int w, int h)
{
    if(!bLoaded) return;


    ofSetColor(255,255,255);
//    currentFrame += 1.0f;
//    if (currentFrame > (totalFrames -1)) currentFrame = 0;
//    int f = currentFrame;

    if(currentFrame < (totalFrames - 1))
    {
        frames[(int)currentFrame].draw( x, y, w, h );
    }

}

//--------------------------------------------------------------
ofPixels & ofxVideoBufferPlayer::getPixels()
{

    return frames[(int)currentFrame].getPixels();
}

//--------------------------------------------------------------
inline void ofxVideoBufferPlayer::frameCalculations()
{
    currentFrame += (desiredFrameRate / frameRate) * speed ;

    if(speed > 0.0f)
    {
        if(currentFrame >= startLoop) { //update looping start
            startFrame = startLoop;
            loopSize = endLoop - startLoop;
        }

        endFrame=startFrame+loopSize;

        if(endFrame > (totalFrames-1) )
        {
            if(currentFrame > (totalFrames-1) )
            {
                //current Frame wraps back around to the begining by subtracting the total number of frames
                //currentFrame=currentFrame-totalFrames-1;
                currentFrame=currentFrame-loopSize;
            }
        }
        else
        {
            if( currentFrame > endFrame )
            {
                currentFrame = startFrame;
            } //if( currentFrame < startFrame){
            else if( currentFrame < startFrame)
            {
                currentFrame = startFrame;
            }
        }
        currentFrame = ofClamp( currentFrame, 0, totalFrames-1);
    }
    return;
}

//--------------------------------------------------------------
unsigned int ofxVideoBufferPlayer::getTotalFrames()
{
    return frames.size();
}

//--------------------------------------------------------------
void ofxVideoBufferPlayer::setSpeed(float _speed)
{
    speed = _speed;
}

//--------------------------------------------------------------
void ofxVideoBufferPlayer::setVideoFrameRate(float _fps)
{
    frameRate = _fps;
}

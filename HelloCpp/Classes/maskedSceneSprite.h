

#ifndef __HelloCpp__maskedSceneSprite__
#define __HelloCpp__maskedSceneSprite__

#include <iostream>
#include "cocos2d.h"
#include "GLProgramWithMyUnifos.h"
using namespace cocos2d;
class CmaskedSceneSprite:public CCSprite
{
public:
    CmaskedSceneSprite(){
        CCSize winSize=CCDirector::sharedDirector()->getWinSize();
        c=CCPoint(winSize.width/2,winSize.height/2);
        maskBaseBrightness=0.5;
        
    }
    virtual ~CmaskedSceneSprite(){
    
    }
    void draw();
    bool init(CCTexture2D*texture,string maskTexFileName);
    CGLProgramWithMyUnifos program;
    CCPoint c;
    CCTexture2D*maskTex;
    float maskBaseBrightness;
    
    
};
#endif /* defined(__HelloCpp__maskedSceneSprite__) */

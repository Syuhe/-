//
//  ClippingNodeTest.h
//  Box2dTest
//
//  Created by WF on 16/3/18.
//
//

#ifndef __Box2dTest__ClippingNodeTest__
#define __Box2dTest__ClippingNodeTest__

#include "cocos2d.h"
using namespace cocos2d;

class ClippingTest:public Layer
{
public:
    static Scene * scene();
    bool init();
    CREATE_FUNC(ClippingTest);
    void menuCloseCallBack(Ref * sender);
    bool onTouchBegan(Touch * touch,Event * event);
};

#endif /* defined(__Box2dTest__ClippingNodeTest__) */

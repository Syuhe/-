//
//  ClippingNodeTest.cpp
//  Box2dTest
//
//  Created by WF on 16/3/18.
//      ClippingNode
//（1）首先它是一个节点，继承于Node，所以它可以像普通节点一样放入Layer，Scene，Node中。
//（2）作为节点，它就可以用作容器，承载其他节点和精灵。我把它叫底板。
//（3）如果想要对一个节点进行裁剪，那需要给出裁剪的部分，这个裁剪区域，我把它叫模版。
//所以ClippingNode裁剪节点在组成上=底板+模版，而在显示上=底板-模版。不知道这样解释会不会好理解一点。

#include "ClippingNodeTest.h"

Scene * ClippingTest::scene()
{
    auto scene = Scene::create();
    auto layer = ClippingTest::create();
    scene->addChild(layer);
    
    return scene;
}
bool ClippingTest::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png");
    closeItem->setCallback(CC_CALLBACK_1(ClippingTest::menuCloseCallBack, this));
    closeItem->setPosition(Vec2(160, 400));
    closeItem->setTag(99);
    
    auto myItem = MenuItemImage::create("BoxA.png","BoxB.png");
    myItem->setCallback(CC_CALLBACK_1(ClippingTest::menuCloseCallBack, this));
    myItem->setPosition(Vec2(500, 400));
    myItem->setTag(999);
    
    auto menu = Menu::create(closeItem,myItem,NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setTag(99);
    this->addChild(menu,2);
    
    // 添加 ClippingNode
    auto clip = ClippingNode::create();//设置裁剪节点
    clip->setInverted(true);//设置底板可见
    clip->setAlphaThreshold(0.0f);//设置透明度Alpha值为 0
    this->addChild(clip,10);
    
    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 150));
    clip->addChild(layerColor,1);//在裁剪节点上添加一个灰色透明层
    
    //创建模板，也就是我们要在裁剪节点上挖出来的那个洞的形状，这里我们用close的图标作为模板
    auto nodeT = Node::create();//创建模板
    auto close = Sprite::create("CloseSelected.png");//
    nodeT->addChild(close);
    nodeT->setPosition(Vec2(160, 400));//与我们关闭按钮的位置一样
    clip->setStencil(nodeT);//给裁剪节点设置模板
    
    /***
        这个时候，场景中的close按钮是高亮的，点击close按钮也是可以响应的，
     但是假设我界面上有许多按钮，它们也都是可以响应，如果我只想让玩家点击close
     按钮，其他按钮不能用，那该怎么做？方法很简单，就是添加一个覆盖整个屏幕的空白按钮
     ***/
    
    auto whiteItem = MenuItem::create();
    whiteItem->setPosition(visibleSize.width/2, visibleSize.height/2);
    whiteItem->setContentSize(visibleSize);//设置大小为整个屏幕大小
    
    auto whiteMenu = Menu::create(whiteItem,NULL);
    whiteMenu->setPosition(Vec2::ZERO);
    whiteMenu->setAnchorPoint(Vec2::ZERO);
    this->addChild(whiteMenu,100);
    
    /*这样点击按钮就无法响应了，因为在按钮上还覆盖着一个霸道的空白按钮。可是...
     现在连close 按钮都点击不了。看来只能出大招了，把触摸监听事件扯出来。我在场
     景的最上方添加一个layer（注意是最上方，如果是在空白按钮的下面，那么就触摸
     不到layer了），当玩家手指触摸到屏幕，我通过判断触摸的位置判断是否在close 
     按钮上，如果是的话，手动的响应close 按钮。
    */
    auto listen_layer = Layer::create();
    this->addChild(listen_layer,200);//zOrder为200，这样能在最上方
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(ClippingTest::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, listen_layer);
    
    return true;
}
bool ClippingTest::onTouchBegan(Touch * touch,Event * event)
{
    auto touchP = touch->getLocation();
    auto rect = Rect(160-20, 400-20, 40, 40);//设置框大小处于close按钮
    if(rect.containsPoint(touchP))
    {
        auto menu = (Menu*)this->getChildByTag(99);
        auto closeItem = (MenuItemImage*)menu->getChildByTag(99);
        closeItem->activate();
    }
    
    return true;
}
void ClippingTest::menuCloseCallBack(Ref * sender)
{
    auto menuItem = (MenuItemImage*)sender;
    if (menuItem->getTag() == 99)
    {
        auto lab = Label::createWithTTF("next", "fonts/arial.ttf", 30);
        lab->setPosition(Vec2(160, 300));
        this->addChild(lab,2);
    }
    else if (menuItem->getTag() == 999)
    {
        auto lab = Label::createWithTTF("Box", "fonts/arial.ttf", 30);
        lab->setPosition(Vec2(500, 300));
        this->addChild(lab,2);
    }
}








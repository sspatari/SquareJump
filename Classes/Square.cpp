#include "Square.h"
#include "Definitions.h"

USING_NS_CC;

Square::Square(cocos2d::Layer *layer, Square::ScreenSide screenSide):
        screenSide(screenSide), squareSprite(nullptr), state(State::RIGHT) {  // square appear on right side of bar

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    std::string str = "flippingSquare/flippingSquare";

    squareSprite = Sprite::createWithSpriteFrameName(str + "1.png");

    setInitPosition(layer);

    layer->addChild(squareSprite);
    
}

void Square::setInitPosition(cocos2d::Layer *layer) {
    const auto lineSprite = layer->getChildByName("line");
    if(screenSide == ScreenSide::LEFT) {
        squareSprite->setPosition(Point(
                lineSprite->getPositionX() - (lineSprite->getContentSize().width / 2) -
                        (squareSprite->getContentSize().width / 2),
                origin.y + visibleSize.height / 2));
    }else if(screenSide == ScreenSide::RIGHT) {
        squareSprite->setPosition(Point(
                origin.x + visibleSize.width - (squareSprite->getContentSize().width / 2),
                origin.y + visibleSize.height / 2));
    }
}

void Square::switchSide() {
    if(state!=State::SWITCHING_SIDES) { //if switching do nothing
        const int coefSide = (state == State::LEFT) ? 1 : -1;
        const State afterActionState = (state == State::LEFT) ? State::RIGHT : State::LEFT;
        state = State::SWITCHING_SIDES;
        const auto switchSideCallback = CallFunc::create([=]() {    // = captures object by reference if it exists
                                                                    // and all automatic variables used in body of lambda by copy
            state = afterActionState;
        });

        const Node *parentNode = squareSprite->getParent();
        squareSprite->stopAllActions();
        squareSprite->runAction(Sequence::create(
                Spawn::create(
                        RotateBy::create(ROTATE_ANIMATION_TIME, coefSide * ROTATION_NUMBER * 360),
                        MoveBy::create(
                                MOVING_ANIMATION_TIME,
                                Point(
                                        coefSide * (visibleSize.width / 2 -
                                                parentNode->getChildByName("line")->getContentSize().width / 2 -
                                                squareSprite->getContentSize().width),
                                        0)),
                        nullptr),
                switchSideCallback,
                nullptr));
    }
}
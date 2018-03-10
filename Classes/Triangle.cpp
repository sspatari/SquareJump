#include "Triangle.h"
#include "MyBodyParser.h"
#include "Definitions.h"

USING_NS_CC;

int Triangle::counter = 0;

Triangle::Triangle(cocos2d::Layer *layer, Triangle::ScreenSide screenSide, Triangle::Side side):
        screenSide(screenSide), side(side), used(true), triangleSprite(nullptr) {

    id = counter++;
//    CCLOG("Triangle Created %d", id);
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    triangleSprite = Sprite::createWithSpriteFrameName("triangle.png");

    MyBodyParser::getInstance()->parseJsonFile("triangle.json");

    //    auto triangleBody = PhysicsBody::createBox(triangleSprite->getContentSize());
    auto triangleBody = MyBodyParser::getInstance()->bodyFormJson(triangleSprite, "triangle", PHYSICSBODY_MATERIAL_DEFAULT);
    triangleBody->setCollisionBitmask(TRIANGLE_COLLISION_BITMASK);
    triangleBody->setContactTestBitmask(true);
    triangleBody->setDynamic(false);
    triangleSprite->setPhysicsBody(triangleBody);

    setInitPosition(layer);

    layer->addChild(triangleSprite);
}

void Triangle::setInitPosition(const cocos2d::Layer *layer) {
    const auto lineSprite = layer->getChildByName("line");
    const auto rotation = -180.0f;
    if(screenSide == ScreenSide::LEFT) {
        if(side == Side::LEFT) {
            triangleSprite->setPosition(Point(
                    origin.x + triangleSprite->getContentSize().width / 2,
                    visibleSize.height + origin.y + triangleSprite->getContentSize().height / 2));
        }else if(side == Side::RIGHT) {
            triangleSprite->setRotation(rotation);
            triangleSprite->setPosition(Point(
                    lineSprite->getPositionX() - (lineSprite->getContentSize().width / 2) -
                    (triangleSprite->getContentSize().width / 2),
                    visibleSize.height + origin.y + triangleSprite->getContentSize().height / 2));
        }
    }else if(screenSide == ScreenSide::RIGHT) {
        if(side == Side::LEFT) {
            triangleSprite->setPosition(Point(
                    lineSprite->getPositionX() + (lineSprite->getContentSize().width / 2) +
                    (triangleSprite->getContentSize().width / 2),
                    visibleSize.height + origin.y + triangleSprite->getContentSize().height / 2));
        }else if(side == Side::RIGHT) {
            triangleSprite->setRotation(rotation);
            triangleSprite->setPosition(Point(
                    origin.x + visibleSize.width - (triangleSprite->getContentSize().width / 2),
                    visibleSize.height + origin.y + triangleSprite->getContentSize().height / 2));
        }
    }
}

void Triangle::moveDown(float dt) {

    if((triangleSprite->getPositionY() + (triangleSprite->getContentSize().height / 2)) > origin.y) {
        triangleSprite->setPositionY(triangleSprite->getPositionY() -
                                    (TRIANGLE_SPEED * visibleSize.height * dt));
    }else {
        triangleSprite->removeFromParent();
        used = false;
    }
}

cocos2d::Sprite *Triangle::getSprite() const {
    return triangleSprite;
}


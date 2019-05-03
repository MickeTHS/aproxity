/*
 Copyright (c) 2014 Mudafar
 GPLv3
 */

#include <string>

#include "NativeCaller.h"
#include "cocos2d.h"

bool NativeCaller::isAdShowing = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>

//const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";
const char* AppActivityClassName = "com/thinkinghatstudios/aproxity/app/AppActivity";

void NativeCaller::shareLinkFacebook() {
    CCLOG("shareLinkFacebook() called");
    
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "facebookShareLink", "()V")) {
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void NativeCaller::hideAd()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = false;
	}
}

void NativeCaller::showAd()
{

	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}

}

void NativeCaller::showMainAd()
{

	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showMainAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}

}

void NativeCaller::loadNewMainAd()
{

	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "loadNewMainAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}

}

std::string NativeCaller::getDeviceHash() {
    cocos2d::JniMethodInfo t;
    if(cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "getDeviceHash", "()Ljava/lang/String;")) {
        jstring ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        const char *js = t.env->GetStringUTFChars(ret, NULL);
        std::string cs(js);
        t.env->ReleaseStringUTFChars(ret, js);
        return cs;
    }
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "AppController.h"

void NativeCaller::shareLinkFacebook() {
    [(AppController*)[[UIApplication sharedApplication] delegate] sendFacebookShare];

}

void NativeCaller::hideAd() {
	CCLOG("hideAd() called");
	isAdShowing = false;
	return; //nothing
}

void NativeCaller::showAd() {
	CCLOG("showAd() called");
	isAdShowing = true;
    
    NSLog(@"NativeCaller::showAd() NSLog");
    [(AppController*)[[UIApplication sharedApplication] delegate] showInterstitialAd];

    
	return; //nothing;

}

void NativeCaller::loadNewMainAd() {
	CCLOG("loadNewMainAd() called");
    NSLog(@"NativeCaller::loadNewMainAd() NSLog");

	return; //nothing;

}

void NativeCaller::showMainAd() {
	CCLOG("showMainAd() called");
    NSLog(@"NativeCaller::showMainAd() NSLog");
    [(AppController*)[[UIApplication sharedApplication] delegate] showInterstitialAd];

	return; //nothing;

}

std::string NativeCaller::getDeviceHash() {
    NSString* str = [(AppController*)[[UIApplication sharedApplication] delegate] getDeviceId];

    CCLOG("getDeviceHash() called");
    return std::string([str UTF8String]);
}

#else

void NativeCaller::shareLinkFacebook() {
    CCLOG("shareLinkFacebook() called");
}

void NativeCaller::hideAd() {
    CCLOG("hideAd() called");
    isAdShowing = false;
    return; //nothing
}

void NativeCaller::showAd() {
    CCLOG("showAd() called");
    isAdShowing = true;
    return; //nothing;
    
}

void NativeCaller::loadNewMainAd() {
    CCLOG("loadNewMainAd() called");
    return; //nothing;
    
}

void NativeCaller::showMainAd() {
    CCLOG("showMainAd() called");
    return; //nothing;
    
}

static std::string NativeCaller::getDeviceHash() {
    CCLOG("getDeviceHash() called");
    return std:string("TESTTEST"); //TEST
}

#endif

#pragma once

#include <string>

class NativeCaller {
public:
	static void hideAd();
	static void showAd();
	static void showMainAd();
	static void loadNewMainAd();
	static bool isAdShowing;
	static std::string getDeviceHash();
    
    static void shareLinkFacebook();

};

#ifndef __STATISTICS_REPORTER_H
#define __STATISTICS_REPORTER_H

#include "cocos2d.h"
//#include "cocos-ext.h"
#include "ResultScene.h"
#include "network/HttpClient.h"

//USING_NS_CC;

using namespace cocos2d;
//using namespace cocos2d::extension;

struct player {
	std::string device_hash;
	std::string gender;
	std::string date_of_birth;
};

typedef std::function<void()> hiscore_callback_func;
typedef std::function<void(long, long)> report_game_callback_func;

class StatisticsReporter : public Ref {

public:
	hiscore_callback_func hiscore_callback;
	report_game_callback_func report_game_callback;

	static StatisticsReporter* get_instance();

	void report_statistics(const char* actual, const char* answer);
	void report_game(player& p, std::vector<lvl*>& game_result);
	void get_hiscore(player& p);
	void parse_hiscore(GraphLine* gl_hs, GraphLine* gl_10th, GraphLine* gl_90th);
	bool has_hiscore();
private:
	static StatisticsReporter* _instance;

	StatisticsReporter();

	bool has_hiscore_flag = false;
	char last_hiscore_response[512];
	char last_hiscore_response2[512];

	std::string buildGameJson(player& p, std::vector<lvl*>& game_result);
	void send_report_game_request(const char* request_data, const char* url);
	void hiscore_request(const char* url);
	void hiscore_response_handler(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void report_game_response_handler(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif

#include "StatisticsReporter.h"

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"
#include "ResultScene.h"
#include "GraphLine.h"

USING_NS_CC;

using namespace cocos2d::extension;

StatisticsReporter* StatisticsReporter::_instance = NULL;

StatisticsReporter* StatisticsReporter::get_instance() {
	if (_instance == NULL) {
		_instance = new StatisticsReporter();
	}

	return _instance;
}

StatisticsReporter::StatisticsReporter() {}

void StatisticsReporter::report_statistics(const char* actual,
		const char* answer) {
	cocos2d::network::HttpRequest* request =
			new cocos2d::network::HttpRequest();
	request->setUrl("https://service.thinkinghatstudios.com:10001/statistics");
	                    
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	//request->setResponseCallback(CC_CALLBACK_2(HttpClientTest::onHttpRequestCompleted, this));

	// write the post data
	std::string postDataString;
	postDataString += "{ \"actual\" : \"" + std::string(actual) + "\", \"answer\" : \"" + std::string(answer) + "\" }";
	//const char* postData = postDataString.c_str();
	char cc[256];
	sprintf(cc, "{ \"actual\" : \"%s\", \"answer\" : \"%s\" }", actual, answer);
	CCLOG("Sending statistics: %s", cc);

	std::vector<std::string> headers;
	headers.push_back("Content-type: application/json");
	request->setHeaders(headers);

	request->setRequestData(cc, strlen(cc));
	CCLOG("Request data: %s", request->getRequestData());

	request->setTag("POST test1");
	std::string certPath("service.thinkinghatstudios.com.cer");
	cocos2d::network::HttpClient* client = cocos2d::network::HttpClient::getInstance();
	client->setSSLVerification(certPath);
	client->send(request);
	request->release();
    
    CCLOG("request done");
}

void StatisticsReporter::report_game(player& p, std::vector<lvl*>& game_result) {
    std::string json = buildGameJson(p, game_result);

	CCLOG("Request data: %s", json.c_str());
	send_report_game_request(json.c_str(), "https://service.thinkinghatstudios.com:10001/statistics/reportGame");
}

void StatisticsReporter::get_hiscore(player& p) {
	std::string request_uri;
	request_uri += "https://service.thinkinghatstudios.com:10001/statistics/hiScoreCsv/" + p.device_hash + "/" + p.gender + "/" + p.date_of_birth;
	hiscore_request(request_uri.c_str());
}

std::string StatisticsReporter::buildGameJson(player& p, std::vector<lvl*>& game_result) {
	std::string json_str;
	char cc[200];
	json_str += "{ \"player\" : { \"deviceHash\" : \"" + p.device_hash + "\", \"gender\" : \"" + p.gender + "\", \"dateOfBirth\" : \"" + p.date_of_birth + "\" },";
	json_str += "\"game\" : { \"resultLine\" : [ ";
	for(int i = 0; i < game_result.size(); ++i) {
		sprintf(cc, "{ \"ratio\" : \"%.2f\", \"averageResponseTime\" : \"%llu\", \"numberOfQuestions\" : \"%d\", \"numberOfCorrectAnswers\" : \"%d\"",
				game_result[i]->ratio, game_result[i]->average_resp_time_ms, game_result[i]->num_total, game_result[i]->num_correct);
		json_str += std::string(cc);
		if(i == game_result.size() - 1) {
			json_str += "}";
		} else {
			json_str += "},";
		}
	}
	json_str += "]}}";

	return json_str;
}

bool StatisticsReporter::has_hiscore() {
	return this->has_hiscore_flag;
}

void StatisticsReporter::send_report_game_request(const char* request_data, const char* url) {
	cocos2d::network::HttpRequest* request =
			new cocos2d::network::HttpRequest();
	request->setUrl(url);

	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(StatisticsReporter::report_game_response_handler, this));
	//request->setResponseCallback(CC_CALLBACK_2(HttpClientTest::onHttpRequestCompleted, this));

	std::vector<std::string> headers;
	headers.push_back("Content-type: application/json");
	request->setHeaders(headers);

	request->setRequestData(request_data, strlen(request_data));
	//CCLOG("Request data: %s", request->getRequestData());
	CCLOG("Sending HTTP POST request");

	request->setTag("POST generic_request");
	cocos2d::network::HttpClient* client = cocos2d::network::HttpClient::getInstance();
	std::string certPath("service.thinkinghatstudios.com.cer");
	client->setSSLVerification(certPath);
	client->send(request);
	request->release();
}

void StatisticsReporter::hiscore_request(const char* url) {
	cocos2d::network::HttpRequest* request =
			new cocos2d::network::HttpRequest();
	request->setUrl(url);

	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(StatisticsReporter::hiscore_response_handler, this));
	//request->setResponseCallback(CC_CALLBACK_2(call_back, this));

	std::vector<std::string> headers;
	headers.push_back("Content-type: application/json");
	request->setHeaders(headers);

//	request->setRequestData(request_data, strlen(request_data));
	//CCLOG("Request data: %s", request->getRequestData());
	CCLOG("Sending HTTP GET request");

	request->setTag("GET generic_request");
	cocos2d::network::HttpClient* client = cocos2d::network::HttpClient::getInstance();
	std::string certPath("service.thinkinghatstudios.com.cer");
	client->setSSLVerification(certPath);
	client->send(request);
	request->release();
}

void StatisticsReporter::hiscore_response_handler(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
    if (response && response->getResponseCode() == 200 && response->getResponseData()) {
        std::vector<char> *data = response->getResponseData();
        std::string ret(&(data->front()), data->size());
        CCLOG("%s", ("Response message: " + ret).c_str());

        //---------
//        std::string str1("first");
//        std::string str2("second");

//        strcpy(this->last_hiscore_response2, str1.c_str());
//        CCLOG("first: %s\n", this->last_hiscore_response2);

//        strcpy(this->last_hiscore_response2, str2.c_str());
//        CCLOG("second: %s\n", this->last_hiscore_response2);
        //--------------

        //strcpy(this->last_hiscore_response, ret.c_str());

        memset(this->last_hiscore_response, '\0', sizeof(char)*512);
        strncpy(&this->last_hiscore_response[0], ret.c_str(), ret.length());

        GraphLine* gl_10th = new GraphLine();
        GraphLine* gl_90th = new GraphLine();

        //this->parse_hiscore(gl_10th, gl_90th);
        CCLOG("Setting has_hiscore_flag = true");
        this->has_hiscore_flag = true;
    }
    else {
        //CCLOG("%s", ("Error " + std::to_string(response->getResponseCode()) + " in request").c_str()); //error
    	CCLOG("ERROR in hiscore_response_handler");
    }

    CCLOG("Checking and calling hiscore_callback");
    if(hiscore_callback != NULL) {
    	hiscore_callback();
    } else {
    	CCLOG("hiscore_callback is NULL");
    }
}

void StatisticsReporter::report_game_response_handler(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
	CCLOG("StatisticsReporter::report_game_response_handler called");
    long game_score = 0;
    long game_rank = 0;
	if (response && response->getResponseCode() == 200 && response->getResponseData()) {
	        std::vector<char> *data = response->getResponseData();
	        std::string ret(&(data->front()), data->size());
	        CCLOG("%s", ("Response message: " + ret).c_str());

	        char score_rank[64];
	        memset(score_rank, '\0', sizeof(char)*64);
	        strncpy(&score_rank[0], ret.c_str(), ret.length());

	        int l = strlen(score_rank);
	        int ix = 32; // start of csv data

	        int tracker;

	    	int i;
	    	for(i = ix; i < l; ++i) {			// Search string
	    		if(score_rank[i] == ';') {		// Find all ;
	    			tracker = i;				// Mark position
	    			score_rank[i] = 0;			// Replace with null
	    			break;						// Break (there is only one ;)
	    		}
	    	}

	    	game_score = atol(score_rank + ix);
	    	game_rank = atol(score_rank + tracker + 1);

	    	//CCLOG("Score: %d", game_score);
	    	//CCLOG("Rank : %d", game_rank);

	}
    else {
    	CCLOG("ERROR in report_game_response_handler");
    }


	CCLOG("Checking and calling report_game_callback");
    if(report_game_callback != NULL) {
    	report_game_callback(game_score, game_rank);
    } else {
    	CCLOG("report_game_callback is NULL");
    }
}

void StatisticsReporter::parse_hiscore(GraphLine* gl_hs, GraphLine* gl_10th, GraphLine* gl_90th) {
	CCLOG("Parsing hiscore response data");
	//const char* str = this->last_hiscore_response.c_str();
	int l = strlen(this->last_hiscore_response);
	int ix = 26; // start of csv data
	int count = 0;
	int tracker[100];
	char csv[512];

	CCLOG("Begin parsing...");
	strcpy(csv, this->last_hiscore_response);

	int i;
	for(i = ix; i < l; ++i) {			// Search string
		if(csv[i] == ';') {				// Find all ;
			tracker[count++] = i;		// Mark position
			csv[i] = 0;					// Replace with null
		}
	}

	CCLOG("%d, %d, %d, %d", i, l, tracker[0], tracker[1]);

	int average_response_time = atoi(csv + 26);
	int total_no_of_questions = atoi(csv + tracker[0] + 1);
	int no_correct_answers = atoi(csv + tracker[1] + 1);
	float hit_rate = atof(csv + tracker[2] + 1);
	int hiscore = atoi(csv + tracker[23] + 1);
	// Next 20 are the hiscore values
	// After that 1 is score of hiscore game

	CCLOG("Average response time: %d", average_response_time);
	CCLOG("Total no of questions: %d", total_no_of_questions);
	CCLOG("No of correct answers: %d", no_correct_answers);
	CCLOG("Hit rate: %f", hit_rate);

	CCLOG("First point (%f, %f)", atof(csv + tracker[3] + 1), atof(csv + tracker[4] + 1));
	CCLOG("Second point (%f, %f)", atof(csv + tracker[5] + 1), atof(csv + tracker[6] + 1));


/*    gl_90th->addPoint(1.0f, 50);
    gl_90th->addPoint(1.06f, 72);
    gl_90th->addPoint(1.13f, 80);
    gl_90th->addPoint(1.16f, 85);
    
    gl_10th->addPoint(1.0f, 50);
    gl_10th->addPoint(1.125f, 60);
    gl_10th->addPoint(1.25f, 68);
    gl_10th->addPoint(1.50f, 80);
*/

	CCLOG("First point (%f, %f)", atof(csv + tracker[3] + 1), atof(csv + tracker[4] + 1));

	gl_hs->addPoint(atof(csv + tracker[3] + 1), atoi(csv + tracker[4] + 1));
	gl_hs->addPoint(atof(csv + tracker[5] + 1), atoi(csv + tracker[6] + 1));
	gl_hs->addPoint(atof(csv + tracker[7] + 1), atoi(csv + tracker[8] + 1));
	gl_hs->addPoint(atof(csv + tracker[9] + 1), atoi(csv + tracker[10] + 1));
	gl_hs->addPoint(atof(csv + tracker[11] + 1), atoi(csv + tracker[12] + 1));
	gl_hs->addPoint(atof(csv + tracker[13] + 1), atoi(csv + tracker[14] + 1));
	gl_hs->addPoint(atof(csv + tracker[15] + 1), atoi(csv + tracker[16] + 1));
	gl_hs->addPoint(atof(csv + tracker[17] + 1), atoi(csv + tracker[18] + 1));
	gl_hs->addPoint(atof(csv + tracker[19] + 1), atoi(csv + tracker[20] + 1));
	gl_hs->addPoint(atof(csv + tracker[21] + 1), atoi(csv + tracker[22] + 1));
    
    gl_10th->addPoint(atof(csv + tracker[24] + 1), atoi(csv + tracker[25] + 1));
    gl_10th->addPoint(atof(csv + tracker[26] + 1), atoi(csv + tracker[27] + 1));
    gl_10th->addPoint(atof(csv + tracker[28] + 1), atoi(csv + tracker[29] + 1));
    gl_10th->addPoint(atof(csv + tracker[30] + 1), atoi(csv + tracker[31] + 1));
    gl_10th->addPoint(atof(csv + tracker[32] + 1), atoi(csv + tracker[33] + 1));
    gl_10th->addPoint(atof(csv + tracker[34] + 1), atoi(csv + tracker[35] + 1));
    gl_10th->addPoint(atof(csv + tracker[36] + 1), atoi(csv + tracker[37] + 1));
    gl_10th->addPoint(atof(csv + tracker[38] + 1), atoi(csv + tracker[39] + 1));
    gl_10th->addPoint(atof(csv + tracker[40] + 1), atoi(csv + tracker[41] + 1));
    gl_10th->addPoint(atof(csv + tracker[42] + 1), atoi(csv + tracker[43] + 1));

    gl_90th->addPoint(atof(csv + tracker[44] + 1), atoi(csv + tracker[45] + 1));
    gl_90th->addPoint(atof(csv + tracker[46] + 1), atoi(csv + tracker[47] + 1));
    gl_90th->addPoint(atof(csv + tracker[48] + 1), atoi(csv + tracker[49] + 1));
    gl_90th->addPoint(atof(csv + tracker[50] + 1), atoi(csv + tracker[51] + 1));
    gl_90th->addPoint(atof(csv + tracker[52] + 1), atoi(csv + tracker[53] + 1));
    gl_90th->addPoint(atof(csv + tracker[54] + 1), atoi(csv + tracker[55] + 1));
    gl_90th->addPoint(atof(csv + tracker[56] + 1), atoi(csv + tracker[57] + 1));
    gl_90th->addPoint(atof(csv + tracker[58] + 1), atoi(csv + tracker[59] + 1));
    gl_90th->addPoint(atof(csv + tracker[60] + 1), atoi(csv + tracker[61] + 1));
    gl_90th->addPoint(atof(csv + tracker[62] + 1), atoi(csv + tracker[63] + 1));

    /*
	// Log the hiscore, 10th percentile and 90th percentile points
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[3] + 1), atoi(csv + tracker[4] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[5] + 1), atoi(csv + tracker[6] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[7] + 1), atoi(csv + tracker[8] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[9] + 1), atoi(csv + tracker[10] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[11] + 1), atoi(csv + tracker[12] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[13] + 1), atoi(csv + tracker[14] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[15] + 1), atoi(csv + tracker[16] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[17] + 1), atoi(csv + tracker[18] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[19] + 1), atoi(csv + tracker[20] + 1));
	CCLOG("hiscore point: (%f, %d)", atof(csv + tracker[21] + 1), atoi(csv + tracker[22] + 1));

    CCLOG("10th point: (%f, %d)", atof(csv + tracker[24] + 1), atoi(csv + tracker[25] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[26] + 1), atoi(csv + tracker[27] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[28] + 1), atoi(csv + tracker[29] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[30] + 1), atoi(csv + tracker[31] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[32] + 1), atoi(csv + tracker[33] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[34] + 1), atoi(csv + tracker[35] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[36] + 1), atoi(csv + tracker[37] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[38] + 1), atoi(csv + tracker[39] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[40] + 1), atoi(csv + tracker[41] + 1));
    CCLOG("10th point: (%f, %d)", atof(csv + tracker[42] + 1), atoi(csv + tracker[43] + 1));

    CCLOG("90th point: (%f, %d)", atof(csv + tracker[44] + 1), atoi(csv + tracker[45] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[46] + 1), atoi(csv + tracker[47] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[48] + 1), atoi(csv + tracker[49] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[50] + 1), atoi(csv + tracker[51] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[52] + 1), atoi(csv + tracker[53] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[54] + 1), atoi(csv + tracker[55] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[56] + 1), atoi(csv + tracker[57] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[58] + 1), atoi(csv + tracker[59] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[60] + 1), atoi(csv + tracker[61] + 1));
    CCLOG("90th point: (%f, %d)", atof(csv + tracker[62] + 1), atoi(csv + tracker[63] + 1)); */

    CCLOG("Done parsing hiscore response data");

}

#include "ResultScene.h"
#include "THDrawSmooth.h"
#include "GraphLine.h"
#include "GameState.h"
#include "THUICoolLabel.h"
#include "StatisticsReporter.h"
#include "NativeCaller.h"


Scene* ResultScene::createScene() {
    CCLOG("ResultScene::createScene");
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ResultScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

ResultScene::ResultScene() {}

ResultScene::~ResultScene() {}

bool ResultScene::init() {
    if ( !LayerColor::initWithColor(Color4B(51, 51, 51, 255)) ) {
        return false;
    }
    
    // Set up StatisticsReporter
    _sr = StatisticsReporter::get_instance();
    _sr->hiscore_callback = CC_CALLBACK_0(ResultScene::hiscore_callback, this);
    _sr->report_game_callback = CC_CALLBACK_2(ResultScene::report_game_callback, this);

    
    CCLOG("ResultScene::init");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto label = Label::createWithTTF("YOUR APROXITY RESULTS", "fonts/FRAMD.TTF", 18);
    label->setPosition(Vec2(origin.x + 8, origin.y + visibleSize.height - label->getContentSize().height - 4));
    label->setTextColor(Color4B::WHITE);
    label->setZOrder(10);
    label->setAnchorPoint(Vec2(0,0));

    float middle = origin.x + visibleSize.width/2;
    float middle_right_width = visibleSize.width / 4;
    float middle_right = middle + visibleSize.width / 4;
    
    auto ts_label = Label::createWithTTF("Total score", "fonts/FRAMD.TTF", 12);
    ts_label->setPosition(Vec2(middle_right, (origin.y + visibleSize.height - label->getContentSize().height) - 120));
    //ts_label->setTextColor(Color4B(81,229,155,255));
    ts_label->setTextColor(Color4B(255,255,255,255));
    ts_label->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(ts_label);

    


    auto node = THDrawSmooth::create();
    node->setZOrder(5);
    
    _graph = LineGraph::create(Color4B(85, 85, 85, 255), "fonts/FRAMD.TTF", 18);
    _graph->retain();
    _graph->setTextureDraw("bg_graph.png");
    Size sprite_content_size = _graph->getTexture()->getContentSize();
    _graph->setContentSize(sprite_content_size);
    _graph->setPosition(origin);
    _graph->createXAxis("ratio", 1.0f, 2.5f);
    //_graph->createYAxis("correct", 40.0f, 100.0f);
    _graph->createYAxis("correct", 0.0f, 100.0f);
    _graph->setAxisPixelOrigin(Vec2(20, 20));
    _graph->setAxisPixelLength(GraphAxisDir::X_AXIS, 100.0f);
    _graph->setAxisPixelLength(GraphAxisDir::Y_AXIS, 100.0f);
    _graph->setTextureDrawHelp(false);
    //_graph->setTextureOrigin(origin);
    _graph->setTextureGraphRect(Vec2(28,80), Vec2(232,231));
    
    Size graph_content_size = _graph->getContentSize();
    
    node->drawFillRect(Vec2(origin.x, origin.y + graph_content_size.height), Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height), Color4B(73, 229, 152, 255));

    // Temp player
    player p;
    p.device_hash = NativeCaller::getDeviceHash();
    //p.gender = std::string("MALE");
    p.gender = GameState::getInstance()->_selected_gender;
    //p.date_of_birth = std::string("2000-01-01");
    p.date_of_birth = GameState::getInstance()->_selected_date_of_birth;
    // Create a StatisicsReporter
//    StatisticsReporter _sr;
    
/*    GraphLine* gl_90th = new GraphLine();
    GraphLine* gl_10th = new GraphLine();
    
    gl_90th->setLineColor(Color4B(81, 126, 171, 255));
    gl_90th->setLineWidth(1.5f);
    
    gl_10th->setLineColor(Color4B(233, 63, 5, 255));
    gl_10th->setLineWidth(1.5f);*/

	_sr->get_hiscore(p);
//	while(!sr.has_hiscore()); // Wait for hiscore response to come in
//    sr.parse_hiscore(gl_10th, gl_90th);
/*
    gl_90th->addPoint(1.0f, 50);
    gl_90th->addPoint(1.06f, 72);
    gl_90th->addPoint(1.13f, 80);
    gl_90th->addPoint(1.16f, 85);
    gl_90th->addPoint(1.20f, 87);
    gl_90th->addPoint(1.25f, 91);
    gl_90th->addPoint(2.50f, 99);
*/
//    _graph->addLine(gl_90th);
    

 /*
    gl_10th->addPoint(1.0f, 50);
    gl_10th->addPoint(1.125f, 60);
    gl_10th->addPoint(1.25f, 68);
    gl_10th->addPoint(1.50f, 80);
    gl_10th->addPoint(1.675f, 82);
    gl_10th->addPoint(1.75f, 87);
    gl_10th->addPoint(2.00f, 90);
    gl_10th->addPoint(2.50f, 99);*/
//    _graph->addLine(gl_10th);
    
    
    GraphLine* gl = new GraphLine();
    
    gl->setLineColor(Color4B(79,167,49,255));
    gl->setLineWidth(1.5f);
    
    std::vector<lvl*> calc;
    
    std::vector<GameLevel*> levels = GameState::getInstance()->_levels;
    
    
    for (auto level : levels) {
        bool ratio_found = false;

        for (auto c: calc) {
            if (c->ratio == level->ratio) {
                ratio_found = true;
                c->num_correct += (level->correct() ? 1 : 0);
                c->num_total++;
                _num_correct += (level->correct() ? 1 : 0);
                _num_total++;
                c->resp_time.push_back(level->time);
                c->resp_time_ms.push_back(level->time_ms);
                
                break;
            }
        }
        
        if (!ratio_found) {
            auto l = lvl::create();
            l->ratio = level->ratio;
            l->num_correct = (level->correct() ? 1 : 0);
            l->num_total = 1;
            _num_correct += (level->correct() ? 1 : 0);
			_num_total++;

            l->resp_time.push_back(level->time);
            l->resp_time_ms.push_back(level->time_ms);
            calc.push_back(l);
        }
    }
    
    std::sort(calc.begin(), calc.end(),
              [](lvl const * a, lvl const * b) -> bool
              { return a->ratio < b->ratio; }
              );
    int i = 0;
    
    for (auto c : calc) {
        int correct = ((float)c->num_correct / (float)c->num_total) * 100.0f;
        if (correct < 40) correct = 40;
        gl->addPoint(c->ratio, correct);
        
        CCLOG("point: %f, %d", c->ratio, correct);
        
        // Calculate average response time
        c->average_resp_time_ms = 0;
        for (auto ms: c->resp_time_ms) {
            c->average_resp_time_ms += ms;
            global_avg_resp += ms;
            i++;
        }
        
        c->average_resp_time_ms = std::floor((c->average_resp_time_ms / c->resp_time_ms.size()) + 0.5f);

    }
    
    global_avg_resp = global_avg_resp / i;

    // Report game to ansa-service
    _sr->report_game(p, calc);
    
    _graph->addLine(gl);
    
    
    addChild(node);
    addChild(label);
    

    return true;
}

void ResultScene::hiscore_callback() {
	CCLOG("ResultScene::hiscore_call_back called!");

	GraphLine* gl_hs = new GraphLine();
    GraphLine* gl_90th = new GraphLine();
    GraphLine* gl_10th = new GraphLine();

    _sr->parse_hiscore(gl_hs, gl_10th, gl_90th);

    gl_hs->setLineColor(Color4B(255, 255, 38, 255));
    gl_hs->setLineWidth(1.5f);

    gl_90th->setLineColor(Color4B(81, 126, 171, 255));
    gl_90th->setLineWidth(1.5f);

    gl_10th->setLineColor(Color4B(233, 63, 5, 255));
    gl_10th->setLineWidth(1.5f);

    _graph->addLine(gl_hs);
    _graph->addLine(gl_90th);
    _graph->addLine(gl_10th);

    _graph->make();
    addChild(_graph);
}

void ResultScene::report_game_callback(long game_score, long game_rank) {
	CCLOG("ResultScene::report_game_callback is called!");


	CCLOG("Score: %d", game_score);
	CCLOG("Rank : %d", game_rank);

	char rank_str[16];
	char score_str[16];
	char avg_resp_str[16];
	char accuracy_str[16];

	sprintf(rank_str, "%d", game_rank);
	sprintf(score_str, "%d", game_score);
	sprintf(avg_resp_str, "%.4fms", global_avg_resp / 1000.0f);
	sprintf(accuracy_str, "%.2f%%", ((float)_num_correct / (float)_num_total) * 100.0f);

	auto blue_label = THUICoolLabel::create("Ranking", std::string(rank_str), "scoreball_blue.png");
	blue_label->setClickCallback(CC_CALLBACK_0(ResultScene::onRankingClicked, this));
	blue_label->setPosition(
		Vec2(middle_right,
			 (origin.y + visibleSize.height - label->getContentSize().height) - 20));
	blue_label->setAnchorPoint(Vec2(0.5, 1));
	addChild(blue_label);

	auto total_score_label = Label::createWithTTF(std::string(score_str), "fonts/AGENCY.TTF", 28);
	total_score_label->setPosition(Vec2(middle_right, (origin.y + visibleSize.height - label->getContentSize().height) - 130));
	total_score_label->setTextColor(Color4B(81,229,155,255));
	//ts_label->setTextColor(Color4B(255,255,255,255));
	total_score_label->setAnchorPoint(Vec2(0.5, 1));
	addChild(total_score_label);


	auto green_label = THUICoolLabel::create("Avg. resp time", std::string(avg_resp_str), "scoreball_green.png");
	green_label->setClickCallback(CC_CALLBACK_0(ResultScene::onAvgRespClicked, this));
	green_label->setPosition(
		Vec2(middle_right - middle_right_width/2 - 10,
			 (origin.y + visibleSize.height - label->getContentSize().height) - 40));
	green_label->setAnchorPoint(Vec2(0.5, 1));
	addChild(green_label);

	auto yellow_label = THUICoolLabel::create("Accuracy", std::string(accuracy_str), "scoreball_yellow.png");
	yellow_label->setClickCallback(CC_CALLBACK_0(ResultScene::onAccuracyClicked, this));
	yellow_label->setPosition(
		Vec2(middle_right + middle_right_width/2 + 10,
			 (origin.y + visibleSize.height - label->getContentSize().height) - 40));
	yellow_label->setAnchorPoint(Vec2(0.5, 1));
	addChild(yellow_label);
	// TODO: Use the values
}


void ResultScene::onRankingClicked() {
    CCLOG("Ranking clicked");
}

void ResultScene::onAccuracyClicked() {
    CCLOG("accuracy clicked");
}

void ResultScene::onAvgRespClicked() {
    CCLOG("avg resp clicked");
}

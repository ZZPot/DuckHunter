#pragma once
#include "../../DuckHunter.h"

#define BG_IMG		"eyes/DH/bg.png"
#define SAFE_IMG	"eyes/DH/safe.png"

class pat_eyes: public hunter_eyes
{
public:
	pat_eyes();
	bool GetContext(game_context* p_context);
protected:
	cv::Mat _bg;
	cv::Mat _safe_mask;
};
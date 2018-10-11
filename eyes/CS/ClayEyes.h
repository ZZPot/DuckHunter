#pragma once
#include "../../DuckHunter.h"

#define BG_IMGC		"eyes/CS/bgc.png"
#define SAFE_IMGC	"eyes/CS/safec.png"

class clay_eyes: public hunter_eyes
{
public:
	clay_eyes();
	cv::Mat GetContext(game_context* p_context);
protected:
	cv::Mat _bg;
	cv::Mat _safe_mask;
};
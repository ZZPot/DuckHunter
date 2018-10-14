#pragma once
#include "../../DuckHunter.h"

#define BG_IMGC		"eyes/CS/bgc.png"
#define SAFE_IMGC	"eyes/CS/safec.png"

class clay_eyes: public hunter_eyes
{
public:
	clay_eyes();
	std::vector<Obj2d> GetObjects(cv::Mat frame);
protected:
	cv::Mat _bg;
	cv::Mat _safe_mask;
};
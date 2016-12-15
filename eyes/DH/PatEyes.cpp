#include "PatEyes.h"
#include <opencv2/imgproc.hpp>

pat_eyes::pat_eyes()
{
	_bg = cv::imread(BG_IMG);
	_safe_mask = cv::imread(SAFE_IMG, cv::IMREAD_GRAYSCALE);
}
bool pat_eyes::GetContext(game_context* p_context)
{
	if(_frame_source.empty())
		return false;
	cv::Mat frame = _frame_source->nextFrame();
	cv::Mat resized_bg;
	cv::resize(_bg, resized_bg, frame.size());
	cv::Mat diff;
	cv::absdiff(resized_bg, frame, diff);
	cv::cvtColor(diff, diff, CV_BGR2GRAY);
	bitwise_or(diff, _safe_mask, diff);
	cv::threshold(diff, diff, 5, 255, cv::THRESH_BINARY);
	cv::imshow("Diff", diff);
	std::vector<type_condition> cond;
	cond.resize(1);
	cond[0].size_ratio[0] = 1; 
	cond[0].size_ratio[1] = 1.5;
	cond[0].rect[0].width = frame.cols * 0.05;
	cond[0].rect[1].width = frame.cols * 0.3;
	cond[0].rect[0].height = frame.rows * 0.05;
	cond[0].rect[1].height = frame.rows * 0.3;
	std::vector<int> check = {FEATURE_CHECK_SIZE | FEATURE_CHECK_SIZE_RATIO};
	p_context->objects = FindObjects(diff, cond, check, cv::RETR_EXTERNAL, 0);
	return true;
}

// dead 36 62
// shot 54 58

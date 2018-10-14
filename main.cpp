#include <opencv2/highgui.hpp>
#include "DuckHunter.h"
#include "eyes/DH/PatEyes.h"
#include "eyes/CS/ClayEyes.h"
#include "ScreenFrames/PartFrames/WindowFrames/WindowFrames.h"
#include "FeatureDetector/Draw/DrawDebug.h"
#include <iostream>


#define CLAY_COLOR cv::Scalar(0, 0, 255)


int main()
{
	cv::Ptr<clay_eyes> ce = cv::makePtr<clay_eyes>();
	int key = 0;
	game_context gc;
	duck_hunter dh;
	dh.SetEyes(ce);
	dh.SetFrameSource(cv::makePtr<window_frames>(true));
	dh.Run();
#pragma region COLORING
	DrawConfig draw_config;
	draw_conf clay_conf;
	clay_conf.stroke_type = STROKE_TYPE::STROKE_RRECT;
	clay_conf.stroke_color = CLAY_COLOR;
	clay_conf.fill_color = CLAY_COLOR;
	clay_conf.caption_color = CLAY_COLOR;
	draw_config.SetTag(TAG_CLAY, &clay_conf);
#pragma endregion
	do
	{
		dh.GetContext(&gc);
		if (!gc.last_frame.empty())
		{
			DrawObjects(gc.last_frame, gc.objects, draw_config);
			cv::imshow("Debug draw", gc.last_frame);
		}
		key = cv::waitKey(TIMER_FREQ);
		
	}while(key != 27);
	return 0;
}
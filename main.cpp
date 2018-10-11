#include <opencv2/highgui.hpp>
#include "DuckHunter.h"
#include "eyes/DH/PatEyes.h"
#include "eyes/CS/ClayEyes.h"
#include "ScreenFrames/PartFrames/WindowFrames/WindowFrames.h"
#include <iostream>

int main()
{
	cv::Ptr<clay_eyes> ce = cv::makePtr<clay_eyes>();
	ce->SetFrameSource(cv::makePtr<window_frames>(true));
	int key = 0;
	game_context gc;
	duck_hunter dh;
	dh.SetEyes(ce);
	dh.Run();
	do
	{
		
		cv::Mat frame = ce->GetContext(&gc);
		for(auto& obj: gc.objects)
		{
			DrawRect(obj.rect, frame, cv::Scalar(255, 255, 255), 3);
		}
		cv::imshow("Objects", frame);
		key = cv::waitKey(20);
		
	}while(key != 27);
	return 0;
}
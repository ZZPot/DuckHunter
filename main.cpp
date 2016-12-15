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
	int key;
	//cv::Ptr<window_frames> second_eyes = cv::makePtr<window_frames>(true);
	//game_context gc;
	duck_hunter dh;
	dh.SetEyes(ce);
	dh.Run();
	/*do
	{
		
		cv::Mat frame = second_eyes->nextFrame();
		ce.GetContext(&gc);
		for(auto& obj: gc.objects)
		{
			DrawRect(obj.rect, frame, cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("Objects", frame);
		
	}while(key != 27);*/
	cv::namedWindow("asdsa");
	Sleep(50000);
	return 0;
}
#include "DuckHunter.h"
#include <iostream>

void hunter_eyes::SetFrameSource(cv::Ptr<part_frames> frame_source)
{
	_frame_source = frame_source;
	// reset?
}
duck_hunter::duck_hunter():
_state(HS_PAUSED)
{
	_pause_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	_run_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD dwThreadId;
	_hunter_thread = BeginThreadEx(NULL, 0, duck_hunter::HunterThreadFunc, this, 0, &dwThreadId);
}
duck_hunter::~duck_hunter()
{
	SetEvent(_stop_event);
	WaitForSingleObject(_hunter_thread, INFINITE);
	CloseHandle(_pause_event);
	CloseHandle(_run_event);
	CloseHandle(_stop_event);
}
void duck_hunter::Run()
{
	if(_state == HS_RUN || _state == HS_UNKNOWN)
		return;
	_state = HS_RUN;
	game_context context;
	if(!_eyes.empty())
		_eyes->GetContext(&context);
	SetEvent(_run_event);
}
void duck_hunter::Pause()
{
	if(_state == HS_PAUSED || _state == HS_UNKNOWN)
		return;
	_state = HS_PAUSED;
	SetEvent(_pause_event);
}
bool duck_hunter::SetEyes(cv::Ptr<hunter_eyes> eyes)
{
	if(_state == HS_RUN)
		return false;
	_eyes = eyes;
	return true;
}
unsigned __stdcall duck_hunter::HunterThreadFunc(void* param)
{
	duck_hunter& hunter = *(duck_hunter*)param;
	HANDLE pause_pair[2] = {hunter._stop_event, hunter._pause_event};
	HANDLE run_pair[2] = {hunter._stop_event, hunter._run_event};
	std::cout << "Waiting for initial start\n";
	DWORD wait_result = WaitForMultipleObjects(2, run_pair, FALSE, INFINITE); // initial run
	if(wait_result == WAIT_OBJECT_0)
		return 0;
	std::cout << "Start\n";
	game_context context;
	do
	{
		wait_result = WaitForMultipleObjects(2, pause_pair, FALSE, TIMER_FREQ);
		switch(wait_result)
		{
		case WAIT_OBJECT_0+1: //called duck_hunter::Pause()
			wait_result = WaitForMultipleObjects(2, run_pair, FALSE, INFINITE); // in-game run
			break;
		case WAIT_TIMEOUT:
			{
				// SH00T!!!!!!!!!!!!
				double time = cv::getTickCount();
				if(hunter._eyes.empty() || !hunter._eyes->GetContext(&context))
					break;
				std::vector<cv::Point> shots;
				for(auto& obj: context.objects)
				{
					if(obj.tag == TAG_DUCK || obj.tag == TAG_CLAY)
					{
						shots.push_back(obj.r_rect.center);
					}
				}
				if(shots.size() > SHOT_PER_FRAME)
					shots.resize(SHOT_PER_FRAME);
				double locate_time = ((double)cv::getTickCount() - time)/cv::getTickFrequency();
				if(locate_time > 0.05)
					std::cout << "Locate time: " << locate_time << std::endl;
				for(auto& point: shots)
				{
					ClickAtPoint(point);
					std::cout << "Shot";
				}
				if(shots.size())
				{
					//std::cout << "\tReloading... ";
					wait_result = WaitForSingleObject(pause_pair[0], RELOAD_TIME);
					//std::cout << "\tReloaded\n";
				}
			}
			break;
		}
	}while(wait_result != WAIT_OBJECT_0 && wait_result != WAIT_FAILED);
	std::cout << "Stopped\n";
	return 0;
}

void ClickAtPoint(cv::Point p, bool rmb)
{
	INPUT mouse_input;
	mouse_input.type = INPUT_MOUSE;
	SetCursorPos(p.x, p.y);
	mouse_input.mi.dx = p.x;
	mouse_input.mi.dy = p.y;
    mouse_input.mi.mouseData = 0;
    mouse_input.mi.dwFlags = rmb ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN;
    mouse_input.mi.time = 0;
	SendInput(1, &mouse_input, sizeof(INPUT));
	Sleep(200);
	mouse_input.mi.dwFlags = rmb ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP;
	SendInput(1, &mouse_input, sizeof(INPUT));
}
void PressKey(int key, int time)
{
	INPUT keyboard_input;
	keyboard_input.type = INPUT_KEYBOARD;
	keyboard_input.ki.wVk = key;
    keyboard_input.ki.time = 0;
	keyboard_input.ki.dwFlags = 0;
	SendInput(1, &keyboard_input, sizeof(INPUT));
	Sleep(time);
	keyboard_input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboard_input, sizeof(INPUT));
}
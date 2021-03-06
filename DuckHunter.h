#pragma once
#include "ScreenFrames/PartFrames/PartFrames.h"
#include "FeatureDetector/FeatureDetector.h"
#include "OpenCV_common.hpp"
#include "common.h"

#include <windows.h>
#include <process.h>
#pragma region WinAPI defines
typedef unsigned (__stdcall *PTHREAD_FUNC)(PVOID);
#define BeginThreadEx(LPSA, SSIZE, PFUNC, pvARG, FLAGS, PTID) \
	(HANDLE)_beginthreadex(	(PVOID)(LPSA), \
					(unsigned)(SSIZE),\
					(PTHREAD_FUNC)(PFUNC),\
					(PVOID)(pvARG),\
					(unsigned)(FLAGS),\
					(unsigned*)(PTID))
#pragma endregion

#define TIMER_FREQ	10
#define RELOAD_TIME	100

#define TAG_UNKNOWN		-1
#define TAG_DUCK		0
#define TAG_DOG			1
#define TAG_DUCK_DOWN	2
#define TAG_CLAY		3
#define TAG_CLAY_SHARD	4
#define CLAY_NAME	"Clay"

#define SHOT_PER_FRAME	1

enum hunter_state
{
	HS_UNKNOWN = 0,
	HS_PAUSED,
	HS_RUN
};
struct game_context
{
	hunter_state hs;
	unsigned ammo;
	unsigned ducks_left;
	unsigned ducks_missed;
	unsigned ducks_shot;
	std::vector<Obj2d> objects;
	std::vector<cv::Point> aimed;
	cv::Mat last_frame;
	bool is_valid;
};

class hunter_eyes
{
public:
	virtual ~hunter_eyes(){}
	virtual std::vector<Obj2d> GetObjects(cv::Mat frame) = 0;
protected:
};

class duck_hunter
{
public:
	duck_hunter();
	virtual ~duck_hunter();
	void Run();
	void Pause();
	bool SetEyes(cv::Ptr<hunter_eyes> eyes);
	virtual int GetContext(game_context* context);
	void SetFrameSource(cv::Ptr<part_frames> frame_source);
protected:
	int LocateTargets();
	int Shoot();
	static unsigned __stdcall HunterThreadFunc(void* param);
protected:
	ts_state<hunter_state> _state;
	HANDLE _pause_event;
	HANDLE _run_event;
	HANDLE _stop_event;
	HANDLE _hunter_thread;
	cv::Ptr<part_frames> _frame_source;
	cv::Ptr<hunter_eyes> _eyes;
	game_context _current_context;
};

void ClickAtPoint(cv::Point p, bool rmb = false, unsigned time = 100);
void PressKey(int key, int time = 200);
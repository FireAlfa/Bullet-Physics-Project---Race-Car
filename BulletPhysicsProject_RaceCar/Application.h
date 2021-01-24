#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	ModuleCamera3D* camera;

private:

	PerfTimer	pTimer;
	Timer dtTimer;
	float frameRateCap;
	float screenTicksCap;
	float fpsCounter = 0;
	float fpsPreUpdate;
	float fpsMSeconds;
	float dt = 0.0f;
	p2List<Module*> list_modules;

public:

	float fps = 0.0f;
	bool debug = false;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
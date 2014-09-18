//
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Flora License, Version 1.1 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://floralicense.org/license/
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FApp.h>
#include <FUi.h>
#include <FSystem.h>

class Calculator
	: public Tizen::App::Application
	, public Tizen::System::IScreenEventListener
{
public:
	// The application must have a factory method that creates an instance of the application.
	static Tizen::App::Application* CreateInstance(void);

public:
	Calculator(void);
	~Calculator(void);

public:
	// This method is called when the application is on initializing.
	virtual bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry);

	// This method is called when the application is on terminating.
	virtual bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool urgentTermination = false);

	// This method is called when the application is brought to the foreground
	virtual void OnForeground(void);

	// This method is called when the application is sent to the background.
	virtual void OnBackground(void);

	// This method is called when the application has little available memory.
	virtual void OnLowMemory(void);

	// This method is called when the device's battery level is changed.
	virtual void OnBatteryLevelChanged(Tizen::System::BatteryLevel batteryLevel);

	// Called when the screen turns on.
	virtual void OnScreenOn(void);

	// Called when the screen turns off.
	virtual void OnScreenOff(void);

};

#endif

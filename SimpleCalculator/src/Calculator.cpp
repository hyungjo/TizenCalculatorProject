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

//Test

//Export Test


#include <new>
#include <FSysPowerManager.h>

#include "Calculator.h"
#include "CalculatorForm.h"

using namespace Tizen::App;
using namespace Tizen::System;
using namespace Tizen::Ui::Controls;

Calculator::Calculator(void)
{
}


Calculator::~Calculator(void)
{
}


Application*
Calculator::CreateInstance(void)
{
	// You can create the instance through another constructor.
	return new (std::nothrow) Calculator();
}


bool
Calculator::OnAppInitializing(AppRegistry& appRegistry)
{
	// Get application frame
	Frame* pFrame = new (std::nothrow) Frame();
	TryReturn(pFrame != null, false, "Unable to get application frame [%s]", GetErrorMessage(GetLastResult()));

	result r = pFrame->Construct();
	TryReturn(!IsFailed(r), false, "Frame::Construct() failed with [%s]", GetErrorMessage(r));

	r = this->AddFrame(*pFrame);
	TryReturn(!IsFailed(r), false, "AddFrame() failed with [%s]", GetErrorMessage(r));

	// Create CalculatorForm, construct and add it to application frame
	CalculatorForm* pCalcForm = new (std::nothrow) CalculatorForm();
	TryReturn(pCalcForm != null, false, "Unable to allocate memory for CalculatorForm");

	r = pCalcForm->Construct();
	TryReturn(!IsFailed(r), false, "CalculatorForm::Construct() failed with [%s]", GetErrorMessage(r));

	// After successful return of AddControl, pCalcForm is owned by application frame.
	r = pFrame->AddControl(pCalcForm);
	TryReturn(!IsFailed(r), false, "Frame::AddControl() failed with [%s]", GetErrorMessage(r));

	// Set CalculatorForm as current form for application
	r = pFrame->SetCurrentForm(pCalcForm);
	TryReturn(!IsFailed(r), false, "Frame::SetCurrentForm() failed with [%s]", GetErrorMessage(r));

	// You should uncomment following statement to listen to the screen on/off events.
	// PowerManager::SetScreenEventListener(*this);

	return true;
}


bool
Calculator::OnAppTerminating(AppRegistry& appRegistry, bool urgentTermination)
{
	return true;
}


void
Calculator::OnForeground(void)
{
}


void
Calculator::OnBackground(void)
{
}


void
Calculator::OnLowMemory(void)
{
}


void
Calculator::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{

}


void
Calculator::OnScreenOn(void)
{
}


void
Calculator::OnScreenOff(void)
{
}

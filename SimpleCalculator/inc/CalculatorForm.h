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

#ifndef _CALCULATOR_FORM_H_
#define _CALCULATOR_FORM_H_

#include <FUiRelativeLayout.h>
#include <FUiGridLayout.h>
#include "Calculator.h"

enum ButtonActionId
{
	BTN_ID_0,
	BTN_ID_1,
	BTN_ID_2,
	BTN_ID_3,
	BTN_ID_4,
	BTN_ID_5,
	BTN_ID_6,
	BTN_ID_7,
	BTN_ID_8,
	BTN_ID_9,
	BTN_ID_DOT,
	BTN_ID_PLUS,
	BTN_ID_MINUS,
	BTN_ID_MULTIPLY,
	BTN_ID_DIVIDE,
	BTN_ID_BACK,
	BTN_ID_CLEAR,
	BTN_ID_EQUAL,
	BTN_ID_COUNT
};

class CalculatorForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
{
public:
	CalculatorForm(void);
	~CalculatorForm(void);

	result Construct(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual result OnDraw(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

private:
	result AddCalculatorPanel(void);
	result AddErrorPopup(void);

private:
	Tizen::Ui::Controls::Label* __pLabelPrint;
	Tizen::Ui::Controls::Popup* __pPopup;

	class CalculatorModel
	{
	public:
		CalculatorModel(void);
		bool HandleNumKeys(int numKey);
		void HandleOperatorKeys(int key);
		void HandleEqualKey(void);
		void HandleBackKey(void);
		void HandleClearKey(void);

		const Tizen::Base::String& GetResult() const;
		bool UpdateOperand(int key);
		void Calculate(void);

	public:
		Tizen::Base::String __errString;

	private:
		enum CalculatorMode
		{
			MODE_OPERAND1,
			MODE_OPERATOR,
			MODE_OPERAND2,
			MODE_RESULT,
			MODE_INVALID,
		};

		CalculatorMode __calcMode;
		int __operator;
		Tizen::Base::String __operand1;
		Tizen::Base::String __operand2;

		static const int MAX_PRINTABLE_CHARS = 8;
		static const int MAX_DISPLAY_BUFFER_SIZE = 32;
		static const int MAX_PRINTABLE_VALUE = 99999999;
	} __calculator;
};

#endif

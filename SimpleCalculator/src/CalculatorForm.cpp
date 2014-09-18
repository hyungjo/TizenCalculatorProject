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

#include <new>
#include <FUiLayout.h>
#include <FLclNumberSymbols.h>
#include <FLocales.h>
#include <cmath>
#include <limits>
#include <clocale>
#include <cerrno>
#include <cwchar>

#include "AppResourceId.h"
#include "CalculatorForm.h"

using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::App;
using namespace Tizen::Locales;

#define MAX_DIGITS_AFTER_DECIMAL_POINT 9
#define MAX_DIGITS_DISPLAY 17
#define DIGITS_AFTER_DECIMAL_POINT 13

static const Dimension POPUP_DIMENSION(400, 200);
static const Rectangle POPUP_LABEL_RECT(0, 0, 350, 75);
static const Rectangle POPUP_BTN_RECT(125, 70, 120, 50);
static const int ACTION_ID_POPUP_BTN_OK = 100;

//////////////////////////////////////////////////////////////////////////
// CalculatorForm

CalculatorForm::CalculatorForm(void)
	: __pLabelPrint(null)
	, __pPopup(null)
{
}


CalculatorForm::~CalculatorForm(void)
{
}


result
CalculatorForm::Construct(void)
{
	result r = Form::Construct(IDF_FORM);
	TryReturn(!IsFailed(r), r, "Form::Construct() failed with [%s]", GetErrorMessage(r));

	Header* pHeader = GetHeader();
	TryReturn(pHeader != null, GetLastResult(), "Unable to get header pointer");

	r = pHeader->SetStyle(HEADER_STYLE_TITLE);
	TryReturn(!IsFailed(r), r, "Header::SetStyle() failed with [%s]", GetErrorMessage(r));

	r = pHeader->SetTitleText("Calculator");
	TryReturn(!IsFailed(r), r, "Header::SetTitleText() failed with [%s]", GetErrorMessage(r));

	return E_SUCCESS;
}


result
CalculatorForm::AddCalculatorPanel(void)
{
	result r = E_SUCCESS;

	const String BUTTON_NAME[] =
	{
		IDC_BTN_0, IDC_BTN_1, IDC_BTN_2, IDC_BTN_3, IDC_BTN_4, IDC_BTN_5, IDC_BTN_6, IDC_BTN_7, IDC_BTN_8, IDC_BTN_9, IDC_BTN_DOT,
		IDC_BTN_PLUS, IDC_BTN_MINUS, IDC_BTN_MULTIPLY, IDC_BTN_DIVIDE, IDC_BTN_BACK, IDC_BTN_CLEAR, IDC_BTN_EQUAL
	};

	Button* pButton = null;

	for (int i = 0; i < BTN_ID_COUNT; i++)
	{
		pButton = static_cast< Button* >(GetControl(BUTTON_NAME[i]));
		TryReturn(pButton != null, r = E_SYSTEM, "Panel::GetControl() failed");

		pButton->SetActionId(i);
		pButton->AddActionEventListener(*this);
	}

	__pLabelPrint = static_cast< Label* >(GetControl(IDC_LBL_DISPLAY));
	TryReturn(__pLabelPrint != null, r = E_SYSTEM, "Panel::GetControl(IDC_LBL_DISPLAY) failed");

	__pLabelPrint->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);

	return r;
}


result
CalculatorForm::AddErrorPopup(void)
{
	result r = E_SUCCESS;
	Label* pLabel = null;
	Button* pBtnOK = null;

	__pPopup = new (std::nothrow) Popup();
	TryReturn(__pPopup != null, E_OUT_OF_MEMORY, "Unable to allocate memory for error pop-up");

	r = __pPopup->Construct(true, POPUP_DIMENSION);
	TryCatch(!IsFailed(r), /* NOP */, "Pop-up::Construct() with [%s]", GetErrorMessage(r));

	pLabel = new (std::nothrow) Label();
	TryCatch(pLabel != null, r = E_OUT_OF_MEMORY, "Unable to allocate memory for error label");

	r = pLabel->Construct(POPUP_LABEL_RECT, L"");
	TryCatch(!IsFailed(r), delete pLabel, "Label::Construct() with [%s]", GetErrorMessage(r));

	pLabel->SetText(L"Can not input more than 8 digits");

	r = __pPopup->AddControl(pLabel);
	TryCatch(!IsFailed(r), delete pLabel, "Pop-up::AddControl() with [%s]", GetErrorMessage(r));

	pBtnOK = new (std::nothrow) Button();
	TryCatch(pBtnOK != null, r = E_OUT_OF_MEMORY, "Unable to allocate memory for ok button");

	r = pBtnOK->Construct(POPUP_BTN_RECT, L"OK");
	TryCatch(!IsFailed(r), delete pBtnOK, "Button::Construct() with [%s]", GetErrorMessage(r));

	pBtnOK->SetActionId(ACTION_ID_POPUP_BTN_OK);
	pBtnOK->AddActionEventListener(*this);

	__pPopup->AddControl(pBtnOK);
	TryCatch(!IsFailed(r), delete pBtnOK, "Pop-up::AddControl() with [%s]", GetErrorMessage(r));

	return E_SUCCESS;

CATCH:
	delete __pPopup;
	__pPopup = null;
	return r;
}


result
CalculatorForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	SetFormBackEventListener(this);

	r = AddCalculatorPanel();
	TryReturn(!IsFailed(r), r, "AddCalculatorPanel() failed with [%s]", GetErrorMessage(r));

	r = AddErrorPopup();
	TryReturn(!IsFailed(r), r, "AddErrorPopup() failed with [%s]", GetErrorMessage(r));

	return r;
}


result
CalculatorForm::OnTerminating(void)
{
	delete __pPopup;
	return E_SUCCESS;
}


result
CalculatorForm::OnDraw(void)
{
	return E_SUCCESS;
}


void
CalculatorForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	switch (actionId)
	{
	case BTN_ID_0:  // fall through
	case BTN_ID_1:  // fall through
	case BTN_ID_2:  // fall through
	case BTN_ID_3:  // fall through
	case BTN_ID_4:  // fall through
	case BTN_ID_5:  // fall through
	case BTN_ID_6:  // fall through
	case BTN_ID_7:  // fall through
	case BTN_ID_8:  // fall through
	case BTN_ID_9:  // fall through
	case BTN_ID_DOT:
		if (__calculator.HandleNumKeys(actionId) == false)
		{
			__pPopup->SetShowState(true);
			__pPopup->Show();
			return;
		}
		break;

	case BTN_ID_PLUS:   // fall through
	case BTN_ID_MINUS:  // fall through
	case BTN_ID_MULTIPLY:   // fall through
	case BTN_ID_DIVIDE:
		__calculator.HandleOperatorKeys(actionId);
		break;

	case BTN_ID_BACK:
		__calculator.HandleBackKey();
		break;

	case BTN_ID_CLEAR:
		__calculator.HandleClearKey();
		break;

	case BTN_ID_EQUAL:
		__calculator.HandleEqualKey();
		break;

	case ACTION_ID_POPUP_BTN_OK:
		__pPopup->SetShowState(false);
		break;
	}

	if ((__calculator.__errString == String("INF")) || (__calculator.__errString == String("-INF")))
	{
		__pLabelPrint->SetText(__calculator.__errString);
		Draw();
		return;
	}

	String displayText = __calculator.GetResult();
	if (displayText.IsEmpty())
	{
		displayText = L"0";
	}

	int indexOfDec = 0;
	displayText.IndexOf(L".", 0, indexOfDec);

	int indexofE = 0;
	displayText.IndexOf(L"e", 0, indexofE);

	int len = displayText.GetLength();
	if(len > MAX_DIGITS_DISPLAY)
	{
		if((len - indexOfDec)  > DIGITS_AFTER_DECIMAL_POINT)
		{
			if(indexofE > 0)
			{
				displayText.Remove((indexOfDec + MAX_DIGITS_AFTER_DECIMAL_POINT), (indexofE - (indexOfDec + MAX_DIGITS_AFTER_DECIMAL_POINT)));
			}
			else
			{
				displayText.Remove(MAX_DIGITS_DISPLAY, (len - MAX_DIGITS_DISPLAY));
			}
		}
	}

	__pLabelPrint->SetText(displayText);

	Draw();
}

void
CalculatorForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}
// Calculator Model
CalculatorForm::CalculatorModel::CalculatorModel(void)
	: __errString()
	, __calcMode(MODE_OPERAND1)
	, __operator(BTN_ID_COUNT)
	, __operand1("0")
	, __operand2()
{
}


const String&
CalculatorForm::CalculatorModel::GetResult(void) const
{
	switch (__calcMode)
	{
	case MODE_OPERATOR:     // fall through
	case MODE_RESULT:       // fall through
	case MODE_OPERAND1:
		return __operand1;

	case MODE_OPERAND2:
		if (__operand2.IsEmpty())
		{
			return __operand1;
		}
		return __operand2;

	case MODE_INVALID:
		return __errString;
	}
	return __errString;
}


bool
CalculatorForm::CalculatorModel::UpdateOperand(int key)
{
	String& op = (__calcMode == MODE_OPERAND1) ? __operand1 : __operand2;
	wchar_t sign;
	int charCount = MAX_PRINTABLE_CHARS;
	int index = -1;

	op.GetCharAt(0,sign);
	if((sign == '-') || (sign == '+'))
	{
		charCount += 1;
	}

	op.IndexOf(L".", 0, index);
	if(index != -1)
	{
		charCount += 1;
	}

	if (op.GetLength() < charCount)
	{
		if (key == BTN_ID_DOT)
		{

			if (IsFailed(op.IndexOf(L".", 0, index)))
			{
				if (op.IsEmpty() || (op == String("-")))
				{
					op.Append("0");
				}
				op.Append(L".");
			}
		}
		else if (key >= BTN_ID_0 && key <= BTN_ID_9)
		{
			if (op == String("0"))
			{
				op.Clear();
			}
			op.Append(key);
		}
		else if (key == BTN_ID_MINUS)
		{
			if (op.IsEmpty())
			{
				op.Append("-");
			}
		}
		return true;
	}
	return false;
}


bool
CalculatorForm::CalculatorModel::HandleNumKeys(int numKey)
{
	switch (__calcMode)
	{
	case MODE_INVALID:
		return true;

	case MODE_RESULT:
		HandleClearKey();
		__calcMode = MODE_OPERAND1;
		break;

	case MODE_OPERATOR:
		__operand2.Clear();
		__calcMode = MODE_OPERAND2;
		break;

	case MODE_OPERAND1:     // fall through
	case MODE_OPERAND2:
		break;
	}
	return UpdateOperand(numKey);
}


void
CalculatorForm::CalculatorModel::HandleOperatorKeys(int key)
{
	switch (__calcMode)
	{
	case MODE_INVALID:
		return;

	case MODE_OPERATOR:
		if ((key == BTN_ID_PLUS) || (key == BTN_ID_MINUS))
		{
			__operand2.Clear();
			UpdateOperand(key);
			__calcMode = MODE_OPERAND2;
			break;
		}

		__operator = key;
		break;

	case MODE_OPERAND1:
		if ((__operand1.IsEmpty()) || (__operand1 == String("0")))
		{
			if ((key == BTN_ID_PLUS) || (key == BTN_ID_MINUS))
			{
				if(__operand1 == String("0"))
				{
					UpdateOperand(key);
				}
				else
				{
					__operand1.Clear();
					UpdateOperand(key);
					break;
				}
			}

			__operand1 = "0";
		}

		__calcMode = MODE_OPERATOR;
		__operator = key;
		break;

	case MODE_OPERAND2:
		HandleEqualKey();
		// fall through

	case MODE_RESULT:
		__calcMode = MODE_OPERAND1;
		__operand2.Clear();
		HandleOperatorKeys(key);
		break;
	}
}


void
CalculatorForm::CalculatorModel::Calculate(void)
{
	if ((__errString == String("INF")) || (__errString == String("-INF")))
	{
		return;
	}

	double op1 = 0;
	double op2 = 0;

	locale_t locale = newlocale(LC_ALL, "C", null);
	uselocale(locale);

	errno = 0;
	op1 = wcstod(__operand1.GetPointer(), null);
	if((errno == ERANGE) || (errno == EINVAL))
	{
		freelocale(locale);
		return;
	}
	op2 = wcstod(__operand2.GetPointer(), null);
	if((errno == ERANGE) || (errno == EINVAL))
	{
		freelocale(locale);
		return;
	}

	switch (__operator)
	{
	case BTN_ID_PLUS:
		op1 += op2;
		break;

	case BTN_ID_MINUS:
		op1 -= op2;
		break;

	case BTN_ID_MULTIPLY:
		op1 *= op2;
		break;

	case BTN_ID_DIVIDE:
		if (op2)
		{
			op1 /= op2;
		}
		else
		{
			__calcMode = MODE_INVALID;
			if (op1 == 0)
			{
				__errString = "NAN";
			}
			else
			{
				__errString = (op1 > 0) ? "INF" : "-INF";
			}
			freelocale(locale);
			return;
		}
		break;
	}

	// As we are using double for storing results, we can get numbers upto 15 digits without any loss of precision
	// after that, result will be shown in scientific notation.
	__operand1.Format(MAX_DISPLAY_BUFFER_SIZE, L"%.15g", op1);

	if (std::fabs(op1 - 0) < std::numeric_limits<double>::epsilon())
	{
		__operand1 = L"0";
	}

	if (__operand1 == String("inf"))
	{
		__errString = "INF";
		__calcMode = MODE_INVALID;
	}
	else if (__operand1 == String("-inf"))
	{
		__errString = "-INF";
		__calcMode = MODE_INVALID;
	}
	else
	{
		__calcMode = MODE_RESULT;
	}
	freelocale(locale);
}


void
CalculatorForm::CalculatorModel::HandleEqualKey(void)
{
	switch (__calcMode)
	{
	case MODE_INVALID:
		return;

	case MODE_OPERATOR:
		__operand2 = __operand1;
		break;

	case MODE_OPERAND1:
		__operand2 = 0;
		__operator = BTN_ID_PLUS;
		break;

	case MODE_OPERAND2:
		if (__operand2.IsEmpty() || (__operand2 == String("-")))
		{
			if ((__operator == BTN_ID_MULTIPLY) || (__operator == BTN_ID_DIVIDE))
			{
				__operand2 = "1";
			}
			else
			{
				__operand2 = "0";
			}
		}
		break;

	case MODE_RESULT:
		break;
	}
	Calculate();
}


void
CalculatorForm::CalculatorModel::HandleBackKey(void)
{
	switch (__calcMode)
	{
	case MODE_RESULT:   // fall through
	case MODE_INVALID:
		return;

	case MODE_OPERATOR:
		__operator = BTN_ID_COUNT;
		__calcMode = MODE_OPERAND1;
		//fall through

	case MODE_OPERAND1:
		if (!__operand1.IsEmpty())
		{
			__operand1.Remove(__operand1.GetLength() - 1, 1);
		}
		break;

	case MODE_OPERAND2:
		if (!__operand2.IsEmpty())
		{
			__operand2.Remove(__operand2.GetLength() - 1, 1);
		}
		else
		{
			__calcMode = MODE_OPERATOR;
			HandleBackKey();
		}
		break;
	}
}


void
CalculatorForm::CalculatorModel::HandleClearKey(void)
{
	__calcMode = MODE_OPERAND1;
	__operator = BTN_ID_COUNT;
	__operand1.Clear();
	__operand2.Clear();
	__errString.Clear();

}

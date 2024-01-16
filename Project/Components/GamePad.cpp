#include "GamePad.h"

GamePad* GamePad::GetInstance() {
	static GamePad instance;

	return &instance;
}

void GamePad::Update() {
	preState_ = state_;
	GetJoystickState(state_);
}

bool GamePad::GetJoystickState(XINPUT_STATE& state) {
	DWORD dwResult = XInputGetState(0, &state);
	if (dwResult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool GamePad::TriggerButton(int GAMEPAD_NUM) {
	if ((state_.Gamepad.wButtons & GAMEPAD_NUM) && !(preState_.Gamepad.wButtons & GAMEPAD_NUM)) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::ReleaseButton(int GAMEPAD_NUM) {
	if (!(state_.Gamepad.wButtons & GAMEPAD_NUM) && (preState_.Gamepad.wButtons & GAMEPAD_NUM)) {
		return true;
	}
	else {
		return false;
	}
}

bool GamePad::PressButton(int GAMEPAD_NUM) {
	if ((state_.Gamepad.wButtons & GAMEPAD_NUM)) {
		return true;
	}
	else {
		return false;
	}
}
#include "engine/input.hpp"

#define KEYS_COUNT 256
#define SPECIAL_KEYS_COUNT 112

InputState::InputState() {
    keysState = new bool[KEYS_COUNT];
    prevKeysState = new bool[KEYS_COUNT];
    specialKeysState = new bool[SPECIAL_KEYS_COUNT];
    prevSpecialKeysState = new bool[SPECIAL_KEYS_COUNT];
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}

bool InputState::keyTapped(int key) {
    return (keysState[key] && !prevKeysState[key]);
}

bool InputState::specialKeyTapped(int specialKey) {
    return (keysState[specialKey] && !prevSpecialKeysState[specialKey]);
}

bool InputState::keyReleased(int key) {
    return (!keysState[key] && prevKeysState[key]);
}

bool InputState::keyHeld(int key) {
    return keysState[key];
}

bool InputState::specialKeyHeld(int specialKey) {
    return specialKeysState[specialKey];
}

int InputState::keyAxisDirection(int keyNeg, int keyPos) {
    return keysState[keyPos] - keysState[keyNeg];
}

int InputState::specialKeyAxisDirection(int keyNeg, int keyPos) {
    return specialKeysState[keyPos] - specialKeysState[keyNeg];
}

int InputState::getMouseDeltaX() {
    return mouseDeltaX;
}

int InputState::getMouseDeltaY() {
    return mouseDeltaY;
}

//State updating
void InputState::pressKey(int key) {
    keysState[key] = true;
}

void InputState::pressSpecialKey(int specialKey) {
    specialKeysState[specialKey] = true;
}

void InputState::releaseKey(int key) {
    keysState[key] = false;
}

void InputState::releaseSpecialKey(int specialKey) {
    specialKeysState[specialKey] = false;
}

void InputState::updateMouseDelta(int mouseX, int mouseY,
                                  int centerX, int centerY) {
    mouseDeltaX = mouseX - centerX;
    mouseDeltaY = mouseY - centerY;
}

void InputState::updatePreviousState() {
    for (int k = 0; k < KEYS_COUNT; k++) {
        prevKeysState[k] = keysState[k];
    }

    for (int k = 0; k < SPECIAL_KEYS_COUNT; k++) {
        prevSpecialKeysState[k] = specialKeysState[k];
    }
}

void InputState::resetMouseDelta() {
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}
#pragma once
#include <cstdlib>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <bitset>
#include <vector>

#define KEYS_COUNT 256
#define SPECIAL_KEYS_COUNT 112

class InputState {
   private:
    std::bitset<KEYS_COUNT> keysState{false};
    std::bitset<KEYS_COUNT> prevKeysState{false};
    std::bitset<SPECIAL_KEYS_COUNT> specialKeysState{false};
    std::bitset<SPECIAL_KEYS_COUNT> prevSpecialKeysState{false};
    int mouseDeltaX = 0;
    int mouseDeltaY = 0;

   public:
    // Constructors
    InputState() = default;

    // Input state getters
    [[nodiscard]] bool keyTapped(int key);
    [[nodiscard]] bool specialKeyTapped(int specialKey);
    [[nodiscard]] bool keyReleased(int key);
    [[nodiscard]] bool specialKeyReleased(int specialKey);
    [[nodiscard]] bool keyHeld(int key);
    [[nodiscard]] bool specialKeyHeld(int specialKey);
    [[nodiscard]] int keyAxisDirection(int keyNeg, int keyPos);
    [[nodiscard]] int specialKeyAxisDirection(int keyNeg, int keyPos);
    [[nodiscard]] int getMouseDeltaX() const;
    [[nodiscard]] int getMouseDeltaY() const;

    // Input state updating
    void pressKey(int key);
    void pressSpecialKey(int specialKey);
    void releaseKey(int key);
    void releaseSpecialKey(int specialKey);
    void updateMouseDelta(int mouseX, int mouseY, int centerX, int centerY);
    void updatePreviousState();
    void resetMouseDelta();
};
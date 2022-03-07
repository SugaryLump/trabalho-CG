#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

class InputState {
    private:
        bool* keysState;
        bool* prevKeysState;
        bool* specialKeysState;
        bool* prevSpecialKeysState;
        int mouseDeltaX;
        int mouseDeltaY;

    public:
        //Constructors
        InputState();

        //Input state getters
        bool keyTapped(int key);
        bool specialKeyTapped(int specialKey);
        bool keyReleased(int key);
        bool specialKeyReleased(int specialKey);
        bool keyHeld(int key);
        bool specialKeyHeld(int specialKey);
        int keyAxisDirection(int keyNeg, int keyPos);
        int specialKeyAxisDirection(int keyNeg, int keyPos);
        int getMouseDeltaX();
        int getMouseDeltaY();
        
        //Input state updating
        void pressKey(int key);
        void pressSpecialKey(int specialKey);
        void releaseKey(int key);
        void releaseSpecialKey(int specialKey);
        void updateMouseDelta(int mouseX, int mouseY, int centerX, int centerY);
        void updatePreviousState();
        void resetMouseDelta();
};
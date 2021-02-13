#pragma once
#include <cstdint>
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

#pragma once
namespace Engine {
    class KeyInput {
    public:
        struct keyPress {
            bool press = false;
        };

        struct keyboardKeys {
            keyPress A;
            keyPress B;
            keyPress C;
            keyPress D;
            keyPress E;
            keyPress F;
            keyPress G;
            keyPress H;
            keyPress I;
            keyPress J;
            keyPress K;
            keyPress L;
            keyPress M;
            keyPress N;
            keyPress O;
            keyPress P;
            keyPress R;
            keyPress S;
            keyPress T;
            keyPress U;
            keyPress V;
            keyPress W;
            keyPress X;
            keyPress Y;
            keyPress Z;
            keyPress add;
            keyPress subtract;
            keyPress zero;
            int8_t modifier;
        };
        
        keyboardKeys keys;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}
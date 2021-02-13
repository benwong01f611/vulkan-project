#include "keyInput.h"
#include "debug.h"

namespace Engine {
    void KeyInput::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        mainProgram* app = reinterpret_cast<mainProgram*> (glfwGetWindowUserPointer(window));
        if (Debug::enableValidationLayers) {
            std::cout << "Key: " << key << "\tScancode: " << scancode << "\tAction: " << action << "\tMods: " << mods << std::endl;
            //std::cout << mipLevels << std::endl;
        }
        app->keyHandler(window, key, scancode, action, mods);
	}
}
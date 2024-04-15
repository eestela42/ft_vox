#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>

#include <interfaces/Interface_Input.hpp>

enum class DrawMode {
    FILL,
    LINE
};

class Window : I_Input
{
    public:
        Window(const char *name, DrawMode drawMode);
        ~Window();

        GLFWwindow *GetWindow() const;
        int ShouldContinue();
		void SetClearColor(u_char r, u_char g, u_char b);
        void Clear();
        void SwapBuffersAndPollEvents();
		void SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) override;
		int GetWidth();
		int GetHeight();
    private:
        GLFWwindow *window;
		const static int defaultWindowWidth = 1540;
		const static int defaultWindowHeight = 840;
		int height;
		int width;
};

#endif
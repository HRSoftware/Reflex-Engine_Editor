#pragma once
#include <memory>
#include <glfw3.h>


struct ImGuiIO;

namespace Reflex {
    namespace Log {
        class Logger;
    }

    class Settings;
    class Scene;
}

namespace Reflex::Window
{
    class Reflex_Window
    {
    public:
        Reflex_Window(int width, int height, char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* window = nullptr);

    Reflex_Window(const Reflex_Window& other) = default;

    Reflex_Window(Reflex_Window&& other) noexcept
        : m_currentScene(std::move(other.m_currentScene)),
          m_currentScreenMode(other.m_currentScreenMode),
          m_GuiIO(std::move(other.m_GuiIO)),
          m_settings(std::move(other.m_settings)),
          m_glfwWindowHandle(other.m_glfwWindowHandle),
          m_glfwMonitor(other.m_glfwMonitor),
          m_logger(std::move(other.m_logger)) {}

    Reflex_Window& operator=(const Reflex_Window& other) = default;

    Reflex_Window& operator=(Reflex_Window&& other) noexcept
    {
        if ( this == &other )
            return *this;
        m_currentScene = std::move(other.m_currentScene);
        m_currentScreenMode = other.m_currentScreenMode;
        m_GuiIO = std::move(other.m_GuiIO);
        m_settings = std::move(other.m_settings);
        m_glfwWindowHandle = other.m_glfwWindowHandle;
        m_glfwMonitor = other.m_glfwMonitor;
        m_logger = std::move(other.m_logger);
        return *this;
    }

        ~Reflex_Window();
        void initUI();
        int shouldClose() const;
        void close() const;
        void toggleFullscreen();
        void setScene(const std::shared_ptr<Reflex::Scene>& scene);

        void setMouseMoveCallback(GLFWcursorposfun func) const;
        void setMouseScrollCallback(GLFWcursorposfun func) const;
    void setWindowFocusCallback(GLFWwindowfocusfun func) const;
    void setMouseButtonCallback(GLFWmousebuttonfun func) const;

        int getKeyState(int key) const;

        bool makeCurrentGLFWContext() const;
        void updateUI() const;
        void swapBuffer() const;
        void pollEvents() const;
        bool hasFocus() const;
        bool hasCameraControl() const;

        void setFocus(bool state);
        void setCameraControl(bool state);

        std::shared_ptr<Reflex::Window::Reflex_Window> getHandle();
        std::shared_ptr<Reflex::Settings> getSettings() const;
        GLFWwindow* getGLFWWindow();

    private:
        bool m_hasCurrentFocus;
        bool m_hasCameraControl;

        std::shared_ptr<Reflex::Scene> m_currentScene;
        enum screenMode
        {
            WINDOWED,
            FULLSCREEN,
            BORDERLESS,
            MINIMISED
        } m_currentScreenMode = WINDOWED;
        std::shared_ptr<ImGuiIO> m_GuiIO;
        std::shared_ptr<Reflex::Settings> m_settings;

        GLFWwindow* m_glfwWindowHandle;
        GLFWmonitor* m_glfwMonitor;

        std::shared_ptr<Reflex::Log::Logger> m_logger;
        
    };


}

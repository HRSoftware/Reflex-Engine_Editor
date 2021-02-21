#include "pch.h"

#include <imgui.h>

#include "Settings.h"
#include "Includes\Reflex_Window.h"

#include <Reflex-Logging.h>

#include "UI/UI.h"


Reflex::Window::Reflex_Window::Reflex_Window(int width, int height, char* title, GLFWmonitor* monitor, GLFWwindow* window)
{
    m_glfwWindowHandle = glfwCreateWindow(width, height, title, monitor, window);
    m_glfwMonitor = monitor;

    m_logger = Reflex::Log::createNewLogger("Window-Log");
    m_logger->writeNotice("Window created");

    makeCurrentGLFWContext();

    m_settings = std::make_shared<Reflex::Settings>();
    m_settings->Title = title;
    m_settings->screenDimensions = { width, height };

    m_logger->writeNotice("OpenGL {0}", glGetString(GL_VERSION));
    glfwSetInputMode(m_glfwWindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_glfwWindowHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_GuiIO = std::make_shared<ImGuiIO>(ImGui::GetIO());
}

Reflex::Window::Reflex_Window::~Reflex_Window()
{
    m_logger->writeNotice("Window closing");

    ImGui::DestroyContext();
    

    m_logger->writeNotice("UIContext Destroyed");

}

void Reflex::Window::Reflex_Window::initUI()
{
    if(m_glfwWindowHandle)
    {
        m_logger->writeNotice("Initialising UI");
        m_GuiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_glfwWindowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 440");
    }
    else
    {
        m_logger->writeCritical("Failed to init UI - No handle to GLFWWindow");
    }
}

int Reflex::Window::Reflex_Window::shouldClose() const
{
    return glfwWindowShouldClose(m_glfwWindowHandle);
}

void Reflex::Window::Reflex_Window::close() const
{
    glfwSetWindowShouldClose(m_glfwWindowHandle, true);
    m_logger->writeNotice("GLWFWindow set to close");
}
void Reflex::Window::Reflex_Window::toggleFullscreen() {}
void Reflex::Window::Reflex_Window::setScene(const std::shared_ptr<Reflex::Scene>& scene)
{
    m_currentScene = scene;
}

void Reflex::Window::Reflex_Window::setMouseMoveCallback(GLFWcursorposfun func) const
{
    glfwSetCursorPosCallback(m_glfwWindowHandle, func);
}
void Reflex::Window::Reflex_Window::setMouseScrollCallback(GLFWcursorposfun func) const
{
    glfwSetScrollCallback(m_glfwWindowHandle, func);
}

void Reflex::Window::Reflex_Window::setWindowFocusCallback(GLFWwindowfocusfun func) const
{
    glfwSetWindowFocusCallback(m_glfwWindowHandle, func);
}
void Reflex::Window::Reflex_Window::setMouseButtonCallback(GLFWmousebuttonfun func) const
{
    glfwSetMouseButtonCallback(m_glfwWindowHandle, func);
}

int Reflex::Window::Reflex_Window::getKeyState(int key) const
{
    return glfwGetKey(m_glfwWindowHandle, key);
}

bool Reflex::Window::Reflex_Window::makeCurrentGLFWContext() const
{

    if(m_glfwWindowHandle == nullptr)
    {
        const char* description;
        int code = glfwGetError(&description);

        m_logger->writeError("Failed to create window - {0}:{1}", code, description);
        return false;
    }


    glfwMakeContextCurrent(m_glfwWindowHandle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        m_logger->writeError("Failed to init OpenGL Context");
        return false;
    }

    m_logger->writeNotice("Successfully initialised OpenGL Context");
    return true;
}

void Reflex::Window::Reflex_Window::updateUI() const
{
    UI::updateUI(m_glfwWindowHandle);
}

void Reflex::Window::Reflex_Window::swapBuffer() const
{
    glfwSwapBuffers(m_glfwWindowHandle); // Flip Buffers and Draw
}

void Reflex::Window::Reflex_Window::pollEvents() const
{
    glfwPollEvents();
}

bool Reflex::Window::Reflex_Window::hasFocus() const
{
    return m_hasCurrentFocus;
}

bool Reflex::Window::Reflex_Window::hasCameraControl() const
{
    m_logger->writeNotice("ImGUI focus: ", ImGui::IsItemFocused() ? "True" : "False");
    return ImGui::IsItemFocused();
}

void Reflex::Window::Reflex_Window::setFocus(bool state)
{
    if(state != m_hasCurrentFocus)
    {
        m_hasCurrentFocus = state;
        if(state)
        {
            glfwFocusWindow(m_glfwWindowHandle);
        }
        m_logger->writeNotice(state ? "Window has gained focus" : "Window has lost focus");
    }
}

void Reflex::Window::Reflex_Window::setCameraControl(bool state)
{
    
    m_logger->writeNotice("ImGUI focus: {0}", ImGui::IsItemFocused() ? "True" : "False");
    m_hasCameraControl = state;
}

std::shared_ptr<Reflex::Window::Reflex_Window> Reflex::Window::Reflex_Window::getHandle()
{
    return std::shared_ptr<Reflex::Window::Reflex_Window>(this);
}

std::shared_ptr<Reflex::Settings> Reflex::Window::Reflex_Window::getSettings() const
{
    return m_settings;
}

GLFWwindow* Reflex::Window::Reflex_Window::getGLFWWindow()
{
    return m_glfwWindowHandle;
}

#include "pch.h"

#include "Reflex-Logging.h"
#include "Reflex-AssetManager.h"
#include "Reflex-Core/Camera.h"
#include "Settings.h"
#include "Globals.h"
#include "Reflex-Renderer/Renderer_OpenGL.h"
#include "Includes/Scene.h"
#include "Includes/Reflex_Window.h"

void error_callback(int error, const char* description)
{
    auto log = spdlog::get("app_Logger");
    if (log) 
    {
        log->error("Error: {0}\n \t-", description);
    }
}

void initGLFW()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

int main()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared <spdlog::sinks::stdout_sink_st>());

    auto appLog = Reflex::Log::createNewLogger("app_Logger");
    appLog->writeNotice("\t Reflex-Engine initialising\n");

    appLog->writeNotice("\t GLFW Initialising...");
    glfwSetErrorCallback(error_callback);
    initGLFW();

    auto _activeCamera = std::make_shared<Reflex::Core::Camera>();

    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //auto videoMode = glfwGetVideoMode(monitor);


    Reflex::Globals::windowGlobals.activeWindow = std::make_shared<Reflex::Window::Reflex_Window>(1920, 1080, (char*) "Relex Engine 1.0");

    Reflex::Globals::windowGlobals.activeWindow->initUI();


    const std::shared_ptr<Reflex::Renderer::Renderer_OpenGL> sceneRender = std::make_shared<Reflex::Renderer::Renderer_OpenGL>();
    Reflex::Scene _newScene;
    const auto assetManager = Reflex::Managers::AssetManagerInstance->getInstance();
    _newScene.initScene(assetManager);
    _newScene.setRenderer(sceneRender);


    _newScene.run();

    appLog->writeNotice("Terminating");
    glfwTerminate();
}


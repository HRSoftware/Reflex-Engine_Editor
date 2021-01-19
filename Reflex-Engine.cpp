#include "pch.h"

#include "Reflex-Logging.h"
#include "Reflex-AssetManager.h"
#include "Reflex-Core/Camera.h"
#include "Settings.h"
#include "Globals.h"
#include "Includes/Scene.h"

void error_callback(int error, const char* description)
{
    auto log = spdlog::get("app_Logger");
    log->error("Error: %s\n", description);
}

int main()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared <spdlog::sinks::stdout_sink_st>());

    Reflex::Log::Logger appLog("app_Logger");
    appLog.writeNotice("\t Reflex-Engine initialising\n");

    appLog.writeNotice("\t GLFW Initialising...");
    glfwSetErrorCallback(error_callback);

    auto _activeCamera = std::make_shared<Reflex::Core::Camera>();
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    std::shared_ptr<Reflex::Settings> settings = Reflex::Settings::getInstance();
    

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    auto videoMode = glfwGetVideoMode(monitor);
    settings->windowHandle = glfwCreateWindow(settings->screenDimensions.first, settings->screenDimensions.second,
        settings->getTitleString(true).c_str(),
        nullptr,
        nullptr);

    Reflex::Globals::Window::_windowHandle = settings->windowHandle;

    if(settings->windowHandle == nullptr)
    {
        const char* description;
        int code = glfwGetError(&description);

        appLog.writeError("Failed to create window - {0}:{1}", code, description);
        return false;
    }


    glfwMakeContextCurrent(settings->windowHandle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        appLog.writeError("Failed to init. OpenGL Context");
        return false;
    }

    appLog.writeNotice("Initialising UI");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(settings->windowHandle, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    appLog.writeNotice("OpenGL {}", glGetString(GL_VERSION));
    glfwSetInputMode(settings->windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(settings->windowHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    Reflex::Scene _newScene;
    auto assetManager = Reflex::Managers::AssetManager::getInstance();
    _newScene.initScene(assetManager);

    //ResourceRegister resourceRegister("Resources/");


    _newScene.run();

    appLog.writeNotice("Terminating");
    glfwTerminate();
}



//assetManage.loadTexture_FromFile("D:/Documents/Programming/Projects/Reflex-EngineOLD/test.yaml");

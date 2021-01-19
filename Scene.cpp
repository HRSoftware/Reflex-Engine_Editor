#include "pch.h"

#include "Reflex-Logging.h"
#include "Includes/Scene.h"

#include "Globals.h"
#include "Settings.h"
#include "UI/UI.h"

void processInput(const double deltaTime)
{
    using Reflex::Enums::Camera_Movement;

    auto settings = Reflex::Settings::getInstance();
    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->toogleLookAround();

    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_W) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_S) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_A) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_D) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_LEFT_ALT) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->toogleLookAround();

    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
        Reflex::Globals::Render::_activeCamera->MovementSpeed = Reflex::Globals::Render::_activeCamera->boostSpeed;

    if ( glfwGetKey(settings->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE )
        Reflex::Globals::Render::_activeCamera->MovementSpeed = Reflex::Globals::Render::_activeCamera->normalSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if ( Reflex::Globals::Window::firstMouse && !Reflex::Globals::Window::UIHasFocus )
    {
        Reflex::Globals::Window::lastX = xpos;
        Reflex::Globals::Window::lastY = ypos;
        Reflex::Globals::Window::firstMouse = false;
    }

    const auto xoffset = xpos - Reflex::Globals::Window::lastX;
    const auto yoffset = Reflex::Globals::Window::lastY - ypos; // reversed since y-coordinates go from bottom to top

    Reflex::Globals::Window::lastX = xpos;
    Reflex::Globals::Window::lastY = ypos;

    Reflex::Globals::Render::_activeCamera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, const double xoffset, const double yoffset)
{
    Reflex::Globals::Render::_activeCamera->ProcessMouseScroll(yoffset);
}

void mouseInput_callback(GLFWwindow* window, int button, int action, int mods) {}

bool Reflex::Scene::initScene(std::shared_ptr<Managers::AssetManager> _manager)
{
    log = std::make_shared<Reflex::Log::Logger>("Scene_Log");

    log->writeNotice("Initialising scene");
    auto settings = Reflex::Settings::getInstance();
    glfwSetCursorPosCallback(settings->windowHandle, mouse_callback);
    glfwSetScrollCallback(settings->windowHandle, scroll_callback);
    glfwSetMouseButtonCallback(settings->windowHandle, mouseInput_callback);

    //m_assetManager = _manager;
    Reflex::Globals::Render::_activeCamera = std::move(std::make_shared<Core::Camera>());

    loadResources();
    //Import::importModel("TestModel", "Resources/Models/House/house_obj.obj");
    //m_skybox.init();
    return true;
}

void Reflex::Scene::loadResources()
{
    m_assetManager->loadAssetsFromYAML("test.yaml");
}

void Reflex::Scene::run()
{
    auto settings = Reflex::Settings::getInstance();
    log->writeNotice("Running scene");

    glEnable(GL_DEPTH_TEST);

    const float near_plane = 0.01f;
    const float far_plane = 10000.f;

    if ( glfwRawMouseMotionSupported() )
        glfwSetInputMode(settings->windowHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    ImGuiIO& io = ImGui::GetIO();

    while ( glfwWindowShouldClose(settings->windowHandle) == false )
    {
        const double currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);
        glViewport(0, 0, 1920, 1080);

        glClearColor(0.25f, 0.25f, 0.5f, 1.0f); // Background Fill Color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UI::updateUI(settings->windowHandle);
        UI::showMainMenu();
        ImGui::ShowMetricsWindow();
        //UI::showTexture(resourceManager._resourceCache->getTexture("sky"));


        ImGui::Render();

        auto proj = glm::perspective(Reflex::Globals::Render::_activeCamera->Zoom,
            (float)1920 / (float)1080,
            near_plane, far_plane);
        glm::mat4 view = Reflex::Globals::Render::_activeCamera->getViewMatrix();

        //Reflex::Rendering::currentRenderValues.update(view, proj);


        //Reflex::Globals::Render::_renderer->renderBatch(_GOVec);

       // m_skybox.Draw(Reflex::Rendering::currentRenderValues);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(settings->windowHandle); // Flip Buffers and Draw
        glfwPollEvents();
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Reflex::Scene::stop()
{
}

void Reflex::Scene::save()
{
}



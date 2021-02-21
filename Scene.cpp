#include "pch.h"

#include "Reflex-Logging.h"
#include "Includes/Scene.h"
#include "Reflex-Renderer/Renderer_OpenGL.h"

#include "Globals.h"
#include "Settings.h"
#include "UI/UI.h"
#include "Reflex-Core/Skybox.h"

void Reflex::Scene::processInput(const double deltaTime)
{
    using Reflex::Enums::Camera_Movement;
    if(!m_window->hasFocus())
    {
        //log->writeNotice("Window does not have focus");
        return;
    }
    if (m_window->getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_window->close();

    if ( m_window->getKeyState(GLFW_KEY_W) == GLFW_PRESS )
        Reflex::Globals::rendererGlobals._activeCamera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if ( m_window->getKeyState(GLFW_KEY_S) == GLFW_PRESS )
        Reflex::Globals::rendererGlobals._activeCamera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if ( m_window->getKeyState(GLFW_KEY_A) == GLFW_PRESS )
        Reflex::Globals::rendererGlobals._activeCamera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if ( m_window->getKeyState(GLFW_KEY_D) == GLFW_PRESS )
        Reflex::Globals::rendererGlobals._activeCamera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

    if ( m_window->getKeyState(GLFW_KEY_LEFT_ALT) == GLFW_PRESS )
        Reflex::Globals::rendererGlobals._activeCamera->toogleLookAround();

    if ( m_window->getKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
        Reflex::Globals::rendererGlobals._activeCamera->MovementSpeed =  Reflex::Globals::rendererGlobals._activeCamera->boostSpeed;

    if ( m_window->getKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE )
        Reflex::Globals::rendererGlobals._activeCamera->MovementSpeed =  Reflex::Globals::rendererGlobals._activeCamera->normalSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if ( Reflex::Globals::windowGlobals.firstMouse && Reflex::Globals::windowGlobals.UIHasFocus )
    {
        Reflex::Globals::windowGlobals.lastX = xpos;
        Reflex::Globals::windowGlobals.lastY = ypos;
        Reflex::Globals::windowGlobals.firstMouse = false;
    }

    const auto xoffset = xpos - Reflex::Globals::windowGlobals.lastX;
    const auto yoffset = Reflex::Globals::windowGlobals.lastY - ypos; // reversed since y-coordinates go from bottom to top

     Reflex::Globals::windowGlobals.lastX = xpos;
     Reflex::Globals::windowGlobals.lastY = ypos;

    Reflex::Globals::rendererGlobals._activeCamera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, const double xoffset, const double yoffset)
{
    Reflex::Globals::rendererGlobals._activeCamera->ProcessMouseScroll(yoffset);
}

void mouseInput_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        Reflex::Globals::rendererGlobals._activeCamera->toogleLookAround();
        Reflex::Globals::rendererGlobals._activeCamera->toggleMovement();
    }
}

void window_focus_callback(GLFWwindow* window, int focused)
{
}


Reflex::Scene::Scene()
{
    m_window = Reflex::Globals::windowGlobals.activeWindow;
    m_sceneRender = std::make_shared<Reflex::Renderer::Renderer_OpenGL>();
}

bool Reflex::Scene::initScene(std::shared_ptr<Managers::AssetManager> _manager)
{
    log = Reflex::Log::createNewLogger("Scene_Log");

    log->writeNotice("Initialising scene");
    m_window->setMouseButtonCallback(mouseInput_callback);
    m_window->setMouseMoveCallback(mouse_callback);
    m_window->setMouseScrollCallback(scroll_callback);
    m_window->setWindowFocusCallback(window_focus_callback);

    m_window->setScene(std::shared_ptr<Scene>(this));

    m_assetManager = _manager;
    Reflex::Globals::rendererGlobals._activeCamera = std::make_shared<Core::Camera>();

    loadResources();
    
    m_skybox = std::make_shared<Reflex::Core::Skybox>(m_assetManager->getMaterial("skyMaterial"));
    return true;
}

void Reflex::Scene::loadResources()
{
    m_assetManager->loadAssetsFromYAML("test.yaml");
}

void Reflex::Scene::run()
{
    log->writeNotice("Running scene");
    glEnable(GL_DEPTH_TEST);

    const float near_plane = 0.01f;
    const float far_plane = 10000.f;

    m_sceneRender->setCamera( Reflex::Globals::rendererGlobals._activeCamera);
    m_sceneRender->setScreenDimensions(m_window->getSettings()->screenDimensions);
    m_sceneRender->setSkybox(m_skybox);

    while ( Reflex::Globals::windowGlobals.activeWindow->shouldClose() == false )
    {
        const double currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);
        glViewport(0, 0, 1920, 1080);

        glClearColor(0.25f, 0.25f, 0.5f, 1.0f); // Background Fill Color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_window->updateUI();
        ImGui::ShowDemoWindow();

        UI::showMainMenu();
        ImGui::ShowMetricsWindow();
        //UI::showTexture(resourceManager._resourceCache->getTexture("sky"));


        ImGui::Render();

        auto proj = glm::perspective( Reflex::Globals::rendererGlobals._activeCamera->Zoom,
            (float)1920 / (float)1080,
            near_plane, far_plane);
        glm::mat4 view =  Reflex::Globals::rendererGlobals._activeCamera->getViewMatrix();

        m_sceneRender->updateCurrentRenderValue(Renderer::CurrentRenderValues({ view, proj }));


       //m_sceneRender->renderBatch(m_GameObjectVec, true);

       m_sceneRender->renderSkybox({view, proj}); 

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window->swapBuffer();
        m_window->pollEvents();
    }
}

void Reflex::Scene::stop()
{
}

void Reflex::Scene::save()
{
}

void Reflex::Scene::setRenderer(const std::shared_ptr<Renderer::Renderer_OpenGL>& renderer)
{
}
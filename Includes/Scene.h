#pragma once
#include <map>
#include <memory>
#include <Reflex-AssetManager.h>
#include <vector>
#include <string>
#include "Reflex-Renderer/Renderer_OpenGL.h"

struct GLFWwindow;
namespace Reflex {
    namespace Core {
        class Camera;
        class GameObject;
        struct Skybox;
    }
    namespace Window
    {
        class Reflex_Window;
    }
    namespace Log
    {
        class Logger;
    }

    class Scene
    {
    public:
        Scene();
        bool initScene(std::shared_ptr<Managers::AssetManager> _manager);
        void loadResources();
        void run();
        void stop();
        void save();
        void setRenderer(const std::shared_ptr<Renderer::Renderer_OpenGL>& renderer);
        void processInput(const double delta);



    private:
        std::map<std::string, std::shared_ptr<Reflex::Core::GameObject>> m_GameObjectMap;
        std::vector<std::shared_ptr<Reflex::Core::GameObject>> m_GameObjectVec;

        //std::shared_ptr<IBaseLight> m_sunLight{};

        double deltaTime = 0.0f;
        double lastFrame = 0.0f;

        std::shared_ptr<Reflex::Log::Logger> log;
        std::shared_ptr<Managers::AssetManager> m_assetManager;
        std::shared_ptr<Reflex::Core::Skybox> m_skybox;
        std::shared_ptr<Reflex::Renderer::Renderer_OpenGL> m_sceneRender;
        std::shared_ptr<Reflex::Window::Reflex_Window> m_window;
    };
}

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
        class Skybox;
    }

    namespace Log
    {
        class Logger;
    }

    class Scene
    {
    public:
        Scene() = default;
        bool initScene(std::shared_ptr<Managers::AssetManager> _manager);
        void loadResources();
        void run();
        void stop();
        void save();
        void setRenderer(const std::shared_ptr<Renderer::Renderer_OpenGL>& renderer);

    private:
        std::map<std::string, std::shared_ptr<Reflex::Core::GameObject>> _GOMap;
        std::vector<std::shared_ptr<Reflex::Core::GameObject>> _GOVec;

        //std::shared_ptr<IBaseLight> m_sunLight{};

        std::shared_ptr<Core::Skybox> m_skybox;

        //std::shared_ptr<Reflex::Managers::ResourceManager> resourceManager{};
        double deltaTime = 0.0f;
        double lastFrame = 0.0f;

        std::shared_ptr<Reflex::Log::Logger> log;
        std::shared_ptr<Managers::AssetManager> m_assetManager;
        std::shared_ptr<Reflex::Core::Skybox> skybox;
        std::shared_ptr<Reflex::Renderer::Renderer_OpenGL> m_sceneRender;



    };
}
//
//static void processInput(double deltaTime);
//
//static void mouse_callback(GLFWwindow*, double xpos, double ypos);
//
//static void scroll_callback(GLFWwindow*, double xoffset, double yoffset);
//
//static void mouseInput_callback(GLFWwindow* window, int button, int action, int mods);

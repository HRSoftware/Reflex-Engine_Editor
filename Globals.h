#pragma once
#include <memory>
#include <Reflex-Core/Camera.h>


#include "Reflex-Core/Material.h"
#include "Reflex-Core/Shader.h"
#include "glfw3.h"
#include "Includes/Reflex_Window.h"
#include "Reflex-Renderer/Renderer_OpenGL.h"

namespace Reflex::Globals
{
    inline struct WindowGlobals
    {
        WindowGlobals();
        std::shared_ptr<Reflex::Window::Reflex_Window> activeWindow;
        float lastX = 0;
        float lastY = 0;
        bool firstMouse = true;
        bool UIHasFocus = false;
        std::shared_ptr<Reflex::Log::Logger> logger;
    } windowGlobals;

    
    inline struct RendererGlobals
    {
        struct Defaults
        {
            std::shared_ptr<Reflex::Core::Material> _defaultMaterial;
            std::shared_ptr<Reflex::Core::Shader> _defaultShader;
        } defaults;

        std::shared_ptr<Reflex::Core::Shader> _activeShader = nullptr;
        std::shared_ptr<Reflex::Core::Material> _activeMaterial = std::make_shared<Reflex::Core::Material>();
        std::shared_ptr<Reflex::Core::Camera> _activeCamera = nullptr;
        //inline std::vector<std::shared_ptr<Reflex::Core::ILighting>> _activeLights;
        std::shared_ptr<Reflex::Log::Logger> logger;
        RendererGlobals();
    } rendererGlobals;



}

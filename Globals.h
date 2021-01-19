#pragma once
#include <memory>
#include <Reflex-Core/Camera.h>


#include "Reflex-Core/Material.h"
#include "Reflex-Core/Shader.h"
#include "glfw3.h"

namespace Reflex::Globals{
    namespace Window {
        static GLFWwindow* _windowHandle;
        inline float lastX = 0;
        inline float lastY = 0;
        inline bool firstMouse = true;
        inline bool UIHasFocus = false;
    };

    namespace Render {

        namespace Defaults
        {
            inline std::shared_ptr<Reflex::Core::Material> _defaultMaterial;
            inline std::shared_ptr<Reflex::Core::Shader> _defaultShader;
        }

        //inline std::shared_ptr<Reflex::Rendering::Renderer> _renderer = nullptr;
        inline std::shared_ptr<Reflex::Core::Shader> _activeShader = nullptr;
        inline std::shared_ptr<Reflex::Core::Material> _activeMaterial = std::make_shared<Reflex::Core::Material>();
        inline std::shared_ptr<Reflex::Core::Camera> _activeCamera = nullptr;
        //inline std::vector<std::shared_ptr<Reflex::Core::ILighting>> _activeLights;
    };



}

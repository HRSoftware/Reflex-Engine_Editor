#pragma once
#include "spdlog/fmt/fmt.h"
namespace Reflex
{
    class Settings
    {

        public:
            glm::vec2 screenDimensions = {1920, 1080};
            std::string Title = "Reflex Engine";


            std::string getVersionString();
            std::string getTitleString(bool bDisplayVersionInformation);
    };
}


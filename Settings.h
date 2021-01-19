#pragma once
#include "spdlog/fmt/fmt.h"
namespace Reflex
{
    class Settings
    {
        static std::shared_ptr<Settings> instance;
        protected:
        Settings();

        public:
            GLFWwindow* windowHandle;
            static std::shared_ptr<Settings> getInstance();
            std::pair<int, int> screenDimensions = {1920, 1080};
            std::string Title = "Reflex Engine";


            std::string getVersionString();

            std::string getTitleString(bool bDisplayVersionInformation);
    };
}


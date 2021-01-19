#include "pch.h"
#include "Settings.h"

std::shared_ptr<Reflex::Settings> Reflex::Settings::instance = nullptr;

Reflex::Settings::Settings()
{
}

std::shared_ptr<Reflex::Settings> Reflex::Settings::getInstance()
{
    if ( !instance )
    {
        instance = std::shared_ptr<Settings>(new Settings);
    }
    return instance;
}

std::string Reflex::Settings::getVersionString()
{
    return fmt::format("{0}.{1}.{2}", 1, 1, 0);
}

std::string Reflex::Settings::getTitleString(bool bDisplayVersionInformation)
{
    return "1.1";
    //return fmt::format("{0}{1}", Title, (bDisplayVersionInformation ? " - " + getVersionString() : ""));
}

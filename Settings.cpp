#include "pch.h"
#include "Settings.h"




std::string Reflex::Settings::getVersionString()
{
    return fmt::format("{0}.{1}.{2}", 1, 1, 0);
}

std::string Reflex::Settings::getTitleString(bool bDisplayVersionInformation)
{
    return "1.1";
    //return fmt::format("{0}{1}", Title, (bDisplayVersionInformation ? " - " + getVersionString() : ""));
}

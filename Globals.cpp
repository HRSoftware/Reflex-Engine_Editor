#include "pch.h"
#include "Globals.h"

Reflex::Globals::WindowGlobals::WindowGlobals()
{
    logger = Reflex::Log::createNewLogger("Windows_Global");
}

Reflex::Globals::RendererGlobals::RendererGlobals()
{
    logger = Reflex::Log::createNewLogger("Render_Global");
}

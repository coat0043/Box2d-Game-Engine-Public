#include "FrameworkPCH.h"
#include "GameCore.h"
#include "Components/ComponentRegistry.h"

namespace fw {

GameCore::GameCore(FWCore* pFramework)
{
    m_pFramework = pFramework;
    m_pComponentRegistry = new ComponentRegistry();
}

GameCore::~GameCore()
{
    delete m_pComponentRegistry;
}

} // namespace fw

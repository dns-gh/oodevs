// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionFlyingShell.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 17:10 $
// $Revision: 5 $
// $Workfile: PHY_PerceptionFlyingShell.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_PerceptionFlyingShell.h"

#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionFlyingShell::PHY_PerceptionFlyingShell( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::~PHY_PerceptionFlyingShell
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionFlyingShell::~PHY_PerceptionFlyingShell()
{
    for( CIT_ZoneVector it = zones_.begin(); it != zones_.end(); ++it )
        delete *it;
    zones_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void* PHY_PerceptionFlyingShell::AddLocalisation( const TER_Localisation& localisation )
{
    TER_Localisation* pLocalisation = new TER_Localisation( localisation );
    zones_.push_back( pLocalisation );
    return pLocalisation;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::RemoveLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::RemoveLocalisation( void* pId )
{
    TER_Localisation* pLoc = static_cast< TER_Localisation* >( pId );

    IT_ZoneVector it = std::find( zones_.begin(), zones_.end(), pLoc );
    if ( it != zones_.end() )
    {
        delete pLoc;
        zones_.erase( it );    
    }
}

// =============================================================================
// AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionFlyingShell::Compute( const MT_Vector2D& /*vPoint*/ ) const
{   
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MIL_EffectManager::T_FlyingShellSet& flyingShells = MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().GetFlyingShells();

    T_FlyingShellSet perceivedFlyingShells;
    for( MIL_EffectManager::CIT_FlyingShellSet itFlyingShell = flyingShells.begin(); itFlyingShell != flyingShells.end(); ++itFlyingShell )
    {
        const MIL_Effect_IndirectFire& flyingShell = (**itFlyingShell);
        for( CIT_ZoneVector itZone = zones_.begin(); itZone != zones_.end(); ++itZone )
        {
            if( flyingShell.FlyThroughLocalisation( **itZone ) )
            {
                perceivedFlyingShells.insert( &flyingShell );
                if( lastPerceivedFlyingShells_.find( &flyingShell ) == lastPerceivedFlyingShells_.end() )
                    perceiver_.NotifyFlyingShellPerception( flyingShell );
            }
        }
    }
    lastPerceivedFlyingShells_.swap( perceivedFlyingShells );
}

// ----------------------------------------------------------------------j-------
// Name: PHY_PerceptionFlyingShell::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionFlyingShell::Compute( const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionFlyingShell::Compute( const DEC_Knowledge_Agent& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionFlyingShell::Compute( const MIL_RealObject_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionFlyingShell::Compute( const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Execute( const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}

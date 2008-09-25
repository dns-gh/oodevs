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

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionFlyingShell.h"

#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/MIL_EntityManager.h"
#include "Tools/MIL_Tools.h"
#include "MIL_AgentServer.h"
#include <xeumeuleu/xml.h>



MT_Float PHY_PerceptionFlyingShell::rRadius_ = 0;

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Initialize
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "cobra-radar" )
            >> xml::attribute( "action-range", rRadius_ )
        >> xml::end();
    
    if( rRadius_ < 0 )
        xis.error( "cobra-radar: action-range < 0" );
    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionFlyingShell::PHY_PerceptionFlyingShell( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
    // NOTHING
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
// Name: PHY_PerceptionFlyingShell::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MIL_EffectManager::T_FlyingShellSet& flyingShells = MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().GetFlyingShells();
    const MT_Vector2D&                         source       = perceiver_.GetPion().GetRole< PHY_RolePion_Location >().GetPosition();

    T_FlyingShellSet perceivedFlyingShells;
    for( MIL_EffectManager::CIT_FlyingShellSet itFlyingShell = flyingShells.begin(); itFlyingShell != flyingShells.end(); ++itFlyingShell )
    {
        const MIL_Effect_IndirectFire& flyingShell = (**itFlyingShell);
        for( CIT_ZoneVector itZone = zones_.begin(); itZone != zones_.end(); ++itZone )
        {
            if( (**itZone).Intersect2DWithCircle( source, rRadius_ ) && flyingShell.IsFlyingThroughLocalisation( **itZone ) )
            {
                perceivedFlyingShells.insert( &flyingShell );
                if( lastPerceivedFlyingShells_.find( &flyingShell ) == lastPerceivedFlyingShells_.end() )
                    perceiver_.NotifyPerception( flyingShell );
            }
        }
    }
    lastPerceivedFlyingShells_.swap( perceivedFlyingShells );
}

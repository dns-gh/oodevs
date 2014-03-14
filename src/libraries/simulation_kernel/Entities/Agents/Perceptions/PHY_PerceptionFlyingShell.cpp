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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Tools/MIL_Tools.h"

namespace
{
    double radius = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Initialize
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "cobra-radar" )
            >> xml::attribute( "action-range", radius )
        >> xml::end;
    if( radius < 0 )
        throw MASA_EXCEPTION( xis.context() + "cobra-radar: action-range < 0" );
    radius = MIL_Tools::ConvertMeterToSim( radius );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionFlyingShell::PHY_PerceptionFlyingShell( PHY_RoleInterface_Perceiver& perceiver )
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
    for( auto it = zones_.begin(); it != zones_.end(); ++it )
        delete *it;
    zones_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionFlyingShell::AddLocalisation( const TER_Localisation& localisation )
{
    TER_Localisation* pLocalisation = new TER_Localisation( localisation );
    zones_.push_back( pLocalisation );
    const int id = PHY_Perception_ABC::GetPerceptionId();
    ids_[ id ] = pLocalisation;
    return id;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::RemoveLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::RemoveLocalisation( int id )
{
    const TER_Localisation* pLoc = ids_[ id ];
    auto it = std::find( zones_.begin(), zones_.end(), pLoc );
    if( it != zones_.end() )
    {
        delete pLoc;
        zones_.erase( it );
    }
    ids_.erase( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MT_Vector2D& source = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    T_FlyingShellSet perceivedFlyingShells;
    auto flyingShells = MIL_EffectManager::GetEffectManager().GetFlyingShells();
    for( auto it = flyingShells.begin(); it != flyingShells.end(); ++it )
    {
        const MIL_Effect_IndirectFire& flyingShell = (**it);
        for( auto itZone = zones_.begin(); itZone != zones_.end(); ++itZone )
            if( (**itZone).Intersect2DWithCircle( source, radius ) && flyingShell.IsFlyingThroughLocalisation( **itZone ) )
            {
                perceivedFlyingShells.insert( &flyingShell );
                if( lastPerceivedFlyingShells_.find( &flyingShell ) == lastPerceivedFlyingShells_.end() )
                    perceiver_.NotifyPerception( flyingShell );
            }
    }
    lastPerceivedFlyingShells_.swap( perceivedFlyingShells );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionFlyingShell::HasLocalisationToHandle() const
{
    return !zones_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::FinalizePerception()
{
    // NOTHING
}

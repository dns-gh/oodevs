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
#include <boost/range/algorithm_ext/erase.hpp>

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionFlyingShell::AddLocalisation( const TER_Localisation& localisation )
{
    boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation( localisation ) );
    const int id = PHY_Perception_ABC::GetPerceptionId();
    perceptions_.insert( T_Perceptions::value_type( id, pLocalisation ) );
    return id;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::RemoveLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::RemoveLocalisation( int id )
{
    perceptions_.left.erase( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MT_Vector2D& source = perceiver_.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    T_Shells perceivedShells;
    const auto& shells = MIL_EffectManager::GetEffectManager().GetFlyingShells();
    for( auto it = shells.begin(); it != shells.end(); ++it )
        for( auto it2 = perceptions_.right.begin(); it2 != perceptions_.right.end(); ++it2 )
            if( it2->first->Intersect2DWithCircle( source, radius ) && (*it)->IsFlyingThroughLocalisation( *it2->first ) )
            {
                // $$$$ MCO 2014-03-14: naked pointers could point to invalid memory later, or even to
                // a brand new re-created effect, but we cannot replace with shared/weak_ptr as effects
                // use a very advanced technique based on the revolutionary "delete this" idiom...
                perceivedShells.insert( *it );
                if( shells_.find( *it ) == shells_.end() )
                {
                    perceiver_.NotifyPerception( **it );
                    break;
                }
            }
    shells_.swap( perceivedShells );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionFlyingShell::HasLocalisationToHandle() const
{
    return !perceptions_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
void PHY_PerceptionFlyingShell::FinalizePerception()
{
    // NOTHING
}

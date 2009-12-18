// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_CapabilityComputer.cpp $
// $Author: Nld $
// $Modtime: 26/10/04 17:03 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_Objects_CapabilityComputer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RoleAction_Objects_CapabilityComputer.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/MIL_AgentPion.h"

#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"
#include "simulation_kernel/AlgorithmsFactories.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer constructor
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_CapabilityComputer::PHY_RoleAction_Objects_CapabilityComputer( const MIL_AgentPion& pion, E_Operation nOperation, const MIL_ObjectType_ABC& objectType )
    : nOperation_    ( nOperation )
    , objectType_    ( objectType )
    , bHasCapability_( false )
{
    CollectData( pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer destructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_CapabilityComputer::~PHY_RoleAction_Objects_CapabilityComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer::CollectData
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_CapabilityComputer::CollectData( const MIL_AgentPion& pion )
{
    MIL_AgentPion& pionT = const_cast< MIL_AgentPion& >( pion );
    std::auto_ptr< OnComponentFunctorComputer_ABC > componentComputer( pionT.GetAlgorithms().onComponentFunctorComputerFactory_->Create( *this ) );
    pionT.Execute( *componentComputer );
    
    if( bHasCapability_ )
        return;

    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        CollectData( **itReinforcement );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer::operator()
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_CapabilityComputer::operator() ( PHY_ComposantePion& composante )
{
    if( bHasCapability_ )
        return;

    switch( nOperation_ )
    {
        case eConstruct: bHasCapability_ = composante.CanConstruct( objectType_ ); break;
        case eDestroy  : bHasCapability_ = composante.CanDestroy  ( objectType_ ); break;
        case eMine     : bHasCapability_ = composante.CanMine     ( objectType_ ); break;
        case eDemine   : bHasCapability_ = composante.CanDemine   ( objectType_ ); break;
        case eBypass   : bHasCapability_ = composante.CanBypass   ( objectType_, false ); break;
        default: assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer::HasCapability
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects_CapabilityComputer::HasCapability() const
{
    return bHasCapability_;
}

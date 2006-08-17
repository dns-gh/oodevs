// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Composantes/PHY_ComposanteState.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 11:05 $
// $Revision: 5 $
// $Workfile: PHY_ComposanteState.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ComposanteState.h"

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const PHY_ComposanteState PHY_ComposanteState::undamaged_                  ( "Disponible"                  , eUndamaged                  , true  , false ); // bUsable, bDamaged
const PHY_ComposanteState PHY_ComposanteState::repairableWithEvacuation_   ( "ReparableAvecEvacuation"     , eRepairableWithEvacuation   , true  , true  );
const PHY_ComposanteState PHY_ComposanteState::repairableWithoutEvacuation_( "ReparableSurPlace"           , eRepairableWithoutEvacuation, true  , true  );
const PHY_ComposanteState PHY_ComposanteState::dead_                       ( "Detruit"                     , eDead                       , false , true  );
const PHY_ComposanteState PHY_ComposanteState::maintenance_                ( "EnMaintenance"               , eMaintenance                , false , false );
const PHY_ComposanteState PHY_ComposanteState::prisoner_                   ( "Prisonnier"                  , ePrisoner                   , true  , false );

PHY_ComposanteState::T_ComposanteStateVector PHY_ComposanteState::composanteStates_( eNbrStates, 0 );

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::Initialize
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ComposanteState::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing composante states" );
    composanteStates_[ undamaged_                  .GetID() ] = &undamaged_;
    composanteStates_[ repairableWithEvacuation_   .GetID() ] = &repairableWithEvacuation_;
    composanteStates_[ repairableWithoutEvacuation_.GetID() ] = &repairableWithoutEvacuation_;
    composanteStates_[ dead_                       .GetID() ] = &dead_;
    composanteStates_[ maintenance_                .GetID() ] = &maintenance_;    
    composanteStates_[ prisoner_                   .GetID() ] = &prisoner_;    
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::Terminate
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ComposanteState::Terminate()
{

}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ComposanteState::PHY_ComposanteState( const std::string& strName, E_State nState, bool bUsable, bool bDamaged )
    : strName_ ( strName  )
    , nState_  ( nState   )
    , bUsable_ ( bUsable  )
    , bDamaged_( bDamaged )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ComposanteState::~PHY_ComposanteState()
{
}


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

#include "simulation_kernel_pch.h"
#include "PHY_ComposanteState.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

const PHY_ComposanteState PHY_ComposanteState::undamaged_                  ( "Disponible"             , eUndamaged                  , true  , false ); // bUsable, bDamaged
const PHY_ComposanteState PHY_ComposanteState::repairableWithEvacuation_   ( "ReparableAvecEvacuation", eRepairableWithEvacuation   , true  , true  );
const PHY_ComposanteState PHY_ComposanteState::repairableWithoutEvacuation_( "ReparableSurPlace"      , eRepairableWithoutEvacuation, true  , true  );
const PHY_ComposanteState PHY_ComposanteState::dead_                       ( "Detruit"                , eDead                       , false , true  );
const PHY_ComposanteState PHY_ComposanteState::maintenance_                ( "EnMaintenance"          , eMaintenance                , false , false );
const PHY_ComposanteState PHY_ComposanteState::prisoner_                   ( "Prisonnier"             , ePrisoner                   , true  , false );

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
    // NOTHING
}

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ComposanteState::~PHY_ComposanteState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetStates
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
PHY_ComposanteState::T_ComposanteStateVector& PHY_ComposanteState::GetStates()
{
    return composanteStates_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_ComposanteState::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::CanTransport
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
bool PHY_ComposanteState::CanTransport() const
{
    return IsUsable() && *this != repairableWithEvacuation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::IsUsable
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteState::IsUsable() const
{
    return bUsable_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::IsDamaged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteState::IsDamaged() const
{
    return bDamaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetID
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteState::GetID() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetNbrStates
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
unsigned int PHY_ComposanteState::GetNbrStates()
{
    return eNbrStates;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::Find
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
const PHY_ComposanteState& PHY_ComposanteState::Find( unsigned int nID )
{
    assert( composanteStates_.size() > nID );
    return *composanteStates_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::Find
// Created: NLD 2006-07-25
// -----------------------------------------------------------------------------
const PHY_ComposanteState* PHY_ComposanteState::Find( const std::string& strName )
{
    for( auto it = composanteStates_.begin(); it != composanteStates_.end(); ++it )
        if( boost::iequals( (**it).GetName(), strName ) )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::operator==
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteState::operator==( const PHY_ComposanteState& rhs ) const
{
    return nState_ == rhs.nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::operator!=
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteState::operator!=( const PHY_ComposanteState& rhs ) const
{
    return nState_ != rhs.nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::operator<
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_ComposanteState::operator<( const PHY_ComposanteState& rhs ) const
{
    return nState_ < rhs.nState_;
}

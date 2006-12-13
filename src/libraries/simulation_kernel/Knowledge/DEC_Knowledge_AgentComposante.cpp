// *****************************************************************************
//
// $Created: NLD 2004-04-01 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentComposante.cpp $
// $Author: Nld $
// $Modtime: 18/04/05 16:38 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentComposante.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentComposante.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_AgentComposante, "DEC_Knowledge_AgentComposante" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante constructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::DEC_Knowledge_AgentComposante()
    : pType_               ( 0 )
    , bCanFire_            ( false )
    , bCanFireWhenUnloaded_( false )
    , nMajorScore_         ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante constructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::DEC_Knowledge_AgentComposante( const PHY_Composante_ABC& composante )
    : pType_               ( &composante.GetType() )
    , bCanFire_            ( composante .CanFire() )
    , bCanFireWhenUnloaded_( composante .CanFireWhenUnloaded() )
    , nMajorScore_         ( composante.GetMajorScore() )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante destructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::~DEC_Knowledge_AgentComposante()
{
    
}

// =============================================================================
// CHECKPOINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentComposante::load( MIL_CheckPointInArchive& file, const uint )
{
    ASN1T_TypeEquipement nMosID;
    file >> nMosID;
    pType_ = PHY_ComposanteTypePion::Find( nMosID );
    
    file >> bCanFire_
         >> bCanFireWhenUnloaded_
         >> nMajorScore_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentComposante::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pType_->GetMosID()
         << bCanFire_
         << bCanFireWhenUnloaded_
         << nMajorScore_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetDangerosity
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentComposante::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposantePion& compTarget, MT_Float rDistBtwSourceAndTarget ) const
{
    assert( pType_ );
    if( !bCanFire_ )
        return 0.;
    return pType_->GetDangerosity( firer, compTarget.GetType(), rDistBtwSourceAndTarget );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetDangerosity
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentComposante::GetDangerosity( const MIL_Agent_ABC& firer, const DEC_Knowledge_AgentComposante& compTarget, MT_Float rDistBtwSourceAndTarget ) const
{
    assert( pType_ );
    if( !bCanFire_ )
        return 0.;
    return pType_->GetDangerosity( firer, compTarget.GetType(), rDistBtwSourceAndTarget );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetMaxRangeToFireOn
// Created: NLD 2004-04-15
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentComposante::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposantePion& compTarget, MT_Float rWantedPH ) const
{
    assert( pType_ );
    if( !bCanFire_ )
        return 0.;
    return pType_->GetMaxRangeToFireOn( firer, compTarget.GetType(), rWantedPH );
}

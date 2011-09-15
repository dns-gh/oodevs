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
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentComposante )

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante constructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::DEC_Knowledge_AgentComposante( const PHY_Composante_ABC& composante )
    : pType_               ( &composante.GetType() )
    , bCanFire_            ( composante.CanFire() )
    , bCanFireWhenUnloaded_( composante.CanFireWhenUnloaded() )
    , nMajorScore_         ( composante.GetMajorScore() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante destructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::~DEC_Knowledge_AgentComposante()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentComposante::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    sword::EquipmentType nMosID;
    int nMosIDoid;
    file >> nMosIDoid;
    nMosID.set_id( nMosIDoid );
    pType_ = PHY_ComposanteTypePion::Find( nMosID );
    file >> bCanFire_
         >> bCanFireWhenUnloaded_
         >> nMajorScore_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentComposante::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    sword::EquipmentType type = pType_->GetMosID();
    int equipmenttype_val = type.id();
    file << equipmenttype_val
         << bCanFire_
         << bCanFireWhenUnloaded_
         << nMajorScore_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetDangerosity
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentComposante::GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& compTarget, double rDistBtwSourceAndTarget, bool bUseAmmo ) const
{
    assert( pType_ );
    if( !bCanFire_ )
        return 0.;
    return pType_->GetDangerosity( firer, compTarget, rDistBtwSourceAndTarget, bUseAmmo );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetMaxRangeToFireOn
// Created: NLD 2004-04-15
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentComposante::GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposantePion& compTarget, double rWantedPH ) const
{
    assert( pType_ );
    if( !bCanFire_ )
        return 0.;
    return pType_->GetMaxRangeToFireOn( firer, compTarget.GetType(), rWantedPH, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetType
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
const PHY_ComposanteType_ABC& DEC_Knowledge_AgentComposante::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetMajorScore
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_AgentComposante::GetMajorScore() const
{
    return nMajorScore_;
}

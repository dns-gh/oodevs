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

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentComposante )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante constructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::DEC_Knowledge_AgentComposante()
    : pType_      ( 0 )
    , bCanFire_   ( false )
    , bMajor_     ( false )
    , nMajorScore_( 0 )
    , maxRange_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante constructor
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentComposante::DEC_Knowledge_AgentComposante( const PHY_ComposantePion& composante )
    : pType_      ( &composante.GetType() )
    , bCanFire_   ( composante.CanFire() )
    , bMajor_     ( composante.IsMajor() )
    , nMajorScore_( composante.GetMajorScore() )
    , maxRange_   ( composante.GetMaxRangeToDirectFire() )
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
    file >> pType_;
    file >> bCanFire_;
    file >> bMajor_;
    file >> nMajorScore_;
    file >> maxRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentComposante::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pType_;
    file << bCanFire_;
    file << bMajor_;
    file << nMajorScore_;
    file << maxRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::WriteKnowledges
// Created: NPT 2012-08-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentComposante::WriteKnowledges( xml::xostream& xos, unsigned int number ) const
{
    if( pType_ )
    {
        xos << xml::start( "composante" )
                << xml::attribute( "type", pType_->GetMosID().id() )
                << xml::attribute( "can-fire", bCanFire_ )
                << xml::attribute( "major", bMajor_ )
                << xml::attribute( "major-score", nMajorScore_ )
                << xml::attribute( "number", number )
            << xml::end;
    }
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
    if( maxRange_ < rDistBtwSourceAndTarget )
        return 0;
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
    return pType_->GetMaxRangeToFireOn( firer, compTarget.GetType(), rWantedPH, nullptr, false );
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::IsMajor
// Created: LDC 2011-09-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentComposante::IsMajor() const
{
    return bMajor_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentComposante::GetMaxRange
// Created: MCO 2012-11-23
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentComposante::GetMaxRange() const
{
    return maxRange_;
}

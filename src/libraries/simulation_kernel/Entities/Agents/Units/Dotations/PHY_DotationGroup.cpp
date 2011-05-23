// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationGroup.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 11 $
// $Workfile: PHY_DotationGroup.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationGroupContainer.h"
#include "PHY_DotationGroup.h"
#include "PHY_Dotation.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationCategory.h"
#include "PHY_AmmoDotationClass.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyDotationRequestContainer.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/split_free.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_DotationGroup )

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationGroup::PHY_DotationGroup( const PHY_DotationType& type, PHY_DotationGroupContainer& groupContainer, bool bInfiniteDotations )
    : pType_             ( &type )
    , pGroupContainer_   ( &groupContainer )
    , bInfiniteDotations_( bInfiniteDotations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationGroup::PHY_DotationGroup()
    : pType_             ( 0 )
    , pGroupContainer_   ( 0 )
    , bInfiniteDotations_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationGroup::~PHY_DotationGroup()
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        delete it->second;
    dotations_.clear();
}

namespace boost
{
namespace serialization
{
    template< typename Archive >
    void serialize( Archive& file, PHY_DotationGroup::T_DotationMap& map, const unsigned int nVersion )
    {
        split_free( file, map, nVersion );
    }

    template< typename Archive >
    void save( Archive& file, const PHY_DotationGroup::T_DotationMap& map, const unsigned int )
    {
        std::size_t size = map.size();
        file << size;
        for( PHY_DotationGroup::CIT_DotationMap it = map.begin(); it != map.end(); ++it )
        {
            unsigned id = it->first->GetMosID();
            file << id;
            file << it->second;
        }
    }

    template< typename Archive >
    void load( Archive& file, PHY_DotationGroup::T_DotationMap& map, const unsigned int )
    {
        std::size_t nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned int nID;
            file >> nID;
            file >> map[ PHY_DotationType::FindDotationCategory( nID ) ];
        }
    }
}
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_DotationGroup::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nID;
    file >> nID;
    pType_ = PHY_DotationType::FindDotationType( nID );
    file >> pGroupContainer_
         >> dotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_DotationGroup::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned type = ( pType_ ? pType_->GetID() : (unsigned int)-1 );
    file << type
         << pGroupContainer_
         << dotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ReadValues
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ReadValues( xml::xistream& xis, const PHY_DotationCategory& category )
{
    PHY_Dotation& dotation = CreateDotation( category );
    dotation.ReadValue( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_DotationGroup::WriteODB( xml::xostream& xos ) const
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        xos << xml::start( "resource" )
                << xml::attribute( "name", it->first->GetName() )
                << xml::attribute( "quantity", it->second->GetValue() )
            << xml::end; // dotation
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::CreateDotation
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_Dotation& PHY_DotationGroup::CreateDotation( const PHY_DotationCategory& dotationCategory )
{
    PHY_Dotation*& pDotation = dotations_[ &dotationCategory ];
    if( !pDotation )
        pDotation = new PHY_Dotation( dotationCategory, *this, bInfiniteDotations_ );
    return *pDotation;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::AddCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroup::AddCapacity( const PHY_DotationCapacity& capacity )
{
    PHY_Dotation& dotation = CreateDotation( capacity.GetCategory() );
    dotation.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::RemoveCapacity
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_DotationGroup::RemoveCapacity( const PHY_DotationCapacity& capacity )
{
    PHY_Dotation* pDotation = GetDotation( capacity.GetCategory() );
    assert( pDotation );
    pDotation->RemoveCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetCapacity
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_DotationGroup::GetCapacity( const PHY_DotationCategory& category ) const
{
    const PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->GetCapacity() : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetValue
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_DotationGroup::GetValue( const PHY_DotationCategory& category ) const
{
    const PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->GetValue() : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_DotationGroup::Consume( const PHY_DotationCategory& category, double rNbr )
{
    PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->Consume( rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Reset
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Reset()
{
    for( T_DotationMap::iterator it = dotations_.begin(); it != dotations_.end(); it++ )
        it->second->Reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetIlluminationDotations
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationGroup::GetIlluminationDotations( float range, bool permanent ) const
{
    for( T_DotationMap::const_iterator it = dotations_.begin(); it != dotations_.end(); it++ )
    {
        if( it->first->IsIlluminating( range, permanent ) )
        {
            return it->first;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetilluminatingRange
// Created: GGE 2010-06-23
// -----------------------------------------------------------------------------
float PHY_DotationGroup::GetIlluminatingRange( ) const
{
    float rangeMax = 0.0;
    for( T_DotationMap::const_iterator it = dotations_.begin(); it != dotations_.end(); it++ )
    {
        if( it->first->GetIlluminatingRange( ) > rangeMax )
        {
            rangeMax = it->first->GetIlluminatingRange( );
        }
    }
    return rangeMax;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Supply
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
double PHY_DotationGroup::Supply( const PHY_DotationCategory& category, double rNbr )
{
    PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->Supply( rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Resupply( double rFactor /* = 1.*/ )
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->Resupply( rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Resupply
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Resupply( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor )
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        PHY_Dotation& dotation = *it->second;
        if( !dotation.GetCategory().GetAmmoDotationClass() || *dotation.GetCategory().GetAmmoDotationClass() != ammoDotationClass )
            continue;
        dotation.Resupply( rFactor );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::AddConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
double PHY_DotationGroup::AddConsumptionReservation( const PHY_DotationCategory& category, double rNbr )
{
    PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->AddConsumptionReservation( rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::CancelConsumptionReservations
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroup::CancelConsumptionReservations()
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->CancelConsumptionReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ConsumeConsumptionReservations
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ConsumeConsumptionReservations()
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->ConsumeConsumptionReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double PHY_DotationGroup::AddFireReservation( const PHY_DotationCategory& category, double rNbr )
{
    PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->AddFireReservation( rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ConsumeFireReservations
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ConsumeFireReservations()
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->ConsumeFireReservation();
}

// =============================================================================
// LOGISTIC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::FillSupplyRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationGroup::FillSupplyRequest( PHY_SupplyDotationRequestContainer& supplyRequest ) const
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        supplyRequest.AddDotation( *it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ChangeDotationsValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ChangeDotationsValueUsingTC2( const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor, MIL_AutomateLOG& tc2 ) const
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        PHY_Dotation& dotation = *it->second;
        if( pAmmoDotationClass )
        {
            if( !dotation.GetCategory().GetAmmoDotationClass() || *dotation.GetCategory().GetAmmoDotationClass() != *pAmmoDotationClass )
            continue;
        }
        dotation.ChangeValueUsingTC2( rCapacityFactor, tc2 );
    }
}

// =============================================================================
// MISC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::NotifyCaptured
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_DotationGroup::NotifyCaptured()
{
    assert( pType_ );

    if( pType_ == PHY_DotationType::ration_ )
        return;

    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->NotifyCaptured();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::NotifyReleased
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_DotationGroup::NotifyReleased()
{
    assert( pType_ );

    if( pType_ == PHY_DotationType::ration_ )
        return;

    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->NotifyReleased();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetDotation
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_Dotation* PHY_DotationGroup::GetDotation( const PHY_DotationCategory& dotationCategory ) const
{
    CIT_DotationMap it = dotations_.find( &dotationCategory );
    if( it == dotations_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::NotifyDotationChanged
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroup::NotifyDotationChanged( const PHY_Dotation& dotation )
{
    assert( pGroupContainer_ );
    pGroupContainer_->NotifyDotationChanged( dotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_DotationGroup::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    assert( pGroupContainer_ );
    pGroupContainer_->NotifySupplyNeeded( dotationCategory, bNewNeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetDotations
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_DotationGroup::T_DotationMap& PHY_DotationGroup::GetDotations() const
{
    return dotations_;
}

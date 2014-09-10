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
#include "Entities/Agents/Units/PHY_UnitType.h"

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
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        delete it->second;
    dotations_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_DotationGroup::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> pType_;
    file >> pGroupContainer_;
    file >> dotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_DotationGroup::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pType_;
    file << pGroupContainer_;
    file << dotations_;
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
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        xos << xml::start( "resource" )
                << xml::attribute( "name", it->first->GetName() )
                << xml::attribute( "quantity", static_cast< int >( it->second->GetValue() ) );
        xos << xml::end; // dotation
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
void PHY_DotationGroup::AddCapacity( const PHY_DotationCapacity& capacity, double toAdd )
{
    PHY_Dotation& dotation = CreateDotation( capacity.GetCategory() );
    dotation.AddCapacity( capacity, toAdd );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::RemoveCapacity
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
double PHY_DotationGroup::RemoveCapacity( const PHY_DotationCapacity& capacity )
{
    PHY_Dotation* pDotation = GetDotation( capacity.GetCategory() );
    assert( pDotation );
    if( pDotation )
        return pDotation->RemoveCapacity( capacity );
    return 0.;
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
// Name: PHY_DotationGroup::GetIlluminationDotations
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationGroup::GetIlluminationDotations( float range, bool permanent ) const
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        if( it->first->IsIlluminating( range, permanent ) )
            return it->first;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetilluminatingRange
// Created: GGE 2010-06-23
// -----------------------------------------------------------------------------
float PHY_DotationGroup::GetIlluminatingRange() const
{
    float rangeMax = 0.0;
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        if( it->first->GetIlluminatingRange( ) > rangeMax )
            rangeMax = it->first->GetIlluminatingRange( );
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
// Name: PHY_DotationGroup::SupplyAll
// Created: BAX 2014-04-03
// -----------------------------------------------------------------------------
void PHY_DotationGroup::SupplyAll( double rFactor )
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->Supply( it->second->GetCapacity() * rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Resupply( double rFactor /* = 1.*/,  bool withLog /* = false */ )
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->Resupply( rFactor, withLog );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Resupply
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Resupply( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor )
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
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
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->CancelConsumptionReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ConsumeConsumptionReservations
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ConsumeConsumptionReservations()
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->ConsumeConsumptionReservation();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::UpdateSupplyNeeded
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
void PHY_DotationGroup::UpdateSupplyNeeded()
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->UpdateSupplyNeeded();
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
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->ConsumeFireReservation();
}

// =============================================================================
// LOGISTIC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Apply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Apply( std::function< void( PHY_Dotation& ) > visitor ) const
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        visitor( *it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ChangeDotationsValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ChangeDotationsValueUsingTC2( const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor, MIL_AutomateLOG& tc2 ) const
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        PHY_Dotation& dotation = *it->second;
        if( pAmmoDotationClass )
        {
            if( !dotation.GetCategory().GetAmmoDotationClass() || *dotation.GetCategory().GetAmmoDotationClass() != *pAmmoDotationClass )
            continue;
        }
        dotation.SetValueUsingTC2( rCapacityFactor * dotation.GetCapacity(), tc2 );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::EnforceAviationResources
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
void PHY_DotationGroup::EnforceAviationResources( E_AviationRange aviationRange, const PHY_UnitType& unitType, MIL_AutomateLOG& tc2 )
{
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        PHY_Dotation& dotation = *it->second;
        double newValue = unitType.GetResourceCapacityWithAviationQuotas( aviationRange, dotation );
        dotation.SetValueUsingTC2( newValue, tc2 );
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

    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
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
    for( auto it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->NotifyReleased();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetDotation
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_Dotation* PHY_DotationGroup::GetDotation( const PHY_DotationCategory& dotationCategory ) const
{
    auto it = dotations_.find( &dotationCategory );
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

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetDotationGroupContainer
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
const PHY_DotationGroupContainer* PHY_DotationGroup::GetDotationGroupContainer() const
{
    return pGroupContainer_;
}

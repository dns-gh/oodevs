// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationGroupContainer.h"
#include "PHY_DotationType.h"
#include "PHY_DotationGroup.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationCategory.h"
#include "PHY_Dotation.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/split_free.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationGroupContainer::PHY_DotationGroupContainer( dotation::PHY_RoleInterface_Dotations& roleDotation, bool bInfiniteDotations )
    : pRoleDotation_     ( &roleDotation )
    , bInfiniteDotations_( bInfiniteDotations )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationGroupContainer::PHY_DotationGroupContainer()
    : pRoleDotation_     ( 0 )
    , bInfiniteDotations_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationGroupContainer::~PHY_DotationGroupContainer()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        delete it->second;
    dotationGroups_.clear();
}

namespace boost
{
namespace serialization
{
    template< typename Archive >
    inline
    void serialize( Archive& file, PHY_DotationGroupContainer::T_DotationGroupMap& map, const unsigned int nVersion )
    {
        split_free( file, map, nVersion );
    }

    template< typename Archive >
    void save( Archive& file, const PHY_DotationGroupContainer::T_DotationGroupMap& map, const unsigned int )
    {
        std::size_t size = map.size();
        file << size;
        for( PHY_DotationGroupContainer::CIT_DotationGroupMap it = map.begin(); it != map.end(); ++it )
        {
            unsigned id = it->first->GetID();
            file << id
                 << it->second;
        }
    }

    template< typename Archive >
    void load( Archive& file, PHY_DotationGroupContainer::T_DotationGroupMap& map, const unsigned int )
    {
        std::size_t n;
        file >> n;
        while( n-- )
        {
            unsigned int id;
            file >> id;
            file >> map[ PHY_DotationType::FindDotationType( id ) ];
        }
    }

    template< typename Archive >
    inline
    void serialize( Archive& file, PHY_DotationGroupContainer::T_DotationSet& set, const unsigned int nVersion )
    {
        split_free( file, set, nVersion );
    }

    template< typename Archive >
    void save( Archive& file, const PHY_DotationGroupContainer::T_DotationSet& set, const unsigned int )
    {
        std::size_t size = set.size();
        file << size;
        for ( PHY_DotationGroupContainer::CIT_DotationSet it = set.begin(); it != set.end(); ++it )
            file << *it;
    }

    template< typename Archive >
    void load( Archive& file, PHY_DotationGroupContainer::T_DotationSet& set, const unsigned int )
    {
        std::size_t n;
        file >> n;
        while ( n-- )
        {
            PHY_Dotation* pDotation;
            file >> pDotation;
            set.insert( pDotation );
        }
    }
}
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::serialize
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::serialize( MIL_CheckPointOutArchive& ar, unsigned int )
{
    ar << pRoleDotation_
         << dotationGroups_
         << dotationsChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::serialize
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::serialize( MIL_CheckPointInArchive& ar, unsigned int )
{
    ar >> pRoleDotation_
         >> dotationGroups_
         >> dotationsChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ReadDotations
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ReadDotations( xml::xistream& xis )
{
    if( xis.has_child( "resources" ) )
    {
        for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
            it->second->Reset();
        xis >> xml::start( "resources" )
                >> xml::list( "resource", *this, &PHY_DotationGroupContainer::ReadDotation )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ReadDotation
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ReadDotation( xml::xistream& xis )
{
    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( xis.attribute< std::string >( "name" ) );
    if( !pDotationCategory )
        xis.error( "Unknown resource type" );
    PHY_DotationGroup* pGroup = GetDotationGroup( pDotationCategory->GetType() ); //$$$$$ TEMPORAIRE : merger PHY_DotationGroupContainer et PHY_DotationGroup
    if( !pGroup )
    {
        pGroup = &CreateDotationGroup( pDotationCategory->GetType() );
        pGroup->AddCapacity( PHY_DotationCapacity( *pDotationCategory, xis.attribute< double >( "quantity" ), xis.attribute( "logistic-threshold", 0. ) ) );
    }
    pGroup->ReadValues( xis, *pDotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "resources" );
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->WriteODB( xos );
    xos << xml::end; // resources
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::CreateDotationGroup
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_DotationGroup& PHY_DotationGroupContainer::CreateDotationGroup( const PHY_DotationType& dotationType )
{
    PHY_DotationGroup*& pGroup = dotationGroups_[ &dotationType ];
    if( !pGroup )
        pGroup = new PHY_DotationGroup( dotationType, *this, bInfiniteDotations_ );
    return *pGroup;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::AddCapacity
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::AddCapacity( const PHY_DotationCapacity& capacity )
{
    PHY_DotationGroup& group = CreateDotationGroup( capacity.GetCategory().GetType() );
    group.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::RemoveCapacity
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::RemoveCapacity( const PHY_DotationCapacity& capacity )
{
    PHY_DotationGroup* pGroup = GetDotationGroup( capacity.GetCategory().GetType() );
    assert( pGroup );
    pGroup->RemoveCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::GetCapacity
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_DotationGroupContainer::GetCapacity( const PHY_DotationCategory& category ) const
{
    const PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->GetCapacity( category ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::GetValue
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_DotationGroupContainer::GetValue( const PHY_DotationCategory& category ) const
{
    const PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->GetValue( category ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_DotationGroupContainer::Consume( const PHY_DotationCategory& category, double rNbr )
{
    PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->Consume( category, rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Supply
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
double PHY_DotationGroupContainer::Supply( const PHY_DotationCategory& category, double rNbr )
{
    PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->Supply( category, rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::Resupply()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Resupply
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::Resupply( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor )
{
    PHY_DotationGroup* pDotationGroup = GetDotationGroup( *PHY_DotationType::munition_ );
    if( pDotationGroup )
        pDotationGroup->Resupply( ammoDotationClass, rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Resupply
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::Resupply( const PHY_DotationType& type, double rFactor )
{
    PHY_DotationGroup* pDotationGroup = GetDotationGroup( type );
    if( pDotationGroup )
        pDotationGroup->Resupply( rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::HasIlluminationDotations
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationGroupContainer::GetIlluminationDotations( float range, bool permanent ) const
{
    for( T_DotationGroupMap::const_iterator it = dotationGroups_.begin(); it != dotationGroups_.end(); it++ )
    {
        const PHY_DotationCategory* dotation = it->second->GetIlluminationDotations( range, permanent );
        if( dotation )
            return dotation;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::GetilluminatingRange
// Created: GGE 2010-06-23
// -----------------------------------------------------------------------------
float PHY_DotationGroupContainer::GetIlluminatingRange() const
{
    float rangeMax = 0;
    for( T_DotationGroupMap::const_iterator it = dotationGroups_.begin(); it != dotationGroups_.end(); it++ )
    {
        float range = it->second->GetIlluminatingRange( );
        if( range > rangeMax )
            rangeMax = range;
    }
    return rangeMax;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::AddConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
double PHY_DotationGroupContainer::AddConsumptionReservation( const PHY_DotationCategory& category, double rNbr )
{
    PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->AddConsumptionReservation( category, rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::CancelConsumptionReservations
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::CancelConsumptionReservations()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->CancelConsumptionReservations();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ConsumeConsumptionReservations
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ConsumeConsumptionReservations()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->ConsumeConsumptionReservations();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double PHY_DotationGroupContainer::AddFireReservation( const PHY_DotationCategory& category, double rNbr )
{
    PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->AddFireReservation( category, rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ConsumeFireReservations
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ConsumeFireReservations()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->ConsumeFireReservations();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    assert( pRoleDotation_ );
    pRoleDotation_->NotifySupplyNeeded( dotationCategory, bNewNeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::FillSupplyRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::FillSupplyRequest( PHY_SupplyDotationRequestContainer& supplyRequest ) const
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->FillSupplyRequest( supplyRequest );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ChangeDotationsValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor, MIL_AutomateLOG& tc2 ) const
{
    CIT_DotationGroupMap it = dotationGroups_.find( &dotationType );
    if( it == dotationGroups_.end() )
        return;
    it->second->ChangeDotationsValueUsingTC2( pAmmoDotationClass, rCapacityFactor, tc2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::NotifyCaptured
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::NotifyCaptured()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->NotifyCaptured();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::NotifyReleased
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::NotifyReleased()
{
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->NotifyReleased();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::SendChangedState( client::UnitAttributes& asn ) const
{
    if( dotationsChanged_.empty() )
        return;
    for( CIT_DotationSet itDotation = dotationsChanged_.begin(); itDotation != dotationsChanged_.end(); ++itDotation )
    {
        const PHY_Dotation& dotation = **itDotation;
        sword::ResourceDotations_ResourceDotation& asnRessource = *asn().mutable_resource_dotations()->add_elem();
        asnRessource.mutable_type()->set_id( dotation.GetCategory().GetMosID() );
        asnRessource.set_quantity( (unsigned int)dotation.GetValue() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::SendFullState( client::UnitAttributes& asn ) const
{
    std::size_t nNbrDotations = 0;
    for( CIT_DotationGroupMap itDotationGroup = dotationGroups_.begin(); itDotationGroup != dotationGroups_.end(); ++itDotationGroup )
    {
        const PHY_DotationGroup::T_DotationMap& dotations = itDotationGroup->second->GetDotations();
        nNbrDotations += dotations.size();
    }
    if( nNbrDotations == 0 )
        return;
    for( CIT_DotationGroupMap itDotationGroup = dotationGroups_.begin(); itDotationGroup != dotationGroups_.end(); ++itDotationGroup )
    {
        const PHY_DotationGroup::T_DotationMap& dotations = itDotationGroup->second->GetDotations();
        for( PHY_DotationGroup::CIT_DotationMap itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
        {
            const PHY_Dotation& dotation = *itDotation->second;
            sword::ResourceDotations_ResourceDotation& asnRessource = *asn().mutable_resource_dotations()->add_elem();
            asnRessource.mutable_type()->set_id( dotation.GetCategory().GetMosID() );
            asnRessource.set_quantity( (unsigned int)dotation.GetValue() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::GetDotationGroup
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_DotationGroup* PHY_DotationGroupContainer::GetDotationGroup( const PHY_DotationType& dotationType ) const
{
    CIT_DotationGroupMap it = dotationGroups_.find( &dotationType );
    if( it == dotationGroups_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::NotifyDotationChanged
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::NotifyDotationChanged( const PHY_Dotation& dotation )
{
    dotationsChanged_.insert( &dotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::HasChanged
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
bool PHY_DotationGroupContainer::HasChanged() const
{
    return !dotationsChanged_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Clean
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::Clean()
{
    dotationsChanged_.clear();
}

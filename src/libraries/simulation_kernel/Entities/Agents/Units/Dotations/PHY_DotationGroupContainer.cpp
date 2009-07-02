// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 10 $
// $Workfile: PHY_DotationGroupContainer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationGroupContainer.h"
#include "PHY_DotationType.h"
#include "PHY_DotationGroup.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationCategory.h"
#include "PHY_Dotation.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Network/NET_ASN_Messages.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( PHY_DotationGroupContainer, "PHY_DotationGroupContainer" )

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationGroupContainer::PHY_DotationGroupContainer( PHY_RolePion_Dotations& roleDotation )
    : pRoleDotation_( &roleDotation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationGroupContainer::PHY_DotationGroupContainer()
    : pRoleDotation_( 0 )
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
        void serialize( Archive& file, PHY_DotationGroupContainer::T_DotationGroupMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_DotationGroupContainer::T_DotationGroupMap& map, const uint )
        {
            unsigned size = map.size();
            file << size;
            for ( PHY_DotationGroupContainer::CIT_DotationGroupMap it = map.begin(); it != map.end(); ++it )
            {
                unsigned id = it->first->GetID();
                file << id
                     << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_DotationGroupContainer::T_DotationGroupMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
                
                file >> nID;
                file >> map[ PHY_DotationType::FindDotationType( nID ) ];
            }
        }
        
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_DotationGroupContainer::T_DotationSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_DotationGroupContainer::T_DotationSet& set, const uint )
        {
            unsigned size = set.size();
            file << size;
            for ( PHY_DotationGroupContainer::CIT_DotationSet it = set.begin(); it != set.end(); ++it )
                file << *it;
        }
                
        template< typename Archive >
        void load( Archive& file, PHY_DotationGroupContainer::T_DotationSet& set, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
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
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_DotationGroupContainer::serialize( Archive& file, const uint )
{
    file & pRoleDotation_
         & dotationGroups_
         & dotationsChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ReadValues
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ReadValues( xml::xistream& xis )
{
    xis >> xml::list( "dotations", *this, &PHY_DotationGroupContainer::ReadDotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ReadDotations
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ReadDotations( xml::xistream& xis )
{
    xis >> xml::list( "dotation", *this, &PHY_DotationGroupContainer::ReadDotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::ReadDotation
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::ReadDotation( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "name", strType );
    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( strType );
    if( !pDotationCategory )
        xis.error( "Unknown dotation type" );

    PHY_DotationGroup* pGroup = GetDotationGroup( pDotationCategory->GetType() ); //$$$$$ TEMPORAIRE : merger PHY_DotationGroupContainer et PHY_DotationGroup
    if( !pGroup )
        xis.error( "Dotation type cannot be overloaded: not in types" );

    pGroup->ReadValues( xis, *pDotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "dotations" );
    for( CIT_DotationGroupMap it = dotationGroups_.begin(); it != dotationGroups_.end(); ++it )
        it->second->WriteODB( xos );
    xos << xml::end(); // dotations
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::CreateDotationGroup
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_DotationGroup& PHY_DotationGroupContainer::CreateDotationGroup( const PHY_DotationType& dotationType )
{
    PHY_DotationGroup*& pGroup = dotationGroups_[ &dotationType ];
    if( !pGroup )
        pGroup = new PHY_DotationGroup( dotationType, *this );
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
MT_Float PHY_DotationGroupContainer::GetCapacity( const PHY_DotationCategory& category ) const
{
    const PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->GetCapacity( category ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::GetValue
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroupContainer::GetValue( const PHY_DotationCategory& category ) const
{
    const PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->GetValue( category ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroupContainer::Consume( const PHY_DotationCategory& category, MT_Float rNbr )
{
    PHY_DotationGroup* pGroup = GetDotationGroup( category.GetType() );
    return pGroup ? pGroup->Consume( category, rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Supply
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroupContainer::Supply( const PHY_DotationCategory& category, MT_Float rNbr )
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
void PHY_DotationGroupContainer::Resupply( const PHY_AmmoDotationClass& ammoDotationClass, MT_Float rFactor )
{
    PHY_DotationGroup* pDotationGroup = GetDotationGroup( PHY_DotationType::munition_ );
    if( pDotationGroup )
        pDotationGroup->Resupply( ammoDotationClass, rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::Resupply
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::Resupply( const PHY_DotationType& type, MT_Float rFactor )
{
    PHY_DotationGroup* pDotationGroup = GetDotationGroup( type );
    if( pDotationGroup )
        pDotationGroup->Resupply( rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::AddConsumptionReservation
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroupContainer::AddConsumptionReservation( const PHY_DotationCategory& category, MT_Float rNbr )
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
MT_Float PHY_DotationGroupContainer::AddFireReservation( const PHY_DotationCategory& category, MT_Float rNbr )
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
void PHY_DotationGroupContainer::ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor, MIL_AutomateLOG& tc2 ) const
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
void PHY_DotationGroupContainer::SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const
{
    if( dotationsChanged_.empty() )
        return;

    ASN1T_ResourceDotations* pResources = new ASN1T_ResourceDotations[ dotationsChanged_.size() ];
    uint i = 0;
    for( CIT_DotationSet itDotation = dotationsChanged_.begin(); itDotation != dotationsChanged_.end(); ++itDotation, ++i )
    {
        const PHY_Dotation& dotation          = **itDotation;
        ASN1T_ResourceDotations& asnRessource = pResources[i];
        asnRessource.ressource_id             = dotation.GetCategory().GetMosID();
        asnRessource.quantite_disponible      = (uint)dotation.GetValue();
    }
    
    asn().dotation_eff_ressource.n        = dotationsChanged_.size();
    asn().dotation_eff_ressource.elem     = pResources;
    asn().m.dotation_eff_ressourcePresent = 1;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroupContainer::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_DotationGroupContainer::SendFullState( NET_ASN_MsgUnitAttributes& asn ) const
{
    uint nNbrDotations = 0;
    for( CIT_DotationGroupMap itDotationGroup = dotationGroups_.begin(); itDotationGroup != dotationGroups_.end(); ++itDotationGroup )
    {
        const PHY_DotationGroup::T_DotationMap& dotations = itDotationGroup->second->GetDotations();
        nNbrDotations += dotations.size();
    }

    if( nNbrDotations == 0 )
        return;

    ASN1T_ResourceDotations* pResources = new ASN1T_ResourceDotations[ nNbrDotations ];
    uint i = 0;

    for( CIT_DotationGroupMap itDotationGroup = dotationGroups_.begin(); itDotationGroup != dotationGroups_.end(); ++itDotationGroup )
    {
        const PHY_DotationGroup::T_DotationMap& dotations = itDotationGroup->second->GetDotations();
        for( PHY_DotationGroup::CIT_DotationMap itDotation = dotations.begin(); itDotation != dotations.end(); ++itDotation )
        {
            const PHY_Dotation& dotation          = *itDotation->second;
            ASN1T_ResourceDotations& asnRessource = pResources[ i++ ];
            asnRessource.ressource_id             = dotation.GetCategory().GetMosID();
            asnRessource.quantite_disponible      = (uint)dotation.GetValue();
        }
    }

    asn().dotation_eff_ressource.n        = nNbrDotations;
    asn().dotation_eff_ressource.elem     = pResources;
    asn().m.dotation_eff_ressourcePresent = 1;
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

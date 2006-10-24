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

#include "PHY_DotationGroup.h"

#include "PHY_Dotation.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationCategory.h"
#include "PHY_AmmoDotationClass.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequestContainer.h"

BOOST_CLASS_EXPORT_GUID( PHY_DotationGroup, "PHY_DotationGroup" )

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationGroup::PHY_DotationGroup( const PHY_DotationType& type, PHY_DotationGroupContainer& groupContainer )
    : pType_          ( &type )
    , pGroupContainer_( &groupContainer )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_DotationGroup::PHY_DotationGroup()
    : pType_          ( 0 )
    , pGroupContainer_( 0 )
{
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

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_DotationGroup::T_DotationMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_DotationGroup::T_DotationMap& map, const uint )
        {
            file << map.size();
            for ( PHY_DotationGroup::CIT_DotationMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID();
                file << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_DotationGroup::T_DotationMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
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
void PHY_DotationGroup::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nID;
    file >> nID;
    pType_ = PHY_DotationType::FindDotationType( nID );
    
    file >> pGroupContainer_
         >> dotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_DotationGroup::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << ( pType_ ? pType_->GetID() : (uint)-1 )
         << pGroupContainer_
         << dotations_;
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::ReadValues
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationGroup::ReadValues( MIL_InputArchive& archive, const PHY_DotationCategory& category )
{
    PHY_Dotation* pDotation = GetDotation( category );
    if( !pDotation )        
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation", archive.GetContext() );

    pDotation->ReadValue( archive );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_DotationGroup::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        archive.Section( "dotation" );
        archive.WriteAttribute( "name"    , it->first->GetName() );
        archive.WriteAttribute( "quantity", it->second->GetValue() );
        archive.EndSection(); // dotation
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
        pDotation = new PHY_Dotation( dotationCategory, *this );
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
MT_Float PHY_DotationGroup::GetCapacity( const PHY_DotationCategory& category ) const
{
    const PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->GetCapacity() : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::GetValue
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroup::GetValue( const PHY_DotationCategory& category ) const
{
    const PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->GetValue() : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Consume
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroup::Consume( const PHY_DotationCategory& category, MT_Float rNbr )
{
    PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->Consume( rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Supply
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
MT_Float PHY_DotationGroup::Supply( const PHY_DotationCategory& category, MT_Float rNbr )
{
    PHY_Dotation* pDotation = GetDotation( category );
    return pDotation ? pDotation->Supply( rNbr ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Resupply( MT_Float rFactor /* = 1.*/ )
{
    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->Resupply( rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationGroup::Resupply
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_DotationGroup::Resupply( const PHY_AmmoDotationClass& ammoDotationClass, MT_Float rFactor )
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
MT_Float PHY_DotationGroup::AddConsumptionReservation( const PHY_DotationCategory& category, MT_Float rNbr )
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
MT_Float PHY_DotationGroup::AddFireReservation( const PHY_DotationCategory& category, MT_Float rNbr )
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
void PHY_DotationGroup::ChangeDotationsValueUsingTC2( const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor, MIL_AutomateLOG& tc2 ) const
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
// Name: PHY_DotationGroup::NotifyTakenPrisoner
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_DotationGroup::NotifyTakenPrisoner()
{
    assert( pType_ );
    
    if ( *pType_ == PHY_DotationType::ration_ )
        return;

    for( CIT_DotationMap it = dotations_.begin(); it != dotations_.end(); ++it )
        it->second->NotifyTakenPrisoner();
}

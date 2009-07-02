// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 16:22 $
// $Revision: 14 $
// $Workfile: PHY_RolePion_Dotations.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RolePion_Dotations.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include <xeumeuleu/xml.h>



BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Dotations, "PHY_RolePion_Dotations" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::PHY_RolePion_Dotations( MT_RoleContainer& roleContainer, MIL_AgentPion& pion )
    : PHY_RoleInterface_Dotations( roleContainer )
    , pPion_                     ( &pion )
    , pCurrentConsumptionMode_   ( 0 )
    , pPreviousConsumptionMode_  ( 0 )
    , reservedConsumptions_      ()
    , pDotations_                ( 0 )
{
    pDotations_ = new PHY_DotationGroupContainer( *this );
    pion.GetType().GetUnitType().GetTC1Capacities().RegisterCapacities( *pDotations_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::PHY_RolePion_Dotations()
    : PHY_RoleInterface_Dotations()
    , pPion_                     ( 0 )
    , pCurrentConsumptionMode_   ( 0 )
    , pPreviousConsumptionMode_  ( 0 )
    , reservedConsumptions_      ()
    , pDotations_                ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::~PHY_RolePion_Dotations()
{
    delete pDotations_;
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
        void serialize( Archive& file, PHY_RolePion_Dotations::T_DotationReservedMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Dotations::T_DotationReservedMap& map, const uint )
        {
            unsigned size = map.size();
            file << size;
            for ( PHY_RolePion_Dotations::CIT_DotationReservedMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Dotations::T_DotationReservedMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                PHY_Dotation* pDotation;
                file >> pDotation;
                file >> map[ pDotation ];
            }            
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Dotations >( *this )
         >> pPion_
         >> pDotations_;
         
    uint nID;
    file >> nID;
    pCurrentConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( nID );
    
    file >> nID;
    pPreviousConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( nID );
    
    file >> reservedConsumptions_;
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned current  = ( pCurrentConsumptionMode_  ? pCurrentConsumptionMode_->GetID()  : (uint)-1 ) ,
             previous = ( pPreviousConsumptionMode_ ? pPreviousConsumptionMode_->GetID() : (uint)-1 );
    file << boost::serialization::base_object< PHY_RoleInterface_Dotations >( *this )
         << pPion_
         << pDotations_
         << current
         << previous
         << reservedConsumptions_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::WriteODB( xml::xostream& xos ) const
{
    pDotations_->WriteODB( xos );
}

// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ReadOverloading
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ReadOverloading( xml::xistream& xis )
{
    assert( pDotations_ );
    pDotations_->ReadValues( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::RegisterDotationsCapacities
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::RegisterDotationsCapacities( const PHY_DotationCapacities& capacities )
{
    assert( pDotations_ );
    capacities.RegisterCapacities( *pDotations_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::UnregisterDotationsCapacities
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::UnregisterDotationsCapacities( const PHY_DotationCapacities& capacities )
{
    assert( pDotations_ );
    capacities.UnregisterCapacities( *pDotations_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifyCaptured
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifyCaptured()
{
    assert( pDotations_ );
    pDotations_->NotifyCaptured();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifyReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifyReleased()
{
    assert( pDotations_ );
    pDotations_->NotifyReleased();
}

// =============================================================================
// CONSUMPTIONS
// =============================================================================
struct sConsumptionReservation : private boost::noncopyable
{

public:
    sConsumptionReservation( const PHY_ConsumptionType& consumptionMode, PHY_DotationGroupContainer& dotations )
        : consumptionMode_( consumptionMode )  
        , dotations_      ( dotations )
        , bReservationOK_ ( true )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( !bReservationOK_ )
            return;
        const PHY_DotationConsumptions* pConsumptions = composante.GetDotationConsumptions( consumptionMode_ );
        if( pConsumptions )
            bReservationOK_ = pConsumptions->RegisterConsumptionReservations( dotations_ );
    }

    const PHY_ConsumptionType&        consumptionMode_;
          PHY_DotationGroupContainer& dotations_;
          bool                        bReservationOK_;
};

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SetConsumptionMode
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_Dotations::SetConsumptionMode( const PHY_ConsumptionType& consumptionMode )
{
    if( pCurrentConsumptionMode_ && consumptionMode < *pCurrentConsumptionMode_  )
        return true;

    assert( pDotations_ ); 
    pDotations_->CancelConsumptionReservations();

    sConsumptionReservation func( consumptionMode, *pDotations_ );
    GetRole< PHY_RolePion_Composantes >().Apply( func );

    if( func.bReservationOK_ )
    {
        pPreviousConsumptionMode_ = pCurrentConsumptionMode_;
        pCurrentConsumptionMode_  = &consumptionMode;
        return true;
    }

    // Rollback
    pDotations_->CancelConsumptionReservations();
    if( pCurrentConsumptionMode_ )
    {
        sConsumptionReservation funcRollback( *pCurrentConsumptionMode_, *pDotations_ );
        GetRole< PHY_RolePion_Composantes >().Apply( funcRollback );
        assert( funcRollback.bReservationOK_ );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::RollbackConsumptionMode
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::RollbackConsumptionMode()
{
    assert( pDotations_ );
    pCurrentConsumptionMode_ = 0;
    if( pPreviousConsumptionMode_ )
        SetConsumptionMode( *pPreviousConsumptionMode_ );
    else
        pDotations_->CancelConsumptionReservations();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::sConsumptionTimeExpectancy
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
struct sConsumptionTimeExpectancy : private boost::noncopyable
{
    
public:
    sConsumptionTimeExpectancy( const PHY_ConsumptionType& consumptionMode )
        : consumptionMode_ ( consumptionMode )
        , consumptions_    ()
    {
    }
    
    void operator() ( const PHY_ComposantePion& composante )
    {
        const PHY_DotationConsumptions* pConsumptions = composante.GetDotationConsumptions( consumptionMode_ );
        
        if ( pConsumptions )
            pConsumptions->AddConsumptionValues( consumptions_ );
    }

    MT_Float GetNbrTicksForConsumption( const PHY_DotationGroupContainer& dotations ) const
    {
        MT_Float rNbrTicks = std::numeric_limits< MT_Float >::max();
        
        for ( PHY_DotationConsumptions::CIT_ConsumptionValue it = consumptions_.begin(); it != consumptions_.end(); ++it )
        {
            assert( it->first );
            assert( it->second > 0. );
        
            const MT_Float rCurrentDotationValue = dotations.GetValue( *it->first );
            
            rNbrTicks = std::min( rNbrTicks, rCurrentDotationValue / it->second );
        }
        
        return rNbrTicks;
    }
    
private:
    const PHY_ConsumptionType&                         consumptionMode_;
          PHY_DotationConsumptions::T_ConsumptionValue consumptions_;    
};


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::PHY_RolePion_Dotations::GetMaxTimeForConsumption
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Dotations::GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const
{
    assert( pDotations_ );
    sConsumptionTimeExpectancy func( mode );
    GetRole< PHY_RolePion_Composantes >().Apply( func );
    return func.GetNbrTicksForConsumption( *pDotations_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetConsumptionMode
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_RolePion_Dotations::GetConsumptionMode() const
{
    if( !pPreviousConsumptionMode_ )
        return PHY_ConsumptionType::engineStopped_;
    return *pPreviousConsumptionMode_;
}

// =============================================================================
// LOGISTIC - SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ChangeDotationsValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor ) const
{
    assert( pPion_ );
    
    MIL_AutomateLOG* pTC2 = pPion_->GetAutomate().GetTC2();
    if( !pTC2 )
        return;
    assert( pDotations_ );
    pDotations_->ChangeDotationsValueUsingTC2( dotationType, pAmmoDotationClass, rCapacityFactor, *pTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    assert( pPion_ );   

    if( bNewNeed )
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_LogisticDotationThresholdExceeded, dotationCategory );

    pPion_->GetAutomate().NotifyDotationSupplyNeeded( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::Update( bool bIsDead )
{
    if( bIsDead )
        return;

    assert( pDotations_ );
    if( !pCurrentConsumptionMode_ )
        SetConsumptionMode( GetRole< PHY_RolePion_Posture >().GetCurrentPosture().GetConsumptionMode() );
    pDotations_->ConsumeConsumptionReservations();
    pPreviousConsumptionMode_ = pCurrentConsumptionMode_;
    pCurrentConsumptionMode_  = 0;

    pDotations_->ConsumeFireReservations();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Dotations::HasChanged() const
{
    assert( pDotations_ );
    return pDotations_->HasChanged();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::Clean()
{
    assert( pDotations_ );
    pDotations_->Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetDotationValue
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Dotations::GetDotationValue( const PHY_DotationCategory& category ) const
{
    assert( pDotations_ );
    return pDotations_->GetValue( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::HasDotation
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Dotations::HasDotation( const PHY_DotationCategory& category ) const
{
    assert( pDotations_ );
    return pDotations_->GetValue( category ) > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetDotationCapacity
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Dotations::GetDotationCapacity( const PHY_DotationCategory& category ) const
{
    assert( pDotations_ );
    return pDotations_->GetCapacity( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ConsumeDotation
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Dotations::ConsumeDotation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pDotations_ );
    return pDotations_->Consume( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SupplyDotation
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Dotations::SupplyDotation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pDotations_ );
    return pDotations_->Supply( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ResupplyDotations
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ResupplyDotations()
{
    assert( pDotations_ );
    pDotations_->Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ResupplyDotations
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ResupplyDotations( const PHY_AmmoDotationClass& ammoDotationClass, MT_Float rFactor )
{
    assert( pDotations_ );
    pDotations_->Resupply( ammoDotationClass, rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ResupplyDotations
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ResupplyDotations( const PHY_DotationType& type, MT_Float rFactor )
{
    assert( pDotations_ );
    pDotations_->Resupply( type, rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Dotations::AddFireReservation( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pDotations_ );
    return pDotations_->AddFireReservation( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::FillSupplyRequest
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::FillSupplyRequest( PHY_SupplyDotationRequestContainer& supplyRequestContainer ) const
{
    assert( pDotations_ );
    pDotations_->FillSupplyRequest( supplyRequestContainer );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const
{
    assert( pDotations_ );
    pDotations_->SendChangedState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::SendFullState( NET_ASN_MsgUnitAttributes& asn ) const
{
    assert( pDotations_ );
    pDotations_->SendFullState( asn );
}

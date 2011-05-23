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
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include <xeumeuleu/xml.hpp>

#include "simulation_kernel/AlgorithmsFactories.h"

#include "simulation_kernel/ConsumptionComputer_ABC.h"
#include "simulation_kernel/ConsumptionComputerFactory_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"
#include "simulation_kernel/DotationComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/ConsumptionModeChangeRequest_ABC.h"

#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"

#include "simulation_kernel/NetworkNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( dotation::PHY_RolePion_Dotations )

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
            void serialize( Archive& file, dotation::PHY_RolePion_Dotations::T_DotationReservedMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const dotation::PHY_RolePion_Dotations::T_DotationReservedMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for ( dotation::PHY_RolePion_Dotations::CIT_DotationReservedMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, dotation::PHY_RolePion_Dotations::T_DotationReservedMap& map, const unsigned int )
        {
            std::size_t nNbr;
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

namespace dotation
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Dotations( *pion, true );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::PHY_RolePion_Dotations( MIL_AgentPion& pion, bool fromArchive /*= false*/ )
    : pion_                      ( pion )
    , pCurrentConsumptionMode_   ( 0 )
    , pPreviousConsumptionMode_  ( 0 )
    , reservedConsumptions_      ()
    , pDotations_                ( 0 )
{
    pDotations_ = new PHY_DotationGroupContainer( *this, fromArchive? false : MIL_AgentServer::GetWorkspace().GetEntityManager().HasInfiniteDotations() );
    pion.GetType().GetUnitType().GetTC1Capacities().RegisterCapacities( *pDotations_ );
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> ::boost::serialization::base_object< PHY_RoleInterface_Dotations >( *this )
         >> pDotations_;

    unsigned int nID;
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
void PHY_RolePion_Dotations::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned current  = ( pCurrentConsumptionMode_  ? pCurrentConsumptionMode_->GetID()  : (unsigned int)-1 ) ,
             previous = ( pPreviousConsumptionMode_ ? pPreviousConsumptionMode_->GetID() : (unsigned int)-1 );
    file << ::boost::serialization::base_object< PHY_RoleInterface_Dotations >( *this )
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
    assert( pDotations_ );
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
    pDotations_->ReadDotations( xis );
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
class sConsumptionReservation : public ::OnComponentFunctor_ABC
{

public:
    sConsumptionReservation( const PHY_ConsumptionType& consumptionMode, PHY_DotationGroupContainer& dotations )
        : consumptionMode_( consumptionMode )
        , dotations_      ( dotations )
        , bReservationOK_ ( true )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
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
    std::auto_ptr< OnComponentComputer_ABC > dotationComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( func ) );
    pion_.Execute( *dotationComputer );

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
        std::auto_ptr< OnComponentComputer_ABC > dotationComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( funcRollback ) );
        pion_.Execute( *dotationComputer );
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
class sConsumptionTimeExpectancy : public ::OnComponentFunctor_ABC
{

public:
    sConsumptionTimeExpectancy( const PHY_ConsumptionType& consumptionMode )
        : consumptionMode_ ( consumptionMode )
        , consumptions_    ()
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        const PHY_DotationConsumptions* pConsumptions = composante.GetDotationConsumptions( consumptionMode_ );

        if( pConsumptions )
            pConsumptions->AddConsumptionValues( consumptions_ );
    }

    double GetNbrTicksForConsumption( const PHY_DotationGroupContainer& dotations ) const
    {
        double rNbrTicks = std::numeric_limits< double >::max();

        for ( PHY_DotationConsumptions::CIT_ConsumptionValue it = consumptions_.begin(); it != consumptions_.end(); ++it )
        {
            assert( it->first );
            assert( it->second > 0. );

            const double rCurrentDotationValue = dotations.GetValue( *it->first );

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
double PHY_RolePion_Dotations::GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const
{
    assert( pDotations_ );
    sConsumptionTimeExpectancy func( mode );
    std::auto_ptr< OnComponentComputer_ABC > dotationComputer( pion_.GetAlgorithms().onComponentFunctorComputerFactory_->Create( func ) );
    pion_.Execute( *dotationComputer );
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
void PHY_RolePion_Dotations::ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor ) const
{
    MIL_AutomateLOG* pTC2 = pion_.GetLogisticHierarchy().GetPrimarySuperior();
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
    if( bNewNeed )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_LogisticDotationThresholdExceeded, dotationCategory );

    pion_.GetAutomate().NotifyDotationSupplyNeeded( dotationCategory );
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

    std::auto_ptr< ConsumptionComputer_ABC > consumptionComputer = pion_.GetAlgorithms().consumptionComputerFactory_->CreateConsumptionComputer();
    pion_.Execute( *consumptionComputer );
    SetConsumptionMode( consumptionComputer->Result() );

    pDotations_->ConsumeConsumptionReservations();
    pPreviousConsumptionMode_ = pCurrentConsumptionMode_;
    pCurrentConsumptionMode_  = 0;

    pDotations_->ConsumeFireReservations();

    if( HasChanged() )
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
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
// Name: PHY_RolePion_Dotations::NotifyConsumeDotation
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifyConsumeDotation( const PHY_DotationCategory& category, double rNbr )
{
    assert( pDotations_ );
    double nConsumed = pDotations_->Consume( category, rNbr );
    assert( nConsumed == rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SupplyDotation
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
double PHY_RolePion_Dotations::SupplyDotation( const PHY_DotationCategory& category, double rNbr )
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
void PHY_RolePion_Dotations::ResupplyDotations( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor )
{
    assert( pDotations_ );
    pDotations_->Resupply( ammoDotationClass, rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ResupplyDotations
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ResupplyDotations( const PHY_DotationType& type, double rFactor )
{
    assert( pDotations_ );
    pDotations_->Resupply( type, rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::AddFireReservation
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double PHY_RolePion_Dotations::AddFireReservation( const PHY_DotationCategory& category, double rNbr )
{
    assert( pDotations_ );
    return pDotations_->AddFireReservation( category, rNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetDotationNumber
// Created: LDC 2010-01-08
// -----------------------------------------------------------------------------
double PHY_RolePion_Dotations::GetDotationNumber( const PHY_DotationCategory& category ) const
{
    return pDotations_->GetValue( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetIlluminationDotations
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_RolePion_Dotations::GetIlluminationDotations( float range, bool permanent ) const
{
    return pDotations_->GetIlluminationDotations( range, permanent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetIlluminatingRange
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
float PHY_RolePion_Dotations::GetIlluminatingRange( ) const
{
    return pDotations_->GetIlluminatingRange( );
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
void PHY_RolePion_Dotations::SendChangedState( client::UnitAttributes& asn ) const
{
    assert( pDotations_ );
    pDotations_->SendChangedState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::SendFullState( client::UnitAttributes& asn ) const
{
    assert( pDotations_ );
    pDotations_->SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::Execute( dotation::DotationComputer_ABC& algorithm ) const
{
    algorithm.SetDotationContainer( *pDotations_ );
    algorithm.SetForbiddenAmmunition( &forbiddenDotations_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ChangeConsumptionMode(ConsumptionModeChangeRequest_ABC& request)
{
    bool ok = SetConsumptionMode(request.GetType());
    request.ConsumptionModeChanged(ok, this);
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::SetForbiddenDotoation
// Created: HBD 2010-04-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::SetForbiddenDotation( const PHY_DotationCategory& category )
{
    std::vector< const PHY_DotationCategory* >::iterator it;
    for( it = forbiddenDotations_.begin(); it != forbiddenDotations_.end(); ++it )
        if( (*it)->GetName() == category.GetName() )
            break;
    if( it == forbiddenDotations_.end() )
    {
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_MunitionInterdite, category );
        forbiddenDotations_.push_back( &category );
    }
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::RemoveForbiddenDotation
// Created: HBD 2010-04-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::RemoveForbiddenDotation( const PHY_DotationCategory& category )
{
    std::vector< const PHY_DotationCategory* >::iterator it;
    for( it = forbiddenDotations_.begin(); it != forbiddenDotations_.end(); ++it )
        if( (*it)->GetName() == category.GetName() )
            break;
    if( it != forbiddenDotations_.end() )
    {
       MIL_Report::PostEvent( pion_, MIL_Report::eReport_MunitionAutorise, category );
       forbiddenDotations_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::AllowAllDotations
// Created: HBD 2010-04-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::AllowAllDotations()
{
     forbiddenDotations_.clear();
}


} // namespace dotation


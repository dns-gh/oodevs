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
#include "AlgorithmsFactories.h"
#include "ConsumptionComputer_ABC.h"
#include "ConsumptionComputerFactory_ABC.h"
#include "ConsumptionModeChangeRequest_ABC.h"
#include "DotationComputer_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "OnComponentFunctorComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include "WeaponAvailabilityComputerFactory_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationConsumptions.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MIL_AgentServer.h"
#include "tools/ExerciseSettings.h"

BOOST_CLASS_EXPORT_IMPLEMENT( dotation::PHY_RolePion_Dotations )

using namespace dotation;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::PHY_RolePion_Dotations()
    : owner_                   ( 0 )
    , pCurrentConsumptionMode_ ( 0 )
    , pPreviousConsumptionMode_( 0 )
    , pDotations_              ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::PHY_RolePion_Dotations( MIL_AgentPion& pion )
    : owner_                   ( &pion )
    , pCurrentConsumptionMode_ ( 0 )
    , pPreviousConsumptionMode_( 0 )
    , pDotations_              ( 0 )
{
    pDotations_ = new PHY_DotationGroupContainer( *this, MIL_AgentServer::GetWorkspace().GetSettings().GetValue< bool >( "infinite-dotation" ) );
    pion.GetType().GetUnitType().GetTC1Capacities().RegisterCapacities( *pDotations_, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Dotations::~PHY_RolePion_Dotations()
{
    delete pDotations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> ::boost::serialization::base_object< PHY_RoleInterface_Dotations >( *this )
         >> owner_
         >> pDotations_
         >> pCurrentConsumptionMode_
         >> pPreviousConsumptionMode_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << ::boost::serialization::base_object< PHY_RoleInterface_Dotations >( *this )
         << owner_
         << pDotations_
         << pCurrentConsumptionMode_
         << pPreviousConsumptionMode_;
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ReadOverloading
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ReadOverloading( xml::xistream& xis )
{
    assert( pDotations_ );
    pDotations_->ReadDotations( xis, owner_->GetType().GetUnitType() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::RegisterDotationsCapacities
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::RegisterDotationsCapacities( const PHY_DotationCapacities& capacities, T_Dotations*& dotations )
{
    assert( pDotations_ );
    capacities.RegisterCapacities( *pDotations_, dotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::UnregisterDotationsCapacities
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::UnregisterDotationsCapacities( const PHY_DotationCapacities& capacities, T_Dotations*& dotations )
{
    assert( pDotations_ );
    T_Dotations dotationsRemoved = capacities.UnregisterCapacities( *pDotations_ );
    if( dotations )
        *dotations = dotationsRemoved;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifyCaptured
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifyCaptured()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifyReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifyReleased()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifySurrendered
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifySurrendered()
{
    assert( pDotations_ );
    pDotations_->NotifyCaptured();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifySurrenderCanceled
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifySurrenderCanceled()
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
    sConsumptionReservation( const PHY_ConsumptionType& consumptionMode, PHY_DotationGroupContainer& dotations, bool isTransported )
        : consumptionMode_( consumptionMode )
        , dotations_      ( dotations )
        , bReservationOK_ ( true )
        , bIsTransported_ ( isTransported )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( !bReservationOK_ )
            return;
        const PHY_DotationConsumptions* pConsumptions = composante.GetDotationConsumptions( consumptionMode_ );
        if( pConsumptions )
            bReservationOK_ = pConsumptions->RegisterConsumptionReservations( dotations_, bIsTransported_ );
    }

    const PHY_ConsumptionType& consumptionMode_;
    PHY_DotationGroupContainer& dotations_;
    bool bReservationOK_;
    bool bIsTransported_;
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

    const transport::PHY_RolePion_Transported& transportRole = owner_->GetRole< transport::PHY_RolePion_Transported >();
    bool bIsTransported = transportRole.IsTransported() || transportRole.HasHumanTransportersToRecover();

    sConsumptionReservation func( consumptionMode, *pDotations_, bIsTransported );
    std::auto_ptr< OnComponentComputer_ABC > dotationComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( func ) );
    owner_->Execute( *dotationComputer );

    if( func.bReservationOK_ )
    {
        pPreviousConsumptionMode_ = pCurrentConsumptionMode_;
        pCurrentConsumptionMode_ = &consumptionMode;
        return true;
    }

    // Rollback
    if( pCurrentConsumptionMode_ || &GetConsumptionMode() != &PHY_ConsumptionType::engineStopped_ )
        pDotations_->CancelConsumptionReservations();
    if( pCurrentConsumptionMode_ )
    {
        sConsumptionReservation funcRollback( *pCurrentConsumptionMode_, *pDotations_, bIsTransported );
        std::auto_ptr< OnComponentComputer_ABC > dotationComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( funcRollback ) );
        owner_->Execute( *dotationComputer );
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
    {}

    void operator() ( PHY_ComposantePion& composante )
    {
        const PHY_DotationConsumptions* pConsumptions = composante.GetDotationConsumptions( consumptionMode_ );
        if( pConsumptions )
            pConsumptions->AddConsumptionValues( consumptions_ );
    }

    double GetNbrTicksForConsumption( const PHY_DotationGroupContainer& dotations ) const
    {
        double rNbrTicks = std::numeric_limits< double >::max();
        for ( auto it = consumptions_.begin(); it != consumptions_.end(); ++it )
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
    std::auto_ptr< OnComponentComputer_ABC > dotationComputer( owner_->GetAlgorithms().onComponentFunctorComputerFactory_->Create( func ) );
    owner_->Execute( *dotationComputer );
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::ChangeDotationsValueUsingTC2
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor ) const
{
    MIL_AutomateLOG* pTC2 = owner_->GetLogisticHierarchy().GetPrimarySuperior();
    if( !pTC2 )
        return;
    assert( pDotations_ );
    pDotations_->ChangeDotationsValueUsingTC2( dotationType, pAmmoDotationClass, rCapacityFactor, *pTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::EnforceAviationResources
// Created: JSR 2013-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::EnforceAviationResources( E_AviationRange aviationRange )
{
    MIL_AutomateLOG* pTC2 = owner_->GetLogisticHierarchy().GetPrimarySuperior();
    if( !pTC2 )
        return;
    assert( pDotations_ );
    pDotations_->EnforceAviationResources( aviationRange, GetPion()->GetType().GetUnitType(), *pTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    if( bNewNeed )
    {
        MIL_Report::PostEvent( *owner_, report::eRC_SeuilLogistiqueDotationDepasse, dotationCategory );
        if( owner_->GetType().IsRefugee() || owner_->GetRole< surrender::PHY_RoleInterface_Surrender >().IsPrisoner() )
            MIL_Report::PostEvent( *owner_, report::eRC_PrisonersUnsupplied );
    }
    if( !owner_->IsJammed() && !owner_->IsLogisticJammed() )
        owner_->GetAutomate().NotifyDotationSupplyNeeded( dotationCategory );
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
    std::auto_ptr< ConsumptionComputer_ABC > consumptionComputer = owner_->GetAlgorithms().consumptionComputerFactory_->CreateConsumptionComputer();
    owner_->Execute( *consumptionComputer );
    SetConsumptionMode( consumptionComputer->Result() );

    pDotations_->ConsumeConsumptionReservations();
    pDotations_->UpdateSupplyNeeded();
    pPreviousConsumptionMode_ = pCurrentConsumptionMode_;
    pCurrentConsumptionMode_ = 0;

    pDotations_->ConsumeFireReservations();

    if( HasChanged() )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
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
void PHY_RolePion_Dotations::NotifyConsumeDotation( const PHY_DotationCategory& category, double& rNbr )
{
    assert( pDotations_ );
    double nConsumed = pDotations_->Consume( category, rNbr );
    rNbr -= nConsumed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::GetPion
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
const MIL_AgentPion* PHY_RolePion_Dotations::GetPion() const
{
    return owner_;
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
void PHY_RolePion_Dotations::ResupplyDotations( bool withLog )
{
    assert( pDotations_ );
    pDotations_->Resupply( withLog );
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
// Name: PHY_RolePion_Dotations::ChangeDotation
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::ChangeDotation( const PHY_DotationCategory& category, unsigned int number, float threshold )
{
    assert( pDotations_ );
    pDotations_->ChangeDotation( category, number, threshold );
}

void PHY_RolePion_Dotations::ChangeDotation( const PHY_DotationCategory& category, double fraction )
{
    pDotations_->Supply( category, pDotations_->GetCapacity( category ) * fraction );
}

void PHY_RolePion_Dotations::ChangeDotation( const PHY_DotationType& type, double fraction )
{
    pDotations_->SupplyAll( type, fraction );
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
// Name: PHY_RolePion_Dotations::GetFuelDotationNumber
// Created: JSR 2012-07-31
// -----------------------------------------------------------------------------
double PHY_RolePion_Dotations::GetFuelDotationNumber() const
{
    assert( pDotations_ );
    return pDotations_->GetFuelDotationNumber();
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
// Name: PHY_RolePion_Dotations::Apply
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RolePion_Dotations::Apply( boost::function< void( PHY_Dotation& ) > visitor ) const
{
    assert( pDotations_ );
    pDotations_->Apply( visitor );
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
    const PHY_ConsumptionType* current = pCurrentConsumptionMode_;
    const PHY_ConsumptionType* previous = pPreviousConsumptionMode_;
    bool ok = SetConsumptionMode(request.GetType());
    request.ConsumptionModeChanged(ok, this);
    pCurrentConsumptionMode_ = current;
    pPreviousConsumptionMode_ = previous;
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
        MIL_Report::PostEvent( *owner_, report::eRC_MunitionInterdite, category );
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
       MIL_Report::PostEvent( *owner_, report::eRC_MunitionAutorise, category );
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Dotations::HasDotationForFiring
// Created: LDC 2011-06-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Dotations::HasDotationForFiring( const PHY_DotationCategory& category, int iterations )
{
    return GetDotationNumber( category ) >= category.ConvertToNbrAmmo( static_cast< double >( iterations ) );
}

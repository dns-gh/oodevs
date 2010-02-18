// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_Human.cpp $
// $Author: Jvt $
// $Modtime: 29/04/05 11:15 $
// $Revision: 13 $
// $Workfile: PHY_Human.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Human.h"
#include "PHY_HumanRank.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "Entities/Orders/MIL_Report.h"
#include "MIL_AgentServer.h"
#include "MIL_Singletons.h"

#include "simulation_kernel/HumansActionsNotificationHandler_ABC.h"

#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_GUID( PHY_Human, "PHY_Human" )

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human( const MIL_Time_ABC& time, PHY_HumansComposante& composante )
    : time_           ( time )
    , pComposante_    ( &composante )
    , pRank_          ( &PHY_HumanRank::militaireDuRang_ )
    , pWound_         ( &PHY_HumanWound::notWounded_     )
    , bMentalDiseased_( false )
    , bContamined_    ( false )
    , nLocation_      ( eBattleField )
    , pMedicalState_  ()
    , nDeathTimeStep_ ( std::numeric_limits< uint >::max() )
{
    pComposante_->NotifyHumanAdded( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human( const PHY_Human& rhs )
    : time_           ( rhs.time_            )
    , pComposante_    ( 0                    )
    , pRank_          ( rhs.pRank_           )
    , pWound_         ( rhs.pWound_          )
    , bMentalDiseased_( rhs.bMentalDiseased_ )
    , bContamined_    ( rhs.bContamined_     )
    , nLocation_      ( rhs.nLocation_       )
    , pMedicalState_  ( rhs.pMedicalState_   )
    , nDeathTimeStep_ ( rhs.nDeathTimeStep_  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human()
    : time_           ( MIL_Singletons::GetTime() )
    , pComposante_    ( 0 )
    , pRank_          ( 0 )
    , pWound_         ( 0 )
    , bMentalDiseased_( false )
    , bContamined_    ( false )
    , nLocation_      ( eBattleField )
    , pMedicalState_  ( 0 )
    , nDeathTimeStep_ ( std::numeric_limits< uint >::max() )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Human destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_Human::~PHY_Human()
{
    if( pComposante_ )
        pComposante_->NotifyHumanRemoved( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Human::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pComposante_;
    
    uint nID;
    file >> nID;
    pRank_ = PHY_HumanRank::Find( nID );
    
    file >> nID;
    pWound_ = PHY_HumanWound::Find( nID );
    
    file >> bMentalDiseased_
         >> bContamined_
         >> nLocation_
         >> pMedicalState_
         >> nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Human::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pRank_ );
    assert( pWound_ );

    unsigned rank  = pRank_->GetID(),
             wound = pWound_->GetID();
    file << pComposante_
         << rank
         << wound
         << bMentalDiseased_
         << bContamined_
         << nLocation_
         << pMedicalState_
         << nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyHumanChanged
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
inline
void PHY_Human::NotifyHumanChanged( const PHY_Human& oldHumanState )
{
    assert( pComposante_ );
    pComposante_->NotifyHumanChanged( *this, oldHumanState );
    
    if( pMedicalState_ )
        pMedicalState_->NotifyHumanChanged();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::CancelLogisticRequest
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::CancelLogisticRequest()
{
    assert( pComposante_ );
    if( pMedicalState_ )
    {
        PHY_Human oldHumanState( *this );
        const_cast< MIL_Agent_ABC& >( pComposante_->GetComposante().GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanBackFromMedical, *pMedicalState_ );
        if( pComposante_->GetComposante().GetState() == PHY_ComposanteState::maintenance_ )
            nLocation_ = eMaintenance;
        else 
            nLocation_ = eBattleField;
        pMedicalState_->Cancel();
        delete pMedicalState_;
        pMedicalState_ = 0;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Evacuate
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_Human::Evacuate( MIL_AutomateLOG& destinationTC2 )
{
    if( NeedEvacuation() )
        const_cast< MIL_Agent_ABC& >( pComposante_->GetComposante().GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanEvacuatedByThirdParty, *this, destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Heal
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
void PHY_Human::Heal()
{
    CancelLogisticRequest();
    HealMentalDisease    ();
    HealContamination    ();
    SetWound             ( PHY_HumanWound::notWounded_ ); //$$$ NB : don't use HealWound() => 'cause it don't heal deads ...    
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::HealMentalDisease
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_Human::HealMentalDisease()
{
    if( !IsMentalDiseased() )
        return;
    PHY_Human oldHumanState( *this );
    bMentalDiseased_ = false;
    NotifyHumanChanged( oldHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::HealWound
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_Human::HealWound()
{
    if( !IsWounded() )
        return;
    PHY_Human oldHumanState( *this );
    pWound_ = &PHY_HumanWound::notWounded_;
    nDeathTimeStep_ = std::numeric_limits< uint >::max();
    NotifyHumanChanged( oldHumanState );
}
 
// -----------------------------------------------------------------------------
// Name: PHY_Human::HealContamination
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_Human::HealContamination()
{
    if( !IsContaminated() )
        return;
    PHY_Human oldHumanState( *this );
    bContamined_ = false;
    NotifyHumanChanged( oldHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyWound
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
bool PHY_Human::ApplyWound( const PHY_HumanWound& newWound )
{
    if( !IsUsable() )
        return false;

    assert( pWound_ );
    if( newWound > *pWound_ )
        return SetWound( newWound );

    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyPoisonous
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool PHY_Human::ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination )
{
    return contamination.ApplyRandomWound( boost::bind( &PHY_Human::ApplyWound, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyContamination
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_Human::ApplyContamination( const MIL_ToxicEffectManipulator& /*contamination*/ )
{
    if( !IsUsable() )
        return;

    if( !bContamined_ )
    {
        PHY_Human oldHumanState( *this );
        bContamined_ = true;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyMentalDisease
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::ApplyMentalDisease()
{
    if( !IsUsable() || IsWounded() || bMentalDiseased_ )
        return;

    if( PHY_HumanWound::ChooseMentalDisease() )
    {
        PHY_Human oldHumanState( *this );
        bMentalDiseased_ = true;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SetRank
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_Human::SetRank( const PHY_HumanRank& newRank )
{
    PHY_Human oldHumanState( *this );
    if( newRank == *pRank_ )
        return false;
    pRank_ = &newRank;
    NotifyHumanChanged( oldHumanState );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SetWound
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_Human::SetWound( const PHY_HumanWound& newWound )
{
    PHY_Human oldHumanState( *this );
    if( newWound == *pWound_ )
        return false;
    pWound_ = &newWound;

    if( *pWound_ == PHY_HumanWound::killed_ )
    {
        nDeathTimeStep_  = 0;
        bMentalDiseased_ = false;
        bContamined_     = false;        
    }
    else if( *pWound_ == PHY_HumanWound::notWounded_ )
        nDeathTimeStep_ = std::numeric_limits< uint >::max();
    else
        nDeathTimeStep_ = std::min( nDeathTimeStep_, time_.GetCurrentTick() + pWound_->GetLifeExpectancy() );
    
    NotifyHumanChanged( oldHumanState );   

    // !!!! $$$ Must be called after NotifyHumanChanged() (CancelLogisticRequest() call NotifyHumanChanged() too
    if( !NeedMedical() )
        CancelLogisticRequest();

    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyBackToWar
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
bool PHY_Human::NotifyBackToWar()
{
    assert( pComposante_ );
    assert( pMedicalState_ );

    //$$$ BOF - PB gestion interrogation �tat composante quand modif �tat humain (doit �tre fait par composante, ou par humain ?) (fait par composante tout le temps, sauf dans ce cas ...)
    if( pComposante_->GetComposante().GetState() == PHY_ComposanteState::dead_ )
        return false;

    CancelLogisticRequest();
    MIL_Report::PostEvent( pComposante_->GetComposante().GetRole().GetPion(), MIL_Report::eReport_HumanBackFromMedical );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyHandledByMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::NotifyHandledByMedical()
{
    assert( nLocation_ != eMedical );

    PHY_Human oldHumanState( *this );
    nLocation_ = eMedical;
    NotifyHumanChanged( oldHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyComposanteHandledByMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::NotifyComposanteHandledByMaintenance()
{
    if( nLocation_ == eBattleField )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eMaintenance;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::NotifyComposanteBackFromMaintenance()
{
    //assert( nLocation_ != eBattleField ); <== Peut asserter si la composante meurt avant d'avoir �t� r�cup�r�e par le remorqueur (=> appel de NotifyComposanteBackFromMaintenance(), mais composante pas encore dans maintenance, juste pMaintenanceState pour la demande)
    if( nLocation_ == eMaintenance )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eBattleField;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Update
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::Update()
{
    assert( pComposante_ );

    if( time_.GetCurrentTick() >= nDeathTimeStep_ )
    {
        MIL_Report::E_EngineReport nReportID;
        if( !pMedicalState_ )
            nReportID = MIL_Report::eReport_WoundedManDeath;
        else if( pMedicalState_->IsInAmbulance() )
            nReportID = MIL_Report::eReport_WoundedManDeathDuringTransport;
        else
            nReportID = MIL_Report::eReport_WoundedManDeathDuringHospitalization;

        if( SetWound( PHY_HumanWound::killed_ ) )
            MIL_Report::PostEvent( pComposante_->GetComposante().GetRole().GetPion(), nReportID );
    }

    // Demande sant�
    if( NeedMedical() && !pMedicalState_ )
        const_cast< MIL_Agent_ABC& >( pComposante_->GetComposante().GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanWaitingForMedical, *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetRank
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
const PHY_HumanRank& PHY_Human::GetRank() const
{
    assert( pRank_ );
    return *pRank_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetWound
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
const PHY_HumanWound& PHY_Human::GetWound() const
{
    assert( pWound_ );
    return *pWound_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetLocation
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
PHY_Human::E_Location PHY_Human::GetLocation() const
{
    return nLocation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsDead
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
bool PHY_Human::IsDead() const
{
    return *pWound_ == PHY_HumanWound::killed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsAvailable
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
bool PHY_Human::IsUsable() const
{
    return !IsDead() && nLocation_ != eMedical;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsWounded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
bool PHY_Human::IsWounded() const
{
    assert( pWound_ );
    return *pWound_ != PHY_HumanWound::killed_ && *pWound_ != PHY_HumanWound::notWounded_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsContaminated
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_Human::IsContaminated() const
{
    return bContamined_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsMentalDiseased
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_Human::IsMentalDiseased() const
{
    return bMentalDiseased_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_Human::IsAnEmergency() const
{
    assert( pWound_ );
    return *pWound_ == PHY_HumanWound::woundedU1_ || *pWound_ == PHY_HumanWound::woundedUE_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NeedMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_Human::NeedMedical() const
{
    return !IsDead() && ( IsWounded() || IsContaminated() || IsMentalDiseased() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NeedEvacuation
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_Human::NeedEvacuation()
{
    return NeedMedical() && !pMedicalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SetMedicalState
// Created: MGd 2009-10-01
// -----------------------------------------------------------------------------
void PHY_Human::SetMedicalState( PHY_MedicalHumanState* pMedicalState )
{
    pMedicalState_ = pMedicalState;
}

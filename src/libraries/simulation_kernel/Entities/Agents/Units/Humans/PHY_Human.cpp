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
#include "Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_NbcAgent.h"
#include "Entities/RC/MIL_RC.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( PHY_Human, "PHY_Human" )

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human( PHY_HumansComposante& composante )
    : pComposante_    ( &composante )
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
    : pComposante_    ( 0                    )
    , pRank_          ( rhs.pRank_           )
    , pWound_         ( rhs.pWound_          )
    , bMentalDiseased_( rhs.bMentalDiseased_ )
    , bContamined_    ( rhs.bContamined_     )
    , nLocation_      ( rhs.nLocation_       )
    , pMedicalState_  ( rhs.pMedicalState_   )
    , nDeathTimeStep_ ( rhs.nDeathTimeStep_  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human()
    : pComposante_    ( 0 )
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

// =============================================================================
// CHECKPOINTS
// =============================================================================

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

    file << pComposante_
         << pRank_->GetID()
         << pWound_->GetID()
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
        pComposante_->NotifyHumanBackFromMedical( *pMedicalState_ );
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
        pMedicalState_ = pComposante_->NotifyHumanEvacuatedByThirdParty( *this, destinationTC2 );
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
bool PHY_Human::ApplyPoisonous( const MIL_NbcAgent& nbcAgent )
{
    return ApplyWound( nbcAgent.GetRandomWound() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyContamination
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_Human::ApplyContamination( const MIL_NbcAgent& /*nbcAgent*/ )
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
        nDeathTimeStep_ = std::min( nDeathTimeStep_, MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pWound_->GetLifeExpectancy() );
    
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

    //$$$ BOF - PB gestion interrogation état composante quand modif état humain (doit être fait par composante, ou par humain ?) (fait par composante tout le temps, sauf dans ce cas ...)
    if( pComposante_->GetComposante().GetState() == PHY_ComposanteState::dead_ )
        return false;

    CancelLogisticRequest();
    MIL_RC::pRcHumainRetourDeSante_->Send( pComposante_->GetComposante().GetRole().GetPion(), MIL_RC::eRcTypeOperational );
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

// =============================================================================
// COMPOSANTE MAINTENANCE
// =============================================================================

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
    //assert( nLocation_ != eBattleField ); <== Peut asserter si la composante meurt avant d'avoir été récupérée par le remorqueur (=> appel de NotifyComposanteBackFromMaintenance(), mais composante pas encore dans maintenance, juste pMaintenanceState pour la demande)
    if( nLocation_ == eMaintenance )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eBattleField;
        NotifyHumanChanged( oldHumanState );
    }
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Human::Update
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::Update()
{
    assert( pComposante_ );

    if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nDeathTimeStep_ )
    {
        const MIL_RC* pRcToSend = 0;
        if( !pMedicalState_ )
            pRcToSend = MIL_RC::pRcDecesBlesse_;
        else if( pMedicalState_->IsInAmbulance() )
            pRcToSend = MIL_RC::pRcDecesBlessePendantTransport_;
        else
            pRcToSend = MIL_RC::pRcDecesBlessePendantHospitalisation_;

        if( SetWound( PHY_HumanWound::killed_ ) )
            pRcToSend->Send( pComposante_->GetComposante().GetRole().GetPion(), MIL_RC::eRcTypeEvent );
    }

    // Demande santé
    if( NeedMedical() && !pMedicalState_ )
        pMedicalState_ = pComposante_->NotifyHumanWaitingForMedical( *this );
}

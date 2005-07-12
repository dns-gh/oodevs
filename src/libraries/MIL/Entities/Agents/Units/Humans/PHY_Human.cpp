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

#include "MIL_Pch.h"

#include "PHY_Human.h"
#include "PHY_HumanRank.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/RC/MIL_RC.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human( PHY_ComposantePion& composante )
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
    pRank_ = PHY_HumanRank::FindHumanRank( nID );
    
    file >> nID;
    pWound_ = PHY_HumanWound::FindHumanWound( nID );
    
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
    file << pComposante_
         << ( pRank_  ? pRank_->GetID()  : (uint)-1 )
         << ( pWound_ ? pWound_->GetID() : (uint)-1 )
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
// Name: PHY_Human::NotifyBackFromMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
void PHY_Human::NotifyBackFromMedical()
{
    assert( pComposante_ );
    if( pMedicalState_ )
    {
        pComposante_->NotifyHumanBackFromMedical( *pMedicalState_ );
        if ( pComposante_->GetState() == PHY_ComposanteState::maintenance_ )
            nLocation_ = eMaintenance;
        else 
            nLocation_ = eBattleField;
        pMedicalState_->Cancel();
        delete pMedicalState_;
        pMedicalState_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyBackToWar
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_Human::NotifyBackToWar()
{
    assert( pComposante_ );
    assert( pMedicalState_ );

    PHY_Human oldHumanState( *this );

    pComposante_->NotifyHumanBackFromMedical( *pMedicalState_ );
    if ( pComposante_->GetState() == PHY_ComposanteState::maintenance_ )
        nLocation_ = eMaintenance;
    else
        nLocation_ = eBattleField;
        
    pComposante_->NotifyHumanChanged( *this, oldHumanState );
    pMedicalState_->Cancel();
    delete pMedicalState_;
    pMedicalState_ = 0;
}


// -----------------------------------------------------------------------------
// Name: PHY_Human::ChangeRank
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_Human::ChangeRank( const PHY_HumanRank& newRank )
{
    PHY_Human oldHumanState( *this );
    if( newRank == *pRank_ )
        return false;
    pRank_ = &newRank;
    NotifyHumanChanged( oldHumanState );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ChangeWound
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_Human::ChangeWound( const PHY_HumanWound& newWound )
{
    PHY_Human oldHumanState( *this );
    if( newWound == *pWound_ )
        return false;
    pWound_ = &newWound;

    if( *pWound_ == PHY_HumanWound::notWounded_ )
    {
        nDeathTimeStep_ = std::numeric_limits< uint >::max();
        NotifyBackFromMedical();
    }
    else if( *pWound_ == PHY_HumanWound::killed_ )
    {
        nDeathTimeStep_ = 0;
        NotifyBackFromMedical();
    }
    else
        nDeathTimeStep_ = std::min( nDeathTimeStep_, MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pWound_->GetLifeExpectancy() );

    NotifyHumanChanged( oldHumanState );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::HealMentalDisease
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_Human::HealMentalDisease()
{
    assert( IsMentalDiseased() );
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
    assert( IsWounded() );
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
    assert( IsContaminated() );
    PHY_Human oldHumanState( *this );
    bContamined_ = false;
    NotifyHumanChanged( oldHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyWound
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_Human::ApplyWound()
{
    return ChangeWound( pWound_->ApplyRandomWound() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyWound
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
bool PHY_Human::ApplyWound( const MIL_NbcAgentType& nbcAgentType )
{
    if( !bContamined_ )
    {
        PHY_Human oldHumanState( *this );
        bContamined_ = true;
        NotifyHumanChanged( oldHumanState );
    }
    return ChangeWound( pWound_->Degrade( nbcAgentType.GetRandomWound() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Resupply
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
void PHY_Human::Resupply()
{
    if( *pWound_ == PHY_HumanWound::notWounded_ && !bContamined_ && !bMentalDiseased_ )
        return;

    PHY_Human oldHumanState( *this );
    bContamined_     = false;
    bMentalDiseased_ = false;
    pWound_          = &PHY_HumanWound::notWounded_;
    nDeathTimeStep_  = std::numeric_limits< uint >::max();
    if( pComposante_->GetState() == PHY_ComposanteState::maintenance_ )
        nLocation_ = eMaintenance;
    else 
        nLocation_ = eBattleField; 
    NotifyBackFromMedical();
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
    //assert( nLocation_ != eBattleField ); <== Peut asserté si la composante meurt avant d'avoir été récupérée par le remorqueur (=> appel de NotifyComposanteBackFromMaintenance(), mais composante pas encore dans maintenance, juste pMaintenanceState pour la demande)
    if( nLocation_ == eMaintenance )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eBattleField;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ApplyMentalDisease
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::ApplyMentalDisease()
{
    if ( bMentalDiseased_ )
        return;

    if ( PHY_HumanWound::ChooseMentalDesease() )
    {
        PHY_Human oldHumanState( *this );
        bMentalDiseased_ = true;
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
    assert( NeedMedical() );
    assert( pComposante_ );

    if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() >= nDeathTimeStep_ )
    {
        if( ChangeWound( PHY_HumanWound::killed_ ) )
        {
            if( !pMedicalState_ )
                MIL_RC::pRcDecesBlesse_->Send( pComposante_->GetRole().GetPion(), MIL_RC::eRcTypeEvent );
            else if( pMedicalState_->IsInAmbulance() )
                MIL_RC::pRcDecesBlessePendantTransport_->Send( pComposante_->GetRole().GetPion(), MIL_RC::eRcTypeEvent );
            else
                MIL_RC::pRcDecesBlessePendantHospitalisation_->Send( pComposante_->GetRole().GetPion(), MIL_RC::eRcTypeEvent );
        }
        return;
    }

    // Demande santé
    if( !pMedicalState_ )
        pMedicalState_ = pComposante_->NotifyHumanWaitingForMedical( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::CancelLogisticRequest
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void PHY_Human::CancelLogisticRequest()
{
    NotifyBackFromMedical();
}


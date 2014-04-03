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
#include "HumansComposante_ABC.h"
#include "PHY_HumanRank.h"
#include "PHY_HumanWound.h"
#include "MIL_Time_ABC.h"
#include "HumansActionsNotificationHandler_ABC.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/FuneralConsign.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h" 

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_Human )

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human( const MIL_Time_ABC& time, HumansComposante_ABC& composante )
    : time_( time )
    , pComposante_( &composante )
    , pRank_( &PHY_HumanRank::militaireDuRang_ )
    , pWound_( &PHY_HumanWound::notWounded_ )
    , bMentalDiseased_( false )
    , bContamined_( false )
    , nLocation_( eHumanLocation_Battlefield )
    , nDeathTimeStep_( std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human( const PHY_Human& rhs )
    : time_( rhs.time_ )
    , pComposante_( 0 )
    , pRank_( rhs.pRank_ )
    , pWound_( rhs.pWound_ )
    , bMentalDiseased_( rhs.bMentalDiseased_ )
    , bContamined_( rhs.bContamined_ )
    , nLocation_( rhs.nLocation_ )
    , pMedicalState_( rhs.pMedicalState_ )
    , nDeathTimeStep_( rhs.nDeathTimeStep_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_Human::PHY_Human()
    : time_( MIL_Time_ABC::GetTime() )
    , pComposante_( 0 )
    , pRank_( 0 )
    , pWound_( 0 )
    , bMentalDiseased_( false )
    , bContamined_( false )
    , nLocation_( eHumanLocation_Battlefield )
    , nDeathTimeStep_( std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
PHY_Human::~PHY_Human()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Human::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< Human_ABC >( *this );
    file >> pComposante_;
    unsigned int nID;
    file >> nID;
    pRank_ = PHY_HumanRank::Find( nID );
    file >> pWound_
         >> bMentalDiseased_
         >> bContamined_
         >> nLocation_
         >> pMedicalState_
         >> nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
void PHY_Human::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pRank_ );
    assert( pWound_ );
    unsigned int rank  = pRank_->GetID();
    file << boost::serialization::base_object< Human_ABC >( *this );
    file << pComposante_
         << rank
         << pWound_
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
void PHY_Human::NotifyHumanChanged( const Human_ABC& oldHumanState )
{
    assert( pComposante_ );
    pComposante_->NotifyHumanChanged( *this, oldHumanState );
    if( pMedicalState_ )
        pMedicalState_->NotifyHumanChanged();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::CancelLogisticRequests
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::CancelLogisticRequests()
{
    CancelMedicalLogisticRequest();
    funeralConsign_.reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::CancelMedicalLogisticRequest
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::CancelMedicalLogisticRequest()
{
    assert( pComposante_ );
    if( pMedicalState_ )
    {
        PHY_Human oldHumanState( *this );
        if( pComposante_->GetComposante().GetState() == PHY_ComposanteState::maintenance_ )
            nLocation_ = eHumanLocation_Maintenance;
        else
            nLocation_ = eHumanLocation_Battlefield;
        pMedicalState_->Cancel();
        pMedicalState_.reset();
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsJammed
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
bool PHY_Human::IsJammed() const
{
    return GetPion().IsJammed() || GetPion().IsLogisticJammed();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Evacuate
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_Human::Evacuate( MIL_AutomateLOG& destinationTC2 )
{
    if( NeedEvacuation() )
        const_cast< MIL_Agent_ABC& >( GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanEvacuatedByThirdParty, *this, destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Heal
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
void PHY_Human::Heal()
{
    CancelMedicalLogisticRequest();
    HealMentalDisease();
    HealContamination();
    SetWound( PHY_HumanWound::notWounded_ ); //$$$ NB : don't use HealWound() => 'cause it doesn't heal deads ...
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
    nDeathTimeStep_ = std::numeric_limits< unsigned int >::max();
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
// Name: PHY_Human::ApplyContamination
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_Human::ApplyContamination()
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
        ForceMentalDisease();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ForceMentalDisease
// Created: ABR 2011-03-07
// -----------------------------------------------------------------------------
void PHY_Human::ForceMentalDisease()
{
    if( bMentalDiseased_ )
        return;
    PHY_Human oldHumanState( *this );
    bMentalDiseased_ = true;
    NotifyHumanChanged( oldHumanState );
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
    if( newWound == *pWound_ )
    {
        if( IsDead() && pMedicalState_ )
            CancelMedicalLogisticRequest();
        return false;
    }
    PHY_Human oldHumanState( *this );
    pWound_ = &newWound;
    if( *pWound_ == PHY_HumanWound::killed_ )
    {
        nDeathTimeStep_ = 0;
        bMentalDiseased_ = false;
        bContamined_ = false;
    }
    else if( *pWound_ == PHY_HumanWound::notWounded_ )
        nDeathTimeStep_ = std::numeric_limits< unsigned int >::max();
    else
        nDeathTimeStep_ = std::min( nDeathTimeStep_, time_.GetCurrentTimeStep() + pWound_->GetLifeExpectancy() );
    NotifyHumanChanged( oldHumanState );
    // !!!! $$$ Must be called after NotifyHumanChanged() (CancelLogisticRequest() call NotifyHumanChanged() too
    if( !IsDead() && !NeedMedical() )
        CancelMedicalLogisticRequest();
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
    CancelMedicalLogisticRequest();
    MIL_Report::PostEvent( GetPion(), report::eRC_HumainRetourDeSante );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyHandledByMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::NotifyHandledByMedical()
{
    if( nLocation_ != eHumanLocation_Medical )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eHumanLocation_Medical;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyComposanteHandledByMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::NotifyComposanteHandledByMaintenance()
{
    if( nLocation_ == eHumanLocation_Battlefield )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eHumanLocation_Maintenance;
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
    if( nLocation_ == eHumanLocation_Maintenance )
    {
        PHY_Human oldHumanState( *this );
        nLocation_ = eHumanLocation_Battlefield;
        NotifyHumanChanged( oldHumanState );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::NotifyHandledByFuneral()
{
    PHY_Human oldHumanState( *this );
    nLocation_ = eHumanLocation_Funeral;
    NotifyHumanChanged( oldHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_Human::NotifyBackFromFuneral()
{
    PHY_Human oldHumanState( *this );
    assert( pComposante_ );
    if( pComposante_->GetComposante().GetState() == PHY_ComposanteState::maintenance_ )
        nLocation_ = eHumanLocation_Maintenance;
    else
        nLocation_ = eHumanLocation_Battlefield;
    NotifyHumanChanged( oldHumanState );
}

namespace
{
    const MIL_DecisionalReport& GetReport( PHY_MedicalHumanState* pMedicalState )
    {
        if( !pMedicalState )
            return report::eRC_DecesBlesse;
        else if( pMedicalState->IsInAmbulance() )
            return report::eRC_DecesBlessePendantTransport;
        return report::eRC_DecesBlessePendantHospitalisation;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::NotifyDiagnosed
// Created: AHC 2011-01-26
// -----------------------------------------------------------------------------
void PHY_Human::NotifyDiagnosed()
{
    if( pWound_ && *pWound_ != PHY_HumanWound::killed_ && *pWound_ != PHY_HumanWound::notWounded_ )
        nDeathTimeStep_ += static_cast< unsigned int >( static_cast< double >( pWound_->GetLifeExpectancy() ) * ( PHY_HumanWound::GetDiagnosisLifeExpectancyFactor() - 1.0 ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Update
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_Human::Update()
{
    assert( pComposante_ );
    if( time_.GetCurrentTimeStep() >= nDeathTimeStep_ )
    {
        const MIL_DecisionalReport& nReportID = GetReport( pMedicalState_.get() );
        if( SetWound( PHY_HumanWound::killed_ ) )
        {
            MIL_Report::PostEvent( GetPion(), nReportID );
            if( pMedicalState_ )
                MIL_Report::PostEvent( GetPion(), report::eRC_DecesBlesse );
        }
    }
    if( !IsJammed() )
    {
        if( NeedMedical() && !pMedicalState_ )
            const_cast< MIL_Agent_ABC& >( GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanWaitingForMedical, *this );
        if( IsDead() && !funeralConsign_ && GetPion().GetLogisticHierarchy().GetPrimarySuperior() )
            funeralConsign_.reset( new logistic::FuneralConsign( *this ) );
        else if( !IsDead() || funeralConsign_ && funeralConsign_->IsFinished() )
            funeralConsign_.reset();
    }
    if( funeralConsign_ )
        funeralConsign_->Update();
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
E_HumanLocation PHY_Human::GetLocation() const
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
// Name: PHY_Human::IsSeriouslyPhysicallyWounded
// Created: JSR 2011-09-15
// -----------------------------------------------------------------------------
bool PHY_Human::IsSeriouslyPhysicallyWounded() const
{
    assert( pWound_ );
    return *pWound_ == PHY_HumanWound::woundedU2_ || *pWound_ == PHY_HumanWound::woundedU1_ || *pWound_ == PHY_HumanWound::woundedUE_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::IsAvailable
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
bool PHY_Human::IsUsable() const
{
    return !IsDead() && nLocation_ != eHumanLocation_Medical;
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
// Name: PHY_Human::NeedUpdate
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_Human::NeedUpdate() const
{
    return IsDead() || NeedMedical() || funeralConsign_ || pMedicalState_;
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
bool PHY_Human::NeedEvacuation() const
{
    return NeedMedical() && !pMedicalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SetMedicalState
// Created: MGD 2009-10-01
// -----------------------------------------------------------------------------
void PHY_Human::SetMedicalState( const boost::shared_ptr< PHY_MedicalHumanState >& medicalState )
{
    pMedicalState_ = medicalState;
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::GetPion
// Created: NLD 2009-10-01
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& PHY_Human::GetPion() const
{
    return pComposante_->GetComposante().GetRole().GetPion();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::ShouldGoBackToWar
// Created: LDC 2013-12-02
// -----------------------------------------------------------------------------
bool PHY_Human::ShouldGoBackToWar() const
{
    return pWound_->ShouldGoBackToWar()
        && ( !IsContaminated() || PHY_HumanWound::ShouldContaminatedGoBackToWar() )
        && ( !IsMentalDiseased() || PHY_HumanWound::ShouldMentalDiseasedGoBackToWar() );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SetState
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
void PHY_Human::SetState( const PHY_HumanWound& newWound, bool mentalDisease, bool contaminated )
{
    PHY_Human oldHumanState( *this );

    bMentalDiseased_ = mentalDisease;
    bContamined_ = contaminated;
    pWound_ = &newWound;
    if( *pWound_ == PHY_HumanWound::killed_ )
    {
        nDeathTimeStep_ = 0;
        assert( !bMentalDiseased_ );
        assert( !bContamined_ );
    }
    else if( *pWound_ == PHY_HumanWound::notWounded_ )
        nDeathTimeStep_ = std::numeric_limits< unsigned int >::max();
    else
        nDeathTimeStep_ = time_.GetCurrentTimeStep() + pWound_->GetLifeExpectancy();

    NotifyHumanChanged( oldHumanState );
    // !!!! $$$ Must be called after NotifyHumanChanged() (CancelLogisticRequest() call NotifyHumanChanged() too
    if( !NeedMedical() ) //$$$$ NLD 2011-10-03 - Quelle merde ! à faire dans l'Update
        CancelMedicalLogisticRequest();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::Clean
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
void PHY_Human::Clean()
{
    if( funeralConsign_ )
        funeralConsign_->Clean();
    if( pMedicalState_ )
        pMedicalState_->Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SendFullsState
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
void PHY_Human::SendFullState( unsigned int context ) const
{
    if( funeralConsign_ )
        funeralConsign_->SendFullState( context );
    if( pMedicalState_ )
        pMedicalState_->SendFullState( context );
}

// -----------------------------------------------------------------------------
// Name: PHY_Human::SendChangedState
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
void PHY_Human::SendChangedState() const
{
    if( funeralConsign_ )
        funeralConsign_->SendChangedState();
    if( pMedicalState_ )
        pMedicalState_->SendChangedState();
}

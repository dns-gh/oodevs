// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RolePion_Humans.cpp $
// $Author: Jvt $
// $Modtime: 28/04/05 16:51 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Humans.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RolePion_Humans.h"

#include "Entities/Agents/Roles/Network/NET_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHumanState.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Humans, "PHY_RolePion_Humans" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::T_HumanData::T_HumanData
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::T_HumanData::T_HumanData()
    : nNbrTotal_                ( 0 )
    , nNbrOperational_          ( 0 )
    , nNbrDead_                 ( 0 )
    , nNbrWounded_              ( 0 )
    , nNbrMentalDiseased_       ( 0 )
    , nNbrNBC_                  ( 0 )
    , nNbrInLogisticMedical_    ( 0 )
    , nNbrInLogisticMaintenance_( 0 )
    , bHasChanged_              ( true )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Humans::T_HumanData::serialize( Archive& file, const uint )
{
    file & nNbrTotal_                
         & nNbrOperational_
         & nNbrDead_                 
         & nNbrWounded_              
         & nNbrMentalDiseased_       
         & nNbrNBC_                  
         & nNbrInLogisticMedical_    
         & nNbrInLogisticMaintenance_
         & bHasChanged_;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::PHY_RolePion_Humans( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Humans( role )
    , pPion_                  ( &pion )
    , humansData_             ( PHY_HumanRank::GetHumanRanks().size(), T_HumanData() )
    , nNbrHumansDataChanged_  ( humansData_.size() )
    , nNbrUsableHumans_       ( 0 )
    , nNbrHumans_             ( 0 )
    , humansToUpdate_         ()
    , medicalHumanStates_     ()
    , nTickRcMedicalQuerySent_( 0 )
    , nEvacuationMode_        ( eEvacuationMode_Auto )
{
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::PHY_RolePion_Humans()
    : PHY_RoleInterface_Humans()
    , pPion_                  ()
    , humansData_             ()
    , nNbrUsableHumans_       ( 0 )
    , nNbrHumansDataChanged_  ( 0 )
    , nNbrHumans_             ( 0 )
    , humansToUpdate_         ()
    , medicalHumanStates_     ()
    , nTickRcMedicalQuerySent_( 0 )
    , nEvacuationMode_        ( eEvacuationMode_Auto )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::~PHY_RolePion_Humans()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Humans::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Humans >( *this )
         & pPion_
         & humansData_
         & nNbrUsableHumans_
         & nNbrHumans_ 
         & humansToUpdate_
         & nNbrHumansDataChanged_
         & medicalHumanStates_
         & nTickRcMedicalQuerySent_;
}

namespace 
{
    struct sRankData
    {
        sRankData() : nNbrOfficiers_( 0 ), nNbrSousOfficiers_( 0 ), nNbrMdrs_( 0 ) {}
        uint nNbrOfficiers_;
        uint nNbrSousOfficiers_;
        uint nNbrMdrs_;
    };
    typedef std::map< const PHY_HumanWound*, sRankData > T_HumanWoundData;
    typedef T_HumanWoundData::const_iterator             CIT_HumanWoundData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::WriteODB( MT_XXmlOutputArchive& archive ) const
{
/*    archive.Section( "Personnels" );

    T_HumanWoundData humanWoundData;
    for( CIT_HumanSet it = humansToUpdate_.begin(); it != humansToUpdate_.end(); ++it )
    {
        const PHY_Human& human = **it;
        if( !human.IsWounded() )
            continue;
        
        if( human.GetRank() == PHY_HumanRank::militaireDuRang_ )
            ++ humanWoundData[ &human.GetWound() ].nNbrMdrs_;

        else if( human.GetRank() == PHY_HumanRank::officier_ )
            ++ humanWoundData[ &human.GetWound() ].nNbrOfficiers_;

        else if( human.GetRank() == PHY_HumanRank::sousOfficier_ )
            ++ humanWoundData[ &human.GetWound() ].nNbrSousOfficiers_;
    }

    for( CIT_HumanWoundData it = humanWoundData.begin(); it != humanWoundData.end(); ++ it )
    {
        archive.Section( "Personnel" );
        archive.WriteAttribute( "etat"             , it->first->GetName()           );
        archive.WriteAttribute( "nbOfficier"       , it->second.nNbrOfficiers_      );
        archive.WriteAttribute( "nbSousOfficier"   , it->second.nNbrSousOfficiers_  );
        archive.WriteAttribute( "nbMilitaireDuRang", it->second.nNbrMdrs_           );
                    
        archive.EndSection(); // Personnel
    }

    archive.EndSection(); // Personnels*/
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeHumansAvailability
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::ChangeHumansAvailability( const PHY_HumanRank& rank, uint nNewNbrFullyAliveHumans )
{
    const T_HumanData& humanData = humansData_[ rank.GetID() ];
    nNewNbrFullyAliveHumans = std::min( nNewNbrFullyAliveHumans, humanData.nNbrTotal_ );

    if( nNewNbrFullyAliveHumans > humanData.nNbrOperational_ )
        GetRole< PHY_RolePion_Composantes >().HealHumans( rank, nNewNbrFullyAliveHumans - humanData.nNbrOperational_ );
    else if( nNewNbrFullyAliveHumans < humanData.nNbrOperational_ )
        GetRole< PHY_RolePion_Composantes >().WoundHumans( rank, humanData.nNbrOperational_ - nNewNbrFullyAliveHumans );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::CureAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::HealAllHumans()
{
    GetRole< PHY_RolePion_Composantes >().HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Update( bool /*bIsDead*/ )
{
    for( CIT_HumanSet it = humansToUpdate_.begin(); it != humansToUpdate_.end(); )
    {
        PHY_Human& human = **it;
        ++it; 
        human.Update(); // !!! Can erase the human from humansToUpdate_
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Clean()
{
    if( nNbrHumansDataChanged_ > 0 )
    {
        for( IT_HumanDataVector it = humansData_.begin(); it != humansData_.end(); ++it )
            it->bHasChanged_ = false;
        nNbrHumansDataChanged_ = 0;
    }

    for( CIT_MedicalHumanStateSet it = medicalHumanStates_.begin(); it != medicalHumanStates_.end(); ++it )
        (**it).Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrHumans
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
uint PHY_RolePion_Humans::GetNbrHumans( const PHY_HumanRank& rank ) const
{
    const T_HumanData& humanData = humansData_[ rank.GetID() ];
    return humanData.nNbrTotal_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrAliveHumans
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
uint PHY_RolePion_Humans::GetNbrAliveHumans( const PHY_HumanRank& rank ) const
{
    const T_HumanData& humanData = humansData_[ rank.GetID() ];
    return humanData.nNbrOperational_;
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::UpdateDataWhenHumanRemoved
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::UpdateDataWhenHumanRemoved( const PHY_Human& human )
{
    assert( nNbrHumans_ > 0 );
    -- nNbrHumans_;

    T_HumanData& humanData = humansData_[ human.GetRank().GetID() ];
    if( !humanData.bHasChanged_ )
    {
        ++nNbrHumansDataChanged_;
        humanData.bHasChanged_ = true;
    }

    assert( humanData.nNbrTotal_ > 0 );
    -- humanData.nNbrTotal_;

    if( human.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        -- nNbrUsableHumans_;
    }
    if( human.IsDead() )
    {
        assert( humanData.nNbrDead_ > 0 );
        -- humanData.nNbrDead_;
    }
    if( human.IsWounded() )
    {
        assert( humanData.nNbrWounded_ > 0 );
        -- humanData.nNbrWounded_;
    }
    if( human.IsContaminated() )
    {
        assert( humanData.nNbrNBC_ > 0 );
        -- humanData.nNbrNBC_;
    }
    if( human.IsMentalDiseased() )
    {
        assert( humanData.nNbrMentalDiseased_ > 0 );
        -- humanData.nNbrMentalDiseased_;
    }
    if( human.IsUsable() && !human.NeedMedical() )
    {
        assert( humanData.nNbrOperational_ > 0 );
        -- humanData.nNbrOperational_;
    }

    switch( human.GetLocation() )
    {
        case PHY_Human::eBattleField: break;
        case PHY_Human::eMaintenance: assert( humanData.nNbrInLogisticMaintenance_ > 0 ); -- humanData.nNbrInLogisticMaintenance_; break;
        case PHY_Human::eMedical    : assert( humanData.nNbrInLogisticMedical_     > 0 ); -- humanData.nNbrInLogisticMedical_; break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::UpdateDataWhenHumanAdded
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::UpdateDataWhenHumanAdded( const PHY_Human& human )
{
    ++ nNbrHumans_;

    T_HumanData& humanData = humansData_[ human.GetRank().GetID() ];
    if( !humanData.bHasChanged_ )
    {
        ++nNbrHumansDataChanged_;
        humanData.bHasChanged_ = true;
    }
    ++ humanData.nNbrTotal_;

    if( human.IsUsable() )
        ++ nNbrUsableHumans_;
    if( human.IsDead() )
        ++ humanData.nNbrDead_;
    if( human.IsWounded() )
        ++ humanData.nNbrWounded_;
    if( human.IsContaminated() )
        ++ humanData.nNbrNBC_;
    if( human.IsMentalDiseased() )
        ++ humanData.nNbrMentalDiseased_;
    if( human.IsUsable() && !human.NeedMedical() )
        ++ humanData.nNbrOperational_;

    switch( human.GetLocation() )
    {
        case PHY_Human::eBattleField: break;
        case PHY_Human::eMaintenance: ++ humanData.nNbrInLogisticMaintenance_; break;
        case PHY_Human::eMedical    : ++ humanData.nNbrInLogisticMedical_    ; break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanAdded( PHY_Human& human )
{
    UpdateDataWhenHumanAdded( human );
    if( human.NeedMedical() )
        humansToUpdate_.insert( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanRemoved( PHY_Human& human )
{
    UpdateDataWhenHumanRemoved( human );
    humansToUpdate_.erase( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState )
{
    UpdateDataWhenHumanRemoved( copyOfOldHumanState );
    UpdateDataWhenHumanAdded  ( human );

    const bool bOldHumanNeedUpdate = copyOfOldHumanState.NeedMedical();
    const bool bNewHumanNeedUpdate = human              .NeedMedical();

    if( bOldHumanNeedUpdate && !bNewHumanNeedUpdate )
        humansToUpdate_.erase( &human );
    else if( !bOldHumanNeedUpdate && bNewHumanNeedUpdate )
        humansToUpdate_.insert( &human );
}

// =============================================================================
// LOGISTIC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    GetRole< PHY_RolePion_Composantes >().EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Humans::HasWoundedHumansToEvacuate() const
{
    return GetRole< PHY_RolePion_Composantes >().HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 )
{
    assert( pPion_ );
    
    PHY_MedicalHumanState* pMedicalHumanState = destinationTC2.MedicalHandleHumanEvacuatedByThirdParty( *pPion_, human );
    if( !pMedicalHumanState )
        return 0;
    
    bool bOut = medicalHumanStates_.insert( pMedicalHumanState ).second;
    assert( bOut );
    return pMedicalHumanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanWaitingForMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePion_Humans::NotifyHumanWaitingForMedical( PHY_Human& human )
{
    assert( pPion_ );
    
    MIL_AutomateLOG* pTC2 = pPion_->GetAutomate().GetTC2();
    if ( !pTC2 || nEvacuationMode_ == eEvacuationMode_Manual )
        return 0;

    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const uint nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMedicalQuerySent_ + 1 ) || nTickRcMedicalQuerySent_ == 0 )
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_MedicalEvacuationRequest );
    nTickRcMedicalQuerySent_ = nCurrentTick;

    PHY_MedicalHumanState* pMedicalHumanState = pTC2->MedicalHandleHumanForEvacuation( *pPion_, human );
    if( !pMedicalHumanState )
        return 0;
    
    bool bOut = medicalHumanStates_.insert( pMedicalHumanState ).second;
    assert( bOut );
    return pMedicalHumanState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanBackFromMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanBackFromMedical( PHY_MedicalHumanState& humanState )
{
    int nOut = medicalHumanStates_.erase( &humanState );
    assert( nOut == 1 );
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const
{
    if( nNbrHumansDataChanged_ == 0 )
        return;
   
    assert( nNbrHumansDataChanged_ <= humansData_.size() );
 
    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    ASN1T_DotationPersonnel* pPersonnel = new ASN1T_DotationPersonnel[ nNbrHumansDataChanged_ ];

    uint i = 0;
    for( PHY_HumanRank::CIT_HumanRankMap itRank = ranks.begin(); itRank != ranks.end(); ++itRank )
    {
        const PHY_HumanRank&           rank      = *itRank->second;
        const T_HumanData&             humanData = humansData_[ rank.GetID() ];

        if( !humanData.bHasChanged_ )
            continue;
              
        ASN1T_DotationPersonnel& personnel = pPersonnel[ i++ ];

        personnel.rang                         = rank.GetAsnID();
        personnel.nb_total                     = humanData.nNbrTotal_;
        personnel.nb_operationnels             = humanData.nNbrOperational_;
        personnel.nb_morts                     = humanData.nNbrDead_;
        personnel.nb_blesses                   = humanData.nNbrWounded_;
        personnel.nb_blesses_mentaux           = humanData.nNbrMentalDiseased_;
        personnel.nb_contamines_nbc            = humanData.nNbrNBC_;
        personnel.nb_dans_chaine_sante         = humanData.nNbrInLogisticMedical_;
        personnel.nb_utilises_pour_maintenance = humanData.nNbrInLogisticMaintenance_;
    }
    assert( i == nNbrHumansDataChanged_ );

    asn().dotation_eff_personnel.n        = nNbrHumansDataChanged_;
    asn().dotation_eff_personnel.elem     = pPersonnel;
    asn().m.dotation_eff_personnelPresent = 1;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( NET_ASN_MsgUnitAttributes& asn ) const
{
    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    ASN1T_DotationPersonnel* pPersonnel = new ASN1T_DotationPersonnel[ ranks.size() ];

    uint i = 0;
    for( PHY_HumanRank::CIT_HumanRankMap itRank = ranks.begin(); itRank != ranks.end(); ++itRank )
    {
        const PHY_HumanRank&           rank      = *itRank->second;
        const T_HumanData&             humanData = humansData_[ rank.GetID() ];
              ASN1T_DotationPersonnel& personnel = pPersonnel[ i++ ];

        personnel.rang                         = rank.GetAsnID();
        personnel.nb_total                     = humanData.nNbrTotal_;
        personnel.nb_operationnels             = humanData.nNbrOperational_;
        personnel.nb_morts                     = humanData.nNbrDead_;
        personnel.nb_blesses                   = humanData.nNbrWounded_;
        personnel.nb_blesses_mentaux           = humanData.nNbrMentalDiseased_;
        personnel.nb_contamines_nbc            = humanData.nNbrNBC_;
        personnel.nb_dans_chaine_sante         = humanData.nNbrInLogisticMedical_;
        personnel.nb_utilises_pour_maintenance = humanData.nNbrInLogisticMaintenance_;
    }

    asn().dotation_eff_personnel.n        = ranks.size();
    asn().dotation_eff_personnel.elem     = pPersonnel;
    asn().m.dotation_eff_personnelPresent = 1;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendLogisticChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendLogisticChangedState() const
{
    for( CIT_MedicalHumanStateSet it = medicalHumanStates_.begin(); it != medicalHumanStates_.end(); ++it )
        (**it).SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendLogisticFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendLogisticFullState() const
{
    for( CIT_MedicalHumanStateSet it = medicalHumanStates_.begin(); it != medicalHumanStates_.end(); ++it )
        (**it).SendFullState();
}
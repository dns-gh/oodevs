// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_Convoy_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:26 $
// $Revision: 8 $
// $Workfile: PHY_Convoy_ABC.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_Convoy_ABC.h"
#include "PHY_SupplyConsign_ABC.h"
#include "PHY_RolePionLOG_Supply.h"
#include "PHY_SupplyDotationState.h"
#include "PHY_SupplyDotationConsign.h"
#include "Decision/DEC_ModelPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Tools/MIL_Tools.h"

      MT_InterpolatedFunction< MT_Float > PHY_Convoy_ABC::formingTime_;
const MIL_AgentTypePion*                  PHY_Convoy_ABC::pConvoyAgentType_   = 0;
const MIL_PionMissionType*                PHY_Convoy_ABC::pConvoyMissionType_ = 0;

BOOST_CLASS_EXPORT_GUID( PHY_Convoy_ABC, "PHY_Convoy_ABC" )

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::Initialize
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "Ravitaillement" );
    archive.Section( "Convois" );

    InitializeConvoyUnitType( archive );
    InitializeConvoyMission ( archive );
    InitializeFormingTimes  ( archive );
   
    archive.EndSection(); // Convois
    archive.EndSection(); // Ravitaillement
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeConvoyUnitType
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeConvoyUnitType( MIL_InputArchive& archive )
{
    archive.Section( "TypeUnite" );
    std::string strConvoyAgentType;
    archive.ReadAttribute( "nom", strConvoyAgentType );

    pConvoyAgentType_ = MIL_AgentTypePion::FindPionType( strConvoyAgentType );
    if( !pConvoyAgentType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown type for convoy", archive.GetContext() );

    archive.EndSection(); // TypeUnite
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeConvoyMission
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeConvoyMission( MIL_InputArchive& archive )
{
    archive.Section( "Mission" );
    std::string strMission;
    archive.ReadAttribute( "nom", strMission );

    pConvoyMissionType_ = MIL_PionMissionType::FindPionMissionType( strMission );
    if( !pConvoyMissionType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid mission name for convoy", archive.GetContext() );

    assert( pConvoyAgentType_ );
    if( !pConvoyAgentType_->GetModel().IsMissionAvailable( *pConvoyMissionType_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Convoy type pion cannot receive convoy mission", archive.GetContext() );

    archive.EndSection(); // Mission
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeFormingTimes
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeFormingTimes( MIL_InputArchive& archive )
{
    archive.BeginList( "TempsConstitution" );

    formingTime_.AddNewPoint( 0., 0. );
    uint     nMaxNbrCamions         = 0;
    MT_Float rTimeWhenMaxNbrCamions = 0.;
    while( archive.NextListElement() )
    {
        archive.Section( "Temps" );

        uint     nNbrCamions;
        MT_Float rTime;

        archive.ReadAttribute( "nbCamions", nNbrCamions, CheckValueGreater( 0  ) );
        archive.ReadTime     (              rTime      , CheckValueGreater( 0. ) );
        rTime = MIL_Tools::ConvertSecondsToSim( rTime );

        formingTime_.AddNewPoint( nNbrCamions, rTime ); 

        if( nNbrCamions >= nMaxNbrCamions )
        {
            rTimeWhenMaxNbrCamions = rTime;
            nMaxNbrCamions = nNbrCamions;
        }
        archive.EndSection(); // Temps
    }   
    formingTime_.SetAfterValue( rTimeWhenMaxNbrCamions );

    archive.EndList(); // TempsConstitution
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::Terminate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::Terminate()
{
    // NOTHING
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::sTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::sTransporter::sTransporter()
    : pTransporter_    ( 0 )
    , pTransporterPion_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::sTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::sTransporter::sTransporter( PHY_ComposantePion* pTransporter, MIL_AgentPion* pTransporterPion )
    : pTransporter_    ( pTransporter )
    , pTransporterPion_( pTransporterPion )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_Convoy_ABC::sTransporter::serialize( Archive& file, const uint )
{
    file & pTransporter_
         & pTransporterPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::PHY_Convoy_ABC( PHY_SupplyConsign_ABC& consign )
    : pConsign_         ( &consign )
    , pCommander_       ( 0 )
    , pCommanderPion_   ( 0 ) 
    , transporters_     ()
    , nFormingTime_     ( 0 )
{
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::PHY_Convoy_ABC()
    : pConsign_         ( 0 )
    , pCommander_       ( 0 )
    , pCommanderPion_   ( 0 ) 
    , transporters_     ()
    , nFormingTime_     ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::~PHY_Convoy_ABC()
{
    UnlockConvoy();
}


// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_Convoy_ABC::serialize( Archive& file, const uint )
{
    file & const_cast< PHY_SupplyConsign_ABC*& >( pConsign_ )
         & pCommander_
         & pCommanderPion_
         & transporters_
         & nFormingTime_;
}

// =============================================================================
// ACCESSORS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetSuppliedAutomate
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_Convoy_ABC::GetSuppliedAutomate() const
{
    assert( pConsign_ );
    return pConsign_->GetSuppliedAutomate();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::UnlockConvoy
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::UnlockConvoy()
{
    for( CIT_TransporterVector it = transporters_.begin(); it != transporters_.end(); ++it )
        it->pTransporterPion_->GetRole< PHY_RolePion_Supply >().StopUsingForLogistic( *it->pTransporter_ );
    transporters_.clear();
    if( pCommander_ )
        pCommanderPion_->GetRole< PHY_RolePion_Supply >().StopUsingForLogistic( *pCommander_ );
    pCommander_     = 0;
    pCommanderPion_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::Form
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
bool PHY_Convoy_ABC::Form()
{
    ++nFormingTime_;

    if( pCommander_ )
        return nFormingTime_ >= formingTime_( transporters_.size() );

    assert( transporters_.empty() );
    assert( pConsign_ );
    
    const MIL_AutomateLOG& supplyAutomate = pConsign_->GetSupplyingAutomate();
    
    MT_Float rUVolumeNeeded = pConsign_->GetUVolumeRequested();
    while( rUVolumeNeeded > 0. )
    {
        PHY_ComposantePion* pConvoyTransporter     = 0;
        MIL_AgentPion*      pConvoyTransporterPion = 0;
        if( !supplyAutomate.SupplyGetAvailableConvoyTransporter( pConvoyTransporter, pConvoyTransporterPion ) )
        {
            UnlockConvoy();
            return false;
        }

        rUVolumeNeeded -= pConvoyTransporter->GetConvoyTransporterUCapacity();
        pConvoyTransporterPion->GetRole< PHY_RolePion_Supply >().StartUsingForLogistic( *pConvoyTransporter );
        transporters_.push_back( sTransporter( pConvoyTransporter, pConvoyTransporterPion ) );
    }

    if( !supplyAutomate.SupplyGetAvailableConvoyCommander( pCommander_, pCommanderPion_ ) )
    {
        UnlockConvoy();
        return false;
    }
    pCommanderPion_->GetRole< PHY_RolePion_Supply >().StartUsingForLogistic( *pCommander_ );

    return nFormingTime_ >= formingTime_( transporters_.size() );
}


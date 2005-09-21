// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataDetection.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 19:03 $
// $Revision: 10 $
// $Workfile: DEC_Knowledge_AgentDataDetection.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_AgentDataDetection.h"

#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "DEC_Knowledge_AgentPerceptionDataDetection.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_AgentDataDetection, "DEC_Knowledge_AgentDataDetection" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataDetection::DEC_Knowledge_AgentDataDetection()
    : nTimeLastUpdate_             ( 0 )
    , rSpeed_                      ( std::numeric_limits< MT_Float >::max() )
    , rAltitude_                   ( std::numeric_limits< MT_Float >::max() )
    , bSurrendered_                ( false )
    , bPrisoner_                   ( false )
    , bRefugeeManaged_             ( false )
    , pLastPosture_                ( 0 )
    , pCurrentPosture_             ( 0 )
    , rPostureCompletionPercentage_( 1. )
    , bDirectionUpdated_           ( true )
    , bSpeedUpdated_               ( true )
    , bPositionUpdated_            ( true )
    , bPrisonerUpdated_            ( true )
    , bSurrenderedUpdated_         ( true )
    , bRefugeeManagedUpdated_      ( true )
{
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataDetection::~DEC_Knowledge_AgentDataDetection()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> nTimeLastUpdate_
         >> vPosition_
         >> vDirection_
         >> rSpeed_
         >> rAltitude_
         >> bSurrendered_
         >> bPrisoner_
         >> bRefugeeManaged_;
         
    uint nID;
    uint nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        file >> nID;
        visionVolumes_.insert( PHY_Volume::FindVolume( nID ) );
    }
    
    file >> nID;
    pLastPosture_ = PHY_Posture::FindPosture( nID );
    
    file >> nID;
    pCurrentPosture_ = PHY_Posture::FindPosture( nID );

    file >> rPostureCompletionPercentage_
         >> bDirectionUpdated_
         >> bSpeedUpdated_
         >> bPositionUpdated_
         >> bPrisonerUpdated_
         >> bSurrenderedUpdated_
         >> bRefugeeManagedUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << nTimeLastUpdate_
         << vPosition_
         << vDirection_
         << rSpeed_
         << rAltitude_
         << bSurrendered_
         << bPrisoner_
         << bRefugeeManaged_;
    
    file << visionVolumes_.size();
    for ( CIT_ComposanteVolumeSet it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
        file << (*it)->GetID();
    
    file << pLastPosture_->GetID()
         << pCurrentPosture_->GetID()
         << rPostureCompletionPercentage_
         << bDirectionUpdated_
         << bSpeedUpdated_
         << bPositionUpdated_
         << bPrisonerUpdated_
         << bSurrenderedUpdated_
         << bRefugeeManagedUpdated_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Prepare
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Prepare()
{
    bDirectionUpdated_      = false;
    bSpeedUpdated_          = false;
    bPositionUpdated_       = false;
    bPrisonerUpdated_       = false;
    bSurrenderedUpdated_    = false;
    bRefugeeManagedUpdated_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Update
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_AgentDataDetection::DoUpdate( const T& data )
{
    if ( data.GetTimeLastUpdate() <= nTimeLastUpdate_ )
        return;

    const MT_Vector2D& vNewPosition = data.GetPosition();
    if( vPosition_ != vNewPosition )
    {
        vPosition_        = vNewPosition;
        bPositionUpdated_ = true;
    }
    
    const MT_Vector2D& vNewDirection = data.GetDirection();
    if( vDirection_ != vNewDirection )
    {
        vDirection_        = vNewDirection;
        bDirectionUpdated_ = true;
    }

    const MT_Float rNewSpeed = data.GetSpeed();
    if( rSpeed_ != rNewSpeed )
    {
        rSpeed_        = rNewSpeed;
        bSpeedUpdated_ = true;
    }
//
    const bool bNewSurrendered = data.IsSurrendered();
    if( bSurrendered_ != bNewSurrendered )
    {
        bSurrendered_        = bNewSurrendered;
        bSurrenderedUpdated_ = true;
    }

    const bool bNewPrisoner = data.IsPrisoner();
    if( bPrisoner_ != bNewPrisoner )
    {
        bPrisoner_        = bNewPrisoner;
        bPrisonerUpdated_ = true;
    }

    const bool bNewRefugeeManaged = data.IsRefugeeManaged();
    if( bRefugeeManaged_ != bNewRefugeeManaged )
    {
        bRefugeeManaged_        = bNewRefugeeManaged;
        bRefugeeManagedUpdated_ = true;
    }
    
    rAltitude_                    =  data.GetAltitude();
    pLastPosture_                 = &data.GetLastPosture();
    pCurrentPosture_              = &data.GetCurrentPosture();
    rPostureCompletionPercentage_ =  data.GetPostureCompletionPercentage();
    visionVolumes_                =  data.GetVisionVolumes();
    nTimeLastUpdate_              =  data.GetTimeLastUpdate();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Update( const DEC_Knowledge_AgentPerceptionDataDetection& data )
{
    DoUpdate( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Update
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Update( const DEC_Knowledge_AgentDataDetection& data )
{
    DoUpdate( data ); 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Extrapolate( const MIL_Agent_ABC& agentKnown )
{   
    // Pas vraiment d'extrapolation : on prend la position réelle du pion
    const MT_Vector2D& vRealPos = agentKnown.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( vRealPos != vPosition_ )
    {
        vPosition_        = vRealPos;
        bPositionUpdated_ = true;
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::SendFullState
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::SendFullState( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const
{
    asnMsg.m.positionPresent = 1;
    NET_ASN_Tools::WritePoint( vPosition_, asnMsg.position );

    asnMsg.m.directionPresent = 1;
    NET_ASN_Tools::WriteDirection( vDirection_, asnMsg.direction );

    asnMsg.m.speedPresent = 1;
    asnMsg.speed = (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );

    asnMsg.m.renduPresent = 1;
    asnMsg.rendu          = bSurrendered_;

    asnMsg.m.prisonnierPresent = 1;
    asnMsg.prisonnier          = bPrisoner_;

    asnMsg.m.refugie_pris_en_comptePresent = 1;
    asnMsg.refugie_pris_en_compte = bRefugeeManaged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::SendChangedState
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::SendChangedState( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const
{
    if( bPositionUpdated_ )
    {
        asnMsg.m.positionPresent = 1;
        NET_ASN_Tools::WritePoint( vPosition_, asnMsg.position );
    }

    if( bDirectionUpdated_ )
    {
        asnMsg.m.directionPresent = 1;
        NET_ASN_Tools::WriteDirection( vDirection_, asnMsg.direction );
    }

    if( bSpeedUpdated_ )
    {
        asnMsg.m.speedPresent = 1;
        asnMsg.speed = (int)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    }

    if( bSurrenderedUpdated_ )
    {
        asnMsg.m.renduPresent = 1;
        asnMsg.rendu          = bSurrendered_;
    }
    
    if( bPrisonerUpdated_ )
    {
        asnMsg.m.prisonnierPresent = 1;
        asnMsg.prisonnier          = bPrisoner_;
    }
    
    if( bRefugeeManagedUpdated_ )
    {
        asnMsg.m.refugie_pris_en_comptePresent = 1;
        asnMsg.refugie_pris_en_compte = bRefugeeManaged_;
    }
}

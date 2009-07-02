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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentDataDetection.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/MIL_Army.h"
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
    , pArmySurrenderedTo_          ( 0 )
    , bPrisoner_                   ( false )
    , bRefugeeManaged_             ( false )
    , bDead_                       ( false )
    , pLastPosture_                ( 0 )
    , pCurrentPosture_             ( 0 )
    , rPostureCompletionPercentage_( 1. )
    , bDirectionUpdated_           ( true )
    , bSpeedUpdated_               ( true )
    , bPositionUpdated_            ( true )
    , bPrisonerUpdated_            ( true )
    , bSurrenderedUpdated_         ( true )
    , bRefugeeManagedUpdated_      ( true )
    , bDeadUpdated_                ( true )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataDetection::~DEC_Knowledge_AgentDataDetection()
{
    // NOTHING
}

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
         >> const_cast< MIL_Army*& >( pArmySurrenderedTo_ )
         >> bPrisoner_
         >> bRefugeeManaged_
         >> bDead_;
         
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
         >> bRefugeeManagedUpdated_
         >> bDeadUpdated_;
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
         << pArmySurrenderedTo_
         << bPrisoner_
         << bRefugeeManaged_
         << bDead_;
    
    unsigned size = visionVolumes_.size();
    file << size;
    for ( CIT_ComposanteVolumeSet it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
    {
        unsigned id = (*it)->GetID();
        file << id;
    }
    
    unsigned last    = pLastPosture_->GetID(),
             current = pCurrentPosture_->GetID();
    file << last
         << current
         << rPostureCompletionPercentage_
         << bDirectionUpdated_
         << bSpeedUpdated_
         << bPositionUpdated_
         << bPrisonerUpdated_
         << bSurrenderedUpdated_
         << bRefugeeManagedUpdated_
         << bDeadUpdated_;
}

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
    bDeadUpdated_           = false;
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

    const MIL_Army* pNewArmySurrenderedTo = data.GetArmySurrenderedTo();
    if( pArmySurrenderedTo_ != pNewArmySurrenderedTo )
    {
        pArmySurrenderedTo_  = pNewArmySurrenderedTo;
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

    const bool bNewDead = data.IsDead();
    if( bDead_ != bNewDead )
    {
        bDead_        = bNewDead;  
        bDeadUpdated_ = true;
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
    asnMsg.rendu          = pArmySurrenderedTo_ ? pArmySurrenderedTo_->GetID() : 0;

    asnMsg.m.prisonnierPresent = 1;
    asnMsg.prisonnier          = bPrisoner_;

    asnMsg.m.refugie_pris_en_comptePresent = 1;
    asnMsg.refugie_pris_en_compte          = bRefugeeManaged_;

    asnMsg.m.mortPresent = 1;
    asnMsg.mort          = bDead_;
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
        asnMsg.rendu          = pArmySurrenderedTo_ ? pArmySurrenderedTo_->GetID() : 0;
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

    if( bDeadUpdated_ )
    {
        asnMsg.m.mortPresent = 1;
        asnMsg.mort          = bDead_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsDead
// Created: NLD 2006-02-01
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsDead() const
{
    return bDead_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsSurrendered() const
{
    return pArmySurrenderedTo_ != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army* DEC_Knowledge_AgentDataDetection::GetArmySurrenderedTo() const
{
    return pArmySurrenderedTo_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsRefugeeManaged() const
{
    return bRefugeeManaged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetSpeed
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentDataDetection::GetSpeed() const
{
    return rSpeed_ == std::numeric_limits< MT_Float >::max() ? 0. : rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetAltitude
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentDataDetection::GetAltitude() const
{
    return rAltitude_ == std::numeric_limits< MT_Float >::max() ? 0. : rAltitude_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetDirection
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_AgentDataDetection::GetDirection() const
{
    return vDirection_;
}
   
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPosition
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_AgentDataDetection::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetLastPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_AgentDataDetection::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetCurrentPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_AgentDataDetection::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPostureCompletionPercentage
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentDataDetection::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetVisionVolumes
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const T_ComposanteVolumeSet& DEC_Knowledge_AgentDataDetection::GetVisionVolumes() const
{
    return visionVolumes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
uint DEC_Knowledge_AgentDataDetection::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::HasChanged
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::HasChanged() const
{
    return bDirectionUpdated_ || bSpeedUpdated_ || bPositionUpdated_ || bPrisonerUpdated_ || bSurrenderedUpdated_ || bRefugeeManagedUpdated_;
}

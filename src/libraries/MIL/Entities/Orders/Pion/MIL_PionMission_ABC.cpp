//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_PionMission_ABC.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:17 $
// $Revision: 8 $
// $Workfile: MIL_PionMission_ABC.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_PionMission_ABC.h"

#include "Entities/Orders/Limit/MIL_LimitManager.h"
#include "Entities/Orders/Lima/MIL_LimaManager.h"
#include "Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "MIL_PionMissionType.h"
#include "MIL_AgentServer.h"

int              MIL_PionMission_ABC::nDIAMissionType_        = 0;
int              MIL_PionMission_ABC::nDIADirectionDangerIdx_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC constructor
// Created: NLD 2002-07-15
// Last modified: JVT 02-09-16
//-----------------------------------------------------------------------------
MIL_PionMission_ABC::MIL_PionMission_ABC( MIL_AgentPion& pion, const MIL_PionMissionType& type )
    : DIA_Thing             ( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( type.GetDIATypeName() ) )
    , pion_                 ( pion )
    , type_                 ( type )
    , bDIABehaviorActivated_( false )
    , pLeftLimit_           ( 0 )
    , pRightLimit_          ( 0 )
    , vDirDanger_           ( 1., 0. )
    , nOrderID_             ( (uint)-1 )
{
    GetVariable( nDIAMissionType_ ).SetValue( (int)type.GetID() );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MIL_PionMission_ABC::~MIL_PionMission_ABC()
{
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::InitializeDIA
// Created: AGN 03-01-17
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Mission_Pion" );
    nDIAMissionType_        = DEC_Tools::InitializeDIAField( "missionType_"  , diaType );
    nDIADirectionDangerIdx_ = DEC_Tools::InitializeDIAField( "dirDangereuse_", diaType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC::Initialize()
{
    assert( !bDIABehaviorActivated_ );
    assert( limaMap_.empty() );

    nOrderID_      = MIL_IDManager::orders_.GetFreeSimID();
    vDirDanger_    = MT_Vector2D( 0., 1. );
    GetVariable( nDIADirectionDangerIdx_  ).SetValue( &vDirDanger_ );
    fuseau_.Reset();
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    assert( !bDIABehaviorActivated_ );
    assert( limaMap_.empty() );

    nOrderID_ = MIL_IDManager::orders_.GetFreeSimID();
    
    NET_ASN_Tools::CopyDirection( const_cast< MIL_AutomateMission_ABC& >( parentMission ).GetVariable( MIL_AutomateMission_ABC::nDIADirectionDangerIdx_ ), GetVariable( nDIADirectionDangerIdx_ ) );
    const MT_Vector2D* pTmp = GetVariable( nDIADirectionDangerIdx_ ).ToUserPtr( pTmp );
    assert( pTmp );
    vDirDanger_ = *pTmp;

    limaMap_ = parentMission.GetLimas ();
    fuseau_  = parentMission.GetFuseau();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC::Initialize( MIL_PionMission_ABC& mission )
{
    assert( limaMap_.empty() );
    assert( mission.type_ == type_ );

    nOrderID_ = MIL_IDManager::orders_.GetFreeSimID();

    // Limas / limits
    fuseau_  = mission.fuseau_;
    limaMap_ = mission.limaMap_;

    // Direction dangereuse
    NET_ASN_Tools::CopyDirection( const_cast< MIL_PionMission_ABC& >( mission ).GetVariable( nDIADirectionDangerIdx_ ), GetVariable( nDIADirectionDangerIdx_ ) );
    const MT_Vector2D* pTmp = GetVariable( nDIADirectionDangerIdx_ ).ToUserPtr( pTmp );
    assert( pTmp );
    vDirDanger_ = *pTmp;

    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2002-07-15
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    assert( !bDIABehaviorActivated_ );
    nOrderID_ = asnMsg.order_id;

    ASN1T_EnumOrderErrorCode nCode = InitializeLimits( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;

    nCode = InitializeLimas( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;

    nCode = InitializeMission( asnMsg );
    return nCode;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::InitializeLimits
// Created: NLD 2002-08-22
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC::InitializeLimits( const ASN1T_MsgPionOrder& asnMsg )
{
	pLeftLimit_ = 0;
	if( asnMsg.m.oid_limite_gauchePresent )
    {
        pLeftLimit_ = MIL_AgentServer::GetWorkspace().GetLimitManager().FindLimit( asnMsg.oid_limite_gauche );
        if( !pLeftLimit_ )
            return EnumOrderErrorCode::error_invalid_limit;
//        pLeftLimit_->NotifyUsedByMission( *this );
    }

    pRightLimit_ = 0;
    if( asnMsg.m.oid_limite_droitePresent )
    {
        pRightLimit_ = MIL_AgentServer::GetWorkspace().GetLimitManager().FindLimit( asnMsg.oid_limite_droite );
        if( !pRightLimit_ )
            return EnumOrderErrorCode::error_invalid_limit;
//        pRightLimit_->NotifyUsedByMission( *this );
    }

    if( pLeftLimit_ && pRightLimit_ && pLeftLimit_ == pRightLimit_ )
        return EnumOrderErrorCode::error_invalid_limit;
    return EnumOrderErrorCode::no_error;     
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::InitializeLimas
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC::InitializeLimas( const ASN1T_MsgPionOrder& asnMsg )
{
    limaMap_.clear();
    for( uint nTmp = 0; nTmp < asnMsg.oid_limas.n; ++ nTmp )
    {
        const MIL_Lima* pLima =  MIL_AgentServer::GetWorkspace().GetLimaManager().FindLima( asnMsg.oid_limas.elem[nTmp] );
        if( !pLima )
            return EnumOrderErrorCode::error_invalid_lima;
        limaMap_.insert( std::make_pair( pLima, false ) );
    }
    return EnumOrderErrorCode::no_error;     
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::InitializeMission
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC::InitializeMission( const ASN1T_MsgPionOrder& asnMsg )
{
    // Direction dangereuse
    if( !NET_ASN_Tools::CopyDirection ( asnMsg.direction_dangereuse, GetVariable( nDIADirectionDangerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    const MT_Vector2D* pTmp = GetVariable( nDIADirectionDangerIdx_ ).ToUserPtr( pTmp );
    assert( pTmp );
    vDirDanger_ = *pTmp;

    return EnumOrderErrorCode::no_error;     
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Terminate
// Created: JVT 02-10-02
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::Terminate()
{ 
    nOrderID_ = (uint)-1;
    limaMap_.clear();
    fuseau_.Reset();
    assert( !bDIABehaviorActivated_ );
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Serialize
// Created: NLD 2003-05-12
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    asnMsg.oid_unite_executante = pion_.GetID();
    asnMsg.order_id             = nOrderID_;

    // Limites
    if( fuseau_.GetLeftLimit() )
    {
        asnMsg.m.oid_limite_gauchePresent = 1;
        asnMsg.oid_limite_gauche          = fuseau_.GetLeftLimit()->GetID();
    }
    else
        asnMsg.m.oid_limite_gauchePresent = 0;

    if( fuseau_.GetRightLimit() )
    {
        asnMsg.m.oid_limite_droitePresent = 1;
        asnMsg.oid_limite_droite          = fuseau_.GetRightLimit()->GetID();
    }
    else
        asnMsg.m.oid_limite_droitePresent = 0;    

    // Limas
    asnMsg.oid_limas.n    = limaMap_.size();
    if( !limaMap_.empty() )
    {
        asnMsg.oid_limas.elem = new ASN1T_OID[ limaMap_.size() ]; //$$$$ RAM
        uint i = 0;
        for( CIT_LimaFlagedPtrMap itLima = limaMap_.begin(); itLima != limaMap_.end(); ++itLima )
            asnMsg.oid_limas.elem[i++] = itLima->first->GetID();
    }

    // Direction dangereuse
    NET_ASN_Tools::CopyDirection( GetVariable( nDIADirectionDangerIdx_ ), asnMsg.direction_dangereuse );
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::CleanAfterSerialization
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    if( asnMsg.oid_limas.n > 0 )
        delete[] asnMsg.oid_limas.elem;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Prepare
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
void MIL_PionMission_ABC::Prepare()
{
    if( !pLeftLimit_ || !pRightLimit_ )
    {
        fuseau_.Reset();
        return;
    }

    assert( pLeftLimit_ != pRightLimit_ );

    const MIL_Lima*    pBeginMissionLima  = GetLima( MIL_Lima::eLimaFuncLDM );
    const MIL_Lima*    pEndMissionLima    = GetLima( MIL_Lima::eLimaFuncLFM );
    const MT_Vector2D& vOrientationRefPos = pion_.GetRole< PHY_RolePion_Location >().GetPosition(); // For fuseau orientation, when LDM and LFM aren't specified
    
    fuseau_.Reset( vOrientationRefPos, *pLeftLimit_, *pRightLimit_, pBeginMissionLima, pEndMissionLima );
}

//=============================================================================
// CONTROL
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Start
// Created: NLD 2003-03-28
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::Start()
{
    if( bDIABehaviorActivated_ )
        return;

    pion_.GetRole< DEC_RolePion_Decision >().StartMissionBehavior( *this );
    bDIABehaviorActivated_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Stop
// Created: NLD 2003-03-28
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::Stop()
{
    if( !bDIABehaviorActivated_ )
        return;

    pion_.GetRole< DEC_RolePion_Decision >().StopMissionBehavior( *this );
    bDIABehaviorActivated_ = false;
}

// =============================================================================
// MISC
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetNameClass
// Created: JVT 02-09-05
//-----------------------------------------------------------------------------
const char* MIL_PionMission_ABC::GetName() const
{
    return type_.GetBehaviorName().c_str();
}



//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateMission_ABC.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:16 $
// $Revision: 7 $
// $Workfile: MIL_AutomateMission_ABC.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_AutomateMission_ABC.h"

#include "MIL_AutomateMissionType.h"

#include "Entities/Orders/Limit/MIL_LimitManager.h"
#include "Entities/Orders/Lima/MIL_LimaManager.h"
#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"

int MIL_AutomateMission_ABC::nDIAMissionType_        = 0;
int MIL_AutomateMission_ABC::nDIADirectionDangerIdx_ = 0;
int MIL_AutomateMission_ABC::nDIAFormationIdx_       = 0;

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Mission_Automate" );
    nDIAMissionType_        = DEC_Tools::InitializeDIAField( "missionType_"    , diaType );
    nDIADirectionDangerIdx_ = DEC_Tools::InitializeDIAField( "directionEnnemi_", diaType );
    nDIAFormationIdx_       = DEC_Tools::InitializeDIAField( "formation_"      , diaType );
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC constructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC::MIL_AutomateMission_ABC( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : DIA_Thing                     ( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( type.GetDIATypeName() ) )
    , automate_                     ( automate )
    , type_                         ( type     )
    , bDIAMRTBehaviorActivated_     ( false )
    , bDIAConduiteBehaviorActivated_( false )
{
    GetVariable( nDIAMissionType_ ).SetValue( (int)type.GetID() );
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC destructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC::~MIL_AutomateMission_ABC()
{
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Initialize
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::Initialize()
{
    bCreatedBySim_ = true;
    nOrderID_      = MIL_PionMission_ABC::idManager_.GetFreeSimID(); //$$$ DEGUEU
    mrt_.Initialize( *this );

    pLeftLimit_ = pRightLimit_ = 0;
    limaMap_.clear();

    vDirDanger_    = MT_Vector2D( 0., 1. );
    GetVariable( nDIADirectionDangerIdx_  ).SetValue( &vDirDanger_ );
    GetVariable( nDIAFormationIdx_        ).SetValue( 0 );
}
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Initialize
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    bCreatedBySim_ = false;
    nOrderID_      = asnMsg.order_id;
    mrt_.Initialize( *this );

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
// Name: MIL_AutomateMission_ABC::InitializeLimits
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC::InitializeLimits( const ASN1T_MsgAutomateOrder& asnMsg )
{
    pLeftLimit_ = MIL_AgentServer::GetWorkspace().GetLimitManager().FindLimit( asnMsg.oid_limite_gauche );
    if( !pLeftLimit_ )
        return EnumOrderErrorCode::error_invalid_limit;

    pRightLimit_ = MIL_AgentServer::GetWorkspace().GetLimitManager().FindLimit( asnMsg.oid_limite_droite );
    if( !pRightLimit_ )
        return EnumOrderErrorCode::error_invalid_limit;

    if( pLeftLimit_ == pRightLimit_ )
        return EnumOrderErrorCode::error_invalid_limit;
    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::InitializeLimas
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC::InitializeLimas( const ASN1T_MsgAutomateOrder& asnMsg )
{
    limaMap_.clear();
    for( uint nTmp = 0; nTmp < asnMsg.oid_limas.n; ++ nTmp )
    {
        const MIL_Lima* pLima =  MIL_AgentServer::GetWorkspace().GetLimaManager().FindLima( asnMsg.oid_limas.elem[nTmp] );
        if( !pLima )
            return EnumOrderErrorCode::error_invalid_lima;
        limaMap_.insert( std::make_pair( pLima, false ) ); //$$$ DEPLACER DANS MIL_Mission_ABC
    }
    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::InitializeMission
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC::InitializeMission( const ASN1T_MsgAutomateOrder& asnMsg )
{
    if( !NET_ASN_Tools::CopyDirection( asnMsg.direction_dangereuse, vDirDanger_, GetVariable( nDIADirectionDangerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    if( !NET_ASN_Tools::CopyEnumeration(  asnMsg.formation, GetVariable( nDIAFormationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    
    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::Terminate()
{
    if( bCreatedBySim_ )
        MIL_PionMission_ABC::idManager_.ReleaseSimID( nOrderID_ );

    nOrderID_  = (uint)-1;
    limaMap_.clear();

    fuseau_.Reset();
    vDirDanger_.Reset();

    mrt_.Terminate();

    assert( !bDIAConduiteBehaviorActivated_ );
    assert( !bDIAMRTBehaviorActivated_ );
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::StartMRT
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::StartMRT()
{
    if( bDIAMRTBehaviorActivated_ )
        return;
    automate_.GetDecision().StartMissionMrtBehavior( *this );
    bDIAMRTBehaviorActivated_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::StopMRT
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::StopMRT()
{
    if( !bDIAMRTBehaviorActivated_ )
        return;
    automate_.GetDecision().StopMissionMrtBehavior( *this );
    bDIAMRTBehaviorActivated_ = false;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::StartConduite
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::StartConduite()
{
    if( bDIAConduiteBehaviorActivated_ )
        return;
    automate_.GetDecision().StartMissionConduiteBehavior( *this );
    bDIAConduiteBehaviorActivated_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::StopConduite
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::StopConduite()
{
    if( !bDIAConduiteBehaviorActivated_ )
        return;
    automate_.GetDecision().StopMissionConduiteBehavior( *this );
    bDIAConduiteBehaviorActivated_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Prepare
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::Prepare()
{
    if( !pLeftLimit_ || !pRightLimit_ )
    {
        fuseau_.Reset();
        return;
    }

    assert( pLeftLimit_ != pRightLimit_ );

    const MIL_Lima*    pBeginMissionLima  = GetLima( MIL_Lima::eLimaFuncLDM );
    const MIL_Lima*    pEndMissionLima    = GetLima( MIL_Lima::eLimaFuncLFM );
    const MT_Vector2D& vOrientationRefPos = automate_.GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition(); // For fuseau orientation, when LDM and LFM aren't specified
    //$$$$$ POURRI
    
    fuseau_.Reset( vOrientationRefPos, *pLeftLimit_, *pRightLimit_, pBeginMissionLima, pEndMissionLima );
}

// =============================================================================
// MISC
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetName
// Created: JVT 02-09-05
//-----------------------------------------------------------------------------
const char* MIL_AutomateMission_ABC::GetName() const
{
    return type_.GetMrtBehaviorName().c_str();
}


//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Serialize
// Created: NLD 2003-05-12
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    asnMsg.oid_unite_executante = automate_.GetID();
    asnMsg.order_id             = nOrderID_;

    // Limites
    asnMsg.oid_limite_gauche = fuseau_.GetLeftLimit () ? fuseau_.GetLeftLimit ()->GetID() : 0;
    asnMsg.oid_limite_droite = fuseau_.GetRightLimit() ? fuseau_.GetRightLimit()->GetID() : 0;

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
// Name: MIL_AutomateMission_ABC::CleanAfterSerialization
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    if( asnMsg.oid_limas.n > 0 )
        delete[] asnMsg.oid_limas.elem;
}
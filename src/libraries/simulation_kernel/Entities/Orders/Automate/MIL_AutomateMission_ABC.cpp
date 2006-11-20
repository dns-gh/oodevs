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

#include "simulation_kernel_pch.h"

#include "MIL_AutomateMission_ABC.h"

#include "MIL_AutomateMissionType.h"

#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Decision/DEC_Tools.h"

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
    , type_                         ( type )
    , mrt_                          ()
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
    assert( !bDIAConduiteBehaviorActivated_ );
    assert( !bDIAMRTBehaviorActivated_      );

    MIL_OrderContext::Initialize();
    GetVariable( nDIADirectionDangerIdx_  ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    GetVariable( nDIAFormationIdx_        ).SetValue( 0 );

    mrt_.Initialize( *this );
}
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Initialize
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC::Initialize( const ASN1T_MsgAutomateOrder& asn )
{
    assert( !bDIAConduiteBehaviorActivated_ );
    assert( !bDIAMRTBehaviorActivated_      );

    ASN1T_EnumOrderErrorCode nCode = MIL_OrderContext::Initialize( asn.order_context, automate_.GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition() /*For fuseau orientation, when LDM and LFM aren't specified*/ );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;

    GetVariable( nDIADirectionDangerIdx_  ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    GetVariable( nDIAFormationIdx_        ).SetValue( asn.formation );

    mrt_.Initialize( *this );

    return EnumOrderErrorCode::no_error;
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
void MIL_AutomateMission_ABC::Serialize( ASN1T_MsgAutomateOrder& asn )
{
    MIL_OrderContext::Serialize( asn.order_context );

    asn.oid_unite_executante = automate_.GetID();
    asn.formation            = EnumAutomateOrderFormation::deux_echelons; //$$$
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::CleanAfterSerialization
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asn )
{
    MIL_OrderContext::CleanAfterSerialization( asn.order_context );
}

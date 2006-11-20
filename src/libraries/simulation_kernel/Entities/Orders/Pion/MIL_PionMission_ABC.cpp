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

#include "simulation_kernel_pch.h"

#include "MIL_PionMission_ABC.h"

#include "Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Decision/DEC_Tools.h"
#include "MIL_PionMissionType.h"

int MIL_PionMission_ABC::nDIAMissionType_        = 0;
int MIL_PionMission_ABC::nDIADirectionDangerIdx_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC constructor
// Created: NLD 2002-07-15
// Last modified: JVT 02-09-16
//-----------------------------------------------------------------------------
MIL_PionMission_ABC::MIL_PionMission_ABC( MIL_AgentPion& pion, const MIL_PionMissionType& type )
    : DIA_Thing             ( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( type.GetDIATypeName() ) )
    , MIL_OrderContext       ()
    , pion_                 ( pion )
    , type_                 ( type )
    , bDIABehaviorActivated_( false )
{
    GetVariable( nDIAMissionType_ ).SetValue( (int)type.GetID() );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MIL_PionMission_ABC::~MIL_PionMission_ABC()
{
    assert( !bDIABehaviorActivated_ );
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
void MIL_PionMission_ABC::Initialize()
{
    assert( !bDIABehaviorActivated_ );
    MIL_OrderContext::Initialize();
    GetVariable( nDIADirectionDangerIdx_  ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2002-07-15
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC::Initialize( const ASN1T_MsgPionOrder& asn )
{
    assert( !bDIABehaviorActivated_ );
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderContext::Initialize( asn.order_context, pion_.GetRole< PHY_RolePion_Location >().GetPosition() );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;

    GetVariable( nDIADirectionDangerIdx_  ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    return nCode;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    assert( !bDIABehaviorActivated_ );
    MIL_OrderContext::Initialize( parentMission );
    
    GetVariable( nDIADirectionDangerIdx_  ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Initialize
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC::Initialize( MIL_PionMission_ABC& mission )
{
    assert( mission.type_ == type_ );
    assert( !bDIABehaviorActivated_ );
    MIL_OrderContext::Initialize( mission );

    GetVariable( nDIADirectionDangerIdx_  ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    return true;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::Serialize
// Created: NLD 2003-05-12
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::Serialize( ASN1T_MsgPionOrder& asn )
{
    MIL_OrderContext::Serialize( asn.order_context );

    asn.oid_unite_executante = pion_.GetID();
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::CleanAfterSerialization
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC::CleanAfterSerialization( ASN1T_MsgPionOrder& asn )
{
    MIL_OrderContext::CleanAfterSerialization( asn.order_context );
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

// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ABC_Surveiller.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_Surveiller::nDIAPositionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::InitializeDIA
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_Surveiller::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionIdx_ = DEC_Tools::InitializeDIAField( "position_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Surveiller::MIL_PionMission_ABC_Surveiller( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Surveiller::~MIL_PionMission_ABC_Surveiller()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_Surveiller::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_Surveiller& asnMission = *asnMsg.mission.u.mission_pion_abc_surveiller;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position, position_, GetVariable( nDIAPositionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Surveiller::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( position_, GetVariable( nDIAPositionIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::Initialize
// Created: 2005-08-17 - 16:30:43
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Surveiller::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_Surveiller& mission = static_cast< MIL_PionMission_ABC_Surveiller& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionIdx_ ), position_, GetVariable( nDIAPositionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Surveiller::Terminate()
{
    NET_ASN_Tools::ResetPoint( position_, GetVariable( nDIAPositionIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Surveiller::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_Surveiller& asnMission = *new ASN1T_Mission_Pion_ABC_Surveiller();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_surveiller;
    asnMsg.mission.u.mission_pion_abc_surveiller  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionIdx_ ), asnMission.position );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Surveiller::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Surveiller::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_surveiller );
    ASN1T_Mission_Pion_ABC_Surveiller& asnMission = *asnMsg.mission.u.mission_pion_abc_surveiller;

    NET_ASN_Tools::Delete( asnMission.position );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

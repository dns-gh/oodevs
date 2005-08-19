// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ASA_DefendreZone.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASA_DefendreZone::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_DefendreZone::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreZone::MIL_PionMission_ASA_DefendreZone( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreZone::~MIL_PionMission_ASA_DefendreZone()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_DefendreZone::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_DefendreZone& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreZone::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreZone::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_DefendreZone& mission = static_cast< MIL_PionMission_ASA_DefendreZone& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneIdx_ ), zone_, GetVariable( nDIAZoneIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreZone::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreZone::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_DefendreZone& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreZone();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_defendre_zone;
    asnMsg.mission.u.mission_pion_asa_defendre_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreZone::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreZone::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_defendre_zone );
    ASN1T_Mission_Pion_ASA_DefendreZone& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_zone;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

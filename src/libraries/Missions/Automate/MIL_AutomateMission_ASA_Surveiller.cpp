// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ASA_Surveiller.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASA_Surveiller::nDIAZoneIdx_ = 0 ;
int MIL_AutomateMission_ASA_Surveiller::nDIAAngleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_Surveiller constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_Surveiller::MIL_AutomateMission_ASA_Surveiller( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_Surveiller destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_Surveiller::~MIL_AutomateMission_ASA_Surveiller()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_Surveiller::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASA_Surveiller::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAAngleIdx_ = DEC_Tools::InitializeDIAField( "angle_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_Surveiller::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASA_Surveiller::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASA_Surveiller& asnMission = *asnMsg.mission.u.mission_automate_asa_surveiller;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.angle, GetVariable( nDIAAngleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_Surveiller::Terminate()
{
    NET_ASN_Tools::ResetNumeric( GetVariable( nDIAAngleIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_Surveiller::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_Surveiller::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASA_Surveiller& asnMission = *new ASN1T_Mission_Automate_ASA_Surveiller();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_asa_surveiller;
    asnMsg.mission.u.mission_automate_asa_surveiller  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyNumeric( GetVariable( nDIAAngleIdx_ ), asnMission.angle );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_Surveiller::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_Surveiller::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_asa_surveiller );
    ASN1T_Mission_Automate_ASA_Surveiller& asnMission = *asnMsg.mission.u.mission_automate_asa_surveiller;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

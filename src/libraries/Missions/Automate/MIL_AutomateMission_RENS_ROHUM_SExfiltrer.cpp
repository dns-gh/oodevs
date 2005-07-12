// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_RENS_ROHUM_SExfiltrer.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_RENS_ROHUM_SExfiltrer::nDIAZoneDeRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROHUM_SExfiltrer::MIL_AutomateMission_RENS_ROHUM_SExfiltrer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROHUM_SExfiltrer::~MIL_AutomateMission_RENS_ROHUM_SExfiltrer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneDeRegroupementIdx_ = DEC_Tools::InitializeDIAField( "zoneDeRegroupement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *asnMsg.mission.u.mission_automate_rens_rohum_sexfiltrer;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_de_regroupement, zoneDeRegroupement_, GetVariable( nDIAZoneDeRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneDeRegroupement_, GetVariable( nDIAZoneDeRegroupementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer;
    asnMsg.mission.u.mission_automate_rens_rohum_sexfiltrer  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneDeRegroupementIdx_ ), asnMission.zone_de_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROHUM_SExfiltrer::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROHUM_SExfiltrer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer );
    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *asnMsg.mission.u.mission_automate_rens_rohum_sexfiltrer;

    NET_ASN_Tools::Delete( asnMission.zone_de_regroupement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

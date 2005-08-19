// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_SeFaireTransporter.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_SeFaireTransporter::nDIAZoneEmbarquementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireTransporter constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_AutomateMission_SeFaireTransporter::MIL_AutomateMission_SeFaireTransporter( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireTransporter destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_AutomateMission_SeFaireTransporter::~MIL_AutomateMission_SeFaireTransporter()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireTransporter::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_SeFaireTransporter::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "zoneEmbarquement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireTransporter::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_SeFaireTransporter::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_SeFaireTransporter& asnMission = *asnMsg.mission.u.mission_automate_se_faire_transporter;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_embarquement, zoneEmbarquement_, GetVariable( nDIAZoneEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeFaireTransporter::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneEmbarquement_, GetVariable( nDIAZoneEmbarquementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireTransporter::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeFaireTransporter::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_SeFaireTransporter& asnMission = *new ASN1T_Mission_Automate_SeFaireTransporter();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_se_faire_transporter;
    asnMsg.mission.u.mission_automate_se_faire_transporter  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneEmbarquementIdx_ ), asnMission.zone_embarquement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireTransporter::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeFaireTransporter::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_se_faire_transporter );
    ASN1T_Mission_Automate_SeFaireTransporter& asnMission = *asnMsg.mission.u.mission_automate_se_faire_transporter;

    NET_ASN_Tools::Delete( asnMission.zone_embarquement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

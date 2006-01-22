// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_Tenir.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_Tenir::nDIAPositionInstallationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Tenir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Tenir::MIL_AutomateMission_ABC_Tenir( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Tenir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Tenir::~MIL_AutomateMission_ABC_Tenir()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Tenir::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_Tenir::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionInstallationIdx_ = DEC_Tools::InitializeDIAField( "positionInstallation_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Tenir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_Tenir::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_Tenir& asnMission = *asnMsg.mission.u.mission_automate_abc_tenir;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.position_installation, GetVariable( nDIAPositionInstallationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Tenir::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Tenir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Tenir::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_Tenir& asnMission = *new ASN1T_Mission_Automate_ABC_Tenir();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_tenir;
    asnMsg.mission.u.mission_automate_abc_tenir  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAPositionInstallationIdx_ ), asnMission.position_installation );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Tenir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Tenir::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_tenir );
    ASN1T_Mission_Automate_ABC_Tenir& asnMission = *asnMsg.mission.u.mission_automate_abc_tenir;

    NET_ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

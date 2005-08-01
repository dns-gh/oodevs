// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_Eclairer.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"



//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Eclairer constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Eclairer::MIL_AutomateMission_ABC_Eclairer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Eclairer destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_Eclairer::~MIL_AutomateMission_ABC_Eclairer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Eclairer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_Eclairer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Eclairer::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_Eclairer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        


    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Eclairer::Terminate()
{

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Eclairer::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Eclairer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_Eclairer& asnMission = *new ASN1T_Mission_Automate_ABC_Eclairer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_eclairer;
    asnMsg.mission.u.mission_automate_abc_eclairer  = &asnMission;


}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_Eclairer::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_Eclairer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_eclairer );
    ASN1T_Mission_Automate_ABC_Eclairer& asnMission = *asnMsg.mission.u.mission_automate_abc_eclairer;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

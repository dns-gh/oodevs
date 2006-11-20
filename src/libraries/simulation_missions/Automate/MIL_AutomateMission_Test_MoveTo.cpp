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

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_Test_MoveTo.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_Test_MoveTo::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Test_MoveTo constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Test_MoveTo::MIL_AutomateMission_Test_MoveTo( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Test_MoveTo destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Test_MoveTo::~MIL_AutomateMission_Test_MoveTo()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Test_MoveTo::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_Test_MoveTo::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Test_MoveTo::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_Test_MoveTo::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_Test_MoveTo& asnMission = *asnMsg.mission.u.mission_automate_test_move_to;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Test_MoveTo::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Test_MoveTo::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *new ASN1T_Mission_Automate_Test_MoveTo();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_test_move_to;
    asnMsg.mission.u.mission_automate_test_move_to  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Test_MoveTo::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Test_MoveTo::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_test_move_to );
    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *asnMsg.mission.u.mission_automate_test_move_to;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

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

#include "MIL_AutomateMission_GEN_RetablirItineraires.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RetablirItineraires::nDIAItinerairesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RetablirItineraires constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RetablirItineraires::MIL_AutomateMission_GEN_RetablirItineraires( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RetablirItineraires destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RetablirItineraires::~MIL_AutomateMission_GEN_RetablirItineraires()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RetablirItineraires::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RetablirItineraires::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItinerairesIdx_ = DEC_Tools::InitializeDIAField( "itineraires_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RetablirItineraires::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RetablirItineraires::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RetablirItineraires& asnMission = *asnMsg.mission.u.mission_automate_gen_retablir_itineraires;
    if( !NET_ASN_Tools::CopyPathList( asnMission.itineraires, GetVariable( nDIAItinerairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RetablirItineraires::Terminate()
{
    NET_ASN_Tools::ResetPathList( GetVariable( nDIAItinerairesIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RetablirItineraires::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RetablirItineraires::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RetablirItineraires& asnMission = *new ASN1T_Mission_Automate_GEN_RetablirItineraires();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_retablir_itineraires;
    asnMsg.mission.u.mission_automate_gen_retablir_itineraires  = &asnMission;

    NET_ASN_Tools::CopyPathList( GetVariable( nDIAItinerairesIdx_ ), asnMission.itineraires );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RetablirItineraires::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RetablirItineraires::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_retablir_itineraires );
    ASN1T_Mission_Automate_GEN_RetablirItineraires& asnMission = *asnMsg.mission.u.mission_automate_gen_retablir_itineraires;

    NET_ASN_Tools::Delete( asnMission.itineraires );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

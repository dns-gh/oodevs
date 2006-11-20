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

#include "MIL_AutomateMission_GEN_OuvrirItineraires.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_OuvrirItineraires::nDIAItinerairesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirItineraires constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_OuvrirItineraires::MIL_AutomateMission_GEN_OuvrirItineraires( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirItineraires destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_OuvrirItineraires::~MIL_AutomateMission_GEN_OuvrirItineraires()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirItineraires::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_OuvrirItineraires::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItinerairesIdx_ = DEC_Tools::InitializeDIAField( "itineraires_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirItineraires::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_OuvrirItineraires::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_OuvrirItineraires& asnMission = *asnMsg.mission.u.mission_automate_gen_ouvrir_itineraires;
    if( !NET_ASN_Tools::CopyPathList( asnMission.itineraires, GetVariable( nDIAItinerairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirItineraires::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_OuvrirItineraires::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_OuvrirItineraires& asnMission = *new ASN1T_Mission_Automate_GEN_OuvrirItineraires();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_ouvrir_itineraires;
    asnMsg.mission.u.mission_automate_gen_ouvrir_itineraires  = &asnMission;

    NET_ASN_Tools::CopyPathList( GetVariable( nDIAItinerairesIdx_ ), asnMission.itineraires );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirItineraires::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_OuvrirItineraires::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_ouvrir_itineraires );
    ASN1T_Mission_Automate_GEN_OuvrirItineraires& asnMission = *asnMsg.mission.u.mission_automate_gen_ouvrir_itineraires;

    NET_ASN_Tools::Delete( asnMission.itineraires );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

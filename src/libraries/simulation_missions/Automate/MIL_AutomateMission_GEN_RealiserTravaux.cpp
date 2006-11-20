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

#include "MIL_AutomateMission_GEN_RealiserTravaux.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RealiserTravaux::nDIATravauxIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravaux constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserTravaux::MIL_AutomateMission_GEN_RealiserTravaux( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravaux destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserTravaux::~MIL_AutomateMission_GEN_RealiserTravaux()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravaux::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RealiserTravaux::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIATravauxIdx_ = DEC_Tools::InitializeDIAField( "travaux_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravaux::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RealiserTravaux::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RealiserTravaux& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_travaux;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.travaux, GetVariable( nDIATravauxIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravaux::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserTravaux::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RealiserTravaux& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserTravaux();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_realiser_travaux;
    asnMsg.mission.u.mission_automate_gen_realiser_travaux  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIATravauxIdx_ ), asnMission.travaux );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravaux::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserTravaux::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_realiser_travaux );
    ASN1T_Mission_Automate_GEN_RealiserTravaux& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_travaux;

    NET_ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

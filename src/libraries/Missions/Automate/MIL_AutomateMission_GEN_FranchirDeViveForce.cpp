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

#include "MIL_AutomateMission_GEN_FranchirDeViveForce.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_FranchirDeViveForce::nDIAObstaclesAFranchirIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FranchirDeViveForce constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_FranchirDeViveForce::MIL_AutomateMission_GEN_FranchirDeViveForce( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FranchirDeViveForce destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_FranchirDeViveForce::~MIL_AutomateMission_GEN_FranchirDeViveForce()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FranchirDeViveForce::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_FranchirDeViveForce::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObstaclesAFranchirIdx_ = DEC_Tools::InitializeDIAField( "obstaclesAFranchir_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FranchirDeViveForce::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_FranchirDeViveForce::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_FranchirDeViveForce& asnMission = *asnMsg.mission.u.mission_automate_gen_franchir_de_vive_force;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.obstacles_a_franchir, GetVariable( nDIAObstaclesAFranchirIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_FranchirDeViveForce::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FranchirDeViveForce::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_FranchirDeViveForce::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_FranchirDeViveForce& asnMission = *new ASN1T_Mission_Automate_GEN_FranchirDeViveForce();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_franchir_de_vive_force;
    asnMsg.mission.u.mission_automate_gen_franchir_de_vive_force  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAObstaclesAFranchirIdx_ ), asnMission.obstacles_a_franchir, automate_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FranchirDeViveForce::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_FranchirDeViveForce::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_franchir_de_vive_force );
    ASN1T_Mission_Automate_GEN_FranchirDeViveForce& asnMission = *asnMsg.mission.u.mission_automate_gen_franchir_de_vive_force;

    NET_ASN_Tools::Delete( asnMission.obstacles_a_franchir );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

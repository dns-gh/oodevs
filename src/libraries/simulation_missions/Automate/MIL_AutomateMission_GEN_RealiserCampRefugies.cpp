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

#include "MIL_AutomateMission_GEN_RealiserCampRefugies.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RealiserCampRefugies::nDIACampIdx_ = 0 ;
int MIL_AutomateMission_GEN_RealiserCampRefugies::nDIATc2Idx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampRefugies constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserCampRefugies::MIL_AutomateMission_GEN_RealiserCampRefugies( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampRefugies destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserCampRefugies::~MIL_AutomateMission_GEN_RealiserCampRefugies()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampRefugies::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RealiserCampRefugies::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACampIdx_ = DEC_Tools::InitializeDIAField( "camp_", diaType );
    nDIATc2Idx_ = DEC_Tools::InitializeDIAField( "tc2_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampRefugies::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RealiserCampRefugies::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RealiserCampRefugies& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_camp_refugies;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.camp, GetVariable( nDIACampIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.tc2, GetVariable( nDIATc2Idx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserCampRefugies::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampRefugies::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserCampRefugies::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserCampRefugies();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_realiser_camp_refugies;
    asnMsg.mission.u.mission_automate_gen_realiser_camp_refugies  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIACampIdx_ ), asnMission.camp );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIATc2Idx_ ), asnMission.tc2 );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampRefugies::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserCampRefugies::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_realiser_camp_refugies );
    ASN1T_Mission_Automate_GEN_RealiserCampRefugies& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_camp_refugies;

    NET_ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

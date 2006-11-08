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

#include "MIL_AutomateMission_INF_Interdire.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_Interdire::nDIAPointsAInterdireIdx_ = 0 ;
int MIL_AutomateMission_INF_Interdire::nDIAPreparerTerrainIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Interdire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Interdire::MIL_AutomateMission_INF_Interdire( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Interdire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Interdire::~MIL_AutomateMission_INF_Interdire()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Interdire::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_Interdire::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointsAInterdireIdx_ = DEC_Tools::InitializeDIAField( "pointsAInterdire_", diaType );
    nDIAPreparerTerrainIdx_ = DEC_Tools::InitializeDIAField( "preparerTerrain_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Interdire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_Interdire::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_Interdire& asnMission = *asnMsg.mission.u.mission_automate_inf_interdire;
    if( !NET_ASN_Tools::CopyPointList( asnMission.points_a_interdire, GetVariable( nDIAPointsAInterdireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.preparer_terrain, GetVariable( nDIAPreparerTerrainIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Interdire::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Interdire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Interdire::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_Interdire& asnMission = *new ASN1T_Mission_Automate_INF_Interdire();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_interdire;
    asnMsg.mission.u.mission_automate_inf_interdire  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPointsAInterdireIdx_ ), asnMission.points_a_interdire );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPreparerTerrainIdx_ ), asnMission.preparer_terrain );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Interdire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Interdire::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_interdire );
    ASN1T_Mission_Automate_INF_Interdire& asnMission = *asnMsg.mission.u.mission_automate_inf_interdire;

    NET_ASN_Tools::Delete( asnMission.points_a_interdire );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

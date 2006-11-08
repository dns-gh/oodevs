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

#include "MIL_AutomateMission_FiltrerPopulations.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_FiltrerPopulations::nDIAPointsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FiltrerPopulations constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_FiltrerPopulations::MIL_AutomateMission_FiltrerPopulations( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FiltrerPopulations destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_FiltrerPopulations::~MIL_AutomateMission_FiltrerPopulations()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FiltrerPopulations::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_FiltrerPopulations::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointsIdx_ = DEC_Tools::InitializeDIAField( "points_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FiltrerPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_FiltrerPopulations::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_FiltrerPopulations& asnMission = *asnMsg.mission.u.mission_automate_filtrer_populations;
    if( !NET_ASN_Tools::CopyPointList( asnMission.points, GetVariable( nDIAPointsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FiltrerPopulations::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FiltrerPopulations::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FiltrerPopulations::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_FiltrerPopulations& asnMission = *new ASN1T_Mission_Automate_FiltrerPopulations();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_filtrer_populations;
    asnMsg.mission.u.mission_automate_filtrer_populations  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPointsIdx_ ), asnMission.points );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FiltrerPopulations::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FiltrerPopulations::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_filtrer_populations );
    ASN1T_Mission_Automate_FiltrerPopulations& asnMission = *asnMsg.mission.u.mission_automate_filtrer_populations;

    NET_ASN_Tools::Delete( asnMission.points );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

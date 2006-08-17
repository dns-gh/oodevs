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

#include "MIL_AutomateMission_GEN_OuvrirAxesProgression.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_OuvrirAxesProgression::nDIAAxesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirAxesProgression constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_OuvrirAxesProgression::MIL_AutomateMission_GEN_OuvrirAxesProgression( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirAxesProgression destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_OuvrirAxesProgression::~MIL_AutomateMission_GEN_OuvrirAxesProgression()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirAxesProgression::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_OuvrirAxesProgression::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAAxesIdx_ = DEC_Tools::InitializeDIAField( "axes_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirAxesProgression::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_OuvrirAxesProgression::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& asnMission = *asnMsg.mission.u.mission_automate_gen_ouvrir_axes_progression;
    if( !NET_ASN_Tools::CopyPathList( asnMission.axes, GetVariable( nDIAAxesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_OuvrirAxesProgression::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirAxesProgression::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_OuvrirAxesProgression::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& asnMission = *new ASN1T_Mission_Automate_GEN_OuvrirAxesProgression();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression;
    asnMsg.mission.u.mission_automate_gen_ouvrir_axes_progression  = &asnMission;

    NET_ASN_Tools::CopyPathList( GetVariable( nDIAAxesIdx_ ), asnMission.axes );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_OuvrirAxesProgression::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_OuvrirAxesProgression::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression );
    ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& asnMission = *asnMsg.mission.u.mission_automate_gen_ouvrir_axes_progression;

    NET_ASN_Tools::Delete( asnMission.axes );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

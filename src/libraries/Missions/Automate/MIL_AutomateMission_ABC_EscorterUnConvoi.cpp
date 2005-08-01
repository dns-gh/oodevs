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

#include "MIL_AutomateMission_ABC_EscorterUnConvoi.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_EscorterUnConvoi::nDIAConvoiAEscorterIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EscorterUnConvoi constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_EscorterUnConvoi::MIL_AutomateMission_ABC_EscorterUnConvoi( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EscorterUnConvoi destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_EscorterUnConvoi::~MIL_AutomateMission_ABC_EscorterUnConvoi()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EscorterUnConvoi::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_EscorterUnConvoi::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAConvoiAEscorterIdx_ = DEC_Tools::InitializeDIAField( "convoiAEscorter_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EscorterUnConvoi::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_EscorterUnConvoi::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_EscorterUnConvoi& asnMission = *asnMsg.mission.u.mission_automate_abc_escorter_un_convoi;
    if( !NET_ASN_Tools::CopyAgent( asnMission.convoi_a_escorter, GetVariable( nDIAConvoiAEscorterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_EscorterUnConvoi::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIAConvoiAEscorterIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EscorterUnConvoi::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_EscorterUnConvoi::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_EscorterUnConvoi& asnMission = *new ASN1T_Mission_Automate_ABC_EscorterUnConvoi();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_escorter_un_convoi;
    asnMsg.mission.u.mission_automate_abc_escorter_un_convoi  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAConvoiAEscorterIdx_ ), asnMission.convoi_a_escorter );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_EscorterUnConvoi::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_EscorterUnConvoi::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_escorter_un_convoi );
    ASN1T_Mission_Automate_ABC_EscorterUnConvoi& asnMission = *asnMsg.mission.u.mission_automate_abc_escorter_un_convoi;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

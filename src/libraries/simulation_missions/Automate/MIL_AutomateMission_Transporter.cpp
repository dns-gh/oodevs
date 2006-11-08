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

#include "MIL_AutomateMission_Transporter.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_Transporter::nDIAUnitesATransporterIdx_ = 0 ;
int MIL_AutomateMission_Transporter::nDIAPointDebarquementIdx_ = 0 ;
int MIL_AutomateMission_Transporter::nDIAPointEmbarquementIdx_ = 0 ;
int MIL_AutomateMission_Transporter::nDIAAvecMaterielIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Transporter constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Transporter::MIL_AutomateMission_Transporter( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Transporter destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_Transporter::~MIL_AutomateMission_Transporter()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Transporter::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_Transporter::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesATransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesATransporter_", diaType );
    nDIAPointDebarquementIdx_ = DEC_Tools::InitializeDIAField( "pointDebarquement_", diaType );
    nDIAPointEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "pointEmbarquement_", diaType );
    nDIAAvecMaterielIdx_ = DEC_Tools::InitializeDIAField( "avecMateriel_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Transporter::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_Transporter::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_Transporter& asnMission = *asnMsg.mission.u.mission_automate_transporter;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_transporter, GetVariable( nDIAUnitesATransporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_debarquement, GetVariable( nDIAPointDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_embarquement, GetVariable( nDIAPointEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.avec_materiel, GetVariable( nDIAAvecMaterielIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Transporter::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Transporter::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Transporter::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_Transporter& asnMission = *new ASN1T_Mission_Automate_Transporter();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_transporter;
    asnMsg.mission.u.mission_automate_transporter  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesATransporterIdx_ ), asnMission.unites_a_transporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDebarquementIdx_ ), asnMission.point_debarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointEmbarquementIdx_ ), asnMission.point_embarquement );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAAvecMaterielIdx_ ), asnMission.avec_materiel );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Transporter::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Transporter::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_transporter );
    ASN1T_Mission_Automate_Transporter& asnMission = *asnMsg.mission.u.mission_automate_transporter;

    NET_ASN_Tools::Delete( asnMission.unites_a_transporter );
    NET_ASN_Tools::Delete( asnMission.point_debarquement );
    NET_ASN_Tools::Delete( asnMission.point_embarquement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

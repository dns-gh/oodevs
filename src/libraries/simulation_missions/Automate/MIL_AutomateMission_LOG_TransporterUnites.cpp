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

#include "MIL_AutomateMission_LOG_TransporterUnites.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_LOG_TransporterUnites::nDIAUnitesATransporterIdx_ = 0 ;
int MIL_AutomateMission_LOG_TransporterUnites::nDIAPointRendezVousIdx_ = 0 ;
int MIL_AutomateMission_LOG_TransporterUnites::nDIAPointDestinationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterUnites constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_TransporterUnites::MIL_AutomateMission_LOG_TransporterUnites( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterUnites destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_TransporterUnites::~MIL_AutomateMission_LOG_TransporterUnites()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterUnites::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_LOG_TransporterUnites::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesATransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesATransporter_", diaType );
    nDIAPointRendezVousIdx_ = DEC_Tools::InitializeDIAField( "pointRendezVous_", diaType );
    nDIAPointDestinationIdx_ = DEC_Tools::InitializeDIAField( "pointDestination_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterUnites::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_LOG_TransporterUnites::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_LOG_TransporterUnites& asnMission = *asnMsg.mission.u.mission_automate_log_transporter_unites;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_transporter, GetVariable( nDIAUnitesATransporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_rendez_vous, GetVariable( nDIAPointRendezVousIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_destination, GetVariable( nDIAPointDestinationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_TransporterUnites::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterUnites::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_TransporterUnites::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_LOG_TransporterUnites& asnMission = *new ASN1T_Mission_Automate_LOG_TransporterUnites();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_log_transporter_unites;
    asnMsg.mission.u.mission_automate_log_transporter_unites  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesATransporterIdx_ ), asnMission.unites_a_transporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRendezVousIdx_ ), asnMission.point_rendez_vous );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDestinationIdx_ ), asnMission.point_destination );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterUnites::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_TransporterUnites::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_log_transporter_unites );
    ASN1T_Mission_Automate_LOG_TransporterUnites& asnMission = *asnMsg.mission.u.mission_automate_log_transporter_unites;

    NET_ASN_Tools::Delete( asnMission.unites_a_transporter );
    NET_ASN_Tools::Delete( asnMission.point_rendez_vous );
    NET_ASN_Tools::Delete( asnMission.point_destination );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

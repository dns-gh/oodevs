// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_LOG_TransporterFormationBlindee.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_LOG_TransporterFormationBlindee::nDIAUnitesATransporterIdx_ = 0 ;
int MIL_AutomateMission_LOG_TransporterFormationBlindee::nDIAPointRendezVousIdx_ = 0 ;
int MIL_AutomateMission_LOG_TransporterFormationBlindee::nDIAPointDestinationIdx_ = 0 ;
int MIL_AutomateMission_LOG_TransporterFormationBlindee::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterFormationBlindee constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_TransporterFormationBlindee::MIL_AutomateMission_LOG_TransporterFormationBlindee( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterFormationBlindee destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_TransporterFormationBlindee::~MIL_AutomateMission_LOG_TransporterFormationBlindee()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterFormationBlindee::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_LOG_TransporterFormationBlindee::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesATransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesATransporter_", diaType );
    nDIAPointRendezVousIdx_ = DEC_Tools::InitializeDIAField( "pointRendezVous_", diaType );
    nDIAPointDestinationIdx_ = DEC_Tools::InitializeDIAField( "pointDestination_", diaType );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterFormationBlindee::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_LOG_TransporterFormationBlindee::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_LOG_TransporterFormationBlindee& asnMission = *asnMsg.mission.u.mission_automate_log_transporter_formation_blindee;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_transporter, GetVariable( nDIAUnitesATransporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_rendez_vous, pointRendezVous_, GetVariable( nDIAPointRendezVousIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_destination, pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_TransporterFormationBlindee::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterFormationBlindee::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_TransporterFormationBlindee::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_LOG_TransporterFormationBlindee& asnMission = *new ASN1T_Mission_Automate_LOG_TransporterFormationBlindee();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_log_transporter_formation_blindee;
    asnMsg.mission.u.mission_automate_log_transporter_formation_blindee  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesATransporterIdx_ ), asnMission.unites_a_transporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRendezVousIdx_ ), asnMission.point_rendez_vous );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDestinationIdx_ ), asnMission.point_destination );
    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_TransporterFormationBlindee::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_TransporterFormationBlindee::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_log_transporter_formation_blindee );
    ASN1T_Mission_Automate_LOG_TransporterFormationBlindee& asnMission = *asnMsg.mission.u.mission_automate_log_transporter_formation_blindee;

    NET_ASN_Tools::Delete( asnMission.point_rendez_vous );
    NET_ASN_Tools::Delete( asnMission.point_destination );
    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

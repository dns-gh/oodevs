// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_LOG_TransporterUnite.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_TransporterUnite::nDIAUnitesATransporterIdx_ = 0 ;
int MIL_PionMission_LOG_TransporterUnite::nDIAPointRendezVousIdx_ = 0 ;
int MIL_PionMission_LOG_TransporterUnite::nDIAPointDestinationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_TransporterUnite::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesATransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesATransporter_", diaType );
    nDIAPointRendezVousIdx_ = DEC_Tools::InitializeDIAField( "pointRendezVous_", diaType );
    nDIAPointDestinationIdx_ = DEC_Tools::InitializeDIAField( "pointDestination_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TransporterUnite::MIL_PionMission_LOG_TransporterUnite( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TransporterUnite::~MIL_PionMission_LOG_TransporterUnite()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_TransporterUnite::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_TransporterUnite& asnMission = *asnMsg.mission.u.mission_pion_log_transporter_unite;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_transporter, GetVariable( nDIAUnitesATransporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_rendez_vous, pointRendezVous_, GetVariable( nDIAPointRendezVousIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_destination, pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TransporterUnite::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesATransporterIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointRendezVous_, GetVariable( nDIAPointRendezVousIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TransporterUnite::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_TransporterUnite& mission = static_cast< MIL_PionMission_LOG_TransporterUnite& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesATransporterIdx_ ), GetVariable( nDIAUnitesATransporterIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRendezVousIdx_ ), pointRendezVous_, GetVariable( nDIAPointRendezVousIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDestinationIdx_ ), pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TransporterUnite::Terminate()
{
    NET_ASN_Tools::ResetPoint( pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TransporterUnite::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_TransporterUnite& asnMission = *new ASN1T_Mission_Pion_LOG_TransporterUnite();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_transporter_unite;
    asnMsg.mission.u.mission_pion_log_transporter_unite  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesATransporterIdx_ ), asnMission.unites_a_transporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRendezVousIdx_ ), asnMission.point_rendez_vous );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDestinationIdx_ ), asnMission.point_destination );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUnite::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TransporterUnite::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_transporter_unite );
    ASN1T_Mission_Pion_LOG_TransporterUnite& asnMission = *asnMsg.mission.u.mission_pion_log_transporter_unite;

    NET_ASN_Tools::Delete( asnMission.unites_a_transporter );
    NET_ASN_Tools::Delete( asnMission.point_rendez_vous );
    NET_ASN_Tools::Delete( asnMission.point_destination );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

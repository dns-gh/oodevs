// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_LOG_TransporterUniteChars.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_LOG_TransporterUniteChars::nDIAUnitesATransporterIdx_ = 0 ;
int MIL_PionMission_LOG_TransporterUniteChars::nDIAPointRendezVousIdx_ = 0 ;
int MIL_PionMission_LOG_TransporterUniteChars::nDIAPointDestinationIdx_ = 0 ;
int MIL_PionMission_LOG_TransporterUniteChars::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_LOG_TransporterUniteChars::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesATransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesATransporter_", diaType );
    nDIAPointRendezVousIdx_ = DEC_Tools::InitializeDIAField( "pointRendezVous_", diaType );
    nDIAPointDestinationIdx_ = DEC_Tools::InitializeDIAField( "pointDestination_", diaType );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TransporterUniteChars::MIL_PionMission_LOG_TransporterUniteChars( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_LOG_TransporterUniteChars::~MIL_PionMission_LOG_TransporterUniteChars()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_LOG_TransporterUniteChars::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_LOG_TransporterUniteChars& asnMission = *asnMsg.mission.u.mission_pion_log_transporter_unite_chars;
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
// Name: MIL_PionMission_LOG_TransporterUniteChars::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TransporterUniteChars::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesATransporterIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointRendezVous_, GetVariable( nDIAPointRendezVousIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) );
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_LOG_TransporterUniteChars::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_LOG_TransporterUniteChars& mission = static_cast< MIL_PionMission_LOG_TransporterUniteChars& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesATransporterIdx_ ), GetVariable( nDIAUnitesATransporterIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRendezVousIdx_ ), pointRendezVous_, GetVariable( nDIAPointRendezVousIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDestinationIdx_ ), pointDestination_, GetVariable( nDIAPointDestinationIdx_ ) );
    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TransporterUniteChars::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TransporterUniteChars::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_LOG_TransporterUniteChars& asnMission = *new ASN1T_Mission_Pion_LOG_TransporterUniteChars();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_log_transporter_unite_chars;
    asnMsg.mission.u.mission_pion_log_transporter_unite_chars  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesATransporterIdx_ ), asnMission.unites_a_transporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRendezVousIdx_ ), asnMission.point_rendez_vous );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDestinationIdx_ ), asnMission.point_destination );
    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_LOG_TransporterUniteChars::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_LOG_TransporterUniteChars::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_log_transporter_unite_chars );
    ASN1T_Mission_Pion_LOG_TransporterUniteChars& asnMission = *asnMsg.mission.u.mission_pion_log_transporter_unite_chars;

    NET_ASN_Tools::Delete( asnMission.point_rendez_vous );
    NET_ASN_Tools::Delete( asnMission.point_destination );
    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

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

#include "MIL_AutomateMission_LOG_ReconnaitreItineraire.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_LOG_ReconnaitreItineraire::nDIAItineraireLogIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_ReconnaitreItineraire constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_ReconnaitreItineraire::MIL_AutomateMission_LOG_ReconnaitreItineraire( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_ReconnaitreItineraire destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_LOG_ReconnaitreItineraire::~MIL_AutomateMission_LOG_ReconnaitreItineraire()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_ReconnaitreItineraire::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_LOG_ReconnaitreItineraire::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireLogIdx_ = DEC_Tools::InitializeDIAField( "itineraireLog_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_ReconnaitreItineraire::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_LOG_ReconnaitreItineraire::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& asnMission = *asnMsg.mission.u.mission_automate_log_reconnaitre_itineraire;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire_log, itineraireLog_, GetVariable( nDIAItineraireLogIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_ReconnaitreItineraire::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraireLog_, GetVariable( nDIAItineraireLogIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_ReconnaitreItineraire::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_ReconnaitreItineraire::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Automate_LOG_ReconnaitreItineraire();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_log_reconnaitre_itineraire;
    asnMsg.mission.u.mission_automate_log_reconnaitre_itineraire  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireLogIdx_ ), asnMission.itineraire_log );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_LOG_ReconnaitreItineraire::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_LOG_ReconnaitreItineraire::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_log_reconnaitre_itineraire );
    ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& asnMission = *asnMsg.mission.u.mission_automate_log_reconnaitre_itineraire;

    NET_ASN_Tools::Delete( asnMission.itineraire_log );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

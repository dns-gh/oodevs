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
#include "MIL_PionMission_NBC_ReconnaitreItineraire.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_NBC_ReconnaitreItineraire::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_NBC_ReconnaitreItineraire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_NBC_ReconnaitreItineraire::MIL_PionMission_NBC_ReconnaitreItineraire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_NBC_ReconnaitreItineraire::~MIL_PionMission_NBC_ReconnaitreItineraire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_NBC_ReconnaitreItineraire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *asnMsg.mission.u.mission_pion_nbc_reconnaitre_itineraire;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_NBC_ReconnaitreItineraire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_NBC_ReconnaitreItineraire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_NBC_ReconnaitreItineraire& mission = static_cast< MIL_PionMission_NBC_ReconnaitreItineraire& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_ReconnaitreItineraire::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_ReconnaitreItineraire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreItineraire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire;
    asnMsg.mission.u.mission_pion_nbc_reconnaitre_itineraire  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_ReconnaitreItineraire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_ReconnaitreItineraire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire );
    ASN1T_Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *asnMsg.mission.u.mission_pion_nbc_reconnaitre_itineraire;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

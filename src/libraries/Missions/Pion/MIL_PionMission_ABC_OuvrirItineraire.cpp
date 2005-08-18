// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:44 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ABC_OuvrirItineraire.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_OuvrirItineraire::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::InitializeDIA
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_OuvrirItineraire::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire constructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_OuvrirItineraire::MIL_PionMission_ABC_OuvrirItineraire( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire destructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_OuvrirItineraire::~MIL_PionMission_ABC_OuvrirItineraire()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_OuvrirItineraire::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *asnMsg.mission.u.mission_pion_abc_ouvrir_itineraire;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_OuvrirItineraire::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::Initialize
// Created: 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_OuvrirItineraire::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_OuvrirItineraire& mission = static_cast< MIL_PionMission_ABC_OuvrirItineraire& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::Terminate
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_OuvrirItineraire::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::Serialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_OuvrirItineraire::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *new ASN1T_Mission_Pion_ABC_OuvrirItineraire();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_ouvrir_itineraire;
    asnMsg.mission.u.mission_pion_abc_ouvrir_itineraire  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_OuvrirItineraire::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_OuvrirItineraire::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_ouvrir_itineraire );
    ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *asnMsg.mission.u.mission_pion_abc_ouvrir_itineraire;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ABC_ContreAttaquer.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_ContreAttaquer::nDIAItineraireAssautIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_ContreAttaquer::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireAssautIdx_ = DEC_Tools::InitializeDIAField( "itineraireAssaut_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ContreAttaquer::MIL_PionMission_ABC_ContreAttaquer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ContreAttaquer::~MIL_PionMission_ABC_ContreAttaquer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_ContreAttaquer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *asnMsg.mission.u.mission_pion_abc_contre_attaquer;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire_assaut, itineraireAssaut_, GetVariable( nDIAItineraireAssautIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ContreAttaquer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPath( itineraireAssaut_, GetVariable( nDIAItineraireAssautIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ContreAttaquer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_ContreAttaquer& mission = static_cast< MIL_PionMission_ABC_ContreAttaquer& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireAssautIdx_ ), itineraireAssaut_, GetVariable( nDIAItineraireAssautIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ContreAttaquer::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraireAssaut_, GetVariable( nDIAItineraireAssautIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ContreAttaquer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_ABC_ContreAttaquer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_contre_attaquer;
    asnMsg.mission.u.mission_pion_abc_contre_attaquer  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireAssautIdx_ ), asnMission.itineraire_assaut );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ContreAttaquer::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ContreAttaquer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_contre_attaquer );
    ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *asnMsg.mission.u.mission_pion_abc_contre_attaquer;

    NET_ASN_Tools::Delete( asnMission.itineraire_assaut );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

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
#include "MIL_PionMission_ASY_CommettreAttentatContreInstallation.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASY_CommettreAttentatContreInstallation::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContreInstallation::nDIAPointRepliIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContreInstallation::nDIAInstallationCibleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASY_CommettreAttentatContreInstallation::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIAPointRepliIdx_ = DEC_Tools::InitializeDIAField( "pointRepli_", diaType );
    nDIAInstallationCibleIdx_ = DEC_Tools::InitializeDIAField( "installationCible_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASY_CommettreAttentatContreInstallation::MIL_PionMission_ASY_CommettreAttentatContreInstallation( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASY_CommettreAttentatContreInstallation::~MIL_PionMission_ASY_CommettreAttentatContreInstallation()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASY_CommettreAttentatContreInstallation::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation& asnMission = *asnMsg.mission.u.mission_pion_asy_commettre_attentatContreInstallation;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_repli, GetVariable( nDIAPointRepliIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.installation_cible, GetVariable( nDIAInstallationCibleIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASY_CommettreAttentatContreInstallation::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

            
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASY_CommettreAttentatContreInstallation::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASY_CommettreAttentatContreInstallation& mission = static_cast< MIL_PionMission_ASY_CommettreAttentatContreInstallation& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRepliIdx_ ), GetVariable( nDIAPointRepliIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAInstallationCibleIdx_ ), GetVariable( nDIAInstallationCibleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContreInstallation::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContreInstallation::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asy_commettre_attentatContreInstallation;
    asnMsg.mission.u.mission_pion_asy_commettre_attentatContreInstallation  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRepliIdx_ ), asnMission.point_repli );
    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAInstallationCibleIdx_ ), asnMission.installation_cible, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreInstallation::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContreInstallation::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asy_commettre_attentatContreInstallation );
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation& asnMission = *asnMsg.mission.u.mission_pion_asy_commettre_attentatContreInstallation;

    NET_ASN_Tools::Delete( asnMission.itineraire );
    NET_ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

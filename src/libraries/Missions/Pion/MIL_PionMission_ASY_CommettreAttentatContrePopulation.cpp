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
#include "MIL_PionMission_ASY_CommettreAttentatContrePopulation.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASY_CommettreAttentatContrePopulation::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContrePopulation::nDIAPointRepliIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContrePopulation::nDIAPopulationCibleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASY_CommettreAttentatContrePopulation::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIAPointRepliIdx_ = DEC_Tools::InitializeDIAField( "pointRepli_", diaType );
    nDIAPopulationCibleIdx_ = DEC_Tools::InitializeDIAField( "populationCible_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASY_CommettreAttentatContrePopulation::MIL_PionMission_ASY_CommettreAttentatContrePopulation( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASY_CommettreAttentatContrePopulation::~MIL_PionMission_ASY_CommettreAttentatContrePopulation()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASY_CommettreAttentatContrePopulation::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation& asnMission = *asnMsg.mission.u.mission_pion_asy_commettre_attentatContrePopulation;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_repli, GetVariable( nDIAPointRepliIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPopulationKnowledge( asnMission.population_cible, GetVariable( nDIAPopulationCibleIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASY_CommettreAttentatContrePopulation::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

            
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASY_CommettreAttentatContrePopulation::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASY_CommettreAttentatContrePopulation& mission = static_cast< MIL_PionMission_ASY_CommettreAttentatContrePopulation& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRepliIdx_ ), GetVariable( nDIAPointRepliIdx_ ) );
    NET_ASN_Tools::CopyPopulationKnowledge( mission.GetVariable( nDIAPopulationCibleIdx_ ), GetVariable( nDIAPopulationCibleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContrePopulation::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContrePopulation::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asy_commettre_attentatContrePopulation;
    asnMsg.mission.u.mission_pion_asy_commettre_attentatContrePopulation  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRepliIdx_ ), asnMission.point_repli );
    NET_ASN_Tools::CopyPopulationKnowledge( GetVariable( nDIAPopulationCibleIdx_ ), asnMission.population_cible, pion_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContrePopulation::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContrePopulation::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asy_commettre_attentatContrePopulation );
    ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation& asnMission = *asnMsg.mission.u.mission_pion_asy_commettre_attentatContrePopulation;

    NET_ASN_Tools::Delete( asnMission.itineraire );
    NET_ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

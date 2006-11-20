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

#include "simulation_missions_pch.h"
#include "MIL_PionMission_ASY_CommettreAttentatContreForcesArmees.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::nDIAPointRepliIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::nDIATypeAttentatForcesArmeesIdx_ = 0 ;
int MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::nDIAUniteCibleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIAPointRepliIdx_ = DEC_Tools::InitializeDIAField( "pointRepli_", diaType );
    nDIATypeAttentatForcesArmeesIdx_ = DEC_Tools::InitializeDIAField( "typeAttentatForcesArmees_", diaType );
    nDIAUniteCibleIdx_ = DEC_Tools::InitializeDIAField( "uniteCible_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::MIL_PionMission_ASY_CommettreAttentatContreForcesArmees( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::~MIL_PionMission_ASY_CommettreAttentatContreForcesArmees()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees& asnMission = *asnMsg.mission.u.mission_pion_asy_commettre_attentatContreForcesArmees;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_repli, GetVariable( nDIAPointRepliIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type_attentat_forces_armees, GetVariable( nDIATypeAttentatForcesArmeesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentKnowledge( asnMission.unite_cible, GetVariable( nDIAUniteCibleIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASY_CommettreAttentatContreForcesArmees& mission = static_cast< MIL_PionMission_ASY_CommettreAttentatContreForcesArmees& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRepliIdx_ ), GetVariable( nDIAPointRepliIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeAttentatForcesArmeesIdx_ ), GetVariable( nDIATypeAttentatForcesArmeesIdx_ ) );
    NET_ASN_Tools::CopyAgentKnowledge( mission.GetVariable( nDIAUniteCibleIdx_ ), GetVariable( nDIAUniteCibleIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asy_commettre_attentatContreForcesArmees;
    asnMsg.mission.u.mission_pion_asy_commettre_attentatContreForcesArmees  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRepliIdx_ ), asnMission.point_repli );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeAttentatForcesArmeesIdx_ ), asnMission.type_attentat_forces_armees );
    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( nDIAUniteCibleIdx_ ), asnMission.unite_cible, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASY_CommettreAttentatContreForcesArmees::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asy_commettre_attentatContreForcesArmees );
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees& asnMission = *asnMsg.mission.u.mission_pion_asy_commettre_attentatContreForcesArmees;

    NET_ASN_Tools::Delete( asnMission.itineraire );
    NET_ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

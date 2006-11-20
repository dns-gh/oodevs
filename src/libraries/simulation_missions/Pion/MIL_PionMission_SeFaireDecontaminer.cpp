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
#include "MIL_PionMission_SeFaireDecontaminer.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_SeFaireDecontaminer::nDIAPointArriveeIdx_ = 0 ;
int MIL_PionMission_SeFaireDecontaminer::nDIASiteDecontaminationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_SeFaireDecontaminer::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointArriveeIdx_ = DEC_Tools::InitializeDIAField( "pointArrivee_", diaType );
    nDIASiteDecontaminationIdx_ = DEC_Tools::InitializeDIAField( "siteDecontamination_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_SeFaireDecontaminer::MIL_PionMission_SeFaireDecontaminer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_SeFaireDecontaminer::~MIL_PionMission_SeFaireDecontaminer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_SeFaireDecontaminer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *asnMsg.mission.u.mission_pion_se_faire_decontaminer;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_arrivee, GetVariable( nDIAPointArriveeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.site_decontamination, GetVariable( nDIASiteDecontaminationIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_SeFaireDecontaminer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_SeFaireDecontaminer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_SeFaireDecontaminer& mission = static_cast< MIL_PionMission_SeFaireDecontaminer& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointArriveeIdx_ ), GetVariable( nDIAPointArriveeIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIASiteDecontaminationIdx_ ), GetVariable( nDIASiteDecontaminationIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_SeFaireDecontaminer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Pion_SeFaireDecontaminer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_se_faire_decontaminer;
    asnMsg.mission.u.mission_pion_se_faire_decontaminer  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointArriveeIdx_ ), asnMission.point_arrivee );
    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIASiteDecontaminationIdx_ ), asnMission.site_decontamination, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireDecontaminer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_SeFaireDecontaminer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_se_faire_decontaminer );
    ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *asnMsg.mission.u.mission_pion_se_faire_decontaminer;

    NET_ASN_Tools::Delete( asnMission.point_arrivee );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

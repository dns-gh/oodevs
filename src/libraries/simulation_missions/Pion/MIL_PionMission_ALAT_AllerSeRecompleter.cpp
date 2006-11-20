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
#include "MIL_PionMission_ALAT_AllerSeRecompleter.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ALAT_AllerSeRecompleter::nDIAPlotsRavitaillementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ALAT_AllerSeRecompleter::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_AllerSeRecompleter::MIL_PionMission_ALAT_AllerSeRecompleter( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ALAT_AllerSeRecompleter::~MIL_PionMission_ALAT_AllerSeRecompleter()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ALAT_AllerSeRecompleter::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ALAT_AllerSeRecompleter& asnMission = *asnMsg.mission.u.mission_pion_alat_aller_se_recompleter;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_AllerSeRecompleter::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ALAT_AllerSeRecompleter::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ALAT_AllerSeRecompleter& mission = static_cast< MIL_PionMission_ALAT_AllerSeRecompleter& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAPlotsRavitaillementIdx_ ), GetVariable( nDIAPlotsRavitaillementIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_AllerSeRecompleter::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ALAT_AllerSeRecompleter& asnMission = *new ASN1T_Mission_Pion_ALAT_AllerSeRecompleter();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_alat_aller_se_recompleter;
    asnMsg.mission.u.mission_pion_alat_aller_se_recompleter  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ALAT_AllerSeRecompleter::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ALAT_AllerSeRecompleter::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_alat_aller_se_recompleter );
    ASN1T_Mission_Pion_ALAT_AllerSeRecompleter& asnMission = *asnMsg.mission.u.mission_pion_alat_aller_se_recompleter;

    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}

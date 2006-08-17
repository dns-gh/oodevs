// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_GEN_FaireFranchir.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_FaireFranchir::nDIASitesDeFranchissementIdx_ = 0 ;
int MIL_AutomateMission_GEN_FaireFranchir::nDIAZoneRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FaireFranchir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_FaireFranchir::MIL_AutomateMission_GEN_FaireFranchir( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FaireFranchir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_FaireFranchir::~MIL_AutomateMission_GEN_FaireFranchir()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FaireFranchir::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_FaireFranchir::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASitesDeFranchissementIdx_ = DEC_Tools::InitializeDIAField( "sitesDeFranchissement_", diaType );
    nDIAZoneRegroupementIdx_ = DEC_Tools::InitializeDIAField( "zoneRegroupement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FaireFranchir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_FaireFranchir::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_FaireFranchir& asnMission = *asnMsg.mission.u.mission_automate_gen_faire_franchir;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.sites_de_franchissement, GetVariable( nDIASitesDeFranchissementIdx_ ), automate_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyLocation( asnMission.zone_regroupement, GetVariable( nDIAZoneRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_FaireFranchir::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FaireFranchir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_FaireFranchir::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_FaireFranchir& asnMission = *new ASN1T_Mission_Automate_GEN_FaireFranchir();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_faire_franchir;
    asnMsg.mission.u.mission_automate_gen_faire_franchir  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIASitesDeFranchissementIdx_ ), asnMission.sites_de_franchissement, automate_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyLocation( GetVariable( nDIAZoneRegroupementIdx_ ), asnMission.zone_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_FaireFranchir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_FaireFranchir::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_faire_franchir );
    ASN1T_Mission_Automate_GEN_FaireFranchir& asnMission = *asnMsg.mission.u.mission_automate_gen_faire_franchir;

    NET_ASN_Tools::Delete( asnMission.sites_de_franchissement );
    NET_ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

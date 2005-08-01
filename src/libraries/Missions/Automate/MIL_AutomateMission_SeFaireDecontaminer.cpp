// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_SeFaireDecontaminer.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_SeFaireDecontaminer::nDIAZoneArriveeIdx_ = 0 ;
int MIL_AutomateMission_SeFaireDecontaminer::nDIASiteDecontaminationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireDecontaminer constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_SeFaireDecontaminer::MIL_AutomateMission_SeFaireDecontaminer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireDecontaminer destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_SeFaireDecontaminer::~MIL_AutomateMission_SeFaireDecontaminer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireDecontaminer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_SeFaireDecontaminer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneArriveeIdx_ = DEC_Tools::InitializeDIAField( "zoneArrivee_", diaType );
    nDIASiteDecontaminationIdx_ = DEC_Tools::InitializeDIAField( "siteDecontamination_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireDecontaminer::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_SeFaireDecontaminer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_SeFaireDecontaminer& asnMission = *asnMsg.mission.u.mission_automate_se_faire_decontaminer;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_arrivee, zoneArrivee_, GetVariable( nDIAZoneArriveeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.site_decontamination, GetVariable( nDIASiteDecontaminationIdx_ ), automate_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeFaireDecontaminer::Terminate()
{
    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIASiteDecontaminationIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireDecontaminer::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeFaireDecontaminer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Automate_SeFaireDecontaminer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_se_faire_decontaminer;
    asnMsg.mission.u.mission_automate_se_faire_decontaminer  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneArriveeIdx_ ), asnMission.zone_arrivee );
    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIASiteDecontaminationIdx_ ), asnMission.site_decontamination, automate_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_SeFaireDecontaminer::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_SeFaireDecontaminer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_se_faire_decontaminer );
    ASN1T_Mission_Automate_SeFaireDecontaminer& asnMission = *asnMsg.mission.u.mission_automate_se_faire_decontaminer;

    NET_ASN_Tools::Delete( asnMission.zone_arrivee );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

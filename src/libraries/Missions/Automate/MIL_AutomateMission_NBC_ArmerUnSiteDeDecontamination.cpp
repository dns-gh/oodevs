// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::nDIASiteIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::~MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteIdx_ = DEC_Tools::InitializeDIAField( "site_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *asnMsg.mission.u.mission_automate_nbc_armer_un_site_de_decontamination;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.site, site_, GetVariable( nDIASiteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Terminate()
{
    NET_ASN_Tools::ResetPolygon( site_, GetVariable( nDIASiteIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *new ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination;
    asnMsg.mission.u.mission_automate_nbc_armer_un_site_de_decontamination  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIASiteIdx_ ), asnMission.site );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination );
    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *asnMsg.mission.u.mission_automate_nbc_armer_un_site_de_decontamination;

    NET_ASN_Tools::Delete( asnMission.site );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

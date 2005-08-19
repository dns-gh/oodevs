// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:55 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_INF_AttaquerSouplesse.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_AttaquerSouplesse::nDIAObjectifsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerSouplesse constructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AttaquerSouplesse::MIL_AutomateMission_INF_AttaquerSouplesse( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerSouplesse destructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AttaquerSouplesse::~MIL_AutomateMission_INF_AttaquerSouplesse()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerSouplesse::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_AttaquerSouplesse::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObjectifsIdx_ = DEC_Tools::InitializeDIAField( "objectifs_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerSouplesse::Initialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_AttaquerSouplesse::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_AttaquerSouplesse& asnMission = *asnMsg.mission.u.mission_automate_inf_attaquer_souplesse;
    if( !NET_ASN_Tools::CopyPointList( asnMission.objectifs, objectifs_, GetVariable( nDIAObjectifsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AttaquerSouplesse::Terminate()
{
    NET_ASN_Tools::ResetPointList( objectifs_, GetVariable( nDIAObjectifsIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerSouplesse::Serialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AttaquerSouplesse::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_AttaquerSouplesse& asnMission = *new ASN1T_Mission_Automate_INF_AttaquerSouplesse();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_attaquer_souplesse;
    asnMsg.mission.u.mission_automate_inf_attaquer_souplesse  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAObjectifsIdx_ ), asnMission.objectifs );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerSouplesse::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AttaquerSouplesse::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_attaquer_souplesse );
    ASN1T_Mission_Automate_INF_AttaquerSouplesse& asnMission = *asnMsg.mission.u.mission_automate_inf_attaquer_souplesse;

    NET_ASN_Tools::Delete( asnMission.objectifs );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

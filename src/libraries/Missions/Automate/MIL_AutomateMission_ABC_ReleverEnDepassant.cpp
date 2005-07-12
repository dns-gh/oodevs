// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_ReleverEnDepassant.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_ReleverEnDepassant::nDIACompagnieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReleverEnDepassant constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ReleverEnDepassant::MIL_AutomateMission_ABC_ReleverEnDepassant( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReleverEnDepassant destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ReleverEnDepassant::~MIL_AutomateMission_ABC_ReleverEnDepassant()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReleverEnDepassant::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_ReleverEnDepassant::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACompagnieIdx_ = DEC_Tools::InitializeDIAField( "compagnie_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReleverEnDepassant::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_ReleverEnDepassant::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_ReleverEnDepassant& asnMission = *asnMsg.mission.u.mission_automate_abc_relever_en_depassant;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.compagnie, GetVariable( nDIACompagnieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ReleverEnDepassant::Terminate()
{
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIACompagnieIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReleverEnDepassant::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ReleverEnDepassant::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_ReleverEnDepassant& asnMission = *new ASN1T_Mission_Automate_ABC_ReleverEnDepassant();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_relever_en_depassant;
    asnMsg.mission.u.mission_automate_abc_relever_en_depassant  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIACompagnieIdx_ ), asnMission.compagnie );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReleverEnDepassant::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ReleverEnDepassant::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_relever_en_depassant );
    ASN1T_Mission_Automate_ABC_ReleverEnDepassant& asnMission = *asnMsg.mission.u.mission_automate_abc_relever_en_depassant;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

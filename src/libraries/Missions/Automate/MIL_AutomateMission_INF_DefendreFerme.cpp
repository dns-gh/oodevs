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

#include "MIL_AutomateMission_INF_DefendreFerme.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_DefendreFerme::nDIAPositionIdx_ = 0 ;
int MIL_AutomateMission_INF_DefendreFerme::nDIAPreparerTerrainIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_DefendreFerme constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_DefendreFerme::MIL_AutomateMission_INF_DefendreFerme( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_DefendreFerme destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_DefendreFerme::~MIL_AutomateMission_INF_DefendreFerme()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_DefendreFerme::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_DefendreFerme::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionIdx_ = DEC_Tools::InitializeDIAField( "position_", diaType );
    nDIAPreparerTerrainIdx_ = DEC_Tools::InitializeDIAField( "preparerTerrain_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_DefendreFerme::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_DefendreFerme::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_DefendreFerme& asnMission = *asnMsg.mission.u.mission_automate_inf_defendre_ferme;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.position, position_, GetVariable( nDIAPositionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.preparer_terrain, GetVariable( nDIAPreparerTerrainIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_DefendreFerme::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPreparerTerrainIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_DefendreFerme::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_DefendreFerme::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_DefendreFerme& asnMission = *new ASN1T_Mission_Automate_INF_DefendreFerme();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_defendre_ferme;
    asnMsg.mission.u.mission_automate_inf_defendre_ferme  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAPositionIdx_ ), asnMission.position );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPreparerTerrainIdx_ ), asnMission.preparer_terrain );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_DefendreFerme::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_DefendreFerme::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_defendre_ferme );
    ASN1T_Mission_Automate_INF_DefendreFerme& asnMission = *asnMsg.mission.u.mission_automate_inf_defendre_ferme;

    NET_ASN_Tools::Delete( asnMission.position );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}

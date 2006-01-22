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

#include "MIL_AutomateMission_INF_AttaquerEnTerrainDifficile.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::nDIAObjectifsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerEnTerrainDifficile constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::MIL_AutomateMission_INF_AttaquerEnTerrainDifficile( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerEnTerrainDifficile destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::~MIL_AutomateMission_INF_AttaquerEnTerrainDifficile()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObjectifsIdx_ = DEC_Tools::InitializeDIAField( "objectifs_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& asnMission = *asnMsg.mission.u.mission_automate_inf_attaquer_en_terrain_difficile;
    if( !NET_ASN_Tools::CopyPointList( asnMission.objectifs, GetVariable( nDIAObjectifsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& asnMission = *new ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile;
    asnMsg.mission.u.mission_automate_inf_attaquer_en_terrain_difficile  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAObjectifsIdx_ ), asnMission.objectifs );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_AttaquerEnTerrainDifficile::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile );
    ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& asnMission = *asnMsg.mission.u.mission_automate_inf_attaquer_en_terrain_difficile;

    NET_ASN_Tools::Delete( asnMission.objectifs );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
